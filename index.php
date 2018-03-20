<?php
require("header.inc");
require("footer.inc");
$h = new header(null, array("js/searchscripts.js"));
$h->emit();
?>
<table>
    <tr><td colspan="2"><a href="user.php">Start with yourself (<?php print $_SERVER["REMOTE_USER"]; ?>)</a></td></tr>
    <tr><td>Look up by Akka ID</td><td><form class="compact" action="user.php" method="GET"><input type="text" name="username"><input type="submit" value="Go"></form></td></tr>
    <tr><td>Search for a user</td><td><input type="text" name="username" onkeyup="doDelayedSearch('user', 'userbox', this)"  autocomplete="off" />
    <br/><div id="userbox"></div></td></tr>
    <tr><td>Search for a group</td><td><input type="text" name="groupDN" onkeyup="doDelayedSearch('group', 'groupbox', this)"  autocomplete="off" />
    <br/><div id="groupbox"></div></td></tr>
</table>
<p class="instructions">You can search on part of a person's full name,
or part of a group's canonical name.<br />The search is performed
automatically after a 1s delay, so don't look for a search button;
just be patient.</p>
<?php
    $f = new footer();
$f->emit();
?>
     