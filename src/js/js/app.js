var url = "http://api.nytimes.com/svc/mostpopular/v2/mostviewed/all-sections/1/?api-key=588faf2c30818c381c3cac757e7f5a27:13:75013790";

function load() {
    var XHR = new XMLHttpRequest();
    XHR.open("GET", url, false);
    XHR.send();

    var json = JSON.parse(XHR.responseText);

    var max = 10;

    if(json.num_results<10){
        var max = json.num_results;
    }

    var dictionary = {
        KEY_NUM: max
    };

    for(var i = 0; i<max; i++){
        dictionary[100+i] = json.results[i].title;
    }

    for(i = 0; i<max; i++){
        dictionary[1000+i] = json.results[i].url;
    }

    loadFirstParagraph(dictionary[1000]);

    //console.log(json);
}

function loadFirstParagraph(url) {
    var url_ = "http://api.nytimes.com/svc/search/v2/articlesearch.json?fq=web_url:(\""+url+"\")&api-key=4b97078a7129d8248bdc530ff27b16ee:9:75013790"

    var XHR = new XMLHttpRequest();
    XHR.open("GET", url_, false);
    XHR.send();

    var json = JSON.parse(XHR.responseText);

    console.log(json.response.docs[0].lead_paragraph);
}


Pebble.addEventListener('ready',
    function (e) {
        load();
    }
);