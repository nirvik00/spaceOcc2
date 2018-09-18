#pragma once

#include "ofMain.h"
#include <vector>

class Areas
{
public:
	vector<float> areas;

	Areas();
	vector<float> getBlockAreas();
	vector<vector<float>> getUnitAreas();
};