<?php

/*
 * Represents a single, active LDAP connection, loading its
 * settings from the "ldap" section of a provided configuration
 * file.
 */
class ldapinfo {
    private $ldapConf;
    // LDAP variables
    private $ldapHost;
    private $ldapUser;
    private $ldapPwd;
    private $ldapBaseDN;
    private $ldapUserBaseDN;
    private $ldapGroupBaseDN;
    private $ldapGroupMask;
    private $ldapconn;
    private $ldapGroupDNParsers;

    public function __construct($inifile) {
        $ini_array = parse_ini_file($inifile, true);
        $this->ldapConf = $ini_array["ldap"];
        $this->ldapHost = $this->ldapConf["url"];
        $this->ldapUser = $this->ldapConf["user"];
        $this->ldapPwd = $this->ldapConf["password"];
        $this->ldapBaseDN = $this->ldapConf["baseDN"];
        $this->ldapUserBaseDN = $this->ldapConf["userBaseDN"];
        $this->ldapGroupBaseDN = $this->ldapConf["groupBaseDN"];
        $this->ldapGroupMask = $this->ldapConf["groupMask"];
        $plist = explode(",", $this->ldapConf["groupDNParser"]);
        foreach ($plist as $pname) {
            $vals = $ini_array[$pname];
            $vals["cat"] = $pname;
            $this->ldapGroupDNParsers[] = $vals;
        }
    
        $this->ldapconn = ldap_connect($this->ldapHost) or die("Could not connect to $ldapHost");
        ldap_set_option($this->ldapconn, LDAP_OPT_PROTOCOL_VERSION, 3);
        ldap_set_option ($this->ldapconn, LDAP_OPT_REFERRALS, 0) or die('Unable to set LDAP opt referrals');
        ldap_bind($this->ldapconn, $this->ldapUser, $this->ldapPwd) or die("Could not bind\n");
    }

    /*
     * Convert a binary Active Directory GUID to a string
     */
    function GUIDtoStr($binary_guid) {
        try {
            $unpacked = unpack('Va/v2b/n2c/Nd', $binary_guid);
            return sprintf('%08X-%04X-%04X-%04X-%04X%08X', $unpacked['a'], $unpacked['b1'], $unpacked['b2'], $unpacked['c1'], $unpacked['c2'], $unpacked['d']);
        } catch (Exception $e) {
            return null;
        }
    }
	
    /*
     * Convert a binary Active Directory SID to a string
     */
    function bin_to_str_sid($binsid) {
        $hex_sid = bin2hex($binsid);
        $rev = hexdec(substr($hex_sid, 0, 2));
        $subcount = hexdec(substr($hex_sid, 2, 2));
        $auth = hexdec(substr($hex_sid, 4, 12));
        $result    = "$rev-$auth";

        for ($x=0;$x < $subcount; $x++) {
            $subauth[$x] = 
                hexdec($this->little_endian(substr($hex_sid, 16 + ($x * 8), 8)));
            $result .= "-" . $subauth[$x];
        }

        // Cheat by tacking on the S-
        return 'S-' . $result;
    }

    /*
     *  Converts a little-endian hex string to big-endian
     */
    function little_endian($hex) {
        $result='';
        for ($x = strlen($hex) - 2; $x >= 0; $x = $x - 2) {
            $result .= substr($hex, $x, 2);
        }
        return $result;
    }

    /*
     * Parse a group DN into components according to the provided rules,
     * each one consisting of a regex and an index for the title and
     * the "section" to extract. If either is non-numeric, it is used
     * as a constant rather than an index.
     * The first rule that matches is used, and the name of the rule
     * is added to the resulting array.
     */
    function parseGroupDN($dn) {
        $res = null;
        foreach ($this->ldapGroupDNParsers as $p) {
            if (preg_match("/^".$p["groupRE"]."$/i", $dn, $matches)) {
                $res = array();
                $res["cat"] = $p["cat"];
                if (is_numeric($p["groupTitle"])) {
                    $res["title"] = $matches[$p["groupTitle"]];
                } else {
                    $res["title"] = $p["groupTitle"];
                }
                if (is_numeric($p["groupSection"])) {
                    $res["section"] = $matches[$p["groupSection"]];
                } else {
                    $res["section"] = $p["groupSection"];
                }
                break;
            }
        }
        return $res;
    }

    /*
     * Compare two group entries, for sorting.
     * Primary criterion is the category, from the parser.
     * Secondary criterion is the section.
     * Tertiary criterion is the title.
     */
    function cmpGrp($a, $b) {
        $v1 = strcmp($a["cat"], $b["cat"]);
        $v2 = strcmp($a["section"], $b["section"]);
        $v3 = strcmp($a["title"], $b["title"]);
        return ($v1 > 0)?1:(($v1 < 0)?-1:(($v2>0)?1:(($v2<0)?-1:$v3)));
    }

