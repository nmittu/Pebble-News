var url = "http://api.nytimes.com/svc/mostpopular/v2/mostviewed/all-sections/1/?api-key=588faf2c30818c381c3cac757e7f5a27:13:75013790";

function load() {
    var XHR = new XMLHttpRequest();
    XHR.open("GET", url, false);
    XHR.send();

    var json = JSON.parse(XHR.responseText);

    var max = 10;

    if(json.num_results<10){
        max = json.num_results;
    }

    var dictionary = {
        KEY_NUM: max,
				KEY_TYPE: 0
    };

    for(var i = 0; i<max; i++){
        dictionary[100+i] = json.results[i].title;
    }

    for(i = 0; i<max; i++){
        dictionary[1000+i] = json.results[i].url;
    }
	
    //console.log(json);
	
		Pebble.sendAppMessage(dictionary,
		function(e){
			console.log('Index Sent');
		},
		function(e){
			console.log('Error Sending Weather To Pebble!');
		}
	);
}

function loadFirstParagraph(url) {
    var url_ = "http://api.nytimes.com/svc/search/v2/articlesearch.json?fq=web_url:(\""+url+"\")&api-key=4b97078a7129d8248bdc530ff27b16ee:9:75013790";

    var XHR = new XMLHttpRequest();
    XHR.open("GET", url_, false);
    XHR.send();

    var json = JSON.parse(XHR.responseText);

		var dictionary = {
			KEY_TYPE: 1,
			KEY_CONTENTS: json.response.docs[0].lead_paragraph
		};
	
	
		Pebble.sendAppMessage(dictionary,
		function(e){
			console.log('ART Sent');
		},
		function(e){
			console.log('Error Sending Weather To Pebble!');
		}
	);
    //console.log(json.response.docs[0].lead_paragraph);
}

Pebble.addEventListener('appmessage',
  function(e){
		if(e.payload.KEY_TYPE === 0){
			loadFirstParagraph(e.payload.KEY_URL);
		}
	}
);

Pebble.addEventListener('ready',
    function (e) {
        load();
    }
);