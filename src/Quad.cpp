#include "Quad.h"

Quad::Quad() {}

Quad::Quad(Seg r_, Seg s_) { 
	R = r_; S = s_; 
	float X[4]; float Y[4];
	X[0] = R.a.x; X[1] = R.b.x; X[2] = S.a.x; X[3] = S.b.x; sort(X, X + 3);
	Y[0] = R.a.y; Y[1] = R.b.y; Y[2] = S.a.y; Y[3] = S.b.y; sort(Y, Y + 3);
	pts[0] = Pt(X[0], Y[0]); pts[1] = Pt(X[2], Y[0]); pts[2] = Pt(X[2], Y[2]); pts[3] = Pt(X[0], Y[2]);
}

Quad::Quad(Pt p, Pt q, Pt r, Pt s) { 
	pts[0] = p; pts[1] = q; pts[2] = r; pts[3] = s; 
	float X[4] = { p.x,q.x,r.x,s.x }; sort(X, X + 3);
	float Y[4] = { p.y,q.y,r.y,s.y }; sort(Y, Y + 3);
	pts[0].setup(X[0], Y[0]);
	pts[1].setup(X[2], Y[0]);
	pts[2].setup(X[2], Y[2]);
	pts[3].setup(X[0], Y[2]);
}

void Quad::setup(Pt p, Pt q, Pt r, Pt s) {
	pts[0] = p; pts[1] = q; pts[2] = r; pts[3] = s;
	float X[4] = { p.x,q.x,r.x,s.x }; sort(X, X + 3);
	float Y[4] = { p.y,q.y,r.y,s.y }; sort(Y, Y + 3);
	pts[0].setup(X[0], Y[0]);
	pts[1].setup(X[2], Y[0]);
	pts[2].setup(X[2], Y[2]);
	pts[3].setup(X[0], Y[2]);
}

void Quad::setupPreservePts(Pt p, Pt q, Pt r, Pt s) {
	pts[0] = p; pts[1] = q; pts[2] = r; pts[3] = s;
}

void Quad::setup(Pt P, float A, float r, int t) {
	/* 
	*	point, area, raiio, true=first
	*	xy=A; x/y=1/r; x=y/r
	*	y.y/r=A; y=sqrt(r.a)
	*/
	AREA = A;
	//right-down
	float y = sqrt(r*A); float x = y / r;
	Pt Q, R, S;
	if(t == 0) {
		Q.setup(P.x + x, P.y);
		R.setup(P.x + x, P.y + y);
		S.setup(P.x, P.y + y);
	}
	//right-up
	else if (t == 1) {
		Q.setup(P.x - x, P.y);
		R.setup(P.x - x, P.y - y);
		S.setup(P.x, P.y - y);
	}
	//left-up
	else if (t == 2) {
		Q.setup(P.x - x, P.y);
		R.setup(P.x - x, P.y - y);
		S.setup(P.x, P.y - y);
	}
	//left-down
	else {
		Q.setup(P.x - x, P.y);
		R.setup(P.x - x, P.y + y);
		S.setup(P.x, P.y + y);
	}
	pts[0] = P;
	pts[1] = Q;
	pts[2] = R;
	pts[3] = S;
	float X[4] = { P.x,Q.x,R.x,S.x }; sort(X, X + 3);
	float Y[4] = { P.y,Q.y,R.y,S.y }; sort(Y, Y + 3);
	pts[0].setup(X[0], Y[0]);
	pts[1].setup(X[2], Y[0]);
	pts[2].setup(X[2], Y[2]);
	pts[3].setup(X[0], Y[2]);
}

