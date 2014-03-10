#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include <iostream>

#define max 5000
//max is the maximum number of lines and images, 5000 for what i was messing with


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

		float generateLines();
		void imageSelect();
		float calcImageSelection(), calcAverage();
		bool neverLines;

        bool init, refresh;
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
        int imageSelection, oldSelection;
        ofPoint start[max][max], end[max][max];
        float angle[max][max];
        int averageAngle[max];
        //vector<float> angle;
        int parallelCount[max];
        //vector<float> parallels;
        //int modeCount;
        float dominantAnglePrevalence[max], dominantAngle[max];



        ofxPanel gui;
        ofxFloatSlider threshold0, threshold1, threshold2;//t0slider, t1slider, t2slider;
        ofxFloatSlider thresholdA, thresholdB, thresholdC, lineWidth;//t0slider, t1slider, t2slider;
        ofxIntSlider heatMapAlpha, blurAmount;
        bool bHide;
        ofxToggle showCanny, showBlur, blurToggle, showOriginal, showLines, showCycle, heatMap, smoothToggle, redGlowToggle, oneShot;//, cannyToggle, linesToggle;
        ofxToggle fastMode, calcIndividual, calcTotal;

        ofxLabel filename, framerate;


};
