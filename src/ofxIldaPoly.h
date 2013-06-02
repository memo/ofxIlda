//
//  ofxIldaPoly.h
//  interactivelaser
//
//  Created by 武内 満 on 2013/06/02.
//
//

#pragma once

namespace ofxIlda {
    
	class Poly: public ofPolyline{
	public:
		ofFloatColor color;
		Poly() : color(ofFloatColor(1, 1, 1, 1)) {}

        Poly(ofFloatColor color) : color(color) {}
        
        Poly(const Poly& poly) : ofPolyline(poly), color(poly.color) {}
        
        Poly(const ofPolyline& polyline) : ofPolyline(polyline), color(ofFloatColor(1, 1, 1, 1)) {}
        
        Poly(const ofPolyline& polyline, ofFloatColor color) : ofPolyline(polyline), color(color) {}

		Poly(const vector<ofPoint>& verts) : ofPolyline(verts), color(ofFloatColor(1, 1, 1, 1)) {}

		Poly(const vector<ofPoint>& verts, ofFloatColor color) : ofPolyline(verts), color(color) {}
        
        void setFromPolyline(const ofPolyline& polyline) { ofFloatColor tmpColor = color; *this = polyline; color = tmpColor; }
	};
}