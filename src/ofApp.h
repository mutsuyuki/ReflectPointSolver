#pragma once

#include "ofMain.h"
#include "MyHoverCamera.hpp"

class ofApp : public ofBaseApp {

public:
	vector<ofVec3f> points;  
	vector<ofVec3f> reflectPoints;

	MyHoverCamera camera;
	ofRectangle viewport;

	float radius = 5.0f;
	int debugCount = 0;

	void setup() {
		cout << "test" << endl;

		viewport = ofRectangle(0, 0, ofGetWidth(), ofGetHeight());

		points.push_back(ofVec3f::zero());
		points.push_back(ofVec3f::zero());
		points.push_back(ofVec3f::zero());
		points.push_back(ofVec3f::zero());
	}

	void update() {
		if (debugCount % 2 == 0) {
			points[0].x = sin(ofGetElapsedTimef() * 2.1) * 20.0 - 80.0;
			points[0].y = sin(ofGetElapsedTimef() * 1.3) * 20.0 + 0.0;
			points[0].z = sin(ofGetElapsedTimef() * 3.5) * 20.0 + 0.0;

			points[1].x = sin(ofGetElapsedTimef() * 1.1) * 20.0 - 35.0;
			points[1].y = sin(ofGetElapsedTimef() * 2.2) * 20.0 + 40.0;
			points[1].z = sin(ofGetElapsedTimef() * 3.5) * 20.0 + 40.0;

			points[2].x = sin(ofGetElapsedTimef() * 3.2) * 20.0 + 35.0;
			points[2].y = sin(ofGetElapsedTimef() * 1.1) * 20.0 + 60.0;
			points[2].z = sin(ofGetElapsedTimef() * 2.4) * 20.0 - 40.0;

			points[3].x = sin(ofGetElapsedTimef() * 1.2) * 20.0 + 80.0;
			points[3].y = sin(ofGetElapsedTimef() * 3.1) * 20.0 + 10.0;
			points[3].z = sin(ofGetElapsedTimef() * 2.4) * 20.0 + 0.0;

			reflectPoints = getReflectPoints(points, radius);
		}
	}

	void draw() {
		ofBackground(0);
		ofSetWindowTitle(ofToString(ofGetFrameRate()));

		camera.begin(viewport);
		ofDrawAxis(1000);
		ofDrawBitmapString("x", 100, 0, 0);
		ofDrawBitmapString("y", 0, 100, 0);
		ofDrawBitmapString("z", 0, 0, 100);

		// center of rotation
		for (ofVec3f point : points) {
			ofSetColor(255);
			ofDrawSphere(point, 1);
		}

		// reflect normal vector ,  reflect surface
		for (int i = 1; i < reflectPoints.size() - 1; ++i) {
			ofSetColor(255, 255, 0, 255);
			ofDrawLine(points[i], reflectPoints[i]);

			ofSetColor(255, 255, 0, 20);
			ofNoFill();
			ofPushMatrix();
			ofTranslate(points[i]);
			ofSpherePrimitive refrectSurface;
			refrectSurface.setRadius(radius);
			refrectSurface.setResolution(8);
			refrectSurface.drawWireframe();
			ofPopMatrix();
		}

		ofSetLineWidth(3);
		// beam
		ofSetColor(0, 255, 255);
		for (int i = 1; i < reflectPoints.size(); ++i) {
			ofDrawLine(reflectPoints[i - 1], reflectPoints[i]);
		}

		camera.end();
	}


