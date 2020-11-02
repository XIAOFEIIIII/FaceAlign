#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	grabber.setup(640, 480);
	tracker.setup();
	std::size_t n = dir.listDir("faces/");

	if (n == 0)
	{
		ofLogError() << "No files in the directory. Quitting.";
		ofExit();
	}

	cur = 0;
	eyeDistance = 32;
	fbo.allocate(ofGetWidth(), ofGetHeight());

}

//- ------------------------------------------------------------
void ofApp::update(){

	/*grabber.update();
	if (grabber.isFrameNew()) {
		tracker.update(grabber);
	}*/


	string filename = dir.getFile(cur).getFileName();
	img.load("faces/" + filename);
	tracker.update(img);
	
	if (tracker.update(img)) {

		if (tracker.size() > 0)
		{
			ofPolyline leftEyePoly = tracker.getInstances()[0].getLandmarks().getImageFeature(ofxFaceTracker2Landmarks::LEFT_EYE);
			ofPolyline rightEyePoly = tracker.getInstances()[0].getLandmarks().getImageFeature(ofxFaceTracker2Landmarks::RIGHT_EYE);

			leftEye.set(0, 0);
			for (int i = 0; i < leftEyePoly.size(); i++)
			{
				leftEye += leftEyePoly[i];
			}
			leftEye /= (float)leftEyePoly.size();

			rightEye.set(0, 0);
			for (int i = 0; i < rightEyePoly.size(); i++) {
				rightEye += rightEyePoly[i];
			}
			rightEye /= (float)rightEyePoly.size();

		}

		leftEyeSmoothed = 0.8 * leftEyeSmoothed + 0.2 * leftEye;
		rightEyeSmoothed = 0.8 * rightEyeSmoothed + 0.2 * rightEye;

		ofDrawLine(leftEye, rightEye);
		//grabber.getTexture().setAnchorPoint(leftEyeSmoothed.x, leftEyeSmoothed.y);
		img.getTexture().setAnchorPoint(leftEyeSmoothed.x, leftEyeSmoothed.y);
		float distance = (leftEyeSmoothed - rightEyeSmoothed).length();
		ofPoint diff = rightEyeSmoothed - leftEyeSmoothed;

		float angle = atan2(diff.y, diff.x);
		float scale = 100. / distance;

		fbo.begin();
		ofPushMatrix();
		ofTranslate(100,100);
		ofScale(scale, scale);
		ofRotateZRad(-angle);
		//img.draw(0, 0);	
		tracker.drawDebug();
		ofPopMatrix();

		fbo.end();
		fbo.readToPixels(pix);
		pix.setImageType(OF_IMAGE_GRAYSCALE);
		ofSaveImage(pix, "aligned/_" + ofToString(tracker.getThreadFps()) + filename + ".tiff");
	}

	cur++;
	if (cur == dir.size()) {
		cur = 0;
		ofLog() << "done";
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	fbo.draw(0, 0);
	ofDrawBitmapString(ofToString((int)ofGetFrameRate()), 10, 20);	
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
