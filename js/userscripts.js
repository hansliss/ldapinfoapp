    function loadUser(username) {
        $.get("adinfo.php/user/" + username, function(data, status){
	    if (data.error) {
		$("#userinfo").html('<h3>Error</h3><p>' + data.error + '</p>');
	    } else {
		str = '<h2>' + data.givenname + ' ' + data.sn + '</h2>';
		str += '<p><em>' + username + '</em></p>';
		str += '<h3>' + data.title + '</h3>';
		str += '<p>Mail: <a href="mailto:' + data.mail + '">' + data.mail + '</a></p>';
		if (data.telephonenumber != null) {
                    str += '<p>Phone: <a href="tel:' + data.telephonenumber + '">' + data.telephonenumber + '</a></p>';
		}
		if (data.mobile != null) {
                    str += '<p>Mobile phone: <a href="tel:' + data.mobile + '">' + data.mobile + '</a></p>';
		}
		if (data.othertelephone != null) {
                    str += '<p>Other phone: <a href="tel:' + data.othertelephone + '">' + data.othertelephone + '</a></p>';
		}
		str += '<h3>Is a member of these groups</h3><div class="groups"><ul>';
		data.groups.forEach(function(item, index) {
                    str += '<li><a href="group.php?dn=' + encodeURIComponent(item.dn) + '">' + item.title + ' (' + item.section + ')</a></li>';
		});
		str += '</ul></div>';
		str += '<h3>Technical stuff</h3><table class="tech">';
		str += '<th>DN</th><td>' + data.dn + '</td></tr>';
		str += '<tr><th>SID</th><td>' + data.objectsid + '</td></tr>';
		str += '<th>GUID</th><td>' + data.objectguid + '</td></tr>';
		str += '</table>';
		$("#userinfo").html(str);
	    }
        }).fail(function() {
	    $("#userinfo").html('<h3>Error contacting directory server</h3>');
        });
    }
