#include "ofApp.h"


void ofApp::setup(){
	
	ofEnableDepthTest();
	ofSetVerticalSync(true);
	cam.setDistance(1000);
	
}

void ofApp::update() {


	
}

void ofApp::draw(){
	ofSetBackgroundColor(255); ofSetColor(0);
	ofSetColor(255, 0, 0); ofFill(); ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, 10, 10);
	ofSetColor(0); ofNoFill(); 

	cam.begin(); ofEnableDepthTest();
	

	cam.end(); ofDisableDepthTest();
	

}

void ofApp::keyPressed(int key){
}

/*
void ofApp::keyReleased(int key){

}

void ofApp::mouseMoved(int x, int y ){

}

void ofApp::mouseDragged(int x, int y, int button){
	if (e.getSelected()==1){
		e.x = mouseX; e.y = mouseY;
	}
}

void ofApp::mousePressed(int x, int y, int button){
	if (abs(e.x - mouseX) < 25 && abs(e.y - mouseY)) {
		e.setSelected(1);
	}
}

void ofApp::mouseReleased(int x, int y, int button){
	if(e.getSelected() == 1){
		e.x = mouseX; e.y = mouseY;
		e.setSelected(0);
	}
}

void ofApp::mouseEntered(int x, int y){

}

void ofApp::mouseExited(int x, int y){

}

void ofApp::windowResized(int w, int h){

}

void ofApp::gotMessage(ofMessage msg){

}

void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

*/