void Quad::setup(Pt P, float A, float r, int t, float dis) {
	/*
	*	point, area, raiio, true=first
	*	xy=A; x/y=1/r; x=y/r
	*	y.y/r=A; y=sqrt(r.a)
	*/
	AREA = A;
	//right-down
	float y = sqrt(r*A); float x = y / r;
	Pt Q, R, S;
	if (t == 0) {
		Q.setup(P.x + x, P.y);
		R.setup(P.x + x, P.y + y);
		S.setup(P.x, P.y + y);
	}
	//right-up
	else if (t == 1) {
		Q.setup(P.x - x, P.y);
		R.setup(P.x - x, P.y - y);
		S.setup(P.x, P.y - y);
	}
	//left-up
	else if (t == 2) {
		Q.setup(P.x - x, P.y);
		R.setup(P.x - x, P.y - y);
		S.setup(P.x, P.y - y);
	}
	//left-down
	else {
		Q.setup(P.x - x, P.y);
		R.setup(P.x - x, P.y + y);
		S.setup(P.x, P.y + y);
	}
	pts[0] = P;
	pts[1] = Q;
	pts[2] = R;
	pts[3] = S;
	float X[4] = { P.x,Q.x,R.x,S.x }; sort(X, X + 3);
	float Y[4] = { P.y,Q.y,R.y,S.y }; sort(Y, Y + 3);
	pts[0].setup(X[0], Y[0]);
	pts[1].setup(X[2]+dis, Y[0]);
	pts[2].setup(X[2]+dis, Y[2]+dis);
	pts[3].setup(X[0], Y[2]+dis);
}

Quad Quad::intoffOrtho(float dis) {
	Pt a = pts[0]; Pt b = pts[1]; Pt c = pts[2]; Pt d = pts[3];
	float t1 = dis; float t2 = dis;
	Pt a1(a.x + (c.x - a.x)*dis, a.y + (c.y - a.y)*dis);
	Pt b1(b.x + (d.x - b.x)*dis, b.y + (d.y - b.y)*dis);
	Pt c1(c.x + (a.x - c.x)*dis, c.y + (a.y - c.y)*dis);
	Pt d1(d.x + (b.x - d.x)*dis, d.y + (b.y - d.y)*dis);
	Quad Q(a1, b1, c1, d1);
	return Q;
}

Quad Quad::intoff(float dis) {
	Pt a = pts[0]; Pt b = pts[1]; Pt c = pts[2]; Pt d = pts[3];
	float t1 = dis; float t2 = dis;
	/*
	//Pt ua((b.x - a.x)*dis / a.di(b), (b.y - a.y)*dis / a.di(b));
	//Pt va((d.x - a.x)*dis / a.di(d), (d.y - a.y)*dis / a.di(d));
	Pt ua(0, 0);
	Pt va(0,0);
	Pt a1(a.x + ua.x + va.x, a.y + ua.y + va.y);

	//Pt ub((a.x - b.x)*dis / b.di(a), (a.y - b.y)*dis / b.di(a));
	//Pt vb((c.x - b.x)*dis / b.di(c), (c.y - b.y)*dis / b.di(c));
	Pt ub(0, 0);
	Pt vb(0, 0);
	Pt b1(b.x + ub.x + vb.x, b.y + ub.y + vb.y);

	//Pt uc((b.x - c.x)*dis / c.di(b), (b.y - c.y)*dis / c.di(b));
	//Pt vc((d.x - c.x)*dis / c.di(d), (d.y - c.y)*dis / c.di(d));
	Pt uc(0, 0);
	Pt vc(0, 0);
	Pt c1(c.x + uc.x + vc.x, c.y + uc.y + vc.y);

	//Pt ud((a.x - d.x)*dis / d.di(a), (a.y - d.y)*dis / d.di(a));
	//Pt vd((c.x - d.x)*dis / d.di(c), (c.y - d.y)*dis / d.di(c));
	Pt ud(0, 0);
	Pt vd(0, 0);
	Pt d1(d.x + ud.x + vd.x, d.y + ud.y + vd.y);
	*/
	Pt a1 = pts[0]; Pt b1 = pts[1]; Pt c1 = pts[2]; Pt d1 = pts[3];
	Quad Q(a1, b1, c1, d1);
	return Q;
}

void Quad::setup(Seg r_, Seg s_) { 
	R = r_; S = s_; 
	Pt p = R.a; Pt q = R.b; Pt r = S.a; Pt s = S.b;
	float X[4] = { p.x,q.x,r.x,s.x }; sort(X, X + 3);
	float Y[4] = { p.y,q.y,r.y,s.y }; sort(Y, Y + 3);
	pts[0].setup(X[0], Y[0]);
	pts[1].setup(X[2], Y[0]);
	pts[2].setup(X[2], Y[2]);
	pts[3].setup(X[0], Y[2]);
}

