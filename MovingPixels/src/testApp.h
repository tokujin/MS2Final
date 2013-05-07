#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxControlPanel.h"


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
    
    // create a variable of the type ofImage
    ofxControlPanel			panel;
	
    int						width, height;
    ofVideoGrabber			video;
    ofxCvColorImage			videoColorCvImage;
    ofxCvGrayscaleImage		videoGrayscaleCvImage;
    ofxCvGrayscaleImage		videoBgImage;
	
    ofxCvGrayscaleImage		videoDiffImage;
    ofxCvGrayscaleImage     modifiedImage;
    
    
    float value, valuetemp;
    
    unsigned char * pixels;
    unsigned char * mpixels;
    
    float pct = 0;
    
    ofSerial mySerial;
    unsigned char arr[104];  //output to
    unsigned char bytesToSend[16];  //output to
    int test;

};

#endif