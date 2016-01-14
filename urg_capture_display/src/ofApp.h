#pragma once

#include "ofMain.h"
#include "ofxCsv.h"
#include "urgDisplay.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

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
    
    urgDisplay urg;

    float scale = 1;
    float slide = 0;
    
    Boolean constantSlide = false;
    
    int autoSlideStep = 12;
    
    Boolean cursorShowing = true;
    
    bool bDrawPanel;
    
    ofParameter<float> xTranslation;
    ofParameter<float> yTranslation;
    ofParameterGroup controls;
    ofxPanel panel;
    
};


