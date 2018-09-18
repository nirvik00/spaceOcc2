#pragma once

#include "ofMain.h"
#include "Pt.h"

class Vec
{
public:
	float x = 0.f, y = 0.f;
	Vec() {};
	Vec(float, float);
	Vec(Pt, Pt);
	Vec unitVector(Pt, Pt);
	Vec unitNormal(Pt p, Pt);
	Vec Scale(Pt, Pt, float);
	Vec Scale(Vec, float);
	Vec Rotate(Pt, Pt, float);
	vector<Pt> Lerp(Pt, Pt, float);
};



