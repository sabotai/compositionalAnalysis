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
    -most similar angles and the avg x/y point of them
    -most vertical/horizontal lines -> should indicate which images are taken from flattened perspective, most vertical lines = buildings?
    -most parallel lines = urban?, sort by angle/position
    -most lines?

    once sort function is chosen, then it reorders the photos

What is the average line angle?  Average line coordinate?

I could recognize so many of images... many vague feelings before confirmation
Also, I could recognize some scenes (e.g. subway car interior) even though I didn't recognize the photo

making sense of lines

PERFORMANCE:

too many images loaded = slow
running heat map = slow
only few images loaded = fast (unless heat map)
ofEnableSmoothing() = slow while drawing tons of lines i.e. heat map


*/



using namespace cv;
using namespace ofxCv;
cv::Mat src;
//--------------------------------------------------------------



void testApp::setup(){
    ofSetWindowTitle("Compositional Analysis Tool");
    ofSetBackgroundAuto(false);
    refresh = true;



    threshold0 = 50;
    threshold1 = 50;
    threshold2 = 10;
    imagesViewCount = 1;
    //heatMap = false;
    showBlur = false;

    //showLines = true;
    //showOriginal = true;
    //howCycle = false;

    //ofImage img;



    string path = "jpg/";
    ofDirectory dir(path);
    //only show jpg files
    dir.allowExt("jpg");
    //populate the directory object
    dir.listDir();
    dir.sort();
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

    //imgMat = toCv(image[imageSelection]);



    gui.setup();
	gui.add(filename.setup("", dir.getName(imageSelection)));
	gui.add(thresholdA.setup("cannyThreshold1", 50, 1, 1000));
	gui.add(thresholdB.setup("cannyThreshold2", 200, 1, 1000));

	gui.add(threshold0.setup("minIntersections", 50, 1, 500));
	gui.add(threshold1.setup("minLinLength", 50, 1, 1000));
	gui.add(threshold2.setup("maxLineGap", 10, 1, 400));

	gui.add(lineWidth.setup("line width", 1.5, 1, 10));
	gui.add(showOriginal.setup("show image", true));
	gui.add(blurToggle.setup("preprocess blur", true));
	gui.add(blurAmount.setup("blur amount", 5, 1, 20));
	//gui.add(showBlur.setup("show blur", false));
	gui.add(showCanny.setup("show edges", false));
	gui.add(showLines.setup("show lines", true));
	gui.add(heatMap.setup("show heat map", false));
	gui.add(heatMapAlpha.setup("heat map alpha", 20, 1, 255));
	gui.add(showCycle.setup("cycling", false));
	gui.add(smoothToggle.setup("smooth", true));
	gui.add(glowToggle.setup("glow", false));
    gui.add(framerate.setup("framerate", ofToString(ofGetFrameRate())));
    gui.add(fastMode.setup("fast mode", false));
    gui.add(oneShot.setup("export pdf", false));

	bHide = true;
	init = true;
	initVal = 1;
	heatMap = false;

	//imageSelection = 0;
	//oldSelection = 100;

	//ofEnableSmoothing();
    imageSelect();


}

//--------------------------------------------------------------
void testApp::update(){

}



