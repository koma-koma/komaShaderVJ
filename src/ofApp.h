#pragma once
#define BUFFER_NUM 256

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxPostGlitch.h"
#include "ofxOsc.h"
#include "ofxSyphon.h"

#define PORT 51200

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
    
    void setupAudio();
    void audioIn(ofSoundBuffer & input);
    
    //オーディオ周り
    ofSoundStream soundStream;
    vector <float> left;
    vector <float> right;
    
    int     bufferCounter;
    int     drawCounter;
    float smoothedVol;
    float scaledVol;
    ofParameter<float> maxVol;
    
    ofShader shader;
    string shaderPath;
    
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
    ofParameter<float> t_brightness;
    float timeSpeed;
    float brightness;

    ofxPanel gui_color;
    ofParameterGroup params_color;
    ofParameter<ofColor> t_col1;
    ofParameter<ofColor> t_col2;
    ofParameter<float> t_col_depth;
    ofParameter<float> t_col_mix;
    ofParameter<bool> bRandomColor;
    ofColor col1;
    ofColor col2;
    float col_depth;
    float col_mix;
    
    ofxPanel gui_bg;
    ofParameterGroup params_bg;
    ofParameter<int> fract_num;
    ofParameter<glm::vec2> t_stpos;
    ofParameter<glm::vec3> t_noise_bg;
    ofParameter<float> t_scale_bg;
    ofParameter<float> t_depth_bg;
    ofParameter<float> t_rotate_bg;
    glm::vec2 stpos;
    glm::vec3 noise1;

    ofxPanel gui_fg;
    ofParameter<bool> bHide_fg;
    ofParameterGroup params_fg;
    ofParameter<glm::vec3> t_noise_fg;
    ofParameter<glm::vec2> t_scale;
    ofParameter<float> t_pow;
    ofParameter<float> t_invert;
    
    glm::vec3 noise2;
    glm::vec2 scale;
    float pow;
    float invert;

    ofParameter<bool> isRand;
    ofParameter<bool> bColorNoise;
    ofParameter<bool> bBgNoise;
    ofParameter<bool> bFgNoise;
    ofParameter<bool> bReflect;
    ofParameter<bool> bInvert;
    ofxButton btnRandomizeBg;
    ofxButton btnRandomizeFg;
    ofxButton btnRandomizeAll;
    
    bool bang;
    ofParameter<float> thresh;

    ofxOscReceiver receiver;

    ofxSyphonServer output;
    
		
};
