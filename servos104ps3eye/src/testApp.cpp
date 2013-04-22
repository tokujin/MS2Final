#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

//+++ setting of ps3eye camera
	camWidth = 624;
	camHeight = 464;
	
	ps3eye.listDevices();
	ps3eye.setDesiredFrameRate(60);
	ps3eye.initGrabber(camWidth,camHeight);
    ps3eye.setAutoGainAndShutter(false); // otherwise we can't set gain or shutter
	ps3eye.setGain(0.5);
	ps3eye.setShutter(1.0);
	ps3eye.setGamma(0.5);
	ps3eye.setBrightness(0.6);
	ps3eye.setContrast(1.0);
//    ps3eye.setHue(0.0);
	ps3eye.setFlicker(1);
    
//+++
    buttonState = "digital pin:";
    potValue = "analog pin:";
    
	font.loadFont("franklinGothic.otf", 20);
    smallFont.loadFont("franklinGothic.otf", 14);
    
// replace the string below with the serial port for your Arduino board
// you can get this from the Arduino application or via command line
// for OSX, in your terminal type "ls /dev/tty.*" to get a list of serial devices
	ard.connect("/dev/tty.usbmodemfd121", 57600);
// listen for EInitialized notification. this indicates that
// the arduino is ready to receive commands and it is safe to
// call setupArduino()
	ofAddListener(ard.EInitialized, this, &testApp::setupArduino);
	bSetupArduino	= false;	// flag so we setup arduino when its ready, you don't need to touch this :)


//+++
    videoGrayscaleCvImage.allocate(camWidth, camHeight);
	videoBgImage.allocate(camWidth, camHeight);
	videoDiffImage.allocate(camWidth, camHeight);
	
// set background color to be white:
	ofBackground(0, 0, 0);
}

//--------------------------------------------------------------
void testApp::update(){
    
	int threshold = 20;// you can change the value here
	ps3eye.update();

    updateArduino();

	if (ps3eye.isFrameNew()){
		
		videoColorCvImage.setFromPixels(ps3eye.getPixels(), camWidth, camHeight);
		videoGrayscaleCvImage = videoColorCvImage;
		
		if (ofGetElapsedTimef() < 1.5){
			videoBgImage = videoGrayscaleCvImage;
		}
		
		videoDiffImage.absDiff(videoGrayscaleCvImage, videoBgImage);
		videoDiffImage.threshold(threshold);
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(255, 255, 255);
    int r =0;
    int n=0;
    unsigned char * pixels = videoDiffImage.getPixels();
    for (int i = 0; i < camWidth; i+=48){
        r++;
		for (int j = 0; j < camHeight; j+=58){
            n++;
            valuetemp=0;
            value=0;
            for (int k = 0; k < 48; k+=4) {
                for (int l = 0; l < 58; l+=4) {
                    valuetemp = valuetemp + pixels[(j+l) * camWidth + (i+k)];
                    value = valuetemp / (12 * 15);
                    //op[r][n] = ofMap(value, 0,255, 0,20);
                    printf("camHeight is %d\n", camHeight);
                }
            }
            float pct = ofMap(value, 0,255, 0,20);
            x[8*i+j] = int(pct);//for the adjustment with the screen
            if (pct > 10){
                ofSetColor(155,ofRandom(255),ofRandom(255));
                ofCircle(600 + 80 + i, 69+ j, pct);
            }
		}
	}
    
    videoDiffImage.draw(20,40);

    
    ps3eye.draw(20,600);

    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 'r'){
        videoBgImage = videoGrayscaleCvImage;
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

