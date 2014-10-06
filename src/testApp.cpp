#include "testApp.h"

//bool bFullscreen = false; //not allowed with 1.7.4

int webTexWidth,webTexHeight,boundBox,imageCounter,bodyCounter;

bool isResizing;

float scaleBox,scaleHaar,scaleEye,scaleBody,scaleCapture;

bool textFace,bSnapshot;

bool bFace,bBody,bEye;

float fX,fY,fW,fH,fCY,fCX;

string serverName = "/Users/mty1245/github_tester/node/socket/public/img/";

//float bX,bY,bW,bH,bCY,bCX;
//float eX,eY,eW,eH,eCY,eCX;

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);

    bSnapshot = false;
    isResizing = false;
    
    scaleHaar = 150;
    scaleBox = 0.25;
    boundBox = 100;
    scaleCapture = 70;
    imageCounter = 0;
   
    webTexWidth = ofGetWindowWidth();
	webTexHeight = ofGetWindowHeight();
    
	browser.setup(webTexWidth ,webTexHeight);
    
//	moustache.loadImage("moustache.png");
    
	browser.loadURL("https://map.google.com");
	
	finder.setup("haar/haarcascade_frontalface_default.xml");
//  body.setup("haar/haarcascade_fullbody.xml");
//	eyes.setup("haar/eye_alt.xml");
    
    scale.set(scaleBox, scaleBox);
    
    
    
    //    UI -------------------------------------------------------
    gui = new ofxUISuperCanvas("VARIABLE BINDING");
    gui->addSpacer();
    gui->addLabel("BACKGROUND", OFX_UI_FONT_MEDIUM);
    gui->addSpacer();
    gui->addSlider("scaleBox", 0, 1, scaleBox);
    gui->addSlider("scaleHaar", 10,200, scaleHaar);
    gui->addSlider("boundBox", -200,200, boundBox);
    gui->addSlider("capture Scale", 0,300, scaleCapture);
    gui->addSpacer();
    gui->addToggle("Find Face", false);
    gui->addToggle("Find Body", false);
    gui->addToggle("Find Eyes", false);
    gui->autoSizeToFitWidgets();
    
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
    gui->loadSettings("guiSettings.xml");
    
    // E UI --------------------------------------------------------
    
    
}

//--------------------------------------------------------------
void testApp::update(){
	
	ofSetWindowTitle(ofToString(ofGetFrameRate())+"fps");
	ofxAwesomium::updateCore();
    bool bIsFrameNew = browser.update();
    isResizing = false;
	
	if(bIsFrameNew) {
        
        
		grayFrame = browser.frame;
		grayFrame.setImageType(OF_IMAGE_GRAYSCALE);
        //ofPoint size = ofGetWindowSize()*scale; // if using direct rectangle
        //grayFrame.resize(size.x, size.y);// if using direct rectangle
		
        
        if (bFace == true){
        finder.findHaarObjects(grayFrame,scaleHaar);
        }
        
       /*
        if (bBody == true) {
        body.findHaarObjects(grayFrame,scaleHaar);
        }
        if (bEye == true) {
        eyes.findHaarObjects(grayFrame,scaleHaar);
        }*/
        
        
        
        if (bSnapshot == true){
            
            if(ofGetFrameNum() % 30 == 0){
                //faceSaver.getPixelsRef() = browser.frame.getPixels
                faceSaver.grabScreen(fX-scaleCapture/2,fY-scaleCapture/2, fW+scaleCapture, fH+scaleCapture);
                //faceSaver.grabScreen(0,0, 900, 500); //debug
                string fileName = serverName + "face_"+ofToString(imageCounter)+".jpg";
                faceSaver.saveImage(fileName);
                bSnapshot = false;
                imageCounter = imageCounter + 1;
                
            
            }
        
        }
    
        if (ofGetFrameNum() % 100 == 0){
            screenSaver.grabScreen(0,0, ofGetWindowWidth(),ofGetWindowHeight());
            string screenName = serverName + "screen_"+ ofToString(imageCounter)+".jpg";
            screenSaver.saveImage(screenName);
            
        }
    
    
    }
	
    
    
	//ofSetWindowTitle(browser.getTitle());

}

