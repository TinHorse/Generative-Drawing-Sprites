#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableAlphaBlending();
	ofSetBackgroundColor(0);
	ofSetFrameRate(100);
	for (int i = 0; i < 5; i++) {
		metros.push_back(make_unique<Metro>(ofVec2f{ ofRandom(-2000, 2000), ofRandom(-1000, 1000) }));
	}
	metros.push_back(make_unique<Metro>(ofVec2f{ 0,0 }));
}

//--------------------------------------------------------------
void ofApp::update(){
	/*
	if (ofGetFrameNum() % 500 == 0) {
		metros.push_back(make_unique<Metro>(ofVec2f{ ofRandom(-2000, 2000), ofRandom(-1000, 1000) }));
	}
	*/
}

//--------------------------------------------------------------
void ofApp::draw(){
	cam.begin();
	for (int i = 0; i < metros.size(); i++) {
		metros[i]->run(ofVec3f(0,0,0));
	}
	cam.end();
	//cout<< ofGetFrameRate() << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
