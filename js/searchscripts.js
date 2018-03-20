var timeout = null;

function doDelayedSearch(type, box, field) {
    if (timeout) {  
	clearTimeout(timeout);
    }
    timeout = setTimeout(function() {
	doSearch(type, box, field);
    }, 1000);
}

function doSearch(type, box, field) {
    if (field.value.length <= 2) {
        mylist = $("#" + box);
        mylist.css('display', 'none');
        return;
    }
    var target = document.getElementById('spinhere');
    var opts = {
	lines: 13, // The number of lines to draw
	length: 38, // The length of each line
	width: 17, // The line thickness
	radius: 45, // The radius of the inner circle
	scale: 1, // Scales overall size of the spinner
	corners: 1, // Corner roundness (0..1)
	color: '#000000', // CSS color or array of colors
	fadeColor: 'transparent', // CSS color or array of colors
	opacity: 0.25, // Opacity of the lines
	rotate: 0, // The rotation offset
	direction: 1, // 1: clockwise, -1: counterclockwise
	speed: 1, // Rounds per second
	trail: 60, // Afterglow percentage
	fps: 20, // Frames per second when using setTimeout() as a fallback in IE 9
	zIndex: 2e9, // The z-index (defaults to 2000000000)
	className: 'spinner', // The CSS class to assign to the spinner
	top: '200px', // Top position relative to parent
	left: '50%', // Left position relative to parent
	shadow: 'none', // Box-shadow for the lines
	position: 'absolute' // Element positioning
    };
    var spinner = new Spinner(opts).spin(target);
    $.get("adinfo.php/"+ type + "search/" + field.value, function(data, status){
	mylist = $("#" + box);
	str = '<table class="searchlist">';
	var dlen = data.length;
	for (var i = 0; i < dlen; i++) {
	    if (type == 'group') {
		str += '<tr><td class="title"><a href="group.php?dn=' + encodeURIComponent(data[i].dn) + '">' + data[i].title + '</a></td><td class="section">' + data[i].section + '</td></tr>';
	    } else if (type == 'user') {
		str += '<tr><td class="title"><a href="user.php?username=' + encodeURIComponent(data[i].uid) + '">' + data[i].displayname + '</a></td><td class="section">' + ((data[i].title==null)?'':data[i].title) + ((data[i].department==null)?'':(', ' + data[i].department)) + '</td></tr>';
	    }
	}
	str += '</table>';
	mylist.html(str);
	mylist.css('display', 'inline-block');
	spinner.stop();
    });
}
