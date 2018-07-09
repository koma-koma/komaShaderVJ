#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxPostGlitch.h"

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void randomPressed();
    
    ofShader shader;
    
    ofFbo fbo;
    ofxPostGlitch myGlitch;
    
    bool bHide;
    ofxPanel gui;
    ofxToggle bAutoLoading;
    ofxToggle bTimeControl;
    ofxFloatSlider time;
    ofxFloatSlider t_timeSpeed;
    ofxColorSlider t_col;
    ofxFloatSlider t_col_depth;
    ofxVec3Slider t_noise1;
    ofxVec3Slider t_noise2;
    
    ofxVec2Slider t_scale;
    ofxFloatSlider t_pow;
    
    float timeSpeed;
    ofColor col;
    float col_depth;
    ofVec3f noise1;
    ofVec3f noise2;
    ofVec2f scale;
    float pow;

    ofxToggle bParamNoise;
    ofxButton bRandom;

    
		
};
