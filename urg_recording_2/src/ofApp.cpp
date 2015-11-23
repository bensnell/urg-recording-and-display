#include "ofApp.h"

///dev/tty.usbserial-AL00EZJ7

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    
    viewParams.setName("View");
    viewParams.add(scale.set("Zoom", 0.05, 0.05, 1));
    viewParams.add(drawTrackPoints.set("Draw Tracker Points", false));
    viewParams.add(drawMesh.set("Draw Point Cloud", true));
    oscParameters.setName("OSC");
    oscParameters.add(oscPort.set("OSC Port", "7777"));
    panel.setup();
    panel.add(viewParams);
    panel.add(oscParameters);
    panel.loadFromFile("settings.xml");
    
    // setup the urg recorder
    myURG.setup(ofToInt(oscPort));
    
    // connect to the razor
    myRazor.connect("/dev/tty.usbserial-AL00EZJ7", 57600);
    
    // start the thread to listen to system volume
    myThread.startThread(true);
}

//--------------------------------------------------------------
void ofApp::update(){
    
//    myThread.lock(); // lock access to the resource
    
    // create a new file for each new recording
    if (myThread.myVol.unmuted()) myURG.newRecording("recording");
    
    // close files at end of recording
    if (myThread.myVol.muted()) myURG.endRecording();
    
//    myThread.unlock(); // done with resource

    // read the razor
    myRazor.readYPR();
    
    // update URG
    // if recordingState is True, this reads and records
    // if drawMesh is True, this reads and makes a mesh
    // if drawTrackPoints is True, this reads and adds the points
    myURG.update(drawMesh, drawTrackPoints, myRazor.lastYPR);
}


//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    
    // draw the mesh and track points if bools are on
    myURG.draw(drawMesh, drawTrackPoints, scale);
    
    panel.draw();
    
    // debug
    string printState = (myURG.recordingState ? "Currently Recording" : "Not Recording");
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()) + "\n" + printState, 10, 20);
}

void ofApp::exit(){
    myThread.stopThread();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
