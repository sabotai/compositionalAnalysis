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

		void generateLines();
		void imageSelect();
		bool neverLines;

        bool init;
        int initVal;

		ofImage img;
		Mat src, cdst, dst, imgMat, bw, blurred;

		vector<Vec4i> lines;
		string titlez;
		//int threshold0, threshold1, threshold2;

		//bool showCanny, showOriginal, showLines, showCycle;

        //ofxCv::ofxCvGrayscaleImage grayscaleImg;
//		ofxCvGrayscaleImage greyImg;



        ofImage image[max];
        int imagesViewed();
        int imageCount, imagesViewCount;
        int imageSelection;
        ofPoint start[max][max], end[max][max];



        ofxPanel gui;
        ofxFloatSlider threshold0, threshold1, threshold2;//t0slider, t1slider, t2slider;
        ofxFloatSlider thresholdA, thresholdB, thresholdC, lineWidth;//t0slider, t1slider, t2slider;
        ofxIntSlider heatMapAlpha, blurAmount;
        bool bHide;
        ofxToggle showCanny, showBlur, blurToggle, showOriginal, showLines, showCycle, heatMap, smoothToggle;//, cannyToggle, linesToggle;
        ofxLabel filename, framerate;


};
