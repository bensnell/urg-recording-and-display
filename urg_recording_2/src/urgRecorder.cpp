//
//  urgRecorder.cpp
//  urg_recording_2
//
//  Created by Ben Snell on 11/14/15.
//
//

#include "urgRecorder.h"

void urgRecorder::setup(int osc_port) {
    
    receiver.setup(osc_port);
}

// ----------------------------------------------------------------------

// start new recording
void urgRecorder::newRecording(string file_title) {
    
    // create a timestamped title and a new file
    stringstream ss;
    ss << ofGetYear() << "-";
    ss <<  setw(2) << setfill('0') << ofGetMonth() << "-";
    ss <<  setw(2) << setfill('0') << ofGetDay() << " ";
    ss <<  setw(2) << setfill('0') << ofGetHours() << "-";
    ss <<  setw(2) << setfill('0') << ofGetMinutes() << "-";
    ss <<  setw(2) << setfill('0') << ofGetSeconds() << " ";
    string timestamp = ss.str();
    fileName = timestamp + file_title + ".csv";
    recFile.open(ofToDataPath(fileName), ofFile::WriteOnly);
    
    // set recordingState to true
    recordingState = true;
    
    setTimeZero = true;
}

// ----------------------------------------------------------------------

// end current recording
void urgRecorder::endRecording() {
    
    recFile.close();
    
    recordingState = false;
    
    fileName.clear();
    
    counter = 0; // reset counter
    
    timeZero = 0;
}

// ----------------------------------------------------------------------

// file writing format (pre 20151113):      x1, y1, z1, x2, y2, z2, ...
// file writing format (post 20151113):     z1, x1, y1, x2, y2, ..

// always reads the incoming messages, but only writes them or adds to mesh if the proper bools are on
void urgRecorder::update(Boolean bDrawMesh_, Boolean bDrawTrackPoints_, float *lastYPR) {
    
    // update local reference bools
    bDrawMesh = bDrawMesh_;
    bDrawTrackPoints = bDrawTrackPoints_;
    
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        // always get the data when it's available (even when not recording)
        if(m.getAddress() == "/urg/raw/data" && (recordingState || bDrawMesh)){
            
            if (bDrawMesh) mesh.clear();  // clear the mesh
            
            // first written to each row (choose):
            if (recordingState) {
                
                unsigned long timeNow = ofGetElapsedTimeMillis();
                if (setTimeZero) {
                    timeZero = timeNow;
                    setTimeZero = false;
                }
                timeNow -= timeZero;
                
                // pass yaw, pitch, and roll into each row
                recFile << ofToString(timeNow) << ",";
                recFile << ofToString(lastYPR[0]) << ",";
                recFile << ofToString(lastYPR[1]) << ",";
                recFile << ofToString(lastYPR[2]) << ",";
                
//                recFile << ofToString(counter) << ",";
//                recFile << ofToString(ofGetElapsedTimeMillis()) << ",";
            }
            
            // get the arguments of the message
            for(int i = 0; i < m.getNumArgs(); i+=2){
                
                float r = m.getArgAsInt32(i);
                float theta = m.getArgAsFloat(i+1);
                
                float x = r * cos(theta);
                float y = r * sin(theta);
                
                if (recordingState) {
                    // don't add a comma to the first (unless) something like the counter is written first (see above)
                    if (i != 0) recFile << ",";
                    recFile << ofToString(x) + "," + ofToString(y);
                }
                
                if (bDrawMesh) mesh.addVertex(ofVec3f(x,y));
            }
            
            if(recordingState) {
                counter++;
                
                recFile << "\n"; // line break
            }
        }
        
        if(m.getAddress() == "/urg/tracker/data" && bDrawTrackPoints){
            for(int i = 0; i < m.getNumArgs(); i+=3){
                points[m.getArgAsInt32(i)] = ofVec2f(m.getArgAsInt32(i+1), m.getArgAsInt32(i+2));
            }
        }
    }
}

// ----------------------------------------------------------------------

// draw the mesh and/or tracking points
void urgRecorder::draw(Boolean bDrawMesh_, Boolean bDrawTrackPoints_, float scale_) {
    
    bDrawMesh = bDrawMesh_;
    bDrawTrackPoints = bDrawTrackPoints_;
    
    if (bDrawMesh || bDrawTrackPoints) {
        ofPushMatrix();
        {
            ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
            ofPushMatrix();
            {
                ofScale(scale_, scale_);
                
                ofPushStyle();
                ofSetCircleResolution(60);
                ofPushMatrix();
                {
                    ofNoFill();
                    for(int i = 0; i < 10; i++) {
                        float radius = ofMap(i, -1, 10 - 1, 0, 5600);
                        ofSetColor(64);
                        ofCircle(0, 0, radius);
                        ofVec2f textPosition = ofVec2f(radius, 0).rotate(45);
                        ofSetColor(255);
                        ofDrawBitmapString(ofToString(radius, 2) + "mm", textPosition);
                    }
                }
                ofPopMatrix();
                if(bDrawTrackPoints){
                    for(map<int, ofVec2f>::iterator iter = points.begin(); iter!=points.end(); iter++){
                        ofPushStyle();
                        ofNoFill();
                        ofSetColor(255, 0, 255);
                        ofCircle(iter->second, 50);
                        ofPopStyle();
                    }
                }
                ofSetColor(255, 255, 0);
                if(bDrawMesh) mesh.drawVertices();
            }
            ofPopMatrix();
        }
        ofPopMatrix();
    }
}
