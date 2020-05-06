#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	// own method
	void drawObject();
	void drawPrimitive();

	// dev setup
	void initSetup();
	void initDraw();


	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	// load 3dObject
	ofxAssimpModelLoader model;
	int catVertexNum;

	// size of Object
	string objName = "CAT";
	int objSize = 20;
	int catSize = 20;
	int penguinSize = 20;
	int rockSize = 20;

	// light
	ofLight light;

	// mesh
	ofMesh mesh;
	ofImage tex;
	ofNode meshNode;
	ofTexture texture;

	// mesh hold for drawAllPrimitive
	ofMesh icosphereMesh;
	ofMesh sphereMesh;
	ofMesh planeMesh;
	ofMesh coneMesh;
	ofMesh cylinderMesh;
	ofMesh boxMesh;

	// index vertex - debugging
	int vertexRemove = 20;
	int i = 0;

	// camera 3d
	ofEasyCam cam;

	//gui - used
	ofxPanel gui;
	ofParameter<bool> vertexMode;
	ofParameter<bool> wireframeMode;
	ofParameter<bool> previewAllPrimitive;
	ofParameter<ofColor> color;
	ofParameter<float> modelScaled = 0.9f;
	ofParameter<glm::vec3> camPosition;
	ofParameter<string> screenSize;

	// gui-1 - primitive visualize
	ofxPanel gui1;
	ofParameter<bool> priVisual;
	//gui 2 - adding hold the camera
	ofxPanel gui2;
	ofParameter<bool> camHold;
	ofParameter<bool> faceHold;
	// gui 3 - adding optional addition for grid
	ofxPanel gui3;
	// grid will flip due to the meshNode.trasnform
	ofParameter<bool> gX; // this is z
	ofParameter<bool> gY; // this is x
	ofParameter<bool> gZ; // this is y
	// --
	ofSpherePrimitive sphereSingle;

	//---------additional primitive start here---------//
	ofVideoGrabber vidGrabber;
	int mode;

	// adding primitive object
	ofSpherePrimitive sphere;
	ofIcoSpherePrimitive icoSphere;
	ofPlanePrimitive plane;
	ofCylinderPrimitive cylinder;
	ofConePrimitive cone;
	ofBoxPrimitive box;

	// light on scene
	ofLight pointLight;
	ofLight pointLight2;
	ofLight pointLight3;
	ofMaterial material;

	// place to store the sides of the box //
	ofVboMesh boxSides[ofBoxPrimitive::SIDES_TOTAL];
	ofVboMesh deformPlane;
	ofVboMesh topCap, bottomCap, body;
	vector<ofMeshFace> triangles;

	bool bFill = true;
	bool bWireframe = true;
	bool bDrawNormals = false;
	bool bDrawAxes = false;
	bool bDrawLights = false;
	bool bHelpText = true;
	bool bMousePressed = false;
	bool bSplitFaces = false;

	bool cullz = true;
	bool noDraw = true;
};
