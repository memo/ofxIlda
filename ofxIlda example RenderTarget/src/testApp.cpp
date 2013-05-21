#include "testApp.h"
#include "ofxSimpleGuiToo.h"
#include "ofxIldaRenderTarget.h"


// VARS
ofxIlda::RenderTarget ildaFbo;
ofxIlda::Frame ildaFrame;

ofVec2f mouseDownPos;   // position of mouse (normalized)
ofVec2f lastMouseDownPos; // last position of mouse (normalized)

// PARAMS
bool doFboClear;
bool doDrawErase;   // whether we are erasing (true) or drawing (erase)
int brushThickness;

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(100);
    
    etherdream.setup();
    etherdream.setPPS(30000);
    
    ildaFbo.setup(256, 256);
    
    gui.addTitle("INPUT");
    gui.addToggle("doFboClear c", doFboClear);
    gui.addToggle("doDrawErase x", doDrawErase);
    gui.addSlider("brushThickness", brushThickness, 1, 50);
    
    gui.addTitle("CV");
    gui.addSlider("cv.blurAmount", ildaFbo.params.cv.blurAmount, 0, 20);
    gui.addSlider("cv.bottomThreshold", ildaFbo.params.cv.bottomThreshold, 0, 20);
    gui.addSlider("cv.thresholdAmount", ildaFbo.params.cv.thresholdAmount, 0, 255);
    gui.addSlider("adaptiveThresholdAmount", ildaFbo.params.cv.adaptiveThresholdAmount, 0, 50);
    gui.addSlider("cv.adaptiveThresholdBlock", ildaFbo.params.cv.adaptiveThresholdBlock, 0, 10);
    gui.addSlider("cv.erodeAmount", ildaFbo.params.cv.erodeAmount, 0, 20);
    gui.addToggle("cv.doCanny", ildaFbo.params.cv.doCanny);
    gui.addSlider("cv.cannyThresh1", ildaFbo.params.cv.cannyThresh1, 0, 10);
    gui.addSlider("cv.cannyThresh2", ildaFbo.params.cv.cannyThresh2, 0, 10);
    gui.addSlider("cv.cannyWindow", ildaFbo.params.cv.cannyWindow, 1, 3);
    gui.addToggle("cv.doFindHoles", ildaFbo.params.cv.doFindHoles);
    
    gui.addTitle("PATH PROCESSING");
    gui.addSlider("path.smoothAmount", ildaFrame.polyProcessor.params.smoothAmount, 0, 100);
    gui.addToggle("path.contourCollapse", ildaFrame.polyProcessor.params.collapse);
    gui.addSlider("path.optimizeTolerance", ildaFrame.polyProcessor.params.optimizeTolerance, 0, 1);
    gui.addSlider("path.targetPointCount", ildaFrame.polyProcessor.params.targetPointCount, 0, 5000);
    gui.addSlider("path.spacing", ildaFrame.polyProcessor.params.spacing, 0, 1);
    gui.addSlider("stats.pointCountOrig", ildaFrame.stats.pointCountOrig, 0, 10000);
    gui.addSlider("stats.pointCountProcessed", ildaFrame.stats.pointCountProcessed, 0, 10000);
    
    gui.addTitle("DISPLAY");
    gui.addToggle("doDrawFbo", ildaFbo.params.draw.fbo);
    gui.addSlider("fboAlpha", ildaFbo.params.draw.fboAlpha, 0, 255);
    gui.addToggle("doDrawLineRaw", ildaFbo.params.draw.linesRaw);
    gui.addToggle("doDrawIldaLines", ildaFrame.params.draw.lines);
    gui.addToggle("doDrawIldaPoints", ildaFrame.params.draw.points);
    gui.addToggle("doDrawIldaPointNumbers", ildaFrame.params.draw.pointNumbers);
    
    gui.addTitle("OUTPUT");
    gui.addColorPicker("color", ildaFrame.params.output.color);
    gui.addSlider("blankCount", ildaFrame.params.output.blankCount, 0, 100);
    gui.addSlider("endCount", ildaFrame.params.output.endCount, 0, 100);
    gui.addToggle("doCapX", ildaFrame.params.output.doCapX);
    gui.addToggle("doCapY", ildaFrame.params.output.doCapY);
    
    gui.addTitle("");
    gui.addContent("fbo", ildaFbo.getFbo());
    gui.addContent("greyImage", ildaFbo.getGreyImage());
    
    gui.setDefaultKeys(true);
    gui.loadFromXML();
    gui.show();
    
    doFboClear = true;
}

//--------------------------------------------------------------
void testApp::update(){
}



//--------------------------------------------------------------
void drawInFbo() {
    ofPushStyle();
    ildaFbo.begin();
    if(doFboClear) {
        doFboClear = false;
        ofClear(0);
    }
    if(ofGetMousePressed() &&  mouseDownPos.x >= 0) {
        ofPushMatrix();
        ofScale(ildaFbo.getWidth(), ildaFbo.getHeight(), 1);
        ofSetColor(doDrawErase ? 0 : 255);
        ofSetLineWidth(brushThickness);
        ofLine(lastMouseDownPos, mouseDownPos);
        ofEllipse(mouseDownPos, brushThickness/2.0f/ildaFbo.getWidth(), brushThickness/2.0f/ildaFbo.getHeight());
        ofPopMatrix();
    }
    
    ildaFbo.end();
    ofPopStyle();
}




//--------------------------------------------------------------
void testApp::draw() {
    // clear the current frame
    ildaFrame.clear();
    
    drawInFbo();    // draw stuff into the ildaRenderTarget
    ildaFbo.update(ildaFrame);  // vectorize and update the ildaFrame
    
    ildaFrame.update();
    
    int dw = ofGetWidth()/2;
    int dh = dw;
    int dx = ofGetWidth() - dw;
    int dy = 0;

    ildaFbo.draw(dx, dy, dw, dh);
    
    ofSetColor(0, 255, 0);
    ildaFrame.draw(dx, dy, dw, dh);
    
    etherdream.setPoints(ildaFrame);
    
    // draw cursor
    ofEnableAlphaBlending();
    ofFill();
    ofSetColor(doDrawErase ? 0 : 255, 128);
    float r = brushThickness/2 * ofGetWidth() /2 / ildaFbo.getWidth();
    ofCircle(ofGetMouseX(), ofGetMouseY(), r);
    ofNoFill();
    ofSetColor(255, 128);
    ofCircle(ofGetMouseX(), ofGetMouseY(), r);
    
    gui.draw();
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key) {
        case 'f': ofToggleFullscreen(); break;
        case 'c': doFboClear ^= true; break;
        case 'x': doDrawErase ^= true; break;
            
        case '=':
        case '+': brushThickness++; break;
            
        case '-': if(brushThickness>1) brushThickness--; break;
        case 't': printf("mouse inside: %i\n", ildaFrame.getPoly(0).inside(mouseDownPos)); break; // test
    }
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    lastMouseDownPos = mouseDownPos;
    mouseDownPos.x = ofMap(x, ofGetWidth()/2, ofGetWidth(), 0, 1);
    mouseDownPos.y = ofMap(y, 0, ofGetWidth()/2, 0, 1);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    mouseDownPos.x = ofMap(x, ofGetWidth()/2, ofGetWidth(), 0, 1);
    mouseDownPos.y = ofMap(y, 0, ofGetWidth()/2, 0, 1);
    lastMouseDownPos = mouseDownPos;
}
