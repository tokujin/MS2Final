#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
	// setup video grabber:
	video.initGrabber(624, 464);
	
	// get the width and height, and allocate color and grayscale images:
	width = video.width;
	height = video.height;
	
    //	videoColorCvImage.allocate(width, height);
	videoGrayscaleCvImage.allocate(width, height);
	videoBgImage.allocate(width, height);
	videoDiffImage.allocate(width, height);
	
	// set background color to be white:
	ofBackground(0, 0, 0);
	
	
	panel.setup("cv settings", 1380, 0, 300, 748);
	panel.addPanel("control", 1, false);
	
	panel.setWhichPanel("control");
	panel.setWhichColumn(0);
	panel.addToggle("learn background ", "B_LEARN_BG", true);
	panel.addSlider("threshold ", "THRESHOLD", 127, 0, 255, true);
	
	
	panel.loadSettings("cvSettings.xml");
}

//--------------------------------------------------------------
void testApp::update(){
    
	panel.update();
	
	
	bool bLearnBg = panel.getValueB("B_LEARN_BG");
	int threshold = panel.getValueI("THRESHOLD");
	
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
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(255, 255, 255);
	videoDiffImage.draw(20,40);
    int r =0;
    int n=0;
    unsigned char * pixels = videoDiffImage.getPixels();
    for (int i = 0; i < width; i+=48){
        r++;
		for (int j = 0; j < height; j+=58){
            n++;
            valuetemp = 0;
            value=0;
            for (int k = 0; k < 48; k+=4) {
                for (int l = 0; l < 58; l+=4) {
                    valuetemp = valuetemp + pixels[(j+l) * width + (i+k)];
                    value = valuetemp / (12 * 15);
                }
            }
            float pct = ofMap(value, 0,255, 0,20);
            
            // THIS DATA WILL BE SENT TO ARDUINO
            if (pct > 10) {
                arr[8*i+j] = true; //COLUMN BECOMES ROW, ROW BECOMES COLLUMN HERE
            }else{
                arr[8*i+j] = false;
            }
            if (pct > 10){
                ofSetColor(155,ofRandom(255),ofRandom(255));
                ofCircle(600 + 80 + i, 69+ j, pct);
            }
		}
	}
    
    
	panel.draw();
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

