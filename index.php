<?php
require("header.inc");
require("footer.inc");
$h = new header(null, array("js/searchscripts.js"));
$h->emit();
?>
<table id="spinhere">
    <tr><td colspan="2"><a href="user.php">Start with yourself</a></td></tr>
    <tr><td>Search for a user</td><td><input type="text" name="username" onkeyup="doDelayedSearch('user', 'userbox', this)"  autocomplete="off" />
    <br/><div id="userbox"></div></td></tr>
    <tr><td>Search for a group</td><td><input type="text" name="groupDN" onkeyup="doDelayedSearch('group', 'groupbox', this)"  autocomplete="off" />
    <br/><div id="groupbox"></div></td></tr>
</table>
<?php
    $f = new footer();
$f->emit();
?>
     