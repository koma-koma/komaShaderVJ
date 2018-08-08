#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    setupAudio();
    
    fbo.allocate(ofGetWidth(), ofGetHeight());
    myGlitch.setup(&fbo);
    
    initParameters();
    
    // load shader
    shaderPath = "shader_main/shader2.frag";
    shader.load("shader_main/shader.vert", shaderPath);
    
    // osc setup
    receiver.setup(PORT);
    
    // syphon
    output.setName("Main");

    
    bang = false;

}

//--------------------------------------------------------------
void ofApp::setupAudio(){
    soundStream.printDeviceList();
    int bufferSize = BUFFER_NUM;
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    
    bufferCounter    = 0;
    drawCounter        = 0;
    smoothedVol     = 0.0;
    scaledVol        = 0.0;
    
    ofSoundStreamSettings settings;
    
    //    auto devices = soundStream.getMatchingDevices("default");
    //    if(!devices.empty()){
    //        settings.setInDevice(devices[0]);
    //    }
    auto devices = soundStream.getDeviceList();
    settings.setInDevice(devices[0]);
    settings.setInListener(this);
    settings.sampleRate = 44100;
    settings.numOutputChannels = 0;
    settings.numInputChannels = 2;
    settings.bufferSize = bufferSize;
    soundStream.setup(settings);
}

