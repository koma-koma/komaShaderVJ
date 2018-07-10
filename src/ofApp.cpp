#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    fbo.allocate(ofGetWidth(), ofGetHeight());
    myGlitch.setup(&fbo);
    
    initParameters();
    
    // load shader
    shader.load("shader_main/shader.vert", "shader_main/shader.frag");
    
    // osc setup
    receiver.setup(PORT);


}

void ofApp::initParameters(){
    // gui setup
    bHide = true;
    params_control.setName("controls");
    params_control.add(bAutoLoading.set("Auto Loading", true));
    params_control.add(bTimeControl.set("Time Control", false));
    params_control.add(time.set("time", 0.0, 0.0, 1000.0));
    params_control.add(t_timeSpeed.set("time speed", 1.0, -1.0, 1.0));
    
    params_color.setName("Color");
    params_color.add(t_col.set("color", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    params_color.add(t_col_depth.set("color depth", 1.0, 0.0, 2.0));
    params_color.add(bColorNoise.set("Random", false));

    
    params_bg.setName("background");
    params_bg.add(fract_num.set("fractal num", 1, 1, 10));
    params_bg.add(t_noise1.set("noise 1", ofVec3f(1.0, 1.0, 1.0), ofVec3f(0, 0, 0), ofVec3f(5.0, 5.0, 1.0)));
    params_bg.add(t_noise2.set("noise 2", ofVec3f(1.0, 1.0, 1.0), ofVec3f(0, 0, 0), ofVec3f(10.0, 10.0, 10.0)));
    
    params_fg.setName("foreground");
    params_fg.add(t_scale.set("scale", ofVec2f(0, 0), ofVec2f(0, 0), ofVec2f(10.0, 10.0)));
    params_fg.add(t_pow.set("pow", 1.0, 0.0, 50.0));
    
    params_fg.add(t_invert.set("Invert Circle", 1.0, 0.0, 1.0));
    params_fg.add(t_circleline.set("circle", ofVec2f(0, 0), ofVec2f(0, 0), ofVec2f(10.0, 10.0)));
    
    randomizeBtn.addListener(this, &ofApp::randomize);
    
    gui.setup(params_control);
    gui.add(params_color);
    gui.add(params_bg);
    gui.add(params_fg);
    gui.add(randomizeBtn.setup("Randomize"));
}


//--------------------------------------------------------------
void ofApp::update(){
    if (bAutoLoading) {
        shader.load("shader_main/shader.vert", "shader_main/shader.frag");
    }
    
    if (!bTimeControl) {
        time += 0.1*timeSpeed;
    }
    
    // fbo draw
    fbo.begin();
    ofClear(0, 0, 0,255);
    ofSetColor(255, 255, 255);
    
    shader.begin();
    shader.setUniform1f("u_time", time);
    shader.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
    shader.setUniform3f("u_color", col.r / 255.0, col.g / 255.0, col.b / 255.0);
    shader.setUniform1f("col_depth", col_depth);
    shader.setUniform1f("fract_num", fract_num);
    shader.setUniform3f("noise1", noise1.x, noise1.y, noise1.z);
    shader.setUniform3f("noise2", noise2.x, noise2.y, noise2.z);
    shader.setUniform2f("scale", scale.x, scale.y);
    shader.setUniform1f("pow", pow);
    shader.setUniform1f("invert", invert);
    shader.setUniform2f("cl", circleline.x, circleline.y);

    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    shader.end();
    
    fbo.end();
    
    easeParameters();

    // ---------------------------

    if (bColorNoise) {
        // 色をノイズで動かす
        t_col = ofColor(ofNoise(time*0.5)*255, ofNoise(time*0.7)*255, ofNoise(time*0.9)*255);
        t_col_depth = ofNoise(time);
    }
    
    // osc control
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        // check for mouse moved message
        if(m.getAddress() == "/1/fader1"){
            t_col_depth = m.getArgAsFloat(0);
        } else if (m.getAddress() == "/1/fader2") {
            t_noise1 = ofVec3f(m.getArgAsFloat(0)*10.0, t_noise1->y, t_noise1->z);
        } else if (m.getAddress() == "/1/fader3") {
            t_noise1 = ofVec3f(t_noise1->x, m.getArgAsFloat(0)*10.0,  t_noise1->z);
        }
    }
}


void ofApp::easeParameters() {
    // easing
    timeSpeed += (t_timeSpeed - timeSpeed) * 0.1;
    
    col.r += (t_col->r - col.r)*0.1;
    col.g += (t_col->g - col.g)*0.1;
    col.b += (t_col->b - col.b)*0.1;
    col_depth = col_depth + (t_col_depth - col_depth)*0.1;
    
    ofVec3f t_n1 = ofVec3f(t_noise1->x, t_noise1->y, t_noise1->z);
    ofVec3f t_n2 = ofVec3f(t_noise2->x, t_noise2->y, t_noise2->z);
    
    noise1 += (t_n1 - noise1)*0.1;
    noise2 += (t_n2 - noise2)*0.1;
    
//    ofVec2f s = ofVec2f(t_scale->x, t_scale->y);
    scale += (t_scale - scale)*0.1;
    pow += (t_pow - pow)*0.1;
    
    invert += (t_invert - invert)*0.1;
//    ofVec2f cl = ofVec2f(t_circleline->x, t_circleline->y);
    circleline += (t_circleline - circleline)*0.1;
}


void ofApp::randomize() {
    t_col = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
    t_col_depth = ofRandom(1.0);
    fract_num = (int)ofRandom(1, 10);
    t_noise1 = ofVec3f(ofRandom(0, 5.0), ofRandom(0, 5), ofRandom(0, 10));
    t_noise2 = ofVec3f(ofRandom(0, 1.0), ofRandom(0, 5), ofRandom(0, 5));
    t_scale = ofVec2f(ofRandom(0, 5.0), ofRandom(0, 5.0));
    t_pow = ofRandom(0, 50.0);
    t_invert = ofRandom(1.0);
    t_circleline = ofVec2f(ofRandom(0, 5), ofRandom(0, 5));
}

//--------------------------------------------------------------
void ofApp::draw(){
    myGlitch.generateFx();
    fbo.draw(0, 0);
    
    // draw gui
    if(!bHide){
        gui.draw();
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'f') {
        ofToggleFullscreen();
    }
    
    if(key == 'h'){
        bHide = !bHide;
    }
    
    if (key == '1') myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE    , true);
    if (key == '2') myGlitch.setFx(OFXPOSTGLITCH_GLOW            , true);
    if (key == '3') myGlitch.setFx(OFXPOSTGLITCH_SHAKER            , true);
    if (key == '4') myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER        , true);
    if (key == '5') myGlitch.setFx(OFXPOSTGLITCH_TWIST            , true);
    if (key == '6') myGlitch.setFx(OFXPOSTGLITCH_OUTLINE        , true);
    if (key == '7') myGlitch.setFx(OFXPOSTGLITCH_NOISE            , true);
    if (key == '8') myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN        , true);
    if (key == '9') myGlitch.setFx(OFXPOSTGLITCH_SWELL            , true);
    if (key == '0') myGlitch.setFx(OFXPOSTGLITCH_INVERT            , true);
    
    if (key == 'q') myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
    if (key == 'w') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE    , true);
    if (key == 'e') myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE    , true);
    if (key == 'r') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE    , true);
    if (key == 't') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT    , true);
    if (key == 'y') myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT    , true);
    if (key == 'u') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT    , true);

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == '1') myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE    , false);
    if (key == '2') myGlitch.setFx(OFXPOSTGLITCH_GLOW            , false);
    if (key == '3') myGlitch.setFx(OFXPOSTGLITCH_SHAKER            , false);
    if (key == '4') myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER        , false);
    if (key == '5') myGlitch.setFx(OFXPOSTGLITCH_TWIST            , false);
    if (key == '6') myGlitch.setFx(OFXPOSTGLITCH_OUTLINE        , false);
    if (key == '7') myGlitch.setFx(OFXPOSTGLITCH_NOISE            , false);
    if (key == '8') myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN        , false);
    if (key == '9') myGlitch.setFx(OFXPOSTGLITCH_SWELL            , false);
    if (key == '0') myGlitch.setFx(OFXPOSTGLITCH_INVERT            , false);
    
    if (key == 'q') myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
    if (key == 'w') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE    , false);
    if (key == 'e') myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE    , false);
    if (key == 'r') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE    , false);
    if (key == 't') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT    , false);
    if (key == 'y') myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT    , false);
    if (key == 'u') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT    , false);

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    fbo.allocate(ofGetWidth(), ofGetHeight());
    myGlitch.setup(&fbo);

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
