#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // setup the mesh
    urg.setupPointMesh();
    
    // set the parameters of the scan
    urg.setMeshParameters(800, 0, 682, 1000.);
    
//    urg.loadData("doherty", 0);
//    urg.fillPointMeshTXY(0, 5000, false);
    
    // try: 15 (/1) (creek), 6 (/8) hi res bamboo forest
    urg.loadData("spherical", 6);
    urg.fillPointMeshTXYSpherical(225./64. / 8., 180., false, 0., 1., -3.8);
    
    // Entrances
//    urg.loadData("entrances", 3); // should be 4
//    urg.fillPointMeshTXYGhostedDouble(-1200, 1075, 100, 2120, .25, false);
//    urg.findPeople(-1200, 1075, 100, 2120, 30, 1000);

    // Sidewalk
//    urg.loadData("carson_sidewalk_3", 0);
//    urg.fillPointMeshTWPRXY(1000., -90., 0., 0., false);
    
//    urg.loadData("carson_sidewalk_2", 0);
//    urg.fillPointMeshTWPRXY(1200., 270., 0., 60.);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(constantSlide) slide -= autoSlideStep;

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
    urg.drawPointMeshSpherical(scale, slide);
    
//    urg.drawPointMeshLinear(scale, slide, 0);
    
//    urg.drawPointMeshTWPRXY(scale, slide);
    
//    urg.drawGhostedPointMeshes(scale, slide, 0., 270., 90., false, false, true);
    
//    urg.drawOrthoGhostedTimeline(270, 0.18, slide);
    
//    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 'f') ofToggleFullscreen();
    if (key == 's') constantSlide = !constantSlide;
    if (key == OF_KEY_UP) scale *= 1.1;
    if (key == OF_KEY_DOWN) scale *= 0.9;
    if (key == OF_KEY_RIGHT) {
        if (constantSlide) {
            autoSlideStep -= 1;
        } else {
            slide += 10 * 1/scale;
        }
    }
    if (key == OF_KEY_LEFT) {
        if (constantSlide) {
            autoSlideStep += 1;
        } else {
            slide -= 10 * 1/scale;
        }
    }
    if (key == 'c') {
        if (cursorShowing) {
            cursorShowing = false;
            ofHideCursor();
        } else {
            cursorShowing = true;
            ofShowCursor();
        }
    }
    
    urg.setKeyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
