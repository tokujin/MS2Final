#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    
    ofSetVerticalSync(true);
	// setup video grabber:
	video.initGrabber(624, 464);
	
	// get the width and height, and allocate color and grayscale images:
	width = video.width;
	height = video.height;
	
    //	videoColorCvImage.allocate(width, height);
	videoGrayscaleCvImage.allocate(width, height);
	videoBgImage.allocate(width, height);
	videoDiffImage.allocate(width, height);
    modifiedImage.allocate(width, height);
	
	// set background color to be white:
	ofBackground(0, 0, 0);
	
	
	panel.setup("cv settings", 1380, 0, 300, 748);
	panel.addPanel("control", 1, false);
	
	panel.setWhichPanel("control");
	panel.setWhichColumn(0);
	panel.addToggle("learn background ", "B_LEARN_BG", true);
	panel.addSlider("threshold ", "THRESHOLD", 20, 0, 255, true);
	panel.loadSettings("cvSettings.xml");
    
    //Serial
	// arduino users check in arduino app....
    
	mySerial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = mySerial.getDeviceList();

	mySerial.setup(0, 9600); //open the first device
    mySerial.setup("cu.usbmodemfd121",9600);
    
    pct = 0;
    
    for (int i = 0; i < 104; i++) {
        arr[i] = 0x00;
    }
    
}

//--------------------------------------------------------------
void testApp::update(){
    //panel
	panel.update();
	
	bool bLearnBg = panel.getValueB("B_LEARN_BG");
	int threshold = panel.getValueI("THRESHOLD");
	
    //video
	video.update();
	
	if (video.isFrameNew()){
		
		videoColorCvImage.setFromPixels(video.getPixels(), width, height);
		videoGrayscaleCvImage = videoColorCvImage;
		
		if (bLearnBg ){
			videoBgImage = videoGrayscaleCvImage;
			panel.setValueB("B_LEARN_BG", false);
		}
		
		if (ofGetElapsedTimef() < 1.5){
			videoBgImage = videoGrayscaleCvImage;
		}
		
		videoDiffImage.absDiff(videoGrayscaleCvImage, videoBgImage);
		videoDiffImage.threshold(threshold);
        
	}
    //OpenCV
    int r =0;
    int n=0;
    unsigned char * pixels = videoDiffImage.getPixels();

    for (int i = 0; i < 13; i++){
        r++;
		for (int j = 0; j < 8; j++){
            n++;
            valuetemp = 0;
            value=0;
            for (int k = 0; k < 48; k+=4) {
                for (int l = 0; l < 58; l+=4) {
                    valuetemp = valuetemp + pixels[(58*j+l) * width + (48*i+k)];
                    value = valuetemp / (12 * 15);
                }
            }
            pct = ofMap(value, 0,255, 0,20);
            
            arr[8*i+j] = pct > 10 ? true : false;
		}
	}
//      cout << int(arr[0]) << endl;
    
    
    
    bytesToSend[0] = 255;
    for (int i=0; i<15; i++) {
        bytesToSend[i+1] = 0;
    }

    
    for (int i=0; i < 104; i++) {
        if(arr[i] == true){
            bytesToSend[1 + (i/7)] |= (0x00000001 << (i%7));
        }
    }
    
    mySerial.writeBytes(bytesToSend, 16);

    
    cout << "------------------------------" << endl;
    cout <<  "bytesToSend[1] "<< " is  " << (int)bytesToSend[1] << endl;
    cout <<  "bytesToSend[2] "<< " is  " << (int)bytesToSend[2] << endl;
    cout <<  "bytesToSend[3] "<< " is  " << (int)bytesToSend[3] << endl;
    cout <<  "bytesToSend[4] "<< " is  " << (int)bytesToSend[4] << endl;
    cout <<  "bytesToSend[5] "<< " is  " << (int)bytesToSend[5] << endl;
    cout <<  "bytesToSend[6] "<< " is  " << (int)bytesToSend[6] << endl;
    cout <<  "bytesToSend[7] "<< " is  " << (int)bytesToSend[7] << endl;
    cout <<  "bytesToSend[8] "<< " is  " << (int)bytesToSend[8] << endl;
    cout <<  "bytesToSend[9] "<< " is  " << (int)bytesToSend[9] << endl;
    cout <<  "bytesToSend[10]"<< " is " << (int)bytesToSend[10] << endl;
    cout <<  "bytesToSend[11]"<< " is " << (int)bytesToSend[11] << endl;
    cout <<  "bytesToSend[12]"<< " is " << (int)bytesToSend[12] << endl;
    cout <<  "bytesToSend[13]"<< " is " << (int)bytesToSend[13] << endl;
    cout <<  "bytesToSend[14]"<< " is " << (int)bytesToSend[14] << endl;
    cout <<  "bytesToSend[15]"<< " is " << (int)bytesToSend[15] << endl;

	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(255, 255, 255);
	videoDiffImage.draw(20,40);
	panel.draw();
    // visualize servo rotation
    
    for (int i = 0; i < 104; i++){
        if (arr[i]) ofSetColor(255,0,0);
        else ofSetColor(255);
        ofRect(624 + 80 + (i/8)*48, 40 + (i%8)*58, 30,30);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	panel.mouseDragged(x,y,button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	panel.mousePressed(x,y,button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	panel.mouseReleased();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}
