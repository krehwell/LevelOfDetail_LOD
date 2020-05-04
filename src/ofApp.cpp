#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(255, 255, 255);
	ofSetVerticalSync(true);

	// primitive setup
	sphereSingle.setResolution(100);
	sphereSingle.setPosition(0, 0, 0);
	sphereSingle.setRadius(200);

	// model setup
	model.loadModel("catOLOD.obj");
	model.setPosition(0, -200, 0);
	model.setRotation(0, 180, 1, 0, 3);
	model.setScale(modelScaled, modelScaled, modelScaled);
	objSize = catSize;

	//adding mesh to count the number of vertices.
	//tex.load("cat.jpg");
	mesh = model.getMesh(i);
	catVertexNum = mesh.getNumVertices();

	// resize the mash if uses - call meshNode.transformGL(); to use
	meshNode.setGlobalOrientation(glm::angleAxis(ofDegToRad(180.f), glm::vec3{ 0.f, 1.f, 0.f }));
	meshNode.setGlobalPosition(0, -120, 0);
	meshNode.rotate(200, 0, 1,0);
	meshNode.setScale(modelScaled * 400);

	// loading texture
	ofDisableArbTex();
	ofLoadImage(texture, "cat.jpg");

	// camera setup
	cam.setDistance(600);

	//gui setup
	gui.setup("PANEL");
	gui.setPosition(10, 60);
	gui.add(vertexMode.set("VERTEX MODE", false));
	gui.add(wireframeMode.set("WIREFRAME MODE", false));
	gui.add(modelScaled.set("OBJECT SCALING", 0.9f, 0.1f, 10.0f));
	gui.add(camPosition.set("CAMERA POSITION", glm::vec3(cam.getX(), cam.getY(), cam.getZ()), glm::vec3(0, 0, 0), glm::vec3(360.0f, 360.0f, 360.0f)));
	gui.add(color.set("BACKGROUND COLOR", ofColor(255, 255, 255), ofColor(0, 0), ofColor(255, 255)));
	gui.add(screenSize.set("screenSize", ""));

	// gui-1 setup
	gui1.setup("OBJECT SELECTOR");
	gui1.setPosition(ofGetWidth()-210, 0);
	gui1.add(priVisual.set("PRIMITIVE VISUALIZE ", false));
	gui1.add(previewAllPrimitive.set("ANIMATED PRIMITIVE", false));

	// gui-2 setup
	gui2.setup("OPTIONS");
	gui2.setPosition(ofGetWidth()-210, 60);
	gui2.add(camHold.set("HOLD CAMERA", false));
	gui2.add(faceHold.set("RENDER FACE", true));

	initSetup();
}

void ofApp::initSetup() {
	ofDisableArbTex();

	float width = ofGetWidth() * .12;
	float height = ofGetHeight() * .12;

	sphere.setResolution(4);
	// icosahedron //
	icoSphere.setResolution(0); // number of subdivides //
	plane.setResolution(3, 2);
	cylinder.setResolution(4, 2, 0);
	cone.setResolution(4, 1, 0);
	box.setResolution(1);

	// init primitive object
	plane.set(width*1.5, height*1.5);
	box.set(width*1.25);
	sphere.setRadius(width);
	icoSphere.setRadius(width);
	cylinder.set(width*.7, height*2.2);
	cone.set(width*.75, height*2.2);

	ofSetSmoothLighting(true);
	pointLight.setDiffuseColor(ofFloatColor(.85, .85, .55));
	pointLight.setSpecularColor(ofFloatColor(0.4f, 0.7f, 1.f));

	pointLight2.setDiffuseColor(ofFloatColor(238.f / 255.f, 57.f / 255.f, 135.f / 255.f));
	pointLight2.setSpecularColor(ofFloatColor(.8f, .8f, .9f));

	pointLight3.setDiffuseColor(ofFloatColor(19.f / 255.f, 94.f / 255.f, 77.f / 255.f));
	pointLight3.setSpecularColor(ofFloatColor(18.f / 255.f, 150.f / 255.f, 135.f / 255.f));

	// shininess is a value between 0 - 128, 128 being the most shiny //
	material.setShininess(120);
	// the light highlight of the material //
	material.setSpecularColor(ofColor(255, 255, 255, 255));

	icosphereMesh = icoSphere.getMesh();
	sphereMesh = sphere.getMesh();
	planeMesh = plane.getMesh();
	coneMesh = cone.getMesh();
	cylinderMesh = cylinder.getMesh();
	boxMesh = box.getMesh();
}