//--------------------------------------------------------------
void testApp::draw(){
	if(browser.getIsLoading()) {
		ofSetColor(0);
		ofDrawBitmapString("Loading...", 10, 15);
	}
	
	ofSetColor(255);
    
    if (isResizing == false) {
        browser.draw(0, 0);
    }else {
        
        
    }
    if (bFace == true){
    
	for(unsigned int i = 0; i < finder.blobs.size(); i++) {
        
         bSnapshot = true;
        
        fX = finder.blobs[i].boundingRect.x+boundBox/2;
        fY = finder.blobs[i].boundingRect.y+boundBox/2;
        fW= finder.blobs[i].boundingRect.width-boundBox;
		fH = finder.blobs[i].boundingRect.height-boundBox;
		fCX = finder.blobs[i].centroid.x;
		fCY = finder.blobs[i].centroid.y;
        
        
        ofSetColor(255,0,0);
        ofNoFill();
        ofRect(fX, fY, fW, fH);
        ofSetHexColor(0xFFFFFF);
        ofDrawBitmapString("FACE", fX, fY-40);
        
       
    
//		ofRectangle cur = finder.blobs[i].boundingRect; // direct rectangle
//		cur.x *= 1/scale.x;
//		cur.y *= 1/scale.y;
//		cur.width *= 1/scale.x;
//		cur.height *= 1/scale.y;
//        
//		moustache.draw(cur);
//        ofNoFill();
//        ofSetColor(255);
//        ofSetLineWidth(4);
//      ofRect(cur.x, cur.y, cur.width, cur.height);
        
        
        glPopMatrix(); // S infos
        ofSetHexColor(0x000000);
        string reportStr= "matches found: "+ ofToString(finder.blobs.size(), 0)+", framerate: "+ofToString(ofGetFrameRate(),0);
        string boxsize = "boundBox size: "+ofToString(boundBox,3);
        ofDrawBitmapString(reportStr, 20, 10);
        ofDrawBitmapString(boxsize, 20, 40); //E info
        
            }
         
     }
/*
 if (bBody == true){
    
	for(unsigned int i = 0; i < body.blobs.size(); i++) {
        
        bodyCounter = bodyCounter + 1;
        
        float x = body.blobs[i].boundingRect.x+boundBox/2;
		float y = body.blobs[i].boundingRect.y+boundBox/2;
		float w = body.blobs[i].boundingRect.width-boundBox;
		float h = body.blobs[i].boundingRect.height-boundBox;
        
		float cx = body.blobs[i].centroid.x;
		float cy = body.blobs[i].centroid.y;
        
        
        ofSetColor(0,255,0);
        ofNoFill();
        ofRect(x, y, w, h);
        ofSetHexColor(0xFFFFFF);
        ofDrawBitmapString("HUMAN", x+20, y+20);
        
        bodySaver.grabScreen(-x,-y, w,h);
        string fileName = "body_"+ofToString(bodyCounter)+".jpg";
        bodySaver.saveImage(fileName);
    
    }
 }
    
    if (bEye == true){
        
        for(unsigned int i = 0; i < eyes.blobs.size(); i++) {
            
            bodyCounter = bodyCounter + 1;
            
            float x = eyes.blobs[i].boundingRect.x+boundBox/2;
            float y = eyes.blobs[i].boundingRect.y+boundBox/2;
            float w = eyes.blobs[i].boundingRect.width-boundBox;
            float h = eyes.blobs[i].boundingRect.height-boundBox;
            
            float cx = eyes.blobs[i].centroid.x;
            float cy = eyes.blobs[i].centroid.y;
            
            
            ofSetColor(0,255,0);
            ofNoFill();
            ofRect(x, y, w, h);
            ofSetHexColor(0xFFFFFF);
            ofDrawBitmapString("HUMAN", x+20, y+20);
            
            eyesSaver.grabScreen(-x,-y, w,h);
            string fileName = "body_"+ofToString(bodyCounter)+".jpg";
            eyesSaver.saveImage(fileName);
        
        
        
        }
    }

    
*/
    
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
    
    switch (key)
    {
        case 'd':
        {
            gui->toggleVisible();
        }
            break;
            default:
            break;
    }
    
    browser.keyPressed(key); //sends keys to browser context
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	browser.keyReleased(key);
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	browser.mouseMoved(x, y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	browser.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	browser.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	browser.mouseReleased(x, y, button);
}


//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
    isResizing = true;
    webTexWidth = w;
	webTexHeight = h;
    browser.resizeWindow(w, h);
    
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}
//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	
	if(name == "scaleBox")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		scaleBox= slider->getScaledValue();
	}else if(name == "scaleHaar"){
        ofxUISlider *slider = (ofxUISlider *) e.widget;
		scaleHaar= slider->getScaledValue();
        
    }
    else if(name == "capture Scale"){
        ofxUISlider *slider = (ofxUISlider *) e.widget;
		scaleCapture= slider->getScaledValue();
        
    }
    else if(name == "boundBox"){
        ofxUISlider *slider = (ofxUISlider *) e.widget;
		boundBox= slider->getScaledValue();
        
    }
    else if(name == "Find Face"){
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		bFace= toggle->getValue();
        
    }
    else if(name == "Find Body"){
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		bBody= toggle->getValue();
        
    }
    else if(name == "Find Eyes"){
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		bEye= toggle->getValue();
        
    }

}
//--------------------------------------------------------------
void testApp::exit(){
    gui->saveSettings("guiSettings.xml");
    delete gui;
    
}

