<?php
require('ldapinfo.inc');

$foo = new ldapinfo('adinfo.ini');
$method = $_SERVER['REQUEST_METHOD'];
$request = explode('/', trim($_SERVER['PATH_INFO'],'/'));
$input = json_decode(file_get_contents('php://input'),true);
header('Content-Type: application/json');
$type = preg_replace('/[^a-z0-9_]/i','',array_shift($request));
$key = array_shift($request);
$m = null;
if ($type == 'user') {
    if ($key == null) {
        $key=str_replace("@user.uu.se", "", $_SERVER["REMOTE_USER"]);
    }
    $attrs = $foo->getUserInfo($key, array("objectguid","objectsid","givenName","sn","mail","telephoneNumber","mobile","otherTelephone","memberOf","title","department","description","wwwhomepage"));
    print json_encode($attrs, JSON_PRETTY_PRINT);
} else if ($type == "group") {
    $m = $foo->getGroupInfo($key, array("objectguid","objectsid","cn","memberof","name","gidnumber"));
} else if ($type == "groupmembers") {
    $m = $foo->getGroupMembers($key);
} else if ($type == "groups") {
    $m = $foo->getGroups();
} else if ($type == "usersearch") {
    $m = $foo->getUsers($key);
} else if ($type == "groupsearch") {
    $m = $foo->getGroups($key);
}
if ($m !== null) print json_encode($m, JSON_NUMERIC_CHECK | JSON_PRETTY_PRINT);
?>
