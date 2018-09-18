#include "Seg.h"

Seg::Seg() {}

Seg::Seg(Pt a_, Pt b_){ a = a_; b = b_; }

void Seg::setup(Pt a_, Pt b_) { a = a_; b = b_; }

void Seg::display() {
	ofSetColor(10, 200, 100, 100); ofSetLineWidth(1);
	ofEllipse(a.x, a.y, 10, 10); ofEllipse(b.x, b.y, 10, 10);
	ofSetColor(10, 200, 100, 100); ofSetLineWidth(5);
	ofDrawLine(a.x, a.y, b.x, b.y);
}
