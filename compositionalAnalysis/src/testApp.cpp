#include "testApp.h"



/*
TO DO:
3/15/14

currently only supports the top mode angle (interModeSorted[0])


3/12/14 -- set the initial angle to something arbitrary like 361 so it can ignore those
look into drawing the lines with small ellipses with xeno interpolation



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
        imagePath[i] = dir.getPath(i);
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
	gui.add(angleAverageThreshold.setup("interangle threshold", 0, 0, 4));
    gui.add(calcIndividual.setup("calculate: individual", false));
    gui.add(calcTotal.setup("calculate: total", false));
    gui.add(automate.setup("automate calculations", false));
    gui.add(sortImages.setup("resort the images", false));
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
        if (heatMapAlpha < 255){
        ofSetColor(0,0,255,heatMapAlpha);
        } else {
        ofSetColor(0,0,255);
        cout << "is it faster?" << endl; //attempt to improve performance by not drawing alpha, but it didnt work
        }


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

        }
        calcAverage();
        cout << " DONE AUTOMATING -- JESUS CHRIST, FINALLY!" << endl;
        automate = false;
    }

    if(sortImages){
        reloadImages();
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
    if (imagesViewCount <= imageCount){
    imagesViewCount += 1;}

    return imagesViewCount;
}

float testApp::generateLines(){
    for (int i = 0; i < max; i++){
        angle[imageSelection][i] = 361;
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
                ofSetColor(255,255,255);//,240);
            } else {
                ofSetColor(255,0,0);}//, 240);}

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
        //angle[imageSelection][z] -= 90; //convert points to 0-180 with 0 at the top of a circular compass

        if ((angle[imageSelection][z] != 361) && ((start[imageSelection][z].x != 0) && ((start[imageSelection][z].y != 0) ))) {
            //cout<< z<<" is " << angle[imageSelection][z] << " start is " << start[imageSelection][z] << " end is " << end[imageSelection][z] << endl;
            average += angle[imageSelection][z];
            averageCount++;

        }
    }

    averageAngle[imageSelection] = average / averageCount;
    cout<< "Image " << imageSelection << " mean angle is " << averageAngle[imageSelection] << endl;

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
        //cout << ii << " sort is " << angle[imageSelection][ii]<< endl;
    }


//FIND MODE ANGLE



    int modeFrequency[360];



    for (int j = 0; j < 360; j++){ //setting everything to 0
        modeFrequency[j] = 0;

        if (j < 5){
            modeSorted[imageSelection][j] = 361;
        }
    }
    for (int xx = 0; xx < averageCount; xx++){ //xx is each line in given imageSelection


        if (int(angle[imageSelection][xx]) == int(angle[imageSelection][xx+1])){ //this is the old line before adding angleTolerance
        //if ( int(angle[imageSelection][xx]) <= int(angle[imageSelection][xx+1]) + angleTolerance){ //failed attempt at adding angle tolerance ... doesnt work because of if statement doesnt check a range, only the next one... needs to be for loop cycling through all of them
            //cout<<"angle same as next angle"<<endl;

            modeFrequency[int(angle[imageSelection][xx])] += 1;

            //cout<<"current angle: " << angle[imageSelection][xx] << "   frequency: " << modeFrequency[int(angle[imageSelection][xx])] + 1 << endl; //the frequency of given mode plus the original
        } else {
            //cout<<"angle is not the same"<<endl;
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
                //cout<<"Angle " << j << " has a frequency of " << modeFrequency[j]<<endl;
                //tmp = modeFrequency[k];
                //modeFrequency[k]
    //            if frequency of next mode is greater than previous mode, assign previously top mode to 2nd, 2nd to third
                if (modeFrequency[j] >= modeFrequency[modeSorted[imageSelection][0]]) {
                    modeSorted[imageSelection][4] = modeSorted[imageSelection][3];
                    modeSorted[imageSelection][3] = modeSorted[imageSelection][2];
                    modeSorted[imageSelection][2] = modeSorted[imageSelection][1];
                    modeSorted[imageSelection][1] = modeSorted[imageSelection][0];
                    modeSorted[imageSelection][0] = j;//modeFrequency[j];
                } else if (modeFrequency[j] >= modeFrequency[modeSorted[imageSelection][1]]){
                    modeSorted[imageSelection][4] = modeSorted[imageSelection][3];
                    modeSorted[imageSelection][3] = modeSorted[imageSelection][2];
                    modeSorted[imageSelection][2] = modeSorted[imageSelection][1];
                    modeSorted[imageSelection][1] = j;//modeFrequency[j];

                } else if (modeFrequency[j] >=  modeFrequency[modeSorted[imageSelection][2]]){
                    modeSorted[imageSelection][4] = modeSorted[imageSelection][3];
                    modeSorted[imageSelection][3] = modeSorted[imageSelection][2];
                    modeSorted[imageSelection][2] = j;//modeFrequency[j];

                } else if (modeFrequency[j] >=  modeFrequency[modeSorted[imageSelection][3]]){
                    modeSorted[imageSelection][4] = modeSorted[imageSelection][3];
                    modeSorted[imageSelection][3] = j;//modeFrequency[j];

                } else if (modeFrequency[j] >=  modeFrequency[modeSorted[imageSelection][4]]){
                    modeSorted[imageSelection][4] = j;//modeFrequency[j];

                }

            }
    } //first find the most common angles themselves and then find the associated frequency

    cout << "top modes: " << modeSorted[imageSelection][0] << "," << modeSorted[imageSelection][1] << "," << modeSorted[imageSelection][2] << "," << modeSorted[imageSelection][3] << "," << modeSorted[imageSelection][4] << endl;

    //find top angle prevelance ratio within a given image

    for (int i = 0; i < 5; i++){
        dominantAnglePrevalence[imageSelection][i] = modeFrequency[modeSorted[imageSelection][i]]/ averageCount;
        dominantAngle[imageSelection] = modeSorted[imageSelection][0]; //indexes the most common angle for this image

        cout << "dominant angle " << i <<" (" << modeSorted[imageSelection][i] << ") is " << dominantAnglePrevalence[imageSelection][i] << endl;
    }


    calcIndividual = false;


}


float testApp::calcAverage(){
//find most common angle to find some kind of most average image
//can then cycle through to least similar -> so if angle 180 is mode angle, things that are closest to mode angle with highest dominanceRatio are next

//arrange the photos by angle (e.g. 10, 10, 20, 90, 90, 90, 120, etc.  the tie breakers would be based on highest dominanceRatio


//also can arrange photos by how dominant dominantRatio is


//first cycle through modeSorted[i(imageSelection)][0-x(threshold)] and total findings

/*
for (int i = 0; i < imageCount; i++){
    for (int j = 0; j < angleAverageThresold; j++){
        modeSorted[i][j]
        interModeFrequency[j]
    }

}
*/
    cout<< " calcAverage function running" <<endl;


    //int modeFrequency[max];
    //angleAverageThreshold = 0;



    for (int j = 0; j < 362; j++){ //setting everything to 0, using 362 since 361 is the ones that are nothing
        interModeFrequency[j] = 0;
        interModeWeight[j] = 0;


    }


    for (int i = 0; i <= imageCount; i++){
        for (int xx = 0; xx <= angleAverageThreshold; xx++){ //xx is each top angle (0-4) in given imageSelection
            interModeFrequency[modeSorted[i][xx]] = 1;  // the frequency for all is one... this also restarts the count when it comes up again
            interModeWeight[modeSorted[i][xx]] = dominantAnglePrevalence[i][xx];
            interSpecial[modeSorted[i][xx]] = 1 * dominantAnglePrevalence[i][xx];
            for (int j = 0; j <= imageCount; j++){;


                    if ((modeSorted[i][xx] == modeSorted[j][xx]) && (modeSorted[i][xx] != 361) && (i != j)){ //this is the old line before adding angleTolerance

                        interModeFrequency[modeSorted[i][xx]] += 1;
                        interModeWeight[modeSorted[i][xx]] += dominantAnglePrevalence[j][xx];
                        interSpecial[modeSorted[i][xx]] += 1 * dominantAnglePrevalence[j][xx];


                       //cout<<"current interImage: " << i << " Interangle: " << modeSorted[i][xx] << " frequency: " << interModeFrequency[modeSorted[i][xx]] + 1 << "dominance: " << interModeWeight[modeSorted[i][xx]]<< endl; //the frequency of given mode plus the original
                    } else {
                        //cout<<"inter angle " << i << " is not the same as "<< j <<endl;
                    }

            }
        }
    }

    for (int j = 0; j <= imageCount; j++){ // this is just a debug loop to compare modesorted to the counts below
        for (int i = 0; i <= angleAverageThreshold; i++){
            cout << "modeSorted for image " << j << " / mode number " << i << " is:   "<<modeSorted[j][i]<< " dominantAnglePrev: " << dominantAnglePrevalence[j][i] << endl;
        }
    }


    for (int i = 0; i < 360; i++){

        if (interModeFrequency[i] > 0) {

            interDistinctiveAngleCount +=1;
            interModeWeight[i] = interModeWeight[i] / interModeFrequency[i]; //first make the weight an average of all the mode occurances at that angle

            cout<<"current interAngle: " << i << " interfrequency: " <<interModeFrequency[i] << " interModeWeight Average: " << interModeWeight[i]<<endl;
            cout << "interSpecial: " << interSpecial[i]<<endl;


        }
            specialSort[i] = interSpecial[i];
    }
    cout<< "There are " << interDistinctiveAngleCount << " different dominant angles to sort."<<endl;


    //now sort them by special score
    for (int i = 0; i < 360; i++){
    for (int j = 0; j < 360; j++){
   //if (specialSort[i] != 0){
        if (specialSort[i] > specialSort[j]){
            float container = specialSort[i];
            specialSort[i] = specialSort[j];
            specialSort[j] = container;

        }
    }
    }

    for (int i = 0; i < 360; i++){
    for (int j = 0; j < 360; j++){
        if (specialSort[i] == interSpecial[j]){
            specialSort[i] = j;
        }
    }
    }

    cout<<"The top angles by special score are: ";
    for(int i = 0; i < interDistinctiveAngleCount; i++){
            cout << specialSort[i] << ", ";

    }
    cout << endl;




    calcTotal = false;


}


