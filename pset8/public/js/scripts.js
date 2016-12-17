/* global google */
/* global _ */
/**
 * scripts.js
 *
 * Computer Science 50
 * Problem Set 8
 *
 * Global JavaScript.
 */

// Google Map
var map;

// markers for map
var markers = [];
var overlays = [];

// info window
var info = new google.maps.InfoWindow();

// execute when the DOM is fully loaded
$(function() {

    // styles for map
    // https://developers.google.com/maps/documentation/javascript/styling
    var styles = [

        // hide Google's labels
        {
            featureType: "all",
            elementType: "labels",
            stylers: [
                {visibility: "off"}
            ]
        },

        // hide roads
        {
            featureType: "road",
            elementType: "geometry",
            stylers: [
                {visibility: "off"}
            ]
        }

    ];

    // options for map
    // https://developers.google.com/maps/documentation/javascript/reference#MapOptions
    var options = {
        center: {lat: 37.4236, lng: -122.1619}, // Stanford, California
        disableDefaultUI: true,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        maxZoom: 14,
        panControl: true,
        styles: styles,
        zoom: 13,
        zoomControl: true
    };

    // get DOM node in which map will be instantiated
    var canvas = $("#map-canvas").get(0);

    // instantiate map
    map = new google.maps.Map(canvas, options);

    // configure UI once Google Map is idle (i.e., loaded)
    google.maps.event.addListenerOnce(map, "idle", configure);

});

/**
 * Adds marker for place to map.
 */
function addMarker(place)
{
    var myLatLng = new google.maps.LatLng(Number(place.latitude), Number(place.longitude));
    
    var marker = new google.maps.Marker({
        position: myLatLng,
        map: map,
        title: place.place_name,
        label: place.place_name
    });
    
    markers.push(marker);
    marker.setIcon({ 
        url: "/img/information.png" ,
        origin: new google.maps.Point(0, 0),
        anchor: new google.maps.Point(16, 37),
        labelOrigin: new google.maps.Point(15, 43)
    });
    
    
    $.ajax(
        "https://ide50-andriydoroshenko.cs50.io/articles.php",
        {
            data: { geo: place.place_name + " " + place.admin_name1 },
            dataType: 'json'
        }
    )
    .done(function(data){
        var content = data.map(function(item){
            return (_.template("<li><a href=\"<%- link %>\" target=\"_blank\"><%- title %></a></li>"))(item);
        }).join("");
        if (data.length > 0) {
            marker.addListener('click', function(){
                showInfo(marker, "<ul>" + content + "</ul>");
            });
        }
    });
    
    $.ajax(
        "https://ide50-andriydoroshenko.cs50.io/weather.php",
        {
            data: { 
                lat: place.latitude,
                lon: place.longitude
            },
            dataType: 'json'
        }
    )
    .done(function(data){
        var fragment = document.createDocumentFragment();
        var elem = document.createElement('ul');
        elem.style.listStyleType = "none";
        elem.style.padding = "0";
        elem.style.margin = "0";
        data.body.weather.forEach(function(weather) {
            /**
             *  http://humaan.com/custom-html-markers-google-maps/
             **/
            var markerUrl = "http://openweathermap.org/img/w/" + weather.icon + ".png"
            var li = document.createElement('li');
            var anc = document.createElement('a');
            anc.href = "http://forecast.weather.gov/MapClick.php?lat=" + data.body.coord.lat + "&lon=" + data.body.coord.lon;
            anc.target = "_blank";
            var img = document.createElement('img');
            img.src = markerUrl;
            img.alt = weather.description;
            anc.appendChild(img);
            li.appendChild(anc);
            elem.appendChild(li);
        })
        fragment.appendChild(elem);

        var overlay = new CustomMarker(
    		myLatLng, 
    		map,
    		{
    		    displacement: { x: -25, y: 0},
    		    fragment: fragment
    		}
    	);
        overlays.push(overlay);

    });
}

/**
 * Configures application.
 */
