#pragma once
#include "Pt.h"

class Seg
{
public:
	Pt a, b;
	Seg();
	Seg(Pt, Pt);
	void setup(Pt, Pt);	
	void display();
};