void ofApp::initParameters(){
    // gui setup
    // main
    bHide = true;
    params_control.setName("controls");
    params_control.add(isRand.set("Auto Randomize", true));
    params_control.add(bAutoLoading.set("Auto Loading", true));
    params_control.add(bTimeControl.set("Time Control", false));
    params_control.add(time.set("time", 0.0, 0.0, 1000.0));
    params_control.add(t_timeSpeed.set("time speed", 0.5, -1.0, 1.0)); // フェーダにアサイン
    params_control.add(t_brightness.set("brightness", 1.0, 0.0, 1.0)); // フェーダにアサイン
    params_control.add(maxVol.set("max volume", 0.5, 0.0, 10.0)); // フェーダにアサイン
    params_control.add(thresh.set("threshold", 0.5, 0.0, 1.0)); // フェーダにアサイン
    params_control.add(bReflect.set("Reflect", false));
    params_control.add(bInvert.set("Invert", false));
    gui.setup(params_control);

    //color
    params_color.setName("Color");
    params_color.add(t_col1.set("color1", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    params_color.add(t_col2.set("color2", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    params_color.add(t_col_depth.set("color depth", 1.0, 0.0, 2.0));
    params_color.add(t_col_mix.set("color mix", 0.5, 0.0, 1.0));
    params_color.add(bColorNoise.set("Random", true));
    gui_color.setup(params_color);
    gui_color.setPosition(220, 10);

    
    params_bg.setName("background");
    params_bg.add(fract_num.set("fractal num", 1, 1, 10));
    params_bg.add(t_stpos.set("center pos", ofVec2f(0, 0), ofVec2f(-1.0, -1.0), ofVec2f(1.0, 1.0)));
    params_bg.add(t_noise_bg.set("noise", ofVec3f(1.0, 1.0, 1.0), ofVec3f(0, 0, 0), ofVec3f(5.0, 5.0, 1.0)));
    params_bg.add(bBgNoise.set("Random", false));
    gui_bg.setup(params_bg);
    gui_bg.setPosition(10, 270);


    
    params_fg.setName("foreground");
    params_fg.add(bHide_fg.set("Hide FG", false));
    params_fg.add(t_noise_fg.set("noise", ofVec3f(1.0, 1.0, 1.0), ofVec3f(0, 0, 0), ofVec3f(10.0, 10.0, 10.0)));
    params_fg.add(t_scale.set("scale", ofVec2f(0, 0), ofVec2f(0, 0), ofVec2f(10.0, 10.0)));
    params_fg.add(t_pow.set("pow", 1.0, 0.0, 50.0));
    params_fg.add(t_invert.set("Invert Circle", 1.0, -1.0, 1.0));
    params_fg.add(bFgNoise.set("Random", false));
    gui_fg.setup(params_fg);
    gui_fg.setPosition(10, 450);

    
    btnRandomizeBg.addListener(this, &ofApp::randomizeBg);
    btnRandomizeFg.addListener(this, &ofApp::randomizeFg);
    btnRandomizeAll.addListener(this, &ofApp::randomizeAll);

    gui.add(btnRandomizeBg.setup("Randomize BG"));
    gui.add(btnRandomizeFg.setup("Randomize FG"));
    gui.add(btnRandomizeAll.setup("Randomize All"));
}


//--------------------------------------------------------------
void ofApp::update(){
    // ボリュームの正規化
    scaledVol = ofMap(smoothedVol, 0.0, maxVol, 0.0, 1.0, true);
    
    if (bang == false && scaledVol > thresh) {
        bang = true;
        
        if (isRand)randomizeAll();
        
//        int filter = ofRandom(5);
//        if (filter == 0) {
//            myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE            , true);
//        } else if (filter == 1) {
//            myGlitch.setFx(OFXPOSTGLITCH_SHAKER    , true);
//        } else if (filter == 2) {
//            myGlitch.setFx(OFXPOSTGLITCH_BLOCKNOISE    , true);
//        } else if (filter == 3) {
//            myGlitch.setFx(OFXPOSTGLITCH_VIGNETTE    , true);
//        } else if (filter == 4) {
//            myGlitch.setFx(OFXPOSTGLITCH_PIXELSORT    , true);
//        }
    }
    
    if(bang == true && scaledVol < thresh){
        bang = false;
    }
    
    if (bAutoLoading) {
        shader.load("shader_main/shader.vert", shaderPath);
    }
    
    if (!bTimeControl) {
        time += 0.01*timeSpeed;
    }
    
    if (bHide_fg) {
        t_invert=0;
    }
    
    if (bInvert) {
        myGlitch.setFx(OFXPOSTGLITCH_INVERT            , true);
    } else {
        myGlitch.setFx(OFXPOSTGLITCH_INVERT            , false);
    }
    
    // fbo draw
    fbo.begin();
    ofClear(0, 0, 0,255);
    ofBackground(0, 0, 0);
    ofSetColor(255, 255, 255);
    
    shader.begin();
    shader.setUniform1f("u_time", time);
    shader.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
    shader.setUniform1f("u_brightness", t_brightness);
    shader.setUniform1f("u_audio", scaledVol);
    shader.setUniform3f("u_color1", col1.r / 255.0, col1.g / 255.0, col1.b / 255.0);
    shader.setUniform3f("u_color2", col2.r / 255.0, col2.g / 255.0, col2.b / 255.0);
    shader.setUniform1f("col_depth", col_depth);
    shader.setUniform1f("col_mix", col_mix);
    shader.setUniform1f("fract_num", fract_num);
    shader.setUniform2f("stpos", stpos.x, stpos.y);
    shader.setUniform3f("noise1", noise1.x, noise1.y, noise1.z);
    shader.setUniform3f("noise2", noise2.x, noise2.y, noise2.z);
    shader.setUniform2f("scale", scale.x, scale.y);
    shader.setUniform1f("pow", pow);
    shader.setUniform1f("invert", invert);
    shader.setUniform1f("bReflect", (int)bReflect);

    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    shader.end();
    
    // symbol
//    ofEnableAlphaBlending();
//    ofBlendMode(OF_BLENDMODE_ADD);
//    ofPushMatrix();
//    ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
//
//    ofRotateDeg(ofGetElapsedTimef()*20, 1, 1, 1);
//    ofSetColor(255, 255, 255);
//    ofNoFill();
//    ofSetBoxResolution(2);
//    ofDrawBox(0, 0, 0, ofGetWidth()*scaledVol);
//    ofPopMatrix();
    
    
    fbo.end();
    
    easeParameters();

    // ---------------------------

    if (bColorNoise) {
        // 色をノイズで動かす
        t_col1 = ofColor(ofNoise(time*0.5)*255, ofNoise(time*0.7)*255, ofNoise(time*0.9)*255);
        t_col2 = ofColor(ofNoise(time*0.78)*255, ofNoise(time*0.9)*255, ofNoise(time*0.87)*255);
        t_col_depth = ofNoise(time);
        t_col_mix = ofNoise(time*0.93);
    }
    if (bBgNoise) {
        t_noise_bg = ofVec3f(ofNoise(time*0.5)*1.0, ofNoise(time*0.6)*2.0, ofNoise(time*0.7)*2.0);
        t_stpos = ofVec2f((ofNoise(time*0.65)-0.5)*2.0, (ofNoise(time*0.55)-0.5)*2.0);
    }
    if (bFgNoise) {
        t_noise_fg = ofVec3f(ofNoise(time*0.89)*5.0, ofNoise(time*0.46)*5.0, ofNoise(time*0.64)*5.0);
        t_scale = ofVec2f((ofNoise(time*0.65))*10.0, (1.0-ofNoise(time*0.65))*10.0);
        t_pow = ofNoise(time*0.67+100.0)*10.0;
    }
    
    // osc control
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        cout << m.getAddress();
        
        // check for mouse moved message
        if(m.getAddress() == "/1/bAutoLoading"){
            bAutoLoading = m.getArgAsBool(0);
        } else if (m.getAddress() == "/1/bReflect") {
            bReflect = m.getArgAsBool(0);
        } else if (m.getAddress() == "/1/fract_num") {
            fract_num = int(m.getArgAsFloat(0)*10);
        } else if (m.getAddress() == "/1/noise_bg_x") {
            t_noise_bg = ofVec3f(m.getArgAsFloat(0)*5.0, t_noise_bg->y, t_noise_bg->z);
        } else if (m.getAddress() == "/1/noise_bg_y") {
            t_noise_bg = ofVec3f(t_noise_bg->x, m.getArgAsFloat(0)*5.0,  t_noise_bg->z);
        } else if (m.getAddress() == "/1/noise_bg_z") {
            t_noise_bg = ofVec3f(t_noise_bg->x,  t_noise_bg->y, m.getArgAsFloat(0)*5.0);
        } else if (m.getAddress() == "/1/bg_pos") {
            t_stpos = ofVec2f((m.getArgAsFloat(0)-0.5)*2.0, (m.getArgAsFloat(1)-0.5)*2.0);
        } else if (m.getAddress() == "/1/noise_fg_x") {
            t_noise_fg = ofVec3f(m.getArgAsFloat(0)*5.0, t_noise_fg->y, t_noise_fg->z);
        } else if (m.getAddress() == "/1/noise_fg_y") {
            t_noise_fg = ofVec3f(t_noise_fg->x, m.getArgAsFloat(0)*5.0,  t_noise_fg->z);
        } else if (m.getAddress() == "/1/noise_fg_z") {
            t_noise_fg = ofVec3f(t_noise_fg->x,  t_noise_fg->y, m.getArgAsFloat(0)*5.0);
        } else if (m.getAddress() == "/1/xy2") {
            t_scale = ofVec2f((m.getArgAsFloat(0))*5.0, (m.getArgAsFloat(1))*5.0);
        } else if (m.getAddress() == "/1/pow") {
            t_pow = m.getArgAsFloat(0)*10;
        } else if (m.getAddress() == "/1/invert") {
            t_invert = (m.getArgAsFloat(0)-0.5)*2.0;
        } else if (m.getAddress() == "/1/noise_bg_rand_x" && m.getArgAsInt(0)==1) {
            t_noise_bg = ofVec3f(ofRandom(1)*5.0, t_noise_bg->y, t_noise_bg->z);
        } else if (m.getAddress() == "/1/noise_bg_rand_y" && m.getArgAsInt(0)==1) {
            t_noise_bg = ofVec3f(t_noise_bg->x, ofRandom(1)*5.0,  t_noise_bg->z);
        } else if (m.getAddress() == "/1/noise_bg_rand_z" && m.getArgAsInt(0)==1) {
            t_noise_bg = ofVec3f(t_noise_bg->x,  t_noise_bg->y, ofRandom(1)*5.0);
        } else if (m.getAddress() == "/1/fract_num_rand" && m.getArgAsInt(0)==1) {
            t_stpos = ofVec2f((ofRandom(1)-0.5)*2.0, (ofRandom(1)-0.5)*2.0);
        } else if (m.getAddress() == "/1/noise_fg_rand_x" && m.getArgAsInt(0)==1) {
            t_noise_fg = ofVec3f(ofRandom(1)*5.0, t_noise_fg->y, t_noise_fg->z);
        } else if (m.getAddress() == "/1/noise_fg_rand_y" && m.getArgAsInt(0)==1) {
            t_noise_fg = ofVec3f(t_noise_fg->x, ofRandom(1)*5.0,  t_noise_fg->z);
        } else if (m.getAddress() == "/1/noise_fg_rand_z" && m.getArgAsInt(0)==1) {
            t_noise_fg = ofVec3f(t_noise_fg->x,  t_noise_fg->y, ofRandom(1)*5.0);
        }
    }
}

void ofApp::easeParameters() {
    // easing
    timeSpeed += (t_timeSpeed - timeSpeed) * 0.1;
    
    col1.r += (t_col1->r - col1.r)*0.1;
    col1.g += (t_col1->g - col1.g)*0.1;
    col1.b += (t_col1->b - col1.b)*0.1;
    
    col2.r += (t_col2->r - col2.r)*0.1;
    col2.g += (t_col2->g - col2.g)*0.1;
    col2.b += (t_col2->b - col2.b)*0.1;
    col_depth = col_depth + (t_col_depth - col_depth)*0.1;
    col_mix = col_mix + (t_col_mix - col_mix)*0.1;

    ofVec3f t_n1 = ofVec3f(t_noise_bg->x, t_noise_bg->y, t_noise_bg->z);
    ofVec3f t_n2 = ofVec3f(t_noise_fg->x, t_noise_fg->y, t_noise_fg->z);
    
    stpos += (t_stpos - stpos)*0.1;
    noise1 += (t_n1 - noise1)*0.1;
    noise2 += (t_n2 - noise2)*0.1;
    
//    ofVec2f s = ofVec2f(t_scale->x, t_scale->y);
    scale += (t_scale - scale)*0.1;
    pow += (t_pow - pow)*0.1;
    
    invert += (t_invert - invert)*0.1;
//    ofVec2f cl = ofVec2f(t_circleline->x, t_circleline->y);
    
}


void ofApp::randomizeBg() {
    fract_num = (int)ofRandom(1, 10);
    t_noise_bg = ofVec3f(ofRandom(0, 2.0), ofRandom(0, 5), ofRandom(0, 10));
    t_stpos = ofVec2f(ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0));
}

void ofApp::randomizeFg() {
    t_noise_fg = ofVec3f(ofRandom(0, 1.0), ofRandom(0, 5), ofRandom(0, 5));
    t_scale = ofVec2f(ofRandom(0, 5.0), ofRandom(0, 5.0));
    t_pow = ofRandom(0, 50.0);
    t_invert = ofRandom(.2, 1.0);
}

void ofApp::randomizeAll() {
    randomizeBg();
    randomizeFg();
    if (ofRandom(0, 1) > 0.8 ) {
        bInvert = true;
    } else {
        bInvert = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    myGlitch.generateFx();
    fbo.draw(0, 0);
    
    output.publishScreen();
    
    // draw gui
    if(!bHide){
        gui.draw();
        gui_color.draw();
        gui_bg.draw();
        gui_fg.draw();
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
    
    if(key == 'x'){
        randomizeAll();
    }
    if(key == 'c'){
        randomizeBg();
    }
    if(key == 'v'){
        randomizeFg();
    }
    if(key == 'a'){
        bReflect = !bReflect;
    }
    if(key == ','){
        shaderPath = "shader_main/shader.frag";
    }
    if(key == '.'){
        shaderPath = "shader_main/shader2.frag";
    }
    
    
    if (key == '1') myGlitch.toggleFx(OFXPOSTGLITCH_CONVERGENCE);
    if (key == '2') myGlitch.toggleFx(OFXPOSTGLITCH_GLOW);
    if (key == '3') myGlitch.toggleFx(OFXPOSTGLITCH_SHAKER);
    if (key == '4') myGlitch.toggleFx(OFXPOSTGLITCH_CUTSLIDER);
    if (key == '5') myGlitch.toggleFx(OFXPOSTGLITCH_TWIST);
    if (key == '6') myGlitch.toggleFx(OFXPOSTGLITCH_OUTLINE);
    if (key == '7') myGlitch.toggleFx(OFXPOSTGLITCH_NOISE);
    if (key == '8') myGlitch.toggleFx(OFXPOSTGLITCH_SLITSCAN);
    if (key == '9') myGlitch.toggleFx(OFXPOSTGLITCH_SWELL);
    if (key == '0') myGlitch.toggleFx(OFXPOSTGLITCH_INVERT);
    
    if (key == 'q') myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
    if (key == 'w') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE    , true);
    if (key == 'e') myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE    , true);
    if (key == 'r') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE    , true);
    if (key == 't') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT    , true);
    if (key == 'y') myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT    , true);
    if (key == 'u') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT    , true);
    if (key == 'i') myGlitch.toggleFx(OFXPOSTGLITCH_PIXELSORT);
    if (key == 'o') myGlitch.toggleFx(OFXPOSTGLITCH_BLOCKNOISE);
    if (key == 'p') myGlitch.toggleFx(OFXPOSTGLITCH_VIGNETTE);
    if (key == '@') myGlitch.toggleFx(OFXPOSTGLITCH_DISTORTION);
    
    if (key == 'j') {
        t_scale = glm::vec2(5.0, 5.0);
    }
    if (key == 'k') {
        t_scale = glm::vec2(1.0, 5.0);
    }
    if (key == 'l') {
        t_scale = glm::vec2(0.1, 10.0);
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
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

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer & input){
    
    float curVol = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0;
    
    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    for (size_t i = 0; i < input.getNumFrames(); i++){
        left[i]        = input[i*2]*0.5;
        right[i]    = input[i*2+1]*0.5;
        
        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted+=2;
    }
    
    //this is how we get the mean of rms :)
    curVol /= (float)numCounted;
    
    // this is how we get the root of rms :)
    curVol = sqrt( curVol );
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    
    bufferCounter++;
    
}

