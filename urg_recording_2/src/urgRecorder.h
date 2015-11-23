//
//  urgRecorder.h
//  urg_recording_2
//
//  Created by Ben Snell on 11/14/15.
//
//

#ifndef __urg_recording_2__urgRecorder__
#define __urg_recording_2__urgRecorder__

#include "ofMain.h"
#include "ofxOsc.h"

class urgRecorder {
    
public:

    void setup(int osc_port);
    ofxOscReceiver receiver;
    
    void newRecording(string file_title);
    string fileName;
    ofFile recFile;
    unsigned long counter = 0; // reset each recording
    
    // handles recording
    Boolean recordingState = false;
    
    void endRecording();
    
    void update(Boolean bDrawMesh_, Boolean bDrawTrackPoints_, float *lastYPR);
    
    void draw(Boolean bDrawMesh_, Boolean bDrawTrackPoints_, float scale_);
    
    ofMesh mesh;                // general lidar points
    Boolean bDrawMesh = false;

    map<int, ofVec2f> points;   // tracking
    Boolean bDrawTrackPoints = false;
    
    Boolean setTimeZero = false;
    unsigned long timeZero;
    
};

#endif /* defined(__urg_recording_2__urgRecorder__) */
