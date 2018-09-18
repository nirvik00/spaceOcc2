#pragma once

#include "ofMain.h"
#include <vector>

class Pt
{

public:
	float x, y;
	int selected = 0;
	
	Pt() ;
	Pt(float, float);
	void setup(float, float);
	float di(Pt);
	float di(Pt, Pt);
	void setSelected(int t);
	int getSelected();
	void display();
	Pt addPt(Pt p);
	Pt findIntx(Pt, Pt, Pt, Pt);
	int ptOnLineSegment(Pt, Pt);

};

