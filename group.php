<html>
<head>
<title>LDAP browser - Group</title>
<link rel="stylesheet" href="jstree/themes/default/style.min.css" />
<link rel="stylesheet" href="css/styles.css">
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
<script src="js/groupscripts.js"></script>
<script src="js/spin.js"></script>
<script src="jstree/jstree.js"></script>
<script>
$(document).ready(function(){
    loadGroup("<?php print preg_replace('[\'"<>]', '', $_REQUEST['dn']); ?>");
    loadGroupMembers("<?php print preg_replace('[\'"<>]', '', $_REQUEST['dn']); ?>");
});
</script>
</head>
<body>
<h1>Group info</h1>
<p><a href="index.php">Back to start</a></p>
<div id="groupinfo"></div>
<h2 id="spinhere">Members</h2>
<div id="groupmembers"></div>
</body>
</html>
