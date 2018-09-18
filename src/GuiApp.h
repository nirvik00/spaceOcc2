#pragma once


#include "ofMain.h"
#include "ofxGui.h"

#include "GuiApp.h"

#include "Areas.h"
#include "Quad.h"
#include "Pt.h"

#include <vector>

struct SortPtAscX {
	bool operator()(Pt a, Pt b) {
		return (a.x < b.x);
	}
};

struct SortPtAscY {
	bool operator()(Pt a, Pt b) {
		return (a.y < b.y);
	}
};

class GuiApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	/*	METHODS	*/
	void initRW();
	void construct(int);
	void initSubdiv();
	void subdiv(Quad, int, int);
	void initPeri();
	void genAdaptiveGrid();
	Pt intx(Pt a, Pt b, Pt c, Pt d);
	int intxPts(Pt p, Pt q, Pt r, Pt s);

	/*	VARIABLES	*/
	vector<Pt> ptVec; // during the rw mcts
	vector<Quad> outerquadvec;	// rw quads 	
	vector<Quad> quadVec;	// rw quads 	
	vector<float> areas;	// rw quads input
	vector<Quad> subdivQuadVec; // temporary subdiv quads
	vector<vector<Quad>> subdivVec; // collects all subdiv quads
	vector<Pt> rwQuadPtVec; // sorting and centering
	vector<Seg> adaptiveGridSegVec;

	/*	CENTERING VARIABLES	*/
	float cenx, ceny;
	float gotx, goty;
	float minx, miny, maxx, maxy;
	Pt GlobalLoc;

	vector<Quad> periQuadCellVec; //peri boundary quads
	vector<vector<Quad>> nestedPeriQuadCellVec; //nested peri quad cell
	vector<Seg> periCorrVec; //peri Seg vectors - cells

	/* GUI VARIABLES */
	const int NUMOBJ = 5;
	float AREA[5], LENGTH[5], DEPTH[5], PlotA[5], PlotB[5], PlotC[5], PlotD[5];
	int SUBDIV[5];
	int lockpt = -1; Pt lockedPt;
	int lock = 0;

	/*	KEYBOARD CONTROLS	*/
	int drawCmd = 0;
	int drawSubdiv = 0;
	int drawPeri = 0;
	int drawAdaptiveGrid = 0;
	string MSG = "";
	int global_image_counter = 0;

	/*	gui 1  parameters and objects	*/
	ofParameterGroup grp1params;
	ofParameter<string>blank0;
	ofParameter<float>Area0;
	ofParameter<int>Length0; 
	ofParameter<int>Depth0;
	ofParameter<bool>Plot0SegA;
	ofParameter<bool>Plot0SegB;
	ofParameter<bool>Plot0SegC;
	ofParameter<bool>Plot0SegD;
	ofParameter<int>Subdiv0;

	ofParameter<string>blank1;
	ofParameter<float>Area1;
	ofParameter<int>Length1;
	ofParameter<int>Depth1;
	ofParameter<bool>Plot1SegA;
	ofParameter<bool>Plot1SegB;
	ofParameter<bool>Plot1SegC;
	ofParameter<bool>Plot1SegD;
	ofParameter<int>Subdiv1;

	ofParameter<string>blank2;
	ofParameter<float>Area2;
	ofParameter<int>Length2;
	ofParameter<int>Depth2;
	ofParameter<bool>Plot2SegA;
	ofParameter<bool>Plot2SegB;
	ofParameter<bool>Plot2SegC;
	ofParameter<bool>Plot2SegD;
	ofParameter<int>Subdiv2;

	ofParameter<string>blank3;
	ofParameter<float>Area3;
	ofParameter<int>Length3;
	ofParameter<int>Depth3;
	ofParameter<bool>Plot3SegA;
	ofParameter<bool>Plot3SegB;
	ofParameter<bool>Plot3SegC;
	ofParameter<bool>Plot3SegD;
	ofParameter<int>Subdiv3;

	ofParameter<string>blank4;
	ofParameter<float>Area4;
	ofParameter<int>Length4;
	ofParameter<int>Depth4;
	ofParameter<bool>Plot4SegA;
	ofParameter<bool>Plot4SegB;
	ofParameter<bool>Plot4SegC;  
	ofParameter<bool>Plot4SegD;
	ofParameter<int>Subdiv4;
	
	ofParameter<string>blank5;

	ofxPanel gui;

	ofParameterGroup grp2params;
	ofParameter<float> spaceiniquads;
	ofParameter<float> transX;
	ofParameter<float> transY;
	ofxPanel gui2;
};