//--------------------------------------------------------------
void ofApp::update() {
	pointLight.setPosition((ofGetWidth()*.5) + cos(ofGetElapsedTimef()*.5)*(ofGetWidth()*.3), ofGetHeight() / 2, 500);
	pointLight2.setPosition((ofGetWidth()*.5) + cos(ofGetElapsedTimef()*.15)*(ofGetWidth()*.3),
		ofGetHeight()*.5 + sin(ofGetElapsedTimef()*.7)*(ofGetHeight()), -300);

	pointLight3.setPosition(
		cos(ofGetElapsedTimef()*1.5) * ofGetWidth()*.5,
		sin(ofGetElapsedTimef()*1.5f) * ofGetWidth()*.5,
		cos(ofGetElapsedTimef()*.2) * ofGetWidth()
	);
}

void ofApp::initDraw() {
	float spinX = sin(ofGetElapsedTimef()*.35f);
	float spinY = cos(ofGetElapsedTimef()*.075f);

	if (bMousePressed) {
		spinX = spinY = 0.0f;
	}

	if (camHold) {
		cam.setGlobalPosition({ 0,0,cam.getImagePlaneDistance(ofGetCurrentViewport()) });
	}

	cam.begin();

	ofEnableDepthTest();

	ofEnableLighting();
	pointLight.enable();
	pointLight2.enable();
	pointLight3.enable();

	// draw the outer sphere
	material.begin();
	ofNoFill();
	ofDrawSphere(0, 0, max(ofGetWidth(), ofGetHeight()));
	material.end();

	if (mode == 2) vidGrabber.getTexture().bind();

	float screenWidth = ofGetWidth();
	float screenHeight = ofGetHeight();

	plane.setPosition(-screenWidth * .5 + screenWidth * 1 / 4.f, screenHeight *  1.1 / 6.f, 0);
	box.setPosition(-screenWidth * .5 + screenWidth * 2 / 4.f, screenHeight *  1.1 / 6.f, 0);
	sphere.setPosition(-screenWidth * .5 + screenWidth * 3 / 4.f, screenHeight *  1.1 / 6.f, 0);
	icoSphere.setPosition(-screenWidth * .5 + screenWidth * 1 / 4.f, screenHeight * -1.1 / 6.f, 0);
	cylinder.setPosition(-screenWidth * .5 + screenWidth * 2 / 4.f, screenHeight * -1.1 / 6.f, 0);
	cone.setPosition(-screenWidth * .5 + screenWidth * 3 / 4.f, screenHeight * -1.1 / 6.f, 0);

	// Plane //

	plane.rotateDeg(spinX, 1.0, 0.0, 0.0);
	plane.rotateDeg(spinY, 0, 1.0, 0.0);



	if (mode == 3) {
		deformPlane = plane.getMesh();
		// x = columns, y = rows //
		glm::vec2 planeDims = plane.getResolution();
		float planeAngleX = ofGetElapsedTimef()*3.6;
		float planeAngleInc = 3.f / (float)planeDims.x;
		glm::vec3 vert;
		for (size_t i = 0; i < deformPlane.getNumIndices(); i++) {
			planeAngleX += planeAngleInc;
			int ii = deformPlane.getIndex(i);
			vert = deformPlane.getVertex(ii);
			vert.z += cos(planeAngleX) * 50;
			deformPlane.setVertex(ii, vert);
		}
	}

	if (!faceHold && wireframeMode) {
		// if we are only drawing the wireframe, use
		// the material to draw it, otherwise the material
		// will be bound and unbound for every geometry
		// and the wireframe will be drawn in black
		material.begin();
	}

	if (faceHold) {
		material.begin();
		ofFill();
		if (mode == 3) {
			plane.transformGL();
			deformPlane.draw();
			plane.restoreTransformGL();
		}
		else {
			plane.draw();
		}
		material.end();
	}

	if (wireframeMode) {
		ofNoFill();
		glColor3f(0, 0, 0);
		plane.setPosition(plane.getPosition().x, plane.getPosition().y, plane.getPosition().z + 1);
		plane.drawWireframe();
		plane.setPosition(plane.getPosition().x, plane.getPosition().y, plane.getPosition().z - 1);

	}

	// Box //

	box.rotateDeg(spinX, 1.0, 0.0, 0.0);
	box.rotateDeg(spinY, 0, 1.0, 0.0);

	if (faceHold) {
		material.begin();
		ofFill();
		if (mode == 3) {
			box.transformGL();
			for (int i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++) {
				ofPushMatrix();
				ofTranslate(boxSides[i].getNormal(0) * sin(ofGetElapsedTimef()) * 50);
				boxSides[i].draw();
				ofPopMatrix();
			}
			box.restoreTransformGL();
		}
		else {
			box.draw();
		}
		material.end();
	}

	if (wireframeMode) {
		ofNoFill();
		glColor3f(0, 0, 0);
		box.setScale(1.01f);
		box.drawWireframe();
		box.setScale(1.f);
	}


	// Sphere //
	sphere.rotateDeg(spinX, 1.0, 0.0, 0.0);
	sphere.rotateDeg(spinY, 0, 1.0, 0.0);

	if (mode == 3) {
		sphere.setMode(OF_PRIMITIVE_TRIANGLES);
		triangles = sphere.getMesh().getUniqueFaces();
	}

	if (faceHold) {
		material.begin();
		ofFill();
		if (mode == 3) {
			float angle = ofGetElapsedTimef()*3.2;
			float strength = (sin(angle + .25)) * .5f * 5.f;
			glm::vec3 faceNormal;
			for (size_t i = 0; i < triangles.size(); i++) {
				// store the face normal here.
				// we change the vertices, which makes the face normal change
				// every time that we call getFaceNormal //
				faceNormal = triangles[i].getFaceNormal();
				for (int j = 0; j < 3; j++) {
					triangles[i].setVertex(j, triangles[i].getVertex(j) + faceNormal * strength);
				}
			}
			sphere.getMesh().setFromTriangles(triangles);
		}
		sphere.draw();
		material.end();
	}

	if (wireframeMode) {
		ofNoFill();
		glColor3f(0, 0, 0);
		sphere.setScale(1.01f);
		sphere.drawWireframe();
		sphere.setScale(1.f);
	}


	// ICO Sphere //

	icoSphere.rotateDeg(spinX, 1.0, 0.0, 0.0);
	icoSphere.rotateDeg(spinY, 0, 1.0, 0.0);

	if (mode == 3) {
		triangles = icoSphere.getMesh().getUniqueFaces();
	}

	if (faceHold) {
		material.begin();
		ofFill();

		if (mode == 3) {
			float angle = (ofGetElapsedTimef() * 1.4);
			glm::vec3 faceNormal;
			for (size_t i = 0; i < triangles.size(); i++) {
				float frc = ofSignedNoise(angle* (float)i * .1, angle*.05) * 4;
				faceNormal = triangles[i].getFaceNormal();
				for (int j = 0; j < 3; j++) {
					triangles[i].setVertex(j, triangles[i].getVertex(j) + faceNormal * frc);
				}
			}
			icoSphere.getMesh().setFromTriangles(triangles);
		}

		icoSphere.draw();
		material.end();
	}

	if (wireframeMode) {
		ofNoFill();
		glColor3f(0, 0, 0);
		icoSphere.setScale(1.01f);
		icoSphere.drawWireframe();
		icoSphere.setScale(1.f);
	}


	// Cylinder //
	if (mode == 3) {
		topCap = cylinder.getTopCapMesh();
		bottomCap = cylinder.getBottomCapMesh();
		body = cylinder.getCylinderMesh();
	}

	cylinder.rotateDeg(spinX, 1.0, 0.0, 0.0);
	cylinder.rotateDeg(spinY, 0, 1.0, 0.0);
	if (faceHold) {
		material.begin();
		ofFill();
		if (mode == 3) {
			cylinder.transformGL();
			ofPushMatrix(); {
				if (topCap.getNumNormals() > 0) {
					ofTranslate(topCap.getNormal(0) * (cos(ofGetElapsedTimef() * 5) + 1)*.5f * 100);
					topCap.draw();
				}
			} ofPopMatrix();
			ofPushMatrix(); {
				if (bottomCap.getNumNormals() > 0) {
					ofTranslate(bottomCap.getNormal(0) * (cos(ofGetElapsedTimef() * 4) + 1)*.5f * 100);
					bottomCap.draw();
				}
			} ofPopMatrix();
			ofPushMatrix(); {
				float scale = (cos(ofGetElapsedTimef() * 3) + 1)*.5f + .2;
				ofScale(scale, scale, scale);
				body.draw();
			} ofPopMatrix();
			cylinder.restoreTransformGL();
		}
		else {
			cylinder.draw();
		}
		material.end();
	}

	if (wireframeMode) {
		ofNoFill();
		glColor3f(0, 0, 0);
		cylinder.setScale(1.01f);
		cylinder.drawWireframe();
		cylinder.setScale(1.0f);
	}


	// Cone //
	cone.rotateDeg(spinX, 1.0, 0.0, 0.0);
	cone.rotateDeg(spinY, 0, 1.0, 0.0);

	if (mode == 3) {
		bottomCap = cone.getCapMesh();
		body = cone.getConeMesh();
	}
	if (faceHold) {
		material.begin();
		ofFill();
		if (mode == 3) {
			cone.transformGL();
			ofPushMatrix();
			if (bottomCap.getNumNormals() > 0) {
				ofTranslate(bottomCap.getNormal(0) * cone.getHeight()*.5);
				ofRotateDeg(sin(ofGetElapsedTimef() * 5) * RAD_TO_DEG, 1, 0, 0);
				bottomCap.draw();
			}
			ofPopMatrix();

			ofPushMatrix();
			ofRotateDeg(90, 1, 0, 0);
			ofRotateDeg((cos(ofGetElapsedTimef() * 6) + 1)*.5 * 360, 1, 0, 0);
			body.draw();
			ofPopMatrix();
			cone.restoreTransformGL();
		}
		else {
			cone.draw();
		}
		material.end();
	}

	if (wireframeMode) {
		ofNoFill();
		glColor3f(0, 0, 0);
		cone.setScale(1.01f);
		cone.drawWireframe();
		cone.setScale(1.0f);
	}

	if (!faceHold && wireframeMode) {
		material.end();
	}

	material.end();
	ofDisableLighting();

	if (bDrawLights) {
		ofFill();
		glColor3f(1, 2, 3);
		pointLight.draw();
		glColor3f(1,2,3);
		pointLight2.draw();
		glColor3f(1, 2, 3);
		pointLight3.draw();
	}

	if (bDrawNormals) {
		glColor3f(1,0,3);
		plane.drawNormals(20, bSplitFaces);
		box.drawNormals(20, bSplitFaces);
		sphere.drawNormals(20, bSplitFaces);
		icoSphere.drawNormals(20, bSplitFaces);
		cylinder.drawNormals(20, bSplitFaces);
		cone.drawNormals(20, bSplitFaces);
	}
	if (bDrawAxes) {
		plane.drawAxes(plane.getWidth()*.5 + 30);
		box.drawAxes(box.getWidth() + 30);
		sphere.drawAxes(sphere.getRadius() + 30);
		icoSphere.drawAxes(icoSphere.getRadius() + 30);
		cylinder.drawAxes(cylinder.getHeight() + 30);
		cone.drawAxes(cone.getHeight() + 30);
	}

	ofDisableDepthTest();

	ofFill();

	cam.end();
}

