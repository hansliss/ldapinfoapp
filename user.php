<?php
require("header.inc");
require("footer.inc");
if (array_key_exists('username', $_REQUEST)) {
    $username = preg_replace('[^a-z0-9-_]', '', $_REQUEST['username']);
} else {
    $username = preg_replace("/@user.uu.se/", "", $_SERVER['REMOTE_USER']);
}
$h = new header("User", array("js/userscripts.js"));
$h->emit();
?>
<script>
$(document).ready(function(){
    loadUser("<?php print $username; ?>");
});
</script>
<p class="backlink"><a href="index.php">Back to start</a></p>
<div id="userinfo"/>
<?php
    $f = new footer();
$f->emit();
?>