void Quad::display() {
	ofSetColor(0); ofSetLineWidth(2);	
	Pt a, b, c, d;
	a.setup(pts[0].x, pts[0].y);
	b.setup(pts[1].x, pts[1].y);
	c.setup(pts[2].x, pts[2].y);
	d.setup(pts[3].x, pts[3].y);
	ofDrawLine(a.x, a.y, b.x, b.y);
	ofDrawLine(b.x, b.y, c.x, c.y);
	ofDrawLine(c.x, c.y, d.x, d.y);
	ofDrawLine(d.x, d.y, a.x, a.y);
	ofSetColor(0);
	Pt M = getMP();
	ofDrawBitmapString(AREA, M.x-20, M.y);
}

void Quad::display(string s) {
	ofSetColor(150); ofSetLineWidth(1);
	Pt a, b, c, d;
	a.setup(pts[0].x, pts[0].y);
	b.setup(pts[1].x, pts[1].y);
	c.setup(pts[2].x, pts[2].y);
	d.setup(pts[3].x, pts[3].y);
	ofDrawLine(a.x, a.y, b.x, b.y);
	ofDrawLine(b.x, b.y, c.x, c.y);
	ofDrawLine(c.x, c.y, d.x, d.y);
	ofDrawLine(d.x, d.y, a.x, a.y);
	ofSetColor(0);
	Pt M = getMP();
	
	stringstream stream;
	stream << fixed << setprecision(2) << AREA;
	string s1 = s+") "+stream.str();
	
	ofDrawBitmapString(s1, M.x-20, M.y);
}

void Quad::display2() {
	ofSetColor(150); ofSetLineWidth(1);
	Pt a, b, c, d;
	a.setup(pts[0].x, pts[0].y);
	b.setup(pts[1].x, pts[1].y);
	c.setup(pts[2].x, pts[2].y);
	d.setup(pts[3].x, pts[3].y);
	ofDrawLine(a.x, a.y, b.x, b.y);
	ofDrawLine(b.x, b.y, c.x, c.y);
	ofDrawLine(c.x, c.y, d.x, d.y);
	ofDrawLine(d.x, d.y, a.x, a.y);
}

void Quad::display3() {
	ofSetColor(0); ofSetLineWidth(1);
	Pt a, b, c, d;
	a.setup(pts[0].x, pts[0].y);
	b.setup(pts[1].x, pts[1].y);
	c.setup(pts[2].x, pts[2].y);
	d.setup(pts[3].x, pts[3].y);
	ofDrawLine(a.x, a.y, b.x, b.y);
	ofDrawLine(b.x, b.y, c.x, c.y);
	ofDrawLine(c.x, c.y, d.x, d.y);
	ofDrawLine(d.x, d.y, a.x, a.y);
}

void Quad::displayPath() {
	ofSetColor(0);
	ofPath path2;
	path2.setStrokeColor(0); path2.setStrokeWidth(1);
	path2.setFillColor(ofColor(255, 0, 0, 50));
	path2.moveTo(ofVec2f(pts[0].x, pts[0].y));
	path2.lineTo(ofVec2f(pts[1].x, pts[1].y));
	path2.lineTo(ofVec2f(pts[2].x, pts[2].y));
	path2.lineTo(ofVec2f(pts[3].x, pts[3].y));
	path2.close();
	path2.draw();
}

vector<Pt> Quad::getPts() {
	vector<Pt> ptvec;
	ptvec.push_back(pts[0]);
	ptvec.push_back(pts[1]);
	ptvec.push_back(pts[2]);
	ptvec.push_back(pts[3]);
	return ptvec;
}

Pt Quad::getMP() {
	Pt M; 
	M.setup((pts[0].x + pts[2].x) / 2, (pts[0].y + pts[2].y) / 2);
	return M;
}

int Quad::contains(Pt p) {
	float X[4]; float Y[4];
	for (int i = 0; i < 4; i++) { X[i] = pts[i].x; Y[i] = pts[i].y; }
	sort(X, X + 4); sort(Y, Y + 4);
	if (p.x > X[0] && p.x < X[3] && p.y > Y[0] && p.y < Y[3]) {
		return 1; 
	}
	else {
		return 0;
	}
}