void ofApp::drawObject() {
	ofDisableArbTex();
	light.disable();

	if (cam.getDistance() > 300 && cam.getDistance() < 1000) {
		mesh = model.getMesh(2);
	}

	if (cam.getDistance() > 1000 && cam.getDistance() < 1800) {
		mesh = model.getMesh(1);
	}

	if (cam.getDistance() > 1800 && cam.getDistance() < 2900) {
		mesh = model.getMesh(0);
	}

	if (vertexMode) {
		ofEnableDepthTest();
		meshNode.transformGL();
		texture.bind();
		mesh.drawVertices();
		texture.unbind();
		meshNode.restoreTransformGL();
		ofDisableDepthTest();
	}
	else if (wireframeMode) {
		ofEnableDepthTest();
		meshNode.transformGL();
		texture.bind();
		mesh.drawWireframe();
		texture.unbind();
		meshNode.restoreTransformGL();
		ofDisableDepthTest();
	}
	else {
		// draw using 3d assimp
		/*ofEnableDepthTest();
		model.drawFaces();
		ofDisableDepthTest();*/

		// draw using meshes
		ofEnableDepthTest();
		meshNode.transformGL();
		texture.bind();
		mesh.drawFaces();
		texture.unbind();
		meshNode.restoreTransformGL();
		ofDisableDepthTest();
	}
	meshNode.setScale(modelScaled * objSize);
}

