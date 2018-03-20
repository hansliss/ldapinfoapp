<?php
require("header.inc");
require("footer.inc");
$h = new header("Group", array("js/groupscripts.js", "jstree/jstree.js"), array("jstree/themes/default/style.min.css"));
$h->emit();
?>
<script>
$(document).ready(function(){
    loadGroup("<?php print preg_replace('[\'"<>]', '', $_REQUEST['dn']); ?>");
    loadGroupMembers("<?php print preg_replace('[\'"<>]', '', $_REQUEST['dn']); ?>");
});
</script>
<p class="backlink"><a href="index.php">Back to start</a></p>
<div id="groupinfo"></div>
<h2 id="spinhere">Members</h2>
<div id="groupmembers"></div>
<?php
    $f = new footer();
$f->emit();
?>
