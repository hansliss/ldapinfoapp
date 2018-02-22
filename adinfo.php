<?php
require('ldapinfo.inc');

$foo = new ldapinfo('adinfo.ini');
$method = $_SERVER['REQUEST_METHOD'];
$request = explode('/', trim($_SERVER['PATH_INFO'],'/'));
$input = json_decode(file_get_contents('php://input'),true);
header('Content-Type: application/json');
$type = preg_replace('/[^a-z0-9_]+/i','',array_shift($request));
$key = preg_replace('/[^a-z0-9_,=-]+/i','',array_shift($request));
if ($type == 'user') {
   if ($key == null) {
     $key=str_replace("@user.uu.se", "", $_SERVER["REMOTE_USER"]);
   }
   $attrs = $foo->getUserInfo($key, array("objectguid","objectsid","givenName","sn","mail","telephoneNumber","mobile","otherTelephone","memberOf","title","description"));
   print json_encode($attrs, JSON_PRETTY_PRINT);
} else if ($type == "group") {
  $m = $foo->getGroupMembers($key);
  print json_encode($m, JSON_NUMERIC_CHECK | JSON_PRETTY_PRINT);
} else if ($type == "groups") {
  $m = $foo->getGroups();
  print json_encode($m, JSON_NUMERIC_CHECK | JSON_PRETTY_PRINT);
}
?>