void ofApp::drawPrimitive() {
	// enable light
	ofEnableLighting();
	pointLight.enable();
	pointLight2.enable();
	pointLight3.enable();
	light.enable();
	material.begin();

	// set the color to the sphere
	glColor3f(0.2, 0.2, 0.1);

	if (cam.getDistance() > 300 && cam.getDistance() < 730) {
		sphereSingle.setResolution(100);
	}

	if (cam.getDistance() > 730 && cam.getDistance() < 1800) {
		sphereSingle.setResolution(80);
	}

	if (cam.getDistance() > 1800 && cam.getDistance() < 2900) {
		sphereSingle.setResolution(30);
	}

	if (cam.getDistance() > 2900) {
		sphereSingle.setResolution(10);
	}

	if (vertexMode) {
		ofEnableDepthTest();
		sphereSingle.drawVertices();
		ofDisableDepthTest();
	}
	else if (wireframeMode) {
		ofEnableDepthTest();
		sphereSingle.drawWireframe();
		ofDisableDepthTest();
	}
	else {
		ofEnableDepthTest();
		sphereSingle.drawFaces();
		ofDisableDepthTest();
	}

	sphereSingle.setScale(modelScaled);

	// disable the light
	light.disable();
	pointLight.disable();
	pointLight2.disable();
	pointLight3.disable();
	ofDisableLighting();
	material.end();
}

