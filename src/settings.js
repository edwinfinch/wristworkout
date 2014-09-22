Pebble.addEventListener("ready",
                        function(e) {
							console.log("connect: " + e.ready);
							console.log(e.type);
                        });

Pebble.addEventListener("appmessage",
                        function(e) {
							console.log(e.payload.message_type);
							console.log("message");
							switch (e.payload.message_type) {
							case 100:
									localStorage.setItem("act_1_n", e.payload.act_1_n);
									localStorage.setItem("act_1_c", e.payload.act_1_c);
									localStorage.setItem("act_1_g", e.payload.act_1_g);
									localStorage.setItem("act_2_n", e.payload.act_2_n);
									localStorage.setItem("act_2_c", e.payload.act_2_c);
									localStorage.setItem("act_2_g", e.payload.act_2_g);
									localStorage.setItem("act_3_n", e.payload.act_3_n);
									localStorage.setItem("act_3_c", e.payload.act_3_c);
									localStorage.setItem("act_3_g", e.payload.act_3_g);
									
									//console.log("act_1_n " + e.payload.act_1_n);
								break;
							}
                        });

var values = 0;
var config_url = "http://www.edwinfinch.com/htn/htn-config-gh-pages?";
var options = JSON.parse(localStorage.getItem('htn-config'));

Pebble.addEventListener("showConfiguration", function(e) {
	if (localStorage.getItem("htn-config") !== null) {
        options = JSON.parse(localStorage.simplycleanops);
    }
		var a_1_n, a_2_n, a_3_n, a_1_c, a_2_c, a_3_c, a_1_g, a_2_g, a_3_g;
		a_1_n = window.localStorage.act_1_n;
		a_2_n = window.localStorage.act_2_n;
		a_3_n = window.localStorage.act_3_n;
		a_1_c = window.localStorage.act_1_c;
		a_2_c = window.localStorage.act_2_c;
		a_3_c = window.localStorage.act_3_c;
		a_1_g = window.localStorage.act_1_g;
		a_2_g = window.localStorage.act_2_g;
		a_3_g = window.localStorage.act_3_g;
		Pebble.openURL(config_url + "a_1_n=" + a_1_n + "&" + 
			"a_1_c=" + a_1_c + "&" + "a_1_g=" + a_1_g + "&" + "a_2_n=" + a_2_n + "&" + 
			"a_2_c=" + a_2_c + "&" + "a_2_g=" + a_2_g + "&" + "a_3_n=" + a_3_n + "&" + 
			"a_3_c=" + a_3_c + "&" + "a_3_g=" + a_3_g);
        console.log(config_url + "a_1_n=" + a_1_n + "&" + 
			"a_1_c=" + a_1_c + "&" + "a_1_g=" + a_1_g + "&" + "a_2_n=" + a_2_n + "&" + 
			"a_2_c=" + a_2_c + "&" + "a_2_g=" + a_2_g + "&" + "a_3_n=" + a_3_n + "&" + 
			"a_3_c=" + a_3_c + "&" + "a_3_g=" + a_3_g);
});

Pebble.addEventListener("webviewclosed", function (e) {
    console.log("Configuration closed");
    console.log("Response = " + e.response.length + "   " + e.response);
    if (e.response) { // user clicked Save/Submit, not Cancel/Done
		console.log("User hit save");
		values = JSON.parse(decodeURIComponent(e.response));
		console.log("stringified options: " + JSON.stringify((values)));
		for(var key in values) {
			localStorage.setItem(key, values[key]);
		}
		Pebble.sendAppMessage(
			values
		);
	}
});