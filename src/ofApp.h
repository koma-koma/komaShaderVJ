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
    void randomizeBg();
    void randomizeFg();
    void randomizeAll();
    
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
    ofParameter<glm::vec2> t_stpos;
    ofParameter<glm::vec3> t_noise_bg;
    
    ofxPanel gui_fg;
    ofParameterGroup params_fg;
    ofParameter<glm::vec3> t_noise_fg;
    ofParameter<glm::vec2> t_scale;
    ofParameter<float> t_pow;
    ofParameter<float> t_invert;
    
    float timeSpeed;
    ofColor col;
    float col_depth;
    glm::vec2 stpos;
    glm::vec3 noise1;
    glm::vec3 noise2;
    glm::vec2 scale;
    float pow;
    float invert;

    ofParameter<bool> bColorNoise;
    ofParameter<bool> bReflect;
    ofxButton btnRandomizeBg;
    ofxButton btnRandomizeFg;
    ofxButton btnRandomizeAll;

    ofxOscReceiver receiver;

    
		
};
