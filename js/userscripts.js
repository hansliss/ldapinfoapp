    function loadUser(username) {
        $.get("adinfo.php/user/" + username, function(data, status){
            $("#userinfo").append('<h2>' + data.givenname + ' ' + data.sn + '</h2>');
            $("#userinfo").append('<h3>' + data.title + '</h3>');
            $("#userinfo").append('<p>Mail: <a href="mailto:' + data.mail + '">' + data.mail + '</a></p>');
            if (data.telephonenumber != null) {
                $("#userinfo").append('<p>Phone: <a href="tel:' + data.telephonenumber + '">' + data.telephonenumber + '</a></p>');
            }
            if (data.mobile != null) {
                $("#userinfo").append('<p>Mobile phone: <a href="tel:' + data.mobile + '">' + data.mobile + '</a></p>');
            }
            if (data.othertelephone != null) {
                $("#userinfo").append('<p>Other phone: <a href="tel:' + data.othertelephone + '">' + data.othertelephone + '</a></p>');
            }
            str='<div class="groups"><ul>';
            data.groups.forEach(function(item, index) {
                str += '<li><a href="group.php?dn=' + encodeURIComponent(item.dn) + '">' + item.title + ' (' + item.section + ')</a></li>';
            });
            $("#userinfo").append(str + '</ul></div>');
            str = '<table class="tech">';
            str += '<th>DN</th><td>' + data.dn + '</td></tr>';
            str += '<tr><th>SID</th><td>' + data.objectsid + '</td></tr>';
            str += '<th>GUID</th><td>' + data.objectguid + '</td></tr>';
            $("#userinfo").append(str + '</table>');
        });
    }
