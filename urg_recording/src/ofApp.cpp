#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    
    viewParams.setName("View");
    viewParams.add(scale.set("Zoom", 0.05, 0.05, 1));
    viewParams.add(drawTrackPoints.set("Draw Tracker Points", true));
    viewParams.add(drawMesh.set("Draw Point Cloud", true));
    renderParams.setName("Real-Time Render");
    renderParams.add(drawRender.set("Draw Real-Time Render", false));
    renderParams.add(capture360.set("Draw Spherical Render", false));
    renderParams.add(zSpacing.set("Z Spacing", 100, 1, 400));
    renderParams.add(nMeshes.set("Number of Layers", 20, 1, 512));
    renderParams.add(renderScale.set("Rendering Scale", 0.2, 0.01, 1.));
    renderParams.add(xTranslate.set("X Translation", 0, -1000, 1000));
    renderParams.add(yTranslate.set("Y Translation", 0, -1000, 1000));
    renderParams.add(zRotation.set("Z Rotation", 0, -180, 180));
    renderParams.add(bFlipXY.set("Flip XY Plane", false));
    oscParameters.setName("OSC");
    oscParameters.add(oscPort.set("OSC Port", "7777"));
    recording.add(newRecording.set("New Recording", false));
    recording.add(endRecording.set("End Recording", false));
    recording.add(recordingState.set("Recording State", false));
    recording.add(liveData.set("Live Data", false));
    panel.setup();
    panel.add(viewParams);
    panel.add(renderParams);
    panel.add(oscParameters);
    panel.add(recording);
    panel.loadFromFile("settings.xml");
    
    
    receiver.setup(ofToInt(oscPort));
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
//    // check to see if the system is muted 10 times per second
//    refTime = floor(ofGetElapsedTimeMillis() % 1000);
//    if (refTime != prevRefTime) {
//        string argument;
//        argument = ofToString(ofSystem("osascript -e 'output muted of (get volume settings)'"));
//        char muteChar = ofToChar(argument);
//        if (muteChar == 't') muteState = true;
//        if (muteChar == 'f') muteState = false;
//    }
////    cout << ofToString(prevRefTime) + "  " + ofToString(refTime) << endl;
//    prevRefTime = refTime;
//    
//    // transition from muted to not muted indicates start of recording
//    if (!muteState && prevMuteState) {
//        newRecording = true;
//    }
//    // transition to muted signals end of recording
//    else if (muteState && !prevMuteState) {
//        endRecording = true;
//    }
//    
//    // set previous mute state
//    prevMuteState = muteState;
    
    // create a new file for each new recording
    if (newRecording) {
        newRecording = false;
        
        // create a timestamped title and a new file
        stringstream ss;
        ss << ofGetYear() << "-";
        ss <<  setw(2) << setfill('0') << ofGetMonth() << "-";
        ss <<  setw(2) << setfill('0') << ofGetDay() << " ";
        ss <<  setw(2) << setfill('0') << ofGetHours() << "-";
        ss <<  setw(2) << setfill('0') << ofGetMinutes() << "-";
        ss <<  setw(2) << setfill('0') << ofGetSeconds() << " ";
        string timestamp = ss.str();
        string fileName = "carnegie_museum/" + timestamp + "recording" + ".csv";
        recFile.open(ofToDataPath(fileName), ofFile::WriteOnly);
        
        // set recordingState to true
        recordingState = true;
        
        counter = 0;
        
        meshes.clear();
    }
    
    // close files at end of recording
    if (endRecording) {
        endRecording = false;
        
        recFile.close();
        
        recordingState = false;
    }
    
    
    // if recording state is true, record to file
    // if any of the draw bools are on,
    if (recordingState || drawMesh || drawTrackPoints || drawRender) {
        
        // check for waiting messages
        while(receiver.hasWaitingMessages()){
            // get the next message
            ofxOscMessage m;
            receiver.getNextMessage(&m);
            
            if(m.getAddress() == "/urg/raw/data"){
                mesh.clear();
                
                // set time zero and find this time
                unsigned long thisTime;
                if (counter == 0) {
                    timeZero = ofGetElapsedTimeMillis();
                    thisTime = 0;
                } else {
                    thisTime = ofGetElapsedTimeMillis() - timeZero;
                }
                
                if (recordingState) {
                    recFile << ofToString(thisTime) << ",";
                }
                
                // write arguments of message to file
                for(int i = 0; i < m.getNumArgs(); i+=2){
                    float r = m.getArgAsInt32(i);
                    float theta = m.getArgAsFloat(i+1);
                    
                    float x = r * cos(theta);
                    float y = r * sin(theta);
                    
                    if (recordingState) {
                        if (i != 0) recFile << ",";
                        recFile << ofToString(x) + "," + ofToString(y);
                    }
                    
                    //debug
                    if (i == 340) {
                        if (r != last340) {
                            liveData = true;
                        } else {
                            liveData = false;
                        }
                        last340 = r;
                    }
                    
                    mesh.addVertex(ofVec3f(x, y, 0.));
                }

                // if we're rendering the mesh in real-time, add this mesh to meshes
                if (drawRender) {
                    
//                    vector<ofMesh>::iterator it;
//                    it = meshes.begin();
                    
                    meshes.insert(meshes.begin(), mesh);
                    
                    // delete meshes beyond the specified bound
                    while (meshes.size() > nMeshes) {
                        meshes.pop_back();
                    }
                }
                
                // increment counter
                counter++;
                // add line break to csv
                recFile << "\n";
            }
            
            if(m.getAddress() == "/urg/tracker/data" && drawTrackPoints){
                cout<<"tracker data"<<endl;
                for(int i = 0; i < m.getNumArgs(); i+=3){
                    points[m.getArgAsInt32(i)] = ofVec2f(m.getArgAsInt32(i+1), m.getArgAsInt32(i+2));
                }
            }
            
            // if we're rendering, increment thisAngle
            if (drawRender) {
                thisAngle += rotationStep / stepResolution;
                if (thisAngle > 360.) thisAngle = fmod(thisAngle, 360.f);
            }
        }
    }
    
}


