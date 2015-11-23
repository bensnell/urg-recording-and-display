#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "urgRecorder.h"
#include "systemVolume.h"
#include "systemVolumeThread.h"
#include "razor.h"

class ofApp : public ofBaseApp {
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit();
    
    ofParameter<float> scale;
    ofParameter<bool> drawTrackPoints;
    ofParameter<bool> drawMesh;
    ofParameterGroup viewParams;
    
    ofParameter<string> oscPort;
    ofParameterGroup oscParameters;
    
    ofxPanel panel;
    
    // handles all recording
    urgRecorder myURG;
//    systemVolume myVol;
    
    ofEasyCam cam;
    
    // create object
    systemVolumeThread myThread;
    
    razor myRazor;
        
};
