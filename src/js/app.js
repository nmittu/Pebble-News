var mostPopularKey = null;
var articleSearchKey = null;

var url = "http://api.nytimes.com/svc/mostpopular/v2/mostviewed/all-sections/1/?api-key=";

function load() {
	
	if(mostPopularKey === null || articleSearchKey === null){
		var dictionary = {
				KEY_TYPE: 2
    };
		
		Pebble.sendAppMessage(dictionary,
		function(e){
			console.log('Index Sent');
		},
		function(e){
			console.log(dumpObject(e));
		}
	);
		return;
	}
	
		//console.log(mostPopularKey === null);
    var XHR = new XMLHttpRequest();
    XHR.open("GET", url+mostPopularKey, false);
    XHR.send();

		console.log(XHR.responseText);
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
    var url_ = "http://api.nytimes.com/svc/search/v2/articlesearch.json?fq=web_url:(\""+url+"\")&api-key="+articleSearchKey;

    var XHR = new XMLHttpRequest();
    XHR.open("GET", encodeURI(url_), false);
    XHR.send();

    var json = JSON.parse(XHR.responseText);

		var dictionary = {
			KEY_TYPE: 1,
			KEY_CONTENTS: json.response.docs[0].lead_paragraph,
			KEY_AUTHOR: json.response.docs[0].byline.original,
			KEY_TITLE: json.response.docs[0].headline.main
		};
	
	
		Pebble.sendAppMessage(dictionary,
		function(e){
			console.log('ART Sent');
		},
		function(e){
			console.log('Error Sending Weather To Pebble');
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
			if(localStorage.getItem("mostPopular") && localStorage.getItem("articleSearch")){
				mostPopularKey = decodeURIComponent(localStorage.getItem("mostPopular"));
				articleSearchKey = decodeURIComponent(localStorage.getItem("articleSearch"));
			}
			
			
        load();
    }
);

Pebble.addEventListener('showConfiguration', function(e) {
  // Show config page
  Pebble.openURL('http://pebbleconf-remote.mittudev.com/PebbleNews.html');
});

Pebble.addEventListener('webviewclosed', function(e) {
  // Decode and parse config data as JSON
	console.log("Config Set");
	
  var config_data = JSON.parse(decodeURIComponent(e.response));

  localStorage.setItem("mostPopular", encodeURIComponent(config_data.mostPopular));
	localStorage.setItem("articleSearch", encodeURIComponent(config_data.articleSearch));
	
	
	mostPopularKey = config_data.mostPopular;
	articleSearchKey = config_data.articleSearch;
	load();
});