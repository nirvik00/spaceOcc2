#include "Pt.h"

Pt::Pt() {}

Pt::Pt(float x_, float y_) { x = x_; y = y_; }

void Pt::setup(float x_, float y_) { x = x_; y = y_; }

float Pt::di(Pt p) { return sqrt((x - p.x)*(x - p.x) + (y - p.y)*(y - p.y)); }

float Pt::di(Pt p, Pt q){ return sqrt((p.x - p.x)*(p.x - p.x) + (p.y - p.y)*(p.y - p.y)); }

void Pt::setSelected(int t) { selected = t; }

int Pt::getSelected() { return selected; }

void Pt::display() { ofFill(); ofColor(255, 0, 0); ofCircle(x, y, 5, 5); }

Pt Pt::addPt(Pt p) { return Pt(x + p.x, y + p.y); }

Pt Pt::findIntx(Pt p, Pt q, Pt r, Pt s) {
	float a1 = q.y - p.y;
	float b1 = p.x - q.x;	
	float a2 = s.y - r.y;
	float b2 = r.x - s.x;
	float c1 = (a1*q.x) - (b1*q.y);
	float c2 = (a2*s.x - b2*s.y);
	float det = (a1*b2) - (a2*b1);
	float m = (c1*b2 - c2*b1) / det;
	float n = (c2*a1 - c1*a2) / det;
	return Pt(m, n);
}

int Pt::ptOnLineSegment(Pt p, Pt q) {
	//a is this point & p,q line segment
	float ap = di(p);
	float aq = di(q);
	float pq = p.di(q);
	if ((ap + aq - pq) < 1) return 1;
	else return 0;
}
