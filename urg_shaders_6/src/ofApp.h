#pragma once
#include "ofMain.h"
#include "ofVbo.h"
#include "urgDisplay.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void exit();
		
    ofVec3f cameraRotation = ofVec3f(0., 0., 180.);
    float zoom = -2000;
    float zoomTarget = -3000;
    float yRotTarget;
    unsigned long nVertices;
    
    // billboard particles
    float *billboardSizeTarget;
    
    ofShader billboardShader;
    ofImage texture;
    
    ofVboMesh billboards;
    ofVec3f *billboardVels;
    
    urgDisplay urg;
    
    void huntForBlendFunc(int period, int defaultSid, int defaultDid);
    
    float weights[6] = {1.0, 0.5, 0.25, 0.125, 0.0625, 0.03125};
    float scaling[6] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    
    ofParameter<float> minPointDist;
    ofParameter<float> maxPointDist;
    ofParameterGroup generalDistance;
    
    ofParameter<float> pointSizeMultiplier;
    ofParameter<float> pointSizePower;
    ofParameter<float> minPointSize;
    ofParameter<float> maxPointSize;
    ofParameterGroup pointSize;
    
    ofParameter<float> pointOpacityMultiplier; // 1 or 255?
    ofParameter<float> pointOpacityPower;
    ofParameter<float> minPointOpacity;
    ofParameter<float> maxPointOpacity;
    ofParameterGroup pointOpacity;
    
    ofParameter<float> xTrans;
    ofParameter<float> yTrans;
    ofParameter<float> rotationalSpeed;
    ofParameter<bool> bSaveImages;
    ofParameterGroup placement;
    
    ofxPanel panel;
    
    // CONTROLS

    bool bConstantSlide = false;
    bool bHideControls = false;
    bool bHideCursor = false;
    
    
};







