<?php
class header {	
    private $subtitle;
    private $scriptlist;
    private $csslist;		
    public function __construct($subtitle, $scriptlist, $csslist=array()) {
        $this->subtitle = $subtitle;
        $this->scriptlist = $scriptlist;
        array_unshift($this->scriptlist, "js/spin.js", "https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js");
        $this->csslist = $csslist;
        array_unshift($this->csslist, "css/styles.css");
    }
    
    public function emit() {
        $title = "Active Directory LDAP Browser";
        if ($this->subtitle) {
            $title .= " - " . $this->subtitle;
		}		  
        echo "<html><head><title>$title</title>";
        foreach ($this->csslist as $css) {
            echo "<link rel=\"stylesheet\" href=\"$css\" />";
        }
        foreach ($this->scriptlist as $scr) {
            echo "<script src=\"$scr\"></script>";
        }
        echo "</head><body>";
	echo "<div class=\"wrapper\" id=\"spinhere\">";
	echo "<div class=\"sigill\"></div>";
	echo "<a href=\"http://www.uu.se\"  title=\"Uppsala universitets startsida\" class=\"logo-wrap\">";
	echo "<div class=\"container top-container clearfix\"><div class=\"top-stripe\"></div><div class=\"top-content\">";
	echo "<img src=\"images/uu_logo_85.png\" alt=\"Uppsala universitet\" class=\"logo\">";
	echo "</a>";
	echo "<h1 id=\"title\">$title</h1>";
	echo "</div></div></div>";
	echo "<div id=\"contents\">";
    }
}

//http://www.uu.se/digitalAssets/242/c_242915-l_3-k_uu_logo.png
?>