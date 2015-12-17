#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"


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
    
    ofParameter<float> scale;
    ofParameter<bool> drawTrackPoints;
    ofParameter<bool> drawMesh;
    ofParameterGroup viewParams;
    
    ofParameter<bool> drawRender;
    ofParameter<bool> capture360;
    ofParameter<float> renderScale;
    ofParameter<int> zSpacing;
    ofParameter<int> nMeshes;
    ofParameter<int> xTranslate;
    ofParameter<int> yTranslate;
    ofParameter<int> zRotation;
    ofParameter<bool> bFlipXY;
    ofParameterGroup renderParams;
    
    ofParameter<string> oscPort;
    ofParameterGroup oscParameters;
    
    ofParameter<bool> newRecording;
    ofParameter<bool> endRecording;
    ofParameter<bool> recordingState;
    ofParameter<bool> liveData;
    ofParameterGroup recording;
    
    ofxPanel panel;
    
    map<int, ofVec2f> points;
    ofxOscReceiver receiver;
    
    ofEasyCam cam;
    ofMesh mesh;
    
    int thisR = 0;
    float thisTheta = 0.0;
    
    ofFile recFile;
    unsigned long counter = 0;
    
    // handles recording state
    Boolean prevMuteState = false;
    Boolean muteState = false;
    
    // handles file creation
//    Boolean newRecording = false;
//    Boolean endRecording = false;
    
    // handles recording
//    Boolean recordingState = false;
    
    // only check mic every 10 sec
    int refTime = 0;
    int prevRefTime = 0;
    
    // for recording time
    unsigned long timeZero;
    
    // real-time render meshes
    vector<ofMesh> meshes;
    
    float thisAngle = 0.; // for rendering real-time spherical capture
    float rotationStep = 360. / 1024.; // horizontal resolution in spherical scan
    int stepResolution = 1; // default; number of motor steps per one relative .35 degree reference step (min = 1; max = 8) --> must change in arduino code too
    
    // last 341 reading
    float last340 = 0.;
    
};
