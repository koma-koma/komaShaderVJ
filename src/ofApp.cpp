#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    fbo.allocate(ofGetWidth(), ofGetHeight());
    myGlitch.setup(&fbo);
    
    bRandom.addListener(this, &ofApp::randomPressed);
    
    // gui setup
    bHide = true;
    gui.setup();
    gui.add(bAutoLoading.setup("Auto Loading", true));
    gui.add(bTimeControl.setup("Time Control", false));
    gui.add(time.setup("time", 0.0, 0.0, 1000.0));
    gui.add(t_timeSpeed.setup("time speed", 1.0, 0.0, 2.0));
    gui.add(t_col.setup("color", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    gui.add(t_col_depth.setup("colpr depth", 1.0, 0.0, 2.0));
    gui.add(t_noise1.setup("noise 1", ofVec3f(1.0, 1.0, 1.0), ofVec3f(0, 0, 0), ofVec3f(10.0, 10.0, 10.0)));
    gui.add(t_noise2.setup("noise 2", ofVec3f(1.0, 1.0, 1.0), ofVec3f(0, 0, 0), ofVec3f(10.0, 10.0, 10.0)));
    gui.add(bParamNoise.setup("Noise", false));
    gui.add(bRandom.setup("Random"));
    
    gui.add(t_scale.setup("scale", ofVec2f(0, 0), ofVec2f(0, 0), ofVec2f(10.0, 10.0)));
    gui.add(t_pow.setup("pow", 1.0, 0.0, 50.0));


    
    // shader
    shader.load("shader_main/shader.vert", "shader_main/shader.frag");
    
    

}

//--------------------------------------------------------------
void ofApp::update(){
    if (bAutoLoading) {
        shader.load("shader_main/shader.vert", "shader_main/shader.frag");
    }
    
    if (!bTimeControl) {
        time = ofGetElapsedTimef()*timeSpeed;
    }
    
    fbo.begin();
    ofClear(0, 0, 0,255);
    ofSetColor(255, 255, 255);
    
    shader.begin();
    shader.setUniform1f("u_time", time);
    shader.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
    shader.setUniform3f("u_color", col.r / 255.0, col.g / 255.0, col.b / 255.0);
    shader.setUniform1f("col_depth", col_depth);
    shader.setUniform3f("noise1", noise1.x, noise1.y, noise1.z);
    shader.setUniform3f("noise2", noise2.x, noise2.y, noise2.z);
    shader.setUniform2f("scale", scale.x, scale.y);
    shader.setUniform1f("pow", pow);


    
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    shader.end();
    
    fbo.end();
    
    // easing
    timeSpeed += (t_timeSpeed - timeSpeed) * 0.1;
    
    ofColor c = t_col;
    col.r += (c.r - col.r)*0.1;
    col.g += (c.g - col.g)*0.1;
    col.b += (c.b - col.b)*0.1;
    col_depth = col_depth + (t_col_depth - col_depth)*0.1;
    
    ofVec3f t_n1 = ofVec3f(t_noise1->x, t_noise1->y, t_noise1->z);
    ofVec3f t_n2 = ofVec3f(t_noise2->x, t_noise2->y, t_noise2->z);
    
    noise1 += (t_n1 - noise1)*0.1;
    noise2 += (t_n2 - noise2)*0.1;
    
    ofVec2f s = ofVec2f(t_scale->x, t_scale->y);
    scale += (s - scale)*0.1;
    pow += (t_pow - pow)*0.1;


    if (bParamNoise) {
        // パラメータをノイズで動かす
        t_col = ofColor(ofNoise(time*0.5)*255, ofNoise(time*0.7)*255, ofNoise(time*0.9)*255);
        t_col_depth = ofNoise(time);
    }
}

void ofApp::randomPressed() {
    t_col = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
    t_col_depth = ofRandom(1.0);
    t_noise1 = ofVec3f(ofRandom(0, 5.0), ofRandom(0, 10), ofRandom(0, 10));
    t_noise2 = ofVec3f(ofRandom(0, 1.0), ofRandom(0, 10), ofRandom(0, 10));
    t_scale = ofVec2f(ofRandom(0, 10.0), ofRandom(0, 10.0));
    t_pow = ofRandom(0, 50.0);
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
