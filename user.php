<?php
if (array_key_exists('username', $_REQUEST)) {
    $username = preg_replace('[^a-z0-9-_]', '', $_REQUEST['username']);
} else {
    $username = preg_replace("/@user.uu.se/", "", $_SERVER['REMOTE_USER']);
}
?>
<html>
<head>
<title>LDAP browser - User</title>
<link rel="stylesheet" href="css/styles.css">
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
<script src="js/userscripts.js"></script>
<script>
$(document).ready(function(){
    loadUser("<?php print $username; ?>");
});
</script>
</head>
<body>
<h1>User info</h1>
<p><a href="index.php">Back to start</a></p>
<div id="userinfo"/>
</body>
</html>
