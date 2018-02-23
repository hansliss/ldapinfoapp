<html>
  <head>
    <title>LDAP Browser</title>
    <link rel="stylesheet" href="css/styles.css" />
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
     <script src="js/spin.js"></script>
     <script src="js/searchscripts.js"></script>
  </head>
  <body>
    <h1>LDAP browser</h1>
    <ul id="spinhere">
      <li><a href="user.php">Start with yourself</a></li>
     <li>Search for a user: <input type="text" name="username" onkeyup="doDelayedSearch('user', 'userbox', this)"  autocomplete="off" />
<br/><div id="userbox"></div></li>
     <li>Search for a group: <input type="text" name="groupDN" onkeyup="doDelayedSearch('group', 'groupbox', this)"  autocomplete="off" />
<br/><div id="groupbox"></div></li>
    </ul>
  </body>
</html>
     