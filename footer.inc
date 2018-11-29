<?php
class footer {
      public function __construct() {
      }

      public function emit() {
         echo "</div></div>";
         echo "<div class=\"footer-element\">";
         echo "<div class=\"footer-site-information\"><div class=\"row\">";
         echo "<span class=\"footer-site-entry\">© Uppsala universitet";
         echo "<span class=\"separator\"></span></span>";
         echo "<span class=\"footer-site-entry\">";
         echo "Telefon: <span class=\"tel\">018-471 00 00</span>";
         echo "<span class=\"separator\"></span></span>";
         echo "<span class=\"footer-site-entry\">";
         echo "Box 256, 751 05 Uppsala";
         echo "</span></div><div class=\"row\">";
         echo "<span class=\"footer-site-entry\">";
         echo "Organisationsnummer: 202100-2932";
         echo "<span class=\"separator\"></span>";
         echo "</span><span class=\"footer-site-entry\">";
         echo "Momsregistreringsnummer: SE202100293201";
         echo "<span class=\"separator\"></span></span>";
         echo "<span class=\"footer-site-entry\">";
         echo "<a href=\"https://www.uu.se/om-uu/kontakta-oss/registrator\">";
         echo "Registrator</a><span class=\"separator\"></span></span>";
         echo "<span class=\"footer-site-entry\">";
         echo "<a href=\"https://www.uu.se/om-webbplatsen\">Om sidan</a>";
         echo "<span class=\"separator\"></span></span>";
         echo "<span class=\"footer-site-entry\">";
         echo "Sidansvarig:&nbsp;<a href=\"https://katalog.uu.se/empinfo?id=N95-14\">Hans Liss</a>";
         echo "</span></div></div>";
         echo "</body></html>";
      }
}
?>
