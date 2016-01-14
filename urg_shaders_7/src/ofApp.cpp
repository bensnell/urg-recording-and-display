#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    generalDistance.setName("General Distance Scaling");
    generalDistance.add(minPointDist.set("Min Point Dist", 0., 0, 100.));
    generalDistance.add(maxPointDist.set("Max Point Dist", 10000., 0., 20000.));
    
    pointSize.setName("Point Size Scaling");
    pointSize.add(pointSizeMultiplier.set("Pt Size Mult", 5., 0., 100.));
    pointSize.add(pointSizePower.set("Pt Size Power", 1., 0., 20.));
    pointSize.add(minPointSize.set("Min Point Size", 0., 0., 10.));
    pointSize.add(maxPointSize.set("Max Point Size", 100., 1., 100.));
    
    pointOpacity.setName("Point Opacity Scaling");
    pointOpacity.add(pointOpacityMultiplier.set("Pt Op Mult", 1., 0., 1.));
    pointOpacity.add(pointOpacityPower.set("Pt Op Power", 2., 0., 5.));
    pointOpacity.add(minPointOpacity.set("Min Pt Opacity", 0., 0., .1));
    pointOpacity.add(maxPointOpacity.set("Max Pt Opacity", 1., 0., 1.));
    
    placement.setName("Placement and Movement");
    placement.add(xTrans.set("X Translation", ofGetWidth()/2., -2000., 2000.));
    placement.add(yTrans.set("Y Translation", ofGetHeight()/2 + 200, -2000., 2000.));
    placement.add(rotationalSpeed.set("Rot Speed", 0.1, -0.5, 0.5));
    placement.add(zoomSpeed.set("Zoom Speed", 0.1, -100., 100.));
    placement.add(translationalSpeed.set("Trans Speed", 1., -20., 20.));
    placement.add(mirrorX.set("Mirror X", false));
    placement.add(mirrorY.set("Mirror Y", false));
    placement.add(bSaveImages.set("Save Image Seq", false));
    placement.add(bPerlin.set("Perlin Noise", true));
    
    panel.setup();
    panel.add(generalDistance);
    panel.add(pointSize);
    panel.add(pointOpacity);
    panel.add(placement);
    
    panel.loadFromFile("settings.xml");
    
    // ----------------------------------
    // --------- ORIGINAL MESH ----------
    // ----------------------------------
    
    // load opaque mesh
    opqMesh.load("carnegie_museum3_opaque.ply");
    unsigned long opqVert = (opqMesh.getVertices()).size();
    
    // load transparent mesh
    tnsMesh.load("carnegie_museum3_transparent.ply");
    unsigned long tnsVert = (tnsMesh.getVertices()).size();
    
    // find total number of vertices
    nVertices = opqVert + tnsVert;
	
    // ----------------------------------
    // ------------- SHADER -------------
    // ----------------------------------
	
    // allocate space for shader
	billboards.getVertices().resize(nVertices);
	billboards.getColors().resize(nVertices);
	billboards.getNormals().resize(nVertices, ofVec3f(0));
    billboardSizeTarget = new float[nVertices];
    billboardVels = new ofVec3f[nVertices];
	
	// Add vertices to the billboard mesh
	for (unsigned long i = 0; i < nVertices; i++) {
		
		billboardVels[i].set(ofRandomf(), -1.0, ofRandomf());
        billboardSizeTarget[i] = ofRandom(4, 14);

        // opaque vertices
        if (i < opqVert) {
            billboards.getVertices()[i].set(opqMesh.getVertices()[i]);
            billboards.getColors()[i].set(ofColor(255, 255));
        }
        // transparent vertices
        else {
            billboards.getVertices()[i].set(tnsMesh.getVertices()[i - opqVert]);
            billboards.getColors()[i].set(ofColor(255, 50));
        }
	}
	
    // Set the mode of the shader
	billboards.setUsage( GL_DYNAMIC_DRAW );
	billboards.setMode(OF_PRIMITIVE_POINTS);
	//billboardVbo.setVertexData(billboardVerts, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	//billboardVbo.setColorData(billboardColor, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	
    // Load the correct billboard shader
	if(ofGetGLProgrammableRenderer()){
		billboardShader.load("shadersGL3/Billboard noise");
	}else{
		billboardShader.load("shadersGL2/Billboard noise");
	}
	
	// disable ARB textures in order to use normalized texcoords
	ofDisableArbTex();
	texture.loadImage("textures/dot4.png");
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::update() {
    
    if (bConstantSlide) {
        zoomTarget += zoomSpeed;
    }
    
    // ease the zoom and rotation of the camera
	zoom += (zoomTarget - zoom) * 0.03;
    cameraRotation.y += (yRotTarget - cameraRotation.y) * 0.02;
    xTrans += (xTransTarget - xTrans) * 0.02;
    
    // save image to file
    if (bSaveImages) {
        ofSaveFrame();
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0);
	
	ofSetColor(255);
    
    huntForBlendFunc(1000, 4, 5); // 4 5 is good
    
	ofPushMatrix();
    
	ofTranslate(xTrans, yTrans, 0);
    ofRotate(cameraRotation.y, 0, 1, 0);
	ofRotate(cameraRotation.x, 1, 0, 0);
	ofRotate(cameraRotation.z, 0, 0, 1);
    ofTranslate(0, 0, zoom);
    ofScale(1 - 2 * mirrorX, 1 - 2 * mirrorY, 1);
    ofScale(0.4, 0.4, 0.4);
	
	// bind the shader so that we can change the
	// size of the points via the vert shader
	billboardShader.begin();
    
    billboardShader.setUniform1f("time", ofGetElapsedTimeMillis());
    billboardShader.setUniform1f("seed", 0.9389124);
    billboardShader.setUniform1fv("weights", &weights[0]);
    billboardShader.setUniform1fv("scaling", &scaling[0]);
    billboardShader.setUniform1f("normalization", 100.0);
    
    billboardShader.setUniform1f("minPointDist", minPointDist);
    billboardShader.setUniform1f("maxPointDist", maxPointDist);
    
    billboardShader.setUniform1f("pointSizeMultiplier", pointSizeMultiplier);
    billboardShader.setUniform1f("pointSizePower", pointSizePower);
    billboardShader.setUniform1f("minPointSize", minPointSize);
    billboardShader.setUniform1f("maxPointSize", maxPointSize);
    
    billboardShader.setUniform1f("pointOpacityMultiplier", pointOpacityMultiplier);
    billboardShader.setUniform1f("pointOpacityPower", pointOpacityPower);
    billboardShader.setUniform1f("minPointOpacity", minPointOpacity);
    billboardShader.setUniform1f("maxPointOpacity", maxPointOpacity);
    
    billboardShader.setUniform1i("bPerlin", int(bPerlin));
    
	ofEnablePointSprites(); // not needed for GL3/4
	texture.getTextureReference().bind();
	billboards.draw();
	texture.getTextureReference().unbind();
	ofDisablePointSprites(); // not needed for GL3/4
	
	billboardShader.end();
    
	ofPopMatrix();
    
    if (!bHideControls) {
        panel.draw();
        ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10, 20);
    }
}

