#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include <iostream>

using namespace cv;
using namespace ofxCv;

class testApp : public ofBaseApp{

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		ofImage img;
		Mat src, cdst, dst, imgMat, bw;
		int threshold0, threshold1, threshold2;

		bool showCanny, showOriginal, showLines, showCycle;

        //ofxCv::ofxCvGrayscaleImage grayscaleImg;
//		ofxCvGrayscaleImage greyImg;

        ofImage image[5000];
        int imageCount;
        int imageSelection;

};
