#include "testApp.h"

#include "ofMain.h"
#include "ofxCv.h"
#include <iostream>

using namespace cv;
using namespace ofxCv;
cv::Mat src;
//--------------------------------------------------------------
void testApp::setup(){
    ofSetLineWidth(1);
    threshold0 = 50;
    threshold1 = 50;
    threshold2 = 10;

    //ofImage img;
    img.loadImage("test9.jpg");
    //grayscaleImg = img;
    imgMat = toCv(img);

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(50,50,50);


    //drawMat(src, 0, 0);
    //drawMat(imgMat, 0, 0);
    ofSetColor(255,255,255);
    //drawMat(dst, 0, 0);




    cvtColor(imgMat, bw, COLOR_RGB2GRAY);
    Canny(bw, dst, 50, 200, 3);

    //Canny(imgMat, dst, 50, 200, 3);
    cvtColor(dst, cdst, COLOR_GRAY2BGR);

    if (showCanny){
        drawMat(cdst, 0, 0);}

    vector<Vec4i> lines;
    HoughLinesP(dst, lines, 1, CV_PI/180, threshold0, threshold1, threshold2 );
    ofSetLineWidth(3);
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        //line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
        ofPoint start, end;
        start.set(l[0], l[1]);
        end.set(l[2], l[3]);

        ofSetColor(255,0,0);
        ofLine(start, end);
        //cout << "start =" << start << endl;
        //cout << "end = " << end << endl;

    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    //ofClamp(threshold0, 1, 1000);
    //ofClamp(threshold1, 0, 1000);
    //ofClamp(threshold2, 0, 1000);


    if (key == 'q'){
        threshold0 += 1;
    } else if (key == 'w' && threshold0 > 1){
        threshold0 -= 1;
    }


    if (key == 'a'){
        threshold1 += 1;
    } else if (key == 's' && threshold1 > 1){
        threshold1 -= 1;
    }


    if (key == 'z'){
        threshold2 += 1;
    } else if (key == 'x' && threshold2 > 1){
        threshold2 -= 1;
    }
    if (key == 'e'){
        showCanny = !showCanny;
    }

    cout << "threshold0: "<<threshold0 <<endl;
    cout << "threshold1: "<<threshold1 <<endl;
    cout << "threshold2: "<<threshold2 <<endl << endl;

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
