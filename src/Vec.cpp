#include "Vec.h"


Vec::Vec(float x_, float y_) { x = x_; y = y_; }

Vec::Vec(Pt p, Pt q) { x = q.x - p.x; y = q.y - p.x; }

Vec Vec::unitVector(Pt a, Pt b) { 
	float d = a.di(b);
	return Vec((b.x - a.x)/d, (b.y + a.y)/d); 
}

Vec Vec::unitNormal(Pt p, Pt q) {
	Vec v = unitVector(p, q);
	return Vec(-v.y, v.x);
}

Vec Vec::Scale(Pt p, Pt q, float s) {
	Vec v = unitVector(p, q);
	return Vec(s*v.x, s*v.y);
}

Vec Vec::Scale(Vec v, float s) {
	return Vec(s*v.x, s*v.y);
}

Vec Vec::Rotate(Pt p, Pt q, float r) {
	Vec v = Vec(p, q);
	return Vec(v.x*cos(r) - v.y*sin(r), v.x*sin(r) + v.y*cos(r));
}

vector<Pt> Vec::Lerp(Pt p, Pt q, float r) {
	vector<Pt> pts;
	/*
	Vec v = unitVector(p, q);
	for (float i = 0; i < 1; i += (1 / r)) {
		Vec u = p.addPt(Scale(v, i));
		Pt a; a.setPt(u.x, u.y);
		pts.push_back(a);
	}
	*/
	return pts;
}
