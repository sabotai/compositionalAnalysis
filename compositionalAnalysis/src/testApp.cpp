#include "testApp.h"
/*
#include "ofMain.h"
#include "ofxCv.h"
#include <iostream>*/



/*
TO DO:

3/10/14

add something to highlight the individual angles when you hover over the line

finish total sorts and calcAverage()

fix the titles

clean up gui

figure out angle tolerance for averages -- some arent calculating well

change color of the circles at end points and make the diameter into a slider





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
    //ofSetBackgroundAuto(false);
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

	gui.add(lineWidth.setup("line width", 2.5, 1, 10));
	gui.add(showOriginal.setup("show image", true));
	gui.add(blurToggle.setup("preprocess blur", true));
	gui.add(blurAmount.setup("blur amount", 5, 1, 20));
	//gui.add(showBlur.setup("show blur", false));
	gui.add(showCanny.setup("show edges", false));
	gui.add(showLines.setup("show lines", true));
	gui.add(heatMap.setup("show heat map", false));
	gui.add(heatMapB.setup("show heat map points", false));
	gui.add(heatMapAlpha.setup("heat map alpha", 20, 1, 255));
	gui.add(showCycle.setup("cycling", false));
	gui.add(smoothToggle.setup("smooth", true));
	gui.add(redGlowToggle.setup("red glow", false));
    gui.add(framerate.setup("framerate", ofToString(ofGetFrameRate())));
    gui.add(fastMode.setup("fast mode", false));
    //gui.add(angleTolerance.setup("angle tolerance", 0, 0, 100));
    gui.add(calcIndividual.setup("calculate: individual", false));
    gui.add(calcTotal.setup("calculate: total", false));
    gui.add(automate.setup("automate calculations", false));
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

    if (heatMapB) {
           ofSetColor(0,0,255,heatMapAlpha);


        for (int x = 0; x < imagesViewCount; x++){
            for (int z = 0; z < max; z++){
                ofCircle(start[x][z], 5);
                ofCircle(end[x][z], 5);
            }
        }



    } else {
        ofSetColor(255,0,0);
        //ofEnableSmoothing();
    }


    generateLines();

    if (calcIndividual) { //
        calcImageSelection();
    }

    if (calcTotal){
        calcAverage();

    }


    refresh = false;
    }

    if (automate){
         //imageSelection = 0;//start at the beginning
         imagesViewCount = 0;

        for (int i = 0; i <= imageCount; i++){ //cycle through and process each image
            imageSelection = i;
            imageSelect();
            imagesViewed();

            if (showOriginal) {
                drawMat(imgMat, 0, 0);
            }

            cvtColor(imgMat, bw, COLOR_RGB2GRAY);

            if (blurToggle){
                blur( bw, blurred, Size(blurAmount,blurAmount) );
                } else {
                bw = blurred;
            }

            Canny(blurred, dst, thresholdA, thresholdB, 3);

            //Canny(imgMat, dst, 50, 200, 3);
            cvtColor(dst, cdst, COLOR_GRAY2BGR);

            if (showCanny){
                drawMat(dst, 0, 0);}

            //vector<Vec4i> lines;
            HoughLinesP(dst, lines, 1, CV_PI/180, threshold0, threshold1, threshold2 );
            //ofSetLineWidth(3);


            generateLines();

            calcImageSelection();
            calcAverage();

        }
        cout << " DONE AUTOMATING -- JESUS CHRIST, FINALLY!" << endl;
        automate = false;
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

float testApp::generateLines(){
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
                angle[imageSelection][i] = atan2(start[imageSelection][i].y - end[imageSelection][i].y, start[imageSelection][i].x - end[imageSelection][i].x);
                angle[imageSelection][i] = angle[imageSelection][i] * 180 / PI;
                //cout << "initial is " << angle[imageSelection][i] << " start is " << start[imageSelection][i] << " end is " << end[imageSelection][i] << endl;
                //cout << angle[imageSelection][i] * 180 / PI << endl;
//                cout<<angle[imageSelection].length<<endl;

                if (redGlowToggle){
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

        //return angle[imageSelection][i];
    }


}

void testApp::imageSelect(){

        imgMat = toCv(image[imageSelection]);
}

float testApp::calcImageSelection(){
    //first sort the parallels

//int ii;
  //  int jj;
    float tmp;

//for (int i = 0; i < max ){
//averageTotal[imageSelection] = 0;}



//FIND AVERAGE ANGLE -- not very useful, must find mode
    float average = 0;
    float averageCount = 0;
    for(int z = 0; z < max; z++){
        if (angle[imageSelection][z] < 0) {
            angle[imageSelection][z] += 360; //convert negative angles into angle/360
        }

        if ((angle[imageSelection][z] != 0) && ((start[imageSelection][z].x != 0) && ((start[imageSelection][z].y != 0) ))) {
            cout<< z<<" is " << angle[imageSelection][z] << " start is " << start[imageSelection][z] << " end is " << end[imageSelection][z] << endl;
            average += angle[imageSelection][z];
            averageCount++;

        }
    }

    averageAngle[imageSelection] = average / averageCount;
    cout<< "average angle is " << averageAngle[imageSelection] << endl;

//SORT
    for(int ii = 0; ii < averageCount; ii++) // arraylength is the length of the array you want to use
    {
        for(int jj = ii + 1; jj < averageCount; jj++) // j will always be after i
        {
            // array is the array itself
            if(angle[imageSelection][ii] > angle[imageSelection][jj]) // check to see if it is larger than the other one
            {
                // if so then swap them
                tmp = angle[imageSelection][jj];
                angle[imageSelection][jj] = angle[imageSelection][ii];
                angle[imageSelection][ii] = tmp;
            }
        }
        cout << ii << " sort is " << angle[imageSelection][ii]<< endl;
    }


//FIND MODE ANGLE



    int modeFrequency[360];
    int modeSorted[4]; //top 5 frequencies



    for (int j = 0; j < 360; j++){ //setting everything to 0
        modeFrequency[j] = 0;

        if (j < 5){
            modeSorted[j] = 0;
        }
    }
    for (int xx = 0; xx < averageCount; xx++){ //xx is each line in given imageSelection


        if (int(angle[imageSelection][xx]) == int(angle[imageSelection][xx+1])){ //this is the old line before adding angleTolerance
        //if ( int(angle[imageSelection][xx]) <= int(angle[imageSelection][xx+1]) + angleTolerance){ //failed attempt at adding angle tolerance ... doesnt work because of if statement doesnt check a range, only the next one... needs to be for loop cycling through all of them
            //cout<<"angle same as next angle"<<endl;

            modeFrequency[int(angle[imageSelection][xx])] += 1;

            cout<<"current angle: " << angle[imageSelection][xx] << "   frequency: " << modeFrequency[int(angle[imageSelection][xx])] + 1 << endl; //the frequency of given mode plus the original
        } else {
            cout<<"angle is not the same"<<endl;
        }


    }

//sort mode frequencies
//should mode frequency comparisons be based on most frequent angle with a minimum frequency of maybe 5 or 10?
//if modeFrequency[imageSelection][j] == modeFrequency[max][j] they have the same most common angle
//then find out what the minimum

//also find top angle ratio to total angles

    //int
    for (int j = 0; j < 360; j++){
        //for (int k = j+1; k < 360; k++);
            //if (modeFrequency[j] > modeFrequency[k]) {
            if (modeFrequency[j] > 0){
                cout<<"Angle " << j << " has a frequency of " << modeFrequency[j]<<endl;
                //tmp = modeFrequency[k];
                //modeFrequency[k]
    //            if frequency of next mode is greater than previous mode, assign previously top mode to 2nd, 2nd to third
                if (modeFrequency[j] >= modeFrequency[modeSorted[0]]) {
                    modeSorted[4] = modeSorted[3];
                    modeSorted[3] = modeSorted[2];
                    modeSorted[2] = modeSorted[1];
                    modeSorted[1] = modeSorted[0];
                    modeSorted[0] = j;//modeFrequency[j];
                } else if (modeFrequency[j] >= modeFrequency[modeSorted[1]]){
                    modeSorted[4] = modeSorted[3];
                    modeSorted[3] = modeSorted[2];
                    modeSorted[2] = modeSorted[1];
                    modeSorted[1] = j;//modeFrequency[j];

                } else if (modeFrequency[j] >=  modeFrequency[modeSorted[2]]){
                    modeSorted[4] = modeSorted[3];
                    modeSorted[3] = modeSorted[2];
                    modeSorted[2] = j;//modeFrequency[j];

                } else if (modeFrequency[j] >=  modeFrequency[modeSorted[3]]){
                    modeSorted[4] = modeSorted[3];
                    modeSorted[3] = j;//modeFrequency[j];

                } else if (modeFrequency[j] >=  modeFrequency[modeSorted[4]]){
                    modeSorted[4] = j;//modeFrequency[j];

                }

            }
    } //first find the most common angles themselves and then find the associated frequency

    cout << "top modes: " << modeSorted[0] << "," << modeSorted[1] << "," << modeSorted[2] << "," << modeSorted[3] << "," << modeSorted[4] << endl;

    //find top angle prevelance ratio within a given image

    dominantAnglePrevalence[imageSelection] = modeFrequency[modeSorted[0]]/ averageCount;
    dominantAngle[imageSelection] = modeSorted[0]; //indexes the most common angle for this image

    cout << "dominant angle (" << modeSorted[0] << ") is " << dominantAnglePrevalence[imageSelection] << endl;













    calcIndividual = false;


}


float testApp::calcAverage(){
//find most common angle to find some kind of most average image
//can then cycle through to least similar -> so if angle 180 is mode angle, things that are closest to mode angle with highest dominanceRatio are next

//arrange the photos by angle (e.g. 10, 10, 20, 90, 90, 90, 120, etc.  the tie breakers would be based on highest dominanceRatio


//also can arrange photos by how dominant dominantRatio is


}