//--------------------------------------------------------------
void ofApp::draw() {
	// background - changing color following the slider of gui
	ofBackground(color);

	stringstream numMesh;

	cam.begin();

	if (priVisual) {
		ofMesh sphereMesh = sphereSingle.getMesh();
		numMesh << "SPHERE VERTICES: " << mesh.getNumVertices() << endl << "SPHERE RESOLUTION: " << sphereSingle.getResolution();
		drawPrimitive();
	}
	else if (previewAllPrimitive) {
		vertexMode.set("VERTEX MODE", false);
		numMesh << "PLANE VERTICES: " << planeMesh.getNumVertices() << endl
			<< "BOX VERTICES: " << boxMesh.getNumVertices() << endl
			<< "SPHERE VERTICES: " << sphereMesh.getNumVertices() << endl
			<< "ICOSPHERE VERTICES: " << icosphereMesh.getNumVertices() << endl
			<< "CYLINDER VERTICES: " << cylinderMesh.getNumVertices() << endl
			<< "CONE VERTICES: " << coneMesh.getNumVertices();
		initDraw();
	}
	else {
		// printing number of meshVertices
		numMesh << objName << " VERTICES: "<< mesh.getNumVertices();
		drawObject();
	}
	cam.end();

	camPosition.set("CAMERA POSITION", glm::vec3(cam.getX(), cam.getY(), cam.getZ()), glm::vec3(0,0,0), glm::vec3(360.0f, 360.0f, 360.0f));
	gui.draw();
	gui1.draw();

	// fps info
	stringstream ss;
	ss << "FPS: " << ofToString(ofGetFrameRate(), 0) << endl << "(double click to reset camera)";
	ofDrawBitmapStringHighlight(ss.str().c_str(), 20, 20);

	// printing info cat
	ofDrawBitmapStringHighlight(numMesh.str().c_str(), 20, 700);

	if (previewAllPrimitive) {
		gui2.draw();
		stringstream toggleInfo;
		toggleInfo << "(1/2/3/4): SET RESOLUTION";
		ofDrawBitmapStringHighlight(toggleInfo.str().c_str(), ofGetWidth()/2-80, 50);
	}
	else if (!previewAllPrimitive && !priVisual) {
		stringstream toggleInfo;
		toggleInfo << "(1/2/3): CHANGE OBJECT";
		ofDrawBitmapStringHighlight(toggleInfo.str().c_str(), ofGetWidth() / 2 - 80, 50);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (previewAllPrimitive) {
		switch (key) {
		case 'f':
			ofToggleFullscreen();
			break;
		case 's':
			faceHold = !faceHold;
			break;
		case '1':
			bSplitFaces = false;
			sphere.setResolution(4);
			// icosahedron //
			icoSphere.setResolution(0); // number of subdivides //
			plane.setResolution(3, 2);
			cylinder.setResolution(4, 2, 0);
			cone.setResolution(4, 1, 0);
			box.setResolution(1);

			icosphereMesh = icoSphere.getMesh();
			sphereMesh = sphere.getMesh();
			planeMesh = plane.getMesh();
			coneMesh = cone.getMesh();
			cylinderMesh = cylinder.getMesh();
			boxMesh = box.getMesh();
			break;
		case '2':
			bSplitFaces = false;
			sphere.setResolution(8);
			icoSphere.setResolution(1);
			plane.setResolution(6, 4);
			cylinder.setResolution(8, 4, 1);
			cone.setResolution(7, 2, 1);
			box.setResolution(2);

			icosphereMesh = icoSphere.getMesh();
			sphereMesh = sphere.getMesh();
			planeMesh = plane.getMesh();
			coneMesh = cone.getMesh();
			cylinderMesh = cylinder.getMesh();
			boxMesh = box.getMesh();
			break;
		case '3':
			bSplitFaces = false;
			sphere.setResolution(16);
			icoSphere.setResolution(2);
			plane.setResolution(8, 5);
			cylinder.setResolution(12, 9, 2);
			cone.setResolution(10, 5, 2);
			box.setResolution(6);

			icosphereMesh = icoSphere.getMesh();
			sphereMesh = sphere.getMesh();
			planeMesh = plane.getMesh();
			coneMesh = cone.getMesh();
			cylinderMesh = cylinder.getMesh();
			boxMesh = box.getMesh();
			break;
		case '4':
			bSplitFaces = false;
			sphere.setResolution(48);
			icoSphere.setResolution(4);
			plane.setResolution(12, 9);
			cylinder.setResolution(20, 13, 4);
			cone.setResolution(20, 9, 3);
			box.setResolution(10);

			icosphereMesh = icoSphere.getMesh();
			sphereMesh = sphere.getMesh();
			planeMesh = plane.getMesh();
			coneMesh = cone.getMesh();
			cylinderMesh = cylinder.getMesh();
			boxMesh = box.getMesh();
			break;
		}
	}
	else if(!previewAllPrimitive && !priVisual){
		switch (key) {
		case '1':
			model.loadModel("catOLOD.obj");
			mesh = model.getMesh(0);
			ofDisableArbTex();
			ofLoadImage(texture, "cat.jpg");
			objSize = catSize;
			objName = "CAT";
			break;

		case '2':
			model.loadModel("penguinOLOD.obj");
			mesh = model.getMesh(0);
			ofDisableArbTex();
			ofLoadImage(texture, "penguin.jpg");
			objSize = penguinSize;
			objName = "PENGUIN";
			break;

		case '3':
			model.loadModel("rockOLOD.obj");
			mesh = model.getMesh(0);
			ofDisableArbTex();
			ofLoadImage(texture, "rock.jpg");
			objSize = rockSize;
			objName = "ROCK";
			break;
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	screenSize = ofToString(w) + "x" + ofToString(h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