//--------------------------------------------------------------
void testApp::draw(){

	if( oneShot ){
		ofBeginSaveScreenAsPDF("screenshot-"+ofGetTimestampString()+".pdf", false);
	}




 //   else { init = false;}

if (refresh == true || fastMode == false){
    framerate = ofToString(ofGetFrameRate());


    ofBackground(10,10,10);
    ofSetLineWidth(lineWidth);

    if (smoothToggle){
        ofEnableSmoothing();
    } else {
        ofDisableSmoothing();
    }

    if (showCycle){
        if (imageSelection < imageCount) {
            imageSelection++;
            imagesViewed();
        } else {
            imageSelection = 0;
        }
        //imgMat = toCv(image[imageSelection]);
        imageSelect();

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

    if (blurToggle){
        blur( bw, blurred, Size(blurAmount,blurAmount) );
        } else {
        bw = blurred;
        cout << "blur bypassed son" << endl;
    }
    if (showBlur){
        drawMat(blurred, 0, 0);
    }

    Canny(blurred, dst, thresholdA, thresholdB, 3);

    //Canny(imgMat, dst, 50, 200, 3);
    cvtColor(dst, cdst, COLOR_GRAY2BGR);

    if (showCanny){
        drawMat(dst, 0, 0);}

    //vector<Vec4i> lines;
    HoughLinesP(dst, lines, 1, CV_PI/180, threshold0, threshold1, threshold2 );
    //ofSetLineWidth(3);


    if (heatMap) {

        //ofDisableSmoothing();
        ofSetColor(0,0,255,heatMapAlpha);


        for (int x = 0; x < imagesViewCount; x++){
            for (int z = 0; z < max; z++){
                ofLine(start[x][z], end[x][z]);
            }
        }
    } else {
        ofSetColor(255,0,0);
        //ofEnableSmoothing();
    }


/*
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

*/
   // if (neverLines){
        generateLines();
        //neverLines = false;}


        refresh = false;
    }


    if( oneShot ){
		ofEndSaveScreenAsPDF();
		oneShot = false;
	}

    if (bHide) {
    /*
        string path = "jpg/";
        ofDirectory dir2(path);
        dir2.listDir();
        dir2.sort();
        filename = dir2.getName(imageSelection);
        //filename = dir2.getName(imageSelection);
    */
        gui.draw();
    }

//fade in for app
    if (initVal < 256){
        ofSetColor(10,10,10,255-initVal);
        ofRect(0,0,ofGetWidth(), ofGetHeight());
        initVal += 4;
        refresh = true;}

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
        case 'f' :
            heatMap = !heatMap;

        case 'b' :
            blurToggle = !blurToggle;
            break;
        case 'h' :
            bHide = !bHide;
            break;
        case 's' :
            //gui.saveToFile("settings.xml");
            //cout << "SETTINGS SAVED TO \"settings.xml\"" << endl;
            oneShot = true;
            cout << "EXPORTING PDF... SLOWLY..." << endl;
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
            imagesViewed();
        }
        imageSelect();//imgMat = toCv(image[imageSelection]);



        cout << "imageSelection = " << imageSelection << endl;
    }

    refresh = true;
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
    refresh = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    refresh = true;
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


int testApp::imagesViewed(){
    if (imagesViewCount < imageCount){
    imagesViewCount += 1;}

    return imagesViewCount;
}

void testApp::generateLines(){
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

            //if (imageSelection != oldSelection || init == true){
                ofLine(start[imageSelection][i], end[imageSelection][i]); //draw lines for current selection

                if (glowToggle){
                    ofSetColor(255, 0, 0, 100);
                    ofLine(start[imageSelection][i] -1, end[imageSelection][i] -1);
                    ofLine(start[imageSelection][i] +1, end[imageSelection][i] +1);
                    ofSetColor(255, 0, 0, 50);
                    ofLine(start[imageSelection][i] -2, end[imageSelection][i] -2);
                    ofLine(start[imageSelection][i] +2, end[imageSelection][i] +2);
                    ofSetColor(255, 0, 0, 30);
                    ofLine(start[imageSelection][i] -3, end[imageSelection][i] -3);
                    ofLine(start[imageSelection][i] +3, end[imageSelection][i] +3);
                    ofSetColor(255, 0, 0, 30);
                    ofLine(start[imageSelection][i] -4, end[imageSelection][i] -4);
                    ofLine(start[imageSelection][i] +4, end[imageSelection][i] +4);
                    ofSetColor(255, 0, 0, 30);
                    ofLine(start[imageSelection][i] -5, end[imageSelection][i] -5);
                    ofLine(start[imageSelection][i] +5, end[imageSelection][i] +5);
                    }
            //oldSelection = imageSelection;
            //}
        }
    }

}

void testApp::imageSelect(){

        imgMat = toCv(image[imageSelection]);

        //run canny and hough and everything for efficiency
/*
            cvtColor(imgMat, bw, COLOR_RGB2GRAY);

    if (blurToggle){
        blur( bw, blurred, Size(blurAmount,blurAmount) );
        } else {
        bw = blurred;
        cout << "blur bypassed son" << endl;
    }
    if (showBlur){
        drawMat(blurred, 0, 0);
    }

    Canny(blurred, dst, thresholdA, thresholdB, 3);

    //Canny(imgMat, dst, 50, 200, 3);
    cvtColor(dst, cdst, COLOR_GRAY2BGR);

    if (showCanny){
        drawMat(dst, 0, 0);}

    //vector<Vec4i> lines;
    HoughLinesP(dst, lines, 1, CV_PI/180, threshold0, threshold1, threshold2 );
    //ofSetLineWidth(3);

    //neverLines = true;
*/
}