    /*
     * Compare two person entries, for sorting.
     * Primary criterion is the last name, "sn".
     * Secondary criterion is the given name.
     */
    function cmpUser($a, $b) {
        $v1 = strcmp($a["sn"], $b["sn"]);
        $v2 = strcmp($a["givenname"], $b["givenname"]);
        return ($v1 > 0)?1:(($v1 < 0)?-1:$v2);
    }

    /*
     * Compare two group entries, for sorting.
     * Primary criterion is the category, from the parser.
     * Secondary criterion is the section.
     * Tertiary criterion is the title.
     */
    function cmpGrpMemb($a, $b) {
        if (array_key_exists("title", $a)) {
            if (array_key_exists("givenname", $b)) {
                return -1;
            }
            $v1 = strcmp($a["cat"], $b["cat"]);
            $v2 = strcmp($a["section"], $b["section"]);
            $v3 = strcmp($a["title"], $b["title"]);
            return ($v1 > 0)?1:(($v1 < 0)?-1:(($v2>0)?1:(($v2<0)?-1:$v3)));
        } else {
            if (array_key_exists("title", $b)) {
                return 1;
            }
	        $v1 = strcmp($a["sn"], $b["sn"]);
            $v2 = strcmp($a["givenname"], $b["givenname"]);
            return ($v1 > 0)?1:(($v1 < 0)?-1:$v2);
        }
    }


function cleanUpLDAPEntries($entries, $sr) {
        for ($eidx = 0; $eidx < $entries["count"]; $eidx++) {
            if (in_array("objectguid", $entries[$eidx])) {
                for($i = 0; $i < $entries[$eidx]["objectguid"]["count"]; $i++) {
                    $res = $this->GUIDtoStr($entries[$eidx]["objectguid"][$i]);
                    if ($res != null) {
                        $entries[$eidx]["objectguid"][$i]=$res;
                    }
                }
            }
            $objectsid_binary = ldap_get_values_len($this->ldapconn, ldap_first_entry($this->ldapconn, $sr), "objectsid");
            for ($i = 0; $i < $objectsid_binary["count"]; $i++) {
                $entries[$eidx]["objectsid"][$i] = $this->bin_to_str_sid($objectsid_binary[$i]);
            }
        }
        $result = array();
        $result["dn"] = $entries[0]["dn"];
        for ($i=0; $i < $entries[0]["count"]; $i++) {
            $key = $entries[0][$i];
            $val = $entries[0][$key];
            if ($val["count"] > 1 || $key == "memberof") {
                $thisval = array();
                for ($j=0; $j < $val["count"]; $j++) {
                    $thisval[] = $val[$j];
                }
                sort($thisval);
                $result[$key] = $thisval;
            } else {
                $result[$key] = $val[0];
            }
        }
        ksort($result);
        $glist=array();
        foreach ($result["memberof"] as $gdn) {
            $g = array();
            $g["dn"] = $gdn;
            $r = $this->parseGroupDN($gdn);
            if ($r != null) {
                $g["cat"] = $r["cat"];
                $g["title"] = $r["title"];
                $g["section"] = $r["section"];
            }
            $glist[] = $g;
        }
        usort($glist, array("ldapinfo", "cmpGrp"));
        $result["groups"] = $glist;
        return $result;
    }
    /*
     * Get info about a user, including group membership.
     * We use a provided list of fields to select what to
     * extract, and replace the complex LDAP entry with a simpler
     * dictionary (array). Group info is parsed and sorted into
     * a secondary value "groups".
     */
    function getUserInfo($username, $fields) {
        $filter="(samAccountName=".$username.")";
        $sr=ldap_search($this->ldapconn, $this->ldapBaseDN, $filter, $fields);
        if (!$sr) return null;
        $entries = ldap_get_entries($this->ldapconn, $sr);

        return $this->cleanUpLDAPEntries($entries, $sr);
    }

    /*
     * Get information about a specific group, including basic info, and which groups
     * of which it is a member.
     */
    function getGroupInfo($dn, $fields) {
        $filter="(objectclass=group)";
        $sr=ldap_read($this->ldapconn, $dn, $filter, $fields);
        if (!$sr) {			   
            print "Error ".ldap_error($this->ldapconn).", base DN=".$dn.", filter=".$filter."\n";
            ldap_get_option($this->ldapconn, LDAP_OPT_DIAGNOSTIC_MESSAGE, $err);
            print "Detailed error: '".$err."'\n";	       
            
            return null;
	      }
        $entries = ldap_get_entries($this->ldapconn, $sr);
        return $this->cleanUpLDAPEntries($entries, $sr);
    }

