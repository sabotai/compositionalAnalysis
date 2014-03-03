#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include <iostream>

#define max 5000
//max is the maximum number of lines and images


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
		//int threshold0, threshold1, threshold2;

		//bool showCanny, showOriginal, showLines, showCycle;

        //ofxCv::ofxCvGrayscaleImage grayscaleImg;
//		ofxCvGrayscaleImage greyImg;

        ofImage image[max];
        int imageCount;
        int imageSelection;
        ofPoint start[max][max], end[max][max];



        ofxPanel gui;
        ofxFloatSlider threshold0, threshold1, threshold2;//t0slider, t1slider, t2slider;
        ofxFloatSlider thresholdA, thresholdB, thresholdC, lineWidth;//t0slider, t1slider, t2slider;
        ofxIntSlider heatMapAlpha;
        bool bHide;
        ofxToggle showCanny, showOriginal, showLines, showCycle, heatMap;//, cannyToggle, linesToggle;

        int heat;

};
