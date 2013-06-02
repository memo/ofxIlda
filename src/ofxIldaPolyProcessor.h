//
//  ofxIldaPolyProcessor.h
//  ofxILDA demo
//
//  Created by Memo Akten on 21/05/2013.
//
//

#pragma once

#include "ofxIldaPoly.h"

namespace ofxIlda {
    class PolyProcessor {
    public:
        struct {
            int smoothAmount;   // how much to smooth the path (zero to ignore)
            float optimizeTolerance;    // howmuch to optimize the path, based on curvature (zero to ignore)
            bool collapse;  // (not implemented yet)
            int targetPointCount;   // how many points in total should ALL paths in this frame be resampled to (zero to ignore)
            float spacing;  // desired spacing between points. Set automatically by targetPointCount, or set manually. (zero to ignore)
        } params;
        
        
        //--------------------------------------------------------------
        PolyProcessor() {
            memset(&params, 0, sizeof(params));
            params.smoothAmount = 0;
            params.optimizeTolerance = 0;
            params.collapse = 0;
            params.targetPointCount = 500;
            params.spacing = 0;
        }
        
        //--------------------------------------------------------------
        string getString() {
            stringstream s;
            s << "polyProcessor.params:" << endl;
            s << "smoothAmount : " << params.smoothAmount << endl;
            s << "optimizeTolerance : " << params.optimizeTolerance << endl;
            s << "collapse : " << params.collapse << endl;
            s << "targetPointCount : " << params.targetPointCount << endl;
            s << "spacing : " << params.spacing << endl;
            return s.str();
        }

        //--------------------------------------------------------------
        void update(const vector<Poly> &origPolys, vector<Poly> &processedPolys) {
            float totalLength = 0;
            vector<int> pathLengths;
            processedPolys = origPolys;
            for(int i=0; i<processedPolys.size(); i++) {
                if(processedPolys[i].size()) {
                    // smooth paths
                    if(params.smoothAmount > 0) processedPolys[i].setFromPolyline(processedPolys[i].getSmoothed(params.smoothAmount));
                    
                    // optimize paths
                    if(params.optimizeTolerance > 0) processedPolys[i].simplify(params.optimizeTolerance);
                    
                    // calculate total length (needed for auto spacing calculation)
                    if(params.targetPointCount > 0) {
                        float l = processedPolys[i].getPerimeter();
                        totalLength += l;
                        pathLengths.push_back(l);
                    }
                } else {
                    pathLengths.push_back(0);
                }
            }
            
            
            // calculate spacing based on desired total number of points
            if(params.targetPointCount > 0 && totalLength > 0) {
                params.spacing = totalLength / params.targetPointCount;
            }
            
            
            // resample paths based on spacing (either as calculated by targetPointCount, or set by user)
            if(params.spacing) {
                for(int i=0; i<processedPolys.size(); i++) {
                    processedPolys[i].setFromPolyline(processedPolys[i].getResampledBySpacing(params.spacing));
                }
            }
        }
    };
}