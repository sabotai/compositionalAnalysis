#include "testApp.h"
/*
#include "ofMain.h"
#include "ofxCv.h"
#include <iostream>*/



/*
TO DO:

3/4/14

Figure out how to scale images and make them comparable -- are portait and landscape images comparison compatible?
        e.g. if image y dimension > ofGetHeight, scale = 0.5

Create vector to store and compare all the different image vec4i

Find angle between 2 points, compare angles

Find length of lines from 2 points, compare lengths and parallel


*/

using namespace cv;
using namespace ofxCv;
cv::Mat src;
//--------------------------------------------------------------
void testApp::setup(){

    threshold0 = 50;
    threshold1 = 50;
    threshold2 = 10;

    //showLines = true;
    //showOriginal = true;
    //howCycle = false;

    //ofImage img;




    string path = "jpg/";
    ofDirectory dir(path);
    //only show jpg files
    //dir.allowExt("jpg");
    //populate the directory object
    dir.listDir();
    imageCount = dir.numFiles() - 1;


    cout << "Image count total = " << imageCount + 1 << endl;
    cout << "Image selection is: " << imageSelection << endl;


    //go through and print out all the paths
    for(int i = 0; i < dir.numFiles(); i++){
        //ofLogNotice(dir.getPath(i));
        image[i].loadImage(dir.getPath(i));
        cout << dir.getPath(i) << endl;

        }

    //img.loadImage(image[]);
    imgMat = toCv(image[imageSelection]);



    gui.setup();
	gui.add(thresholdA.setup("cannyThreshold1", 50, 1, 1000));
	gui.add(thresholdB.setup("cannyThreshold2", 200, 1, 1000));
	gui.add(threshold0.setup("minIntersections", 50, 1, 500));
	gui.add(threshold1.setup("minLinLength", 50, 1, 1000));
	gui.add(threshold2.setup("maxLineGap", 10, 1, 400));
	gui.add(lineWidth.setup("line width", 3, 1, 10));
	gui.add(showOriginal.setup("show image", true));
	gui.add(showCanny.setup("show edges", false));
	gui.add(showLines.setup("show lines", true));
	gui.add(heatMap.setup("show heat map", false));
	gui.add(heatMapAlpha.setup("heat map alpha", 20, 1, 255));
	gui.add(showCycle.setup("cycling", false));

	bHide = true;


}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(10,10,10);
    ofSetLineWidth(lineWidth);

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
    Canny(bw, dst, thresholdA, thresholdB, 3);

    //Canny(imgMat, dst, 50, 200, 3);
    cvtColor(dst, cdst, COLOR_GRAY2BGR);

    if (showCanny){
        drawMat(dst, 0, 0);}

    vector<Vec4i> lines;
    HoughLinesP(dst, lines, 1, CV_PI/180, threshold0, threshold1, threshold2 );
    //ofSetLineWidth(3);


    if (heatMap) {
        ofSetColor(0,0,255,heatMapAlpha);


        for (int x = 0; x < imageCount; x++){
            for (int z = 0; z < max; z++){
                ofLine(start[x][z], end[x][z]);
            }
        }
    } else {
        ofSetColor(255,0,0);
    }



    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        //line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
        //ofPoint start, end;
        start[imageSelection][i].set(l[0], l[1]);
        end[imageSelection][i].set(l[2], l[3]);
            if (showLines){
                if (heatMap){
                    ofSetColor(255,255,255,240);
                } else {
                    ofSetColor(255,0,0, 240);}
                ofLine(start[imageSelection][i], end[imageSelection][i]);
            }
    }






    if (bHide) {
        gui.draw();}

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    //ofClamp(threshold0, 1, 1000);
    //ofClamp(threshold1, 0, 1000);
    //ofClamp(threshold2, 0, 1000);

    switch (key) {
        /*case 'q' :
            threshold0 += 1;
            break;
        case 'w' :
            if (threshold0 > 1) {
                threshold0 -= 1;}
            break;
        case 'a' :
            threshold1 += 1;
            break;
        case 's' :
            if (threshold1 > 0){
                threshold1 -= 1;}
            break;
        case 'z' :
            threshold2 += 1;
            break;
        case 'x' :
            if (threshold2 > 0) {
                threshold2 -= 1;}
            break;*/
        case 'e' :
            showCanny = !showCanny;
            break;
        case 'd' :
            showOriginal = !showOriginal;
            break;
        case 'c' :
            showLines = !showLines;
            break;
        case 'r' :
            showCycle = !showCycle;
            break;
        case 'h' :
            bHide = !bHide;
            break;
        case 's' :
            gui.saveToFile("settings.xml");
            cout << "SETTINGS SAVED TO \"settings.xml\"" << endl;
            break;
        case 'l' :
            gui.loadFromFile("settings.xml");
            cout << "SETTINGS LOADED FROM \"settings.xml\"" << endl;
            break;
    }
/*
    cout << "threshold0: "<<threshold0 <<endl;
    cout << "threshold1: "<<threshold1 <<endl;
    cout << "threshold2: "<<threshold2 <<endl << endl;
*/



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
