#pragma once
#include "ofMain.h"
#include "Pt.h"
#include "Seg.h"

#include <vector>
#include <iomanip>
#include <sstream>

class Quad
{
public:	
	Pt pts[4];	// rw, subdiv rect
	float AREA;	//	rw
	Seg R, S;	// quadcell 
	vector<Quad> intQuadVec; // peripheral system variable
	vector<Seg> segCells1;
	vector<Seg> segCells4;
	vector<Seg> segIntCells;

	/* CONSTRUCTORS	*/
	Quad(); 
	Quad(Seg, Seg);
	Quad(Pt, Pt, Pt, Pt);
	
	void setup(Pt, Pt, Pt, Pt);
	void setup(Pt, float, float, int); // rw quad inital no spacing
	void setup(Pt, float, float, int, float); // rw quad with spacing
	void setup(Seg r, Seg s);
	void setupPreservePts(Pt, Pt, Pt, Pt);
	Quad intoff(float);
	Quad intoffOrtho(float);

	/*	GETTERS & SETTERS	*/
	vector<Quad> getIntQuadCells();


	/*	
	* INTERPOLATED GRID SYSTEM	
	* WIP
	*/
	vector<Seg> adaptiveGrids(float, float, float);
	

	/*	ADAPTIVE PERIPHERAL GRID SYSTEM */
	Pt initPeripheralSys(Pt, Pt, Pt, float);
	Pt initPeripheralSysR(Pt, Pt, float);
	Quad genDepthQuads(Pt, Pt, Pt, Pt, float);

	/*	UTILITY FUNCTIONS	*/
	void display();
	void display(string);
	void display2();
	void display3();
	void displayPath();// for rect instead of the curve skeleton
	void displayPts();

	vector<Pt> getPts();
	Pt getMP();

	int contains(Pt);
	int detectIntx(Quad);
	int intxPts(Pt, Pt, Pt, Pt);
	Pt intxPt(Pt, Pt, Pt, Pt);
	Pt proj(Pt, Pt, Pt);
	Pt extend(Pt, Pt, Pt, Pt);
	float heron(Pt, Pt, Pt);
	int heronContains(Pt);
};