int Quad::detectIntx(Quad r) {
	int t = 0; int sum = 0;
	if (r.contains(pts[0]) == 1) sum++;
	if (r.contains(pts[1]) == 1) sum++;
	if (r.contains(pts[2]) == 1) sum++;
	if (r.contains(pts[3]) == 1) sum++;
	Pt mp = getMP();
	if (r.contains(mp) == 1) sum++;
	if (sum == 0) { return 0; }
	else { return 1; }
}

int Quad::intxPts(Pt p, Pt q, Pt r, Pt s) {
	float a1 = q.y - p.y; float b1 = p.x - q.x; float c1 = (a1*q.x) + (b1*q.y);
	float a2 = s.y - r.y; float b2 = r.x - s.x;	float c2 = (a2*s.x) + (b2*s.y);
	float det = (a1*b2) - (b1*a2); if (det == 0) return 0; //parallel
	float m = ((c1*b2) - (c2*b1)) / det; float n = ((c2*a1) - (c1*a2)) / det;
	Pt I(m, n);
	float dIp = I.di(p); float dIq = I.di(q); float dpq = p.di(q);
	float dIr = I.di(r); float dIs = I.di(s); float drs = r.di(s);
	if ((dIp + dIq - dpq) < 1 && (dIr + dIs - drs) < 1) { return 1; }
	else { return 0; }
}

Pt Quad::intxPt(Pt p, Pt q, Pt r, Pt s) {
	float a1 = q.y - p.y; float b1 = p.x - q.x;	float c1 = a1*q.x + b1*q.y;
	float a2 = s.y - r.y; float b2 = r.x - s.x; float c2 = a2*s.x + b2*s.y;
	float det = a1*b2 - a2*b1; if (det == 0) { return Pt(-100, -100); }
	float mx = (c1*b2 - c2*b1) / det; float my = (c2*a1 - c1*a2) / det;
	return Pt(mx, my);
}

/*	interpolated grid system	*/
vector<Seg> Quad::adaptiveGrids(float LENGTH, float DEPTH, float CORR) {
	vector<Seg>segvec;
	Pt a = pts[0];
	Pt b = pts[1];
	Pt c = pts[2];
	Pt d = pts[3];
	Pt ab((b.x - a.x), (b.y - a.y));
	Pt dc((c.x - d.x), (c.y - d.y));

	Pt bc((c.x - b.x), (c.y - b.y));
	Pt ad((d.x - a.x), (d.y - a.y));

	float itr = 0;
	for (float i = 0; i<1; i += 0.1) {
		Pt e(a.x + ab.x*i, a.y + ab.y*i);
		Pt f(d.x + dc.x*i, d.y + dc.y*i);
		segvec.push_back(Seg(e, f));
		Pt g(a.x + ad.x*i, a.y + ad.y*i);
		Pt h(b.x + bc.x*i, b.y + bc.y*i);
		segvec.push_back(Seg(g,h));
		itr++;
	}
	return segvec;
}