//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
//    ofPushMatrix();
//    {
//        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
//        ofPushMatrix();
//        {
//            ofScale(scale, scale);
//            
//            ofPushStyle();
//            ofSetCircleResolution(60);
//            ofPushMatrix();
//            {
//                ofNoFill();
//                for(int i = 0; i < 10; i++) {
//                    float radius = ofMap(i, -1, 10 - 1, 0, 5600);
//                    ofSetColor(64);
//                    ofCircle(0, 0, radius);
//                    ofVec2f textPosition = ofVec2f(radius, 0).rotate(45);
//                    ofSetColor(255);
//                    ofDrawBitmapString(ofToString(radius, 2) + "mm", textPosition);
//                }
//            }
//            ofPopMatrix();
//            if(drawTrackPoints){
//                for(map<int, ofVec2f>::iterator iter = points.begin(); iter!=points.end(); iter++){
//                    ofPushStyle();
//                    ofNoFill();
//                    ofSetColor(255, 0, 255);
//                    ofCircle(iter->second, 50);
//                    ofPopStyle();
//                }
//            }
//            ofSetColor(255, 255, 0);
//            if(drawMesh) mesh.drawVertices();
//        }
//        ofPopMatrix();
//    }
//    ofPopMatrix();
    
    // draw real-time render
    if (drawRender) {
        ofPushMatrix();
        ofTranslate(ofGetWidth() / 2. + xTranslate, ofGetHeight() / 2. + yTranslate, 0.);
        ofTranslate(-2500. * renderScale, 0.);
        ofScale(renderScale, renderScale, renderScale);
        
        // if not drawing spherical 360 render, render by translating each scan
        if (!capture360) {
            
            // draw each mesh to screen, increasingly further back
            for (int i = 0; i < meshes.size(); i++) {
                ofPushMatrix();
                ofTranslate(0., 0., - i * zSpacing / 2);
                ofRotateZ(zRotation);
                if (bFlipXY) ofRotateY(180);
                meshes[i].drawVertices();
                ofPopMatrix();
            }
        }
        // otherwise, capture as a sphere
        else {
            
            for (int i = 0; i < meshes.size(); i++) {
                ofPushMatrix();
//                ofTranslate(0., 0., - i * zSpacing / 2);
                ofRotateZ(-90.);
                
                // rotate to thisAngle + the step of this scan
                ofRotateX(thisAngle - i * rotationStep / stepResolution);
                
//                if (bFlipXY) ofRotateY(180); // change to mirror
                
                meshes[i].drawVertices();
                ofPopMatrix();
            }
        }
        
        ofPopMatrix();
    }
    
    panel.draw();
    
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if (key == 'f') ofToggleFullscreen();
    
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
