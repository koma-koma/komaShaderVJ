#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxPostGlitch.h"
#include "ofxOsc.h"

#define PORT 8000

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
    
    void initParameters();
    void easeParameters();
    void randomize();
    
    ofShader shader;
    
    ofFbo fbo;
    ofxPostGlitch myGlitch;
    
    // gui
    bool bHide;
    ofxPanel gui;
    ofParameterGroup params_control;
    ofParameter<bool> bAutoLoading;
    ofParameter<bool> bTimeControl;
    ofParameter<float> time;
    ofParameter<float> t_timeSpeed;
    
    ofxPanel gui_color;
    ofParameterGroup params_color;
    ofParameter<ofColor> t_col;
    ofParameter<float> t_col_depth;
    
    ofxPanel gui_bg;
    ofParameterGroup params_bg;
    ofParameter<int> fract_num;
    ofParameter<glm::vec3> t_noise1;
    ofParameter<glm::vec3> t_noise2;
    
    ofxPanel gui_fg;
    ofParameterGroup params_fg;
    ofParameter<glm::vec2> t_scale;
    ofParameter<float> t_pow;
    ofParameter<float> t_invert;
    ofParameter<glm::vec2> t_circleline;
    
    float timeSpeed;
    ofColor col;
    float col_depth;
    ofVec3f noise1;
    ofVec3f noise2;
    ofVec2f scale;
    float pow;
    float invert;
    ofVec2f circleline;

    ofParameter<bool> bColorNoise;
    ofxButton randomizeBtn;
    
    ofxOscReceiver receiver;

    
		
};