function configure()
{
    // update UI after map has been dragged
    google.maps.event.addListener(map, "dragend", function() {
        update();
    });

    // update UI after zoom level changes
    google.maps.event.addListener(map, "zoom_changed", function() {
        update();
    });

    // remove markers whilst dragging
    google.maps.event.addListener(map, "dragstart", function() {
        removeMarkers();
    });

    // configure typeahead
    // https://github.com/twitter/typeahead.js/blob/master/doc/jquery_typeahead.md
    $("#q").typeahead({
        autoselect: true,
        highlight: true,
        minLength: 1
    },
    {
        source: search,
        templates: {
            empty: "no places found yet",
            suggestion: _.template("<p><%- place_name %>, <%- admin_name1 %>, <%- postal_code %></p>")
        }
    });

    // re-center map after place is selected from drop-down
    $("#q").on("typeahead:selected", function(eventObject, suggestion, name) {

        // ensure coordinates are numbers
        var latitude = (_.isNumber(suggestion.latitude)) ? suggestion.latitude : parseFloat(suggestion.latitude);
        var longitude = (_.isNumber(suggestion.longitude)) ? suggestion.longitude : parseFloat(suggestion.longitude);

        // set map's center
        map.setCenter({lat: latitude, lng: longitude});

        // update UI
        update();
    });

    // hide info window when text box has focus
    $("#q").focus(function(eventData) {
        hideInfo();
    });

    // re-enable ctrl- and right-clicking (and thus Inspect Element) on Google Map
    // https://chrome.google.com/webstore/detail/allow-right-click/hompjdfbfmmmgflfjdlnkohcplmboaeo?hl=en
    document.addEventListener("contextmenu", function(event) {
        event.returnValue = true; 
        event.stopPropagation && event.stopPropagation(); 
        event.cancelBubble && event.cancelBubble();
    }, true);

    // update UI
    update();

    // give focus to text box
    $("#q").focus();
}

/**
 * Hides info window.
 */
function hideInfo()
{
    info.close();
}

/**
 * Removes markers from map.
 */
function removeMarkers()
{
    markers.forEach(function(marker){ marker.setMap(null); });
    markers.length = 0;
    overlays.forEach(function(overlay){ overlay.setMap(null); });
    overlays.length = 0;
}

/**
 * Searches database for typeahead's suggestions.
 */
function search(query, cb)
{
    // get places matching query (asynchronously)
    var parameters = {
        geo: query
    };
    $.getJSON("search.php", parameters)
    .done(function(data, textStatus, jqXHR) {

        // call typeahead's callback with search results (i.e., places)
        cb(data);
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        // log error to browser's console
        console.log(errorThrown.toString());
    });
}

/**
 * Shows info window at marker with content.
 */
function showInfo(marker, content)
{
    // start div
    var div = "<div id='info'>";
    if (typeof(content) === "undefined")
    {
        // http://www.ajaxload.info/
        div += "<img alt='loading' src='img/ajax-loader.gif'/>";
    }
    else
    {
        div += content;
    }

    // end div
    div += "</div>";

    // set info window's content
    info.setContent(div);

    // open info window (if not already open)
    info.open(map, marker);
}

/**
 * Updates UI's markers.
 */
function update() 
{
    // get map's bounds
    var bounds = map.getBounds();
    var ne = bounds.getNorthEast();
    var sw = bounds.getSouthWest();

    // get places within bounds (asynchronously)
    var parameters = {
        ne: ne.lat() + "," + ne.lng(),
        q: $("#q").val(),
        sw: sw.lat() + "," + sw.lng()
    };
    $.getJSON("update.php", parameters)
    .done(function(data, textStatus, jqXHR) {

        // remove old markers from map
        removeMarkers();

        // add new markers to map
        for (var i = 0; i < data.length; i++)
        {
            addMarker(data[i]);
        }
     })
     .fail(function(jqXHR, textStatus, errorThrown) {

         // log error to browser's console
         console.log(errorThrown.toString());
     });
}