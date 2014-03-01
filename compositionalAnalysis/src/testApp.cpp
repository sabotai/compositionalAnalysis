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

    showLines = true;
    showOriginal = true;
    showCycle = false;

    //ofImage img;




    string path = "jpg/";
    ofDirectory dir(path);
    //only show png files
    //dir.allowExt("jpg");
    //populate the directory object
    dir.listDir();
    imageCount = dir.numFiles() - 1;
    cout << "Image count total = " << imageCount << endl;
    cout << "Image selection is: " << imageSelection << endl;


    //go through and print out all the paths
    for(int i = 0; i < dir.numFiles(); i++){
        //ofLogNotice(dir.getPath(i));
        image[i].loadImage(dir.getPath(i));
        cout << dir.getPath(i) << endl;

        }

    //img.loadImage(image[]);
    imgMat = toCv(image[imageSelection]);

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(50,50,50);

    if (showCycle){
        if (imageSelection < imageCount) {
            imageSelection++;
        } else {
            imageSelection = 0;
        }
        imgMat = toCv(image[imageSelection]);
        cout << "CYCLING -- SELECTION IS: " << imageSelection << endl;
    }



    //drawMat(src, 0, 0);
    //drawMat(imgMat, 0, 0);
    ofSetColor(255,255,255);
    //drawMat(dst, 0, 0);

    if (showOriginal) {
        drawMat(imgMat, 0, 0);
    }


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


        if (showLines){
            ofSetColor(255,0,0);
            ofLine(start, end);
            //cout << "start =" << start << endl;
            //cout << "end = " << end << endl;
        }

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
    if (key == 'd'){
        showOriginal = !showOriginal;
    }
    if (key == 'c'){
        showLines = !showLines;
    }

    if (key == 'r') {
        showCycle = !showCycle;
    }

    cout << "threshold0: "<<threshold0 <<endl;
    cout << "threshold1: "<<threshold1 <<endl;
    cout << "threshold2: "<<threshold2 <<endl << endl;




    if (key == OF_KEY_RIGHT || key == OF_KEY_LEFT){
        if (key == OF_KEY_LEFT){
            if (imageSelection > 0){
                imageSelection -= 1;
            } else {
                imageSelection = imageCount;
            }
        }
        if (key == OF_KEY_RIGHT){
            if (imageSelection < imageCount){
                imageSelection += 1;
            } else {
                imageSelection = 0;
            }

        }
        imgMat = toCv(image[imageSelection]);
        cout << "imageSelection = " << imageSelection << endl;
    }
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