//-------------------------------------------------------------

void ofApp::exit(){
    panel.saveToFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
	if(key == 'f') ofToggleFullscreen();
	if(key == OF_KEY_UP) zoomTarget += zoomSpeed;
	if(key == OF_KEY_DOWN) zoomTarget -= zoomSpeed;
    if(key == OF_KEY_LEFT) yRotTarget -= rotationalSpeed;
    if(key == OF_KEY_RIGHT) yRotTarget += rotationalSpeed;
    if(key == 's') bConstantSlide = !bConstantSlide;
    if(key == 'h') bHideControls = !bHideControls;
    if(key == 'c') {
        if (bHideCursor) {
            bHideCursor = false;
            ofHideCursor();
        } else {
            bHideCursor = true;
            ofShowCursor();
        }
    }
    if (key == 'a') xTransTarget += translationalSpeed;
    if (key == 'd') xTransTarget -= translationalSpeed;
}

//--------------------------------------------------------------
void ofApp::huntForBlendFunc(int period, int defaultSid, int defaultDid){
    // sets all possible combinations of blend functions,
    // changing modes every [period] milliseconds.
    
    // All checked out, works well.
    
    int sfact[] = {
        GL_ZERO,
        GL_ONE,
        GL_DST_COLOR,
        GL_ONE_MINUS_DST_COLOR,
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA,
        GL_DST_ALPHA,
        GL_ONE_MINUS_DST_ALPHA,
        GL_SRC_ALPHA_SATURATE
    };
    
    int dfact[] = {
        GL_ZERO,
        GL_ONE,
        GL_SRC_COLOR,
        GL_ONE_MINUS_SRC_COLOR,
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA,
        GL_DST_ALPHA,
        GL_ONE_MINUS_DST_ALPHA
    };
    
    
    
    glEnable(GL_BLEND);
    
    if ((defaultSid == -1) && (defaultDid == -1)) {
        
        int sid =  (ofGetElapsedTimeMillis()/(8*period))%9;
        int did =  (ofGetElapsedTimeMillis()/period)%8;
        glBlendFunc(sfact[sid], dfact[did]);
        // ofLog(OF_LOG_NOTICE, "SRC %d	DST %d\n", sid, did);
        printf("SRC %d	DST %d\n", sid, did);
        
    } else if (defaultDid == -1){
        
        int did =  (ofGetElapsedTimeMillis()/period)%8;
        glBlendFunc(sfact[defaultSid], dfact[did]);
        // ofLog(OF_LOG_NOTICE, "SRC %d	DST %d\n", defaultSid, did);
        printf("SRC %d	DST %d\n", defaultSid, did);
        
    } else if (defaultSid == -1){
        
        int sid =  (ofGetElapsedTimeMillis()/(8*period))%9;
        glBlendFunc(sfact[sid], dfact[defaultDid]);
        // ofLog(OF_LOG_NOTICE, "SRC %d	DST %d\n", sid, defaultDid);
        printf("SRC %d	DST %d\n", sid, defaultDid);
        
    } else {
        
        glBlendFunc(sfact[defaultSid], dfact[defaultDid]);
        
    }
    
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
