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
		Poly(){clear();};
		Poly(const vector<ofPoint>& verts){
			clear();
			addVertices(verts);
		};
		//----------------------------------------------------------
		static Poly fromRectangle(const ofRectangle& rect) {
			Poly polyline;
			polyline.addVertex(rect.getMin());
			polyline.addVertex(rect.getMaxX(),rect.getMinY());
			polyline.addVertex(rect.getMax());
			polyline.addVertex(rect.getMinX(),rect.getMaxY());
			polyline.close();
			return polyline;
		};
	};
}