    /*
     * Get a list of users, by searching for a key.
     */
    function getUsers($searchterm) {
        $filter = "(&(objectclass=person)(displayname=*".$searchterm."*))";
        $baseDN = $this->ldapUserBaseDN;
        $sr = ldap_search($this->ldapconn, $baseDN, $filter, array("DN","displayName","sn","givenName","samaccountname","title","department"));
        if (!$sr) return null;
        $entries = ldap_get_entries($this->ldapconn, $sr);
        $users = array();
        for ($eidx = 0; $eidx < $entries["count"]; $eidx++) {
            $u["dn"] = $entries[$eidx]["dn"];
            $u["displayname"] = $entries[$eidx]["displayname"][0];
            $u["givenname"] = $entries[$eidx]["sn"][0];
            $u["sn"] = $entries[$eidx]["sn"][0];
            $u["uid"] = $entries[$eidx]["samaccountname"][0];
            $u["title"] = $entries[$eidx]["title"][0];
            $u["department"] = $entries[$eidx]["department"][0];
            $users[] = $u;
        }
        usort($users, array("ldapinfo", "cmpUser"));
        return $users;
    }

    /*
     * Get a list of groups using a given Base DN and filter.
     * Parse the group info according to a list of rules.
     */
    function getGroups($searchterm = null) {
        $filter = "(&(objectclass=group)".$this->ldapGroupMask.")";
        $baseDN = $this->ldapGroupBaseDN;
        if ($searchterm) {
            $filter = "(&(objectclass=group)(cn=*$searchterm*))";
            $baseDN = $this->ldapBaseDN;
        }
        $sr = ldap_search($this->ldapconn, $baseDN, $filter, array("DN","CN"));
        if (!$sr) return null;
        $entries = ldap_get_entries($this->ldapconn, $sr);
        $groups = array();
        for ($eidx = 0; $eidx < $entries["count"]; $eidx++) {
            $g["dn"] = $entries[$eidx]["dn"];
            $g["cn"] = $entries[$eidx]["cn"][0];
            $groups[] = $g;
        }
        $r = array();
        foreach ($groups as $group) {
            $vals = $this->parseGroupDN($group["dn"]);
            if ($vals != null) {
                $group["cat"]=$vals["cat"];
                $group["title"]=$vals["title"];
                $group["section"]=$vals["section"];
            }
            $r[] = $group;
        }
        usort($r, array("ldapinfo", "cmpGrp"));
        return $r;
    }

    /*
     * Recursively build a list of group members, including both groups and people.
     * Groups are parsed according to the given list of rules.
     */
    function getGroupMembers($dn) {
        $filter = "(memberOf=$dn)";
        $members=array();
        $sr = ldap_search($this->ldapconn, $this->ldapBaseDN, $filter, array("DN","objectclass","displayname","samAccountName","cn","sn","givenname"), 0, 0, 0, LDAP_DEREF_NEVER);
        if (!$sr) {			   
/*            print "Error ".ldap_error($this->ldapconn).", base DN=".$this->ldapBaseDN.", filter=".$filter."\n";
              ldap_get_option($this->ldapconn, LDAP_OPT_DIAGNOSTIC_MESSAGE, $err);
              print "Detailed error: '".$err."'\n";
*/
            return null;
	      }
        $entries = ldap_get_entries($this->ldapconn, $sr);
        for ($eidx = 0; $eidx < $entries["count"]; $eidx++) {
            // print $entries[$eidx]["dn"]."\n";
            if (in_array("group", $entries[$eidx]["objectclass"])) {
                $moremembers = $this->getGroupMembers($entries[$eidx]["dn"]);
                if (is_array($moremembers)) {
                    $members[$entries[$eidx]["cn"][0]]["dn"] = $entries[$eidx]["dn"];
                    $members[$entries[$eidx]["cn"][0]]["members"] = $moremembers;
                    $r = $this->parseGroupDN($entries[$eidx]["dn"]);
                    if ($r != null) {
                        $members[$entries[$eidx]["cn"][0]]["cat"] = $r["cat"];
                        $members[$entries[$eidx]["cn"][0]]["title"] = $r["title"];
                        $members[$entries[$eidx]["cn"][0]]["section"] = $r["section"];
                    }
                }
            } else {
                $members[$entries[$eidx]["samaccountname"][0]]["dn"] = $entries[$eidx]["dn"];
                $members[$entries[$eidx]["samaccountname"][0]]["displayname"] = $entries[$eidx]["displayname"][0];
                $members[$entries[$eidx]["samaccountname"][0]]["sn"] = $entries[$eidx]["sn"][0];
                $members[$entries[$eidx]["samaccountname"][0]]["givenname"] = $entries[$eidx]["givenname"][0];
            }
        }
        //$members=array_unique($members);
        uasort($members, array("ldapinfo", "cmpGrpMemb"));
        return $members;
    }
}
?>
