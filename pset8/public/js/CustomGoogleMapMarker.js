function CustomMarker(latlng, map, args) {
	this.latlng = latlng;	
	this.args = args;	
	this.setMap(map);	
}

CustomMarker.prototype = new google.maps.OverlayView();

CustomMarker.prototype.draw = function() {
	
	var self = this;
	
	var div = this.div;
	
	if (!div) {
	
		div = this.div = document.createElement('div');
		
		div.className = 'marker';
		
		div.style.position = 'absolute';
		
		if (typeof(self.args.fragment) !== 'undefined') {
			div.appendChild(self.args.fragment);
		} else {
    		div.style.cursor = 'pointer';
    		div.style.width = '20px';
    		div.style.height = '20px';
    		div.style.background = 'blue';
		}
		
		if (typeof(self.args.marker_id) !== 'undefined') {
			div.dataset.marker_id = self.args.marker_id;
		}
		
		google.maps.event.addDomListener(div, "click", function(event) {
			//alert('You clicked on a custom marker!');			
			//google.maps.event.trigger(self, "click");
		});
		
		var panes = this.getPanes();
		panes.overlayImage.appendChild(div);
	}
	
	var point = this.getProjection().fromLatLngToDivPixel(this.latlng);
	
	if (point) {
	    var dispX = typeof(self.args.displacement) !== 'undefined' ? self.args.displacement.x : 0;
	    var dispY = typeof(self.args.displacement) !== 'undefined' ? self.args.displacement.y : 0;
		div.style.left = (point.x + dispX) + 'px';
		div.style.top = (point.y + dispY) + 'px';
	}
};

CustomMarker.prototype.remove = function() {
	if (this.div) {
		this.div.parentNode.removeChild(this.div);
		this.div = null;
	}	
};

CustomMarker.prototype.getPosition = function() {
	return this.latlng;	
};