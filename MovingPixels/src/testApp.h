#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxMacamPs3Eye.h"
#include "ofEvents.h"


//#include "ofxControlPanel.h"


class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    ofVideoGrabber video;
    int 				camWidth;
    int 				camHeight;
    
    
    ofxMacamPs3Eye ps3eye;
    ofxCvColorImage			videoColorCvImage;
    ofxCvGrayscaleImage		videoGrayscaleCvImage;
    ofxCvGrayscaleImage		videoBgImage;
    ofxCvGrayscaleImage		videoDiffImage;
    
    float value, valuetemp;
    int x[104]  = {}; //output to
	
    unsigned char * pixels;
    
	ofTrueTypeFont		font;
    ofTrueTypeFont      smallFont;
	ofArduino	ard;
	bool		bSetupArduino;			// flag variable for setting up arduino once
    
private:
    
    void setupArduino(const int & version);
    void digitalPinChanged(const int & pinNum);
    void analogPinChanged(const int & pinNum);
	void updateArduino();
    
    string buttonState;
    string potValue;
    
    
    
};

#endif
