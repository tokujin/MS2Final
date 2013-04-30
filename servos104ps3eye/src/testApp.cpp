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
void testApp::setupArduino(const int & version) {
	
	// remove listener because we don't need it anymore
	ofRemoveListener(ard.EInitialized, this, &testApp::setupArduino);
    
    // it is now safe to send commands to the Arduino
    bSetupArduino = true;
    
    // print firmware name and version to the console
    cout << ard.getFirmwareName() << endl;
    cout << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion() << endl;
    
    // Note: pins A0 - A5 can be used as digital input and output.
    // Refer to them as pins 14 - 19 if using StandardFirmata from Arduino 1.0.
    // If using Arduino 0022 or older, then use 16 - 21.
    // Firmata pin numbering changed in version 2.3 (which is included in Arduino 1.0)
    
    // set pin D9, D10, D11 as PWM (analog output)
  	ard.sendDigitalPinMode( 9, ARD_PWM);
	ard.sendDigitalPinMode(10, ARD_PWM);
	ard.sendDigitalPinMode(11, ARD_PWM);
    
    // attach a servo to pin D9
    // servo motors can only be attached to pin D3, D5, D6, D9, D10, or D11
    ard.sendServoAttach(9);
    ard.sendServoAttach(10);
    ard.sendServoAttach(11);

    // Listen for changes on the digital and analog pins
    ofAddListener(ard.EDigitalPinChanged, this, &testApp::digitalPinChanged);
    ofAddListener(ard.EAnalogPinChanged, this, &testApp::analogPinChanged);
}

//--------------------------------------------------------------
void testApp::updateArduino(){
    
	// update the arduino, get any data or messages.
    // the call to ard.update() is required
	ard.update();
	
	// do not send anything until the arduino has been set up
	if (bSetupArduino) {
        // fade the led connected to pin D11
		ard.sendPwm(11, (int)(128 + 128 * sin(ofGetElapsedTimef())));   // pwm... Nthis has to be changed
	}
    
}

// digital pin event handler, called whenever a digital pin value has changed
// note: if an analog pin has been set as a digital pin, it will be handled
// by the digitalPinChanged function rather than the analogPinChanged function.

//--------------------------------------------------------------
void testApp::digitalPinChanged(const int & pinNum) {
    // do something with the digital input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    buttonState = "digital pin: " + ofToString(pinNum) + " = " + ofToString(ard.getDigital(pinNum));
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

            if (pct > 10){
                ofSetColor(155,ofRandom(255),ofRandom(255));
                ofCircle(600 + 80 + i, 69+ j, pct);
            x[8*i+j] = int(pct);//for the adjustment with the screen
            
                
                //need to be modified according to the settings of daisy chain
                if (x[8*i+j] > 10) {
                    ard.sendServo(9, 0, false);
                    ard.sendServo(10, 0, false);
                    ard.sendServo(11, 0, false);
                }
            
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