float testApp::reloadImages(){
//reload the images based on the calculations

//if the dominant angle matches the specialSort, then move it up

    for (int i = 0; i <=imageCount; i++){
        cout<< "image " << i << " presorted path is " <<  imagePath[i] << " angle: " <<dominantAngle[i] << endl;
    }

    int currentPosition = 0;
    //for (int i = 0; i < interDistinctiveAngleCount; i++){ // i is current top angle, like 0-180, 1-90, 2-150, etc.
        for (int j = 0; j <= imageCount; j++){
        //for (int i = 0; i <= imageCount; i++){
            if (dominantAngle[j] == specialSort[i]){
                //move forward
                string container = imagePath[currentPosition];
                imagePath[currentPosition] = imagePath[j];
                imagePath[j] = container;
                currentPosition++;
                //cout<<"swap"<<endl;
            }
        //}
    }


    for (int i = 0; i <=imageCount; i++){
        cout<< "image " << i << " sorted path is " <<  imagePath[i] << " angle: " <<dominantAngle[i] << endl;
    }

/*    for(int i = 0; i < dir.numFiles(); i++){
        //ofLogNotice(dir.getPath(i));
        image[i].loadImage(dir.getPath(i));
        cout << dir.getPath(i) << endl;

        }
*/
    sortImages = false;
}
