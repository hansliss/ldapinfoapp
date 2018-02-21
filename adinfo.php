<?php

class ldapinfo {
  // LDAP variables
  private $ldaphost;
  private $ldapuser;
  private $ldappwd;
  private $ldapbaseDN;
  private $ldapconn;

  public function __construct($inifile) {
    $ini_array = parse_ini_file($inifile, true);
    $ldapconf = $ini_array["ldap"];
    $this->ldaphost = $ldapconf["url"];
    $this->ldapuser = $ldapconf["user"];
    $this->ldappwd = $ldapconf["password"];
    $this->ldapbaseDN = $ldapconf["baseDN"];
    $this->ldapconn = ldap_connect($this->ldaphost) or die("Could not connect to $ldaphost");
    ldap_set_option($this->ldapconn, LDAP_OPT_PROTOCOL_VERSION, 3);
    ldap_bind($this->ldapconn, $this->ldapuser, $this->ldappwd) or die("Could not bind\n");
  }

  function GUIDtoStr($binary_guid) {
    try {
      $unpacked = unpack('Va/v2b/n2c/Nd', $binary_guid);
      return sprintf('%08X-%04X-%04X-%04X-%04X%08X', $unpacked['a'], $unpacked['b1'], $unpacked['b2'], $unpacked['c1'], $unpacked['c2'], $unpacked['d']);
    } catch (Exception $e) {
      return null;
    }
  }
	
  // Returns the textual SID
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

  // Converts a little-endian hex-number to one, that 'hexdec' can convert
  function little_endian($hex) {
    $result='';
    for ($x = strlen($hex) - 2; $x >= 0; $x = $x - 2) {
      $result .= substr($hex, $x, 2);
    }
    return $result;
  }

  function getldapinfo($username, $fields) {
    $filter="(samAccountName=$username)";
    $sr=ldap_search($this->ldapconn, $this->ldapbaseDN, $filter, $fields);
    if (!$sr) die("Search failed\n");
    $entries = ldap_get_entries($this->ldapconn, $sr);

    foreach (array_keys($entries) as $ekey) {
      if (is_array($entries[$ekey])) {
        if (in_array("objectguid", $entries[$ekey])) {
          for($i = 0; $i < $entries[$ekey]["objectguid"]["count"]; $i++) {
	    $res = $this->GUIDtoStr($entries[$ekey]["objectguid"][$i]);
	    if ($res != null) {
	      $entries[$ekey]["objectguid"][$i]=$res;
            }
          }
        }
        $objectsid_binary = ldap_get_values_len($this->ldapconn, ldap_first_entry($this->ldapconn, $sr), "objectsid");
        for ($i = 0; $i < $objectsid_binary["count"]; $i++) {
          $entries[$ekey]["objectsid"][$i] = $this->bin_to_str_sid($objectsid_binary[$i]);
        }
      }
    }

    return $entries;
  }
}

$foo = new ldapinfo('adinfo.ini');
$entries = $foo->getldapinfo("hansliss", array("objectguid","objectsid"));
echo $entries[0]["objectguid"][0]."\n";
echo $entries[0]["objectsid"][0]."\n";

?>