	vector<ofVec3f> getReflectPoints(vector<ofVec3f>& __points, float __radius) {

		vector<ofVec3f> resultPoints;
		for (int i = 0; i < __points.size() - 2; ++i) {
			ofVec3f startPoint = __points[i];
			ofVec3f cornerPoint = __points[i + 1];
			ofVec3f endPoint = __points[i + 2];
			resultPoints.push_back(((startPoint + endPoint) / 2.0).getMiddle(cornerPoint));
		}

		ofVec3f deltaPoint;
		float gradDirection;
		for (float itr = 0; itr < 200; ++itr) {
			for (int i = 0; i < resultPoints.size(); ++i) {
				ofVec3f startPoint = __points[i];
				ofVec3f cornerPoint = __points[i + 1];
				ofVec3f endPoint = __points[i + 2];
				float moveRate = abs(cornerPoint.distance(resultPoints[i]) - __radius) / 2.0 + 0.01;

				// x update
				deltaPoint = ofVec3f(moveRate, 0.0, 0.0);
				gradDirection = getGradDirection(resultPoints[i], deltaPoint, startPoint, cornerPoint, endPoint, __radius);
				resultPoints[i].x += gradDirection * moveRate;

				// y update
				deltaPoint = ofVec3f(0.0, moveRate, 0.0);
				gradDirection = getGradDirection(resultPoints[i], deltaPoint, startPoint, cornerPoint, endPoint, __radius);
				resultPoints[i].y += gradDirection * moveRate;

				// z update
				deltaPoint = ofVec3f(0.0, 0.0, moveRate);
				gradDirection = getGradDirection(resultPoints[i], deltaPoint, startPoint, cornerPoint, endPoint, __radius);
				resultPoints[i].z += gradDirection * moveRate;
			}
		}

		vector<ofVec3f> result;
		result.push_back(__points.front());
		for (int i = 0; i < resultPoints.size(); ++i) {
			result.push_back(ofVec3f(resultPoints[i]));
		}
		result.push_back(__points.back());
		return result;
	}


	float getGradDirection(ofVec3f& __resultPoint, ofVec3f& __deltaPoint, ofVec3f& __startPoint, ofVec3f& __cornerPoint, ofVec3f& __endPoint, float __radius) {
		ofVec3f tempPoint;

		tempPoint = __resultPoint - __deltaPoint;
		float cost1 = calcCost(tempPoint, __startPoint, __cornerPoint, __endPoint, __radius);

		tempPoint = __resultPoint + __deltaPoint;
		float cost2 = calcCost(tempPoint, __startPoint, __cornerPoint, __endPoint, __radius);;

		if (cost1 < cost2) return -1.0;
		if (cost1 > cost2) return 1.0;
		if (cost1 == cost2) return 0.0;
	}

	float calcCost(ofVec3f& __tempPoint, ofVec3f& __startPoint, ofVec3f& __cornerPoint, ofVec3f& __endPoint, float __radius) {
		ofVec3f normalizedStartVec = (__startPoint - __tempPoint).normalize();
		ofVec3f normalizedEndVec = (__endPoint - __tempPoint).normalize();
		ofVec3f cornerToResult = (__tempPoint - __cornerPoint);

		// equation formula	1
		ofVec3f costVec = __radius * (normalizedStartVec + normalizedEndVec).normalize() - cornerToResult;
		float cost1 = abs(costVec.x) + abs(costVec.y) + abs(costVec.z);

		// equation formula	2
		float cost2 = abs(__cornerPoint.distance(__tempPoint) - __radius);

		return cost1 + cost2;
	}


	void keyPressed(int key) {

		debugCount++;

//		if (debugCount == 0) {
//			points[1].x = -10.0f / sqrt(2.0);
//			points[1].z = 80.0f + 10.0f / sqrt(2.0);
//		}
//
//		if (debugCount == 1) {
//			points[1].x = -10.0f;
//			points[1].z = 80.0f;
//		}
//
//		if (debugCount == 2) {
//			points[1].x = 0;
//			points[1].z = 90.0f;
//		}
//
//		if (debugCount == 4) {
//			points[1].x = 0.0f;
//			points[1].z = 40.0f;
//		}
//
//
//		if (debugCount == 5) {
//			points[1].x = 40.0f;
//			points[1].z = 80.0f;
//			debugCount = -1;
//		}


	}

	//		void keyReleased(int key);
	//		void mouseMoved(int x, int y );
	//		void mouseDragged(int x, int y, int button);
	//		void mousePressed(int x, int y, int button);
	//		void mouseReleased(int x, int y, int button);
	//		void mouseEntered(int x, int y);
	//		void mouseExited(int x, int y);
	//		void windowResized(int w, int h);
	//		void dragEvent(ofDragInfo dragInfo);
	//		void gotMessage(ofMessage msg);

};