/*	PERIPHERAL CELL SYSTEM - ANALYSIS AND GENERATON: end seg in triangle (ang. bisector)	*/
Pt Quad::initPeripheralSys( Pt p, Pt q, Pt r, float DEPTH) {
	//need to find angular bisector at p between pq, pr
	Pt u(q.x - p.x, q.y - p.y); Pt v(r.x - p.x, r.y - p.y);
	float t = 0.5*(acos((u.x*v.x + u.y*v.y) / (p.di(q)*p.di(r)))); //half-angle between vectors
	Pt E(u.x*cos(t) - u.y*sin(t), u.x*sin(t) + u.y*cos(t)); //rotate vector by angle - get pt
	Pt R(p.x + E.x, p.y + E.y); //move to pt p
	Pt W(p.x + (p.x - R.x)*DEPTH / p.di(R), p.y + (p.y - R.y)*DEPTH / p.di(R));
	
	// at mid-point take unit normal to u and scale to f
	Pt mp((p.x + q.x) / 2, (p.y + q.y) / 2); // midpoint between p,q
	Pt nmp(mp.x - (u.y*DEPTH / p.di(q)), mp.y + (u.x*DEPTH / p.di(q))); //normal at midpoint at scale
	int U = heronContains(nmp); // check if normal is inside
	Pt nxy(p.x - (u.y*DEPTH / p.di(q)), p.y + (u.x*DEPTH / p.di(q))); // normal at p at scale
	if (U == 0) {
		nmp.setup(mp.x + (u.y*DEPTH / p.di(q)), mp.y - (u.x*DEPTH / p.di(q))); //other normal at midpoint at scale
		nxy.setup(p.x + (u.y*DEPTH / p.di(q)), p.y - (u.x*DEPTH / p.di(q)));//other normal at p at scale
	}
	Pt ux((nxy.x - nmp.x) / nmp.di(nxy), (nxy.y - nmp.y) / nmp.di(nxy));

	//if the angle bisector is outside the triangle, adjust it
	int T = heronContains(W);
	if (T == 0) {
		t = PI - t;
		E.setup(u.x*cos(t) - u.y*sin(t), u.y*sin(t) + u.y*cos(t));
		R.setup(p.x + E.x, p.y + E.y);
		W.setup(p.x + (p.x - R.x)*DEPTH / E.di(p), p.y + (p.y - R.y)*DEPTH / E.di(p));
	}
	Pt fpt = intxPt(nmp, nxy, p, W);
	return fpt;
}

/*	PERIPHERAL CELL SYSTEM - non-quad peripheral: end is rect	*/
Pt Quad::initPeripheralSysR(Pt p, Pt r, float DEPTH) {
	//just use diags - end is rect
	Pt v((p.x - r.x) / (r.di(p)), (p.y - r.y) / (r.di(p)));
	Pt R(p.x + v.x*DEPTH, p.y + v.y*DEPTH); //move to pt p
	return R;
}

void Quad::displayPts() {
	ofFill();
	ofSetColor(200, 0, 0, 50); ofCircle(pts[0].x, pts[0].y, 10, 10);
	ofSetColor(0, 200, 0, 50); ofCircle(pts[1].x, pts[1].y, 10, 10);
	ofSetColor(0, 0, 200, 50); ofCircle(pts[2].x, pts[2].y, 10, 10);
	ofSetColor(200, 0, 200, 50); ofCircle(pts[3].x, pts[3].y, 10, 10);
}

Pt Quad::proj(Pt a, Pt p, Pt q) {
	Pt u(a.x - p.x, a.y - p.y); Pt v(q.x - p.x, q.y - p.y);
	float r = ((u.x*v.x + u.y*v.y) / (p.di(q)*p.di(q)));
	return Pt(p.x+r*v.x, p.y+r*v.y);
}

Pt Quad::extend(Pt p, Pt q, Pt r, Pt s) {
	float a1 = q.y - p.y;
	float b1 = p.x - q.x;
	float c1 = a1*q.x + b1*q.y;
	float a2 = s.y - r.y;
	float b2 = r.x - s.x;
	float c2 = a2*s.x + b2*s.y;
	float det = a1*b2 - a2*b1;
	if (det == 0) return p;
	else {
		float x = (c1*b2 - c2*b1)/det;
		float y = (c2*a1 - c1*a2)/det;
		return Pt(x, y);
	}
}

float Quad::heron(Pt a,Pt b, Pt c) {
	float l = a.di(b);
	float m = a.di(c);
	float n = c.di(b);
	float s = (l + m + n) / 2;
	float ar = sqrt(s*(s - l)*(s - m)*(s - n));
	return ar;
}

int Quad::heronContains(Pt p) {
	Pt a = pts[0]; Pt b = pts[1]; Pt c = pts[2]; Pt d = pts[3];
	float ar = heron(a, b, c) + heron(a, d, c);
	float ar0 = heron(a, p, b);
	float ar1 = heron(b, p, c);
	float ar2 = heron(c, p, d);
	float ar3 = heron(d, p, a);
	float diff = abs(ar - (ar0 + ar1 + ar2 + ar3));
	if ( diff < 10) {
		return 1;
	}
	else {
		return 0;
	}
}