#include "ofApp.h"
#include "ofxTimeMeasurements.h"
#include "ofxFastFboReader.h"

ofFbo fbo;

int mode = 0;

ofxFastFboReader reader;

ofImage image;
ofPixels pix;

void ofApp::setup() {

    //ofFbo settings
    float w, h;
    w = ofGetWidth();
    h = ofGetHeight();
    cap_Fbo_Settings.width = w;
    cap_Fbo_Settings.height = h;

    cap_Fbo_Settings.internalformat = GL_RGB;
    //cap_Fbo_Settings.internalformat = GL_RGBA;
    //cap_Fbo_Settings.internalformat = GL_RGB32F;
    cap_Fbo_Settings.useDepth = true;//3d depth
    cap_Fbo_Settings.useStencil = true;//?
    //cap_Fbo_Settings.depthStencilAsTexture = true;//?

    //The problem: Can't antialias -->

    //->this breakes the recording...grey screen
    cap_Fbo_Settings.numSamples = 32;

    cap_Fbo.allocate(cap_Fbo_Settings);

    blitFbo.allocate(cap_Fbo.getWidth(), cap_Fbo.getHeight(), GL_RGB);

    //--

    //hq
    cap_Recorder.setBitRate(80000);
    cap_Recorder.setup(true, false, glm::vec2(w, h), 60);

    //options
    
    //cap_Recorder.setOverWrite(true);
    //cap_Recorder.setFFmpegPathToAddonsPath();
    cap_Recorder.setFFmpegPath(ofToDataPath("ffmpeg"));

    shader.load("", "shader/Mondria.frag");
    zoom = 1;

}

void ofApp::update() {
}

void ofApp::draw() {

    float w, h;
    w = ofGetWidth();
    h = ofGetHeight();

    cap_Fbo.begin();
    {
        shader.begin();
        shader.setUniform1f("time", ofGetElapsedTimef());
        shader.setUniform1f("zoom", zoom * zoom);
        shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
        shader.setUniform2f("mouse", ofGetMouseX(), ofGetMouseY());
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        shader.end();
    }
    cap_Fbo.end();

    //-

    if (cap_Recorder.isRecording())
    {
        blitFbo.begin();
        ofClear(0);
        cap_Fbo.draw(0, 0, w, h);
        blitFbo.end();

        cap_Reader.readToPixels(blitFbo, cap_Pix, OF_IMAGE_COLOR);
        if (cap_Pix.getWidth() > 0 && cap_Pix.getHeight() > 0) {
            cap_Recorder.addFrame(cap_Pix);
        }
    }

    //-

    cap_Fbo.draw(0, 0);//drawing is required outside fbo
}


void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
	zoom += scrollY * 0.04;
}

void ofApp::keyPressed(int key)
{

}

void ofApp::keyReleased(int key) {

    switch (key) {
    case 'r':
        bRecording = !bRecording;

        if (cap_Recorder.isRecording()) {
            cap_Recorder.stop();
        }
        else {
            cap_Recorder.setOutputPath(ofToDataPath(".mp4", true));
            cap_Recorder.startCustomRecord();
        }
        break;

    default:
        break;
    }

    /*
    if (key == ' ') {

        if (cap_Recorder.isRecording()) {
            // stop
            cap_Recorder.stop();
        }
        else {
#if defined(TARGET_OSX)
            m_Recorder.setOutputPath(ofToDataPath(ofGetTimestampString() + ".mp4", true));
#else
            cap_Recorder.setOutputPath(ofToDataPath(ofGetTimestampString() + ".avi", true));
#endif
            cap_Recorder.startCustomRecord();
        }
    }
    else if (key == 't') {
        cap_Recorder.saveThumbnail(0, 0, 2, "thumbnail.png", ofVec2f(0, 0), ofRectangle(0, 0, 500, 400));
    }
    */
}

void ofApp::mouseMoved(int x, int y)
{

}

void ofApp::mouseDragged(int x, int y, int button)
{

}

void ofApp::mousePressed(int x, int y, int button)
{

}

void ofApp::mouseReleased(int x, int y, int button)
{

}

void ofApp::mouseEntered(int x, int y)
{

}

void ofApp::mouseExited(int x, int y)
{

}

void ofApp::windowResized(int w, int h)
{

}

void ofApp::gotMessage(ofMessage msg)
{

}

void ofApp::dragEvent(ofDragInfo dragInfo)
{

}