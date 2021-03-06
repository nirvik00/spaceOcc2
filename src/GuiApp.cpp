#include "GuiApp.h"

/* Boolean intersection between 4 points */
int GuiApp::intxPts(Pt p, Pt q, Pt r, Pt s) {
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

/* ns util functions */
vector<Pt>GuiApp::Lerp(Pt a, Pt b, Pt c) {
	Pt ab[20]; Pt bc[20]; vector<Pt> pts;
	int k = 0;
	float n = 1.f / 20.f;
	for (float i = 0.f; i < 1.f; i += n) {
		ab[k].setup(a.x + (b.x - a.x)*i, a.y + (b.y - a.y)*i);
		bc[k].setup(b.x + (c.x - b.x)*i, b.y + (c.y - b.y)*i);
		k++;
	}
	k = 0;
	for (float i = 0.f; i < 1.f; i += n) {
		float x = ab[k].x + (bc[k].x - ab[k].x)*i;
		float y = ab[k].y + (bc[k].y - ab[k].y)*i;
		Pt e(x, y);
		pts.push_back(e);
		k++;
	}
	return pts;
}


void GuiApp::genconvexhullcrv() {
	convexhullcrvpts.clear();
	for (int i = 0; i < convexhullpts.size(); i++) {
		Pt a, b, c;
		if (i == convexhullpts.size() - 2) {
			a = convexhullpts[convexhullpts.size() - 2];
			b = convexhullpts[convexhullpts.size() - 1];
			c = convexhullpts[0];
		}
		else if (i == convexhullpts.size() - 1) {
			a = convexhullpts[convexhullpts.size() - 1];
			b = convexhullpts[0];
			c = convexhullpts[1];
		}
		else {
			a = convexhullpts[i];
			b = convexhullpts[i + 1];
			c = convexhullpts[i + 2];
		}

		Pt a_(a.x + (b.x - a.x)*ICurvature, a.y + (b.y - a.y)*ICurvature);
		Pt c_(c.x + (b.x - c.x)*ICurvature, c.y + (b.y - c.y)*ICurvature);
		Pt ba(b.x + (a.x - b.x)*ICurvature, b.y + (a.y - b.y)*ICurvature);
		Pt bc(b.x + (c.x - b.x)*ICurvature, b.y + (c.y - b.y)*ICurvature);

		for (float j = 0; j < 1.0; j += 0.1) {
			Pt p(ba.x + (a_.x - ba.x)*j, ba.y + (a_.y - ba.y)*j);
			convexhullcrvpts.push_back(p);
		}
		vector<Pt> pts = Lerp(a_, b, c_);
		for (int j = 0; j < pts.size(); j++) {
			Pt p = pts[j];
			convexhullcrvpts.push_back(p);
		}
	}
}

/* rw system */
void GuiApp::initRW() {
	outerquadvec.clear(); quadVec.clear(); ptVec.clear(); subdivVec.clear(); 
	rwQuadPtVec.clear(); subdivVec.clear(); subdivQuadVec.clear(); 
	nestedPeriQuadCellVec.clear(); periQuadCellVec.clear(); periCorrVec.clear();
	
	// start at center
	Pt p0; p0.setup(0, 0);
	float Ra = 0.75; int t = (int)ofRandom(0, 4);
	Quad Q; Q.setup(p0, AREA[0], Ra, t, spaceiniquads);
	
	quadVec.push_back(Q);

	ptVec.push_back(Q.pts[0]);
	ptVec.push_back(Q.pts[1]);
	ptVec.push_back(Q.pts[2]);
	ptVec.push_back(Q.pts[3]);
	construct(1);
	if (quadVec.size() > 0) {
		vector<Pt> ptvec;
		for (int i = 0; i < quadVec.size(); i++) {
			vector<Pt> ptvec = quadVec[i].getPts();
			for (int j = 0; j < ptvec.size(); j++) {
				rwQuadPtVec.push_back(ptvec[j]);
			}
		}
	}
	std::function<bool(Pt, Pt)>sorterx = SortPtAscX();
	std::sort(rwQuadPtVec.begin(), rwQuadPtVec.end(), sorterx);
	minx = rwQuadPtVec[0].x; maxx = rwQuadPtVec[rwQuadPtVec.size() - 1].x;
	std::function<bool(Pt, Pt)> sortery = SortPtAscY();
	std::sort(rwQuadPtVec.begin(), rwQuadPtVec.end(), sortery);
	miny = rwQuadPtVec[0].y; maxy = rwQuadPtVec[rwQuadPtVec.size() - 1].y;
	Pt gotcen((minx + maxx) / 2, (miny + maxy) / 2);
	Pt u(GlobalLoc.x - minx, GlobalLoc.y - miny);
	ptVec.clear();
	for (int i = 0; i < quadVec.size(); i++) {
		for (int j = 0; j < 4; j++) {
			quadVec[i].pts[j].setup(u.x + quadVec[i].pts[j].x, u.y + quadVec[i].pts[j].y);
			ptVec.push_back(quadVec[i].pts[j]);
		}
	}
}

/*	recursive function for rw quads	*/
void GuiApp::construct(int I) {
	float Ra = 0.75;
	int t = (int)ofRandom(0, ptVec.size() - 1);
	int r = (int)ofRandom(0, 4);
	Pt P = ptVec[t];
	Quad R; R.setup(P, AREA[I], Ra, r);
	int sum = 0;
	for (int j = 0; j < quadVec.size(); j++) {
		int T0 = R.detectIntx(quadVec[j]);
		int T1 = quadVec[j].detectIntx(R);
		if (T0 == 1) { sum++; break; }
		if (T1 == 1) { sum++; break; }
	}
	if (sum == 0) {
		quadVec.push_back(R);
		ptVec.push_back(R.pts[0]);
		ptVec.push_back(R.pts[1]);
		ptVec.push_back(R.pts[2]);
		ptVec.push_back(R.pts[3]);
		if (I < NUMOBJ - 1) { I++; construct(I); }
	}
	else { construct(I); }
}

/* init subdiv process  */
void GuiApp::initSubdiv() {
	subdivVec.clear(); subdivQuadVec.clear(); nestedPeriQuadCellVec.clear();
	periQuadCellVec.clear(); periCorrVec.clear(); adaptiveGridSegVec.clear();

	for (int i = 0; i < NUMOBJ; i++) {
		Quad q = quadVec[i];
		float dis = spaceiniquads;

		//construct the offset
		Pt a = q.pts[0]; Pt b = q.pts[1]; Pt c = q.pts[2]; Pt d = q.pts[3];
		Pt ua((b.x - a.x)*dis / a.di(b), (b.y - a.y)*dis / a.di(b));
		Pt va((d.x - a.x)*dis / a.di(d), (d.y - a.y)*dis / a.di(d));
		Pt a1(a.x + ua.x + va.x, a.y + ua.y + va.y);

		Pt ub((a.x - b.x)*dis / b.di(a), (a.y - b.y)*dis / b.di(a));
		Pt vb((c.x - b.x)*dis / b.di(c), (c.y - b.y)*dis / b.di(c));
		Pt b1(b.x + ub.x + vb.x, b.y + ub.y + vb.y);

		Pt uc((b.x - c.x)*dis / c.di(b), (b.y - c.y)*dis / c.di(b));
		Pt vc((d.x - c.x)*dis / c.di(d), (d.y - c.y)*dis / c.di(d));
		Pt c1(c.x + uc.x + vc.x, c.y + uc.y + vc.y);

		Pt ud((a.x - d.x)*dis / d.di(a), (a.y - d.y)*dis / d.di(a));
		Pt vd((c.x - d.x)*dis / d.di(c), (c.y - d.y)*dis / d.di(c));
		Pt d1(d.x + ud.x + vd.x, d.y + ud.y + vd.y);
		
		
		Quad Q; Q.setupPreservePts (a1, b1, c1, d1);

		subdivQuadVec.clear();
		subdiv(Q, 0, SUBDIV[i]);
		subdivVec.push_back(subdivQuadVec);
		subdivQuadVec.clear();
	}
}

/*	recursive function for subdividion process	*/
void GuiApp::subdiv(Quad Q, int t, int w) {
	Pt a, b, c, d;
	a.setup(Q.pts[0].x, Q.pts[0].y);
	b.setup(Q.pts[1].x, Q.pts[1].y);
	c.setup(Q.pts[2].x, Q.pts[2].y);
	d.setup(Q.pts[3].x, Q.pts[3].y);
	int g = (int)ofRandom(0, 2);
	Quad R, S; Pt m, n;
	if (g == 0) {
		m.setup((a.x + b.x) / 2, (a.y + b.y) / 2);
		n.setup((c.x + d.x) / 2, (c.y + d.y) / 2);
		R.setupPreservePts(a, m, n, d);
		S.setupPreservePts(m, b, c, n);
	}
	else {
		m.setup((b.x + c.x) / 2, (b.y + c.y) / 2);
		n.setup((a.x + d.x) / 2, (a.y + d.y) / 2);
		R.setupPreservePts(a, b, m, n);
		S.setupPreservePts(n, m, c, d);
	}
	if (t < w) {
		t++;
		subdiv(R, t, w);
		subdiv(S, t, w);
	}
	else {
		subdivQuadVec.push_back(R);
		subdivQuadVec.push_back(S);
	}
}

/*	PERIPHERAL SYSTEM	*/
void GuiApp::initPeri() {
	float maxD0 = 4.00; float maxD1 = 2.50;
	periQuadCellVec.clear(); periCorrVec.clear(); adaptiveGridSegVec.clear();
	nestedPeriQuadCellVec.clear();
	if (subdivVec.size() > 0) {
		for (int i = 0; i < NUMOBJ; i++) {
			vector<Quad> temp;
			for (int j = 0; j < subdivVec[i].size(); j++) {
				Quad Q = subdivVec[i][j];
				Pt a = Q.pts[0];
				Pt b = Q.pts[1];
				Pt c = Q.pts[2];
				Pt d = Q.pts[3];
				float D = -2 * DEPTH[i];

				if (a.di(d) > maxD0*DEPTH[i] && b.di(c) > maxD0*DEPTH[i]) {
					Pt p = Q.initPeripheralSys(a, b, d, D);
					Pt q = Q.initPeripheralSys(b, c, a, D);
					Pt r = Q.initPeripheralSys(c, d, b, D);
					Pt s = Q.initPeripheralSys(d, a, c, D);

					Quad quad0; quad0.setupPreservePts(a, p, q, b);
					Quad quad1; quad1.setupPreservePts(b, q, r, c);
					Quad quad2; quad2.setupPreservePts(c, r, s, d);
					Quad quad3; quad3.setupPreservePts(d, s, p, a);
					
					if (PlotA[i] == true) {	temp.push_back(quad0); }
					if (PlotB[i] == true) { temp.push_back(quad1); }
					if (PlotC[i] == true) { temp.push_back(quad2); }
					if (PlotD[i] == true) { temp.push_back(quad3); }
				}
				else if (a.di(d) < maxD0 * DEPTH[i] && b.di(c) < maxD0*DEPTH[i] && a.di(d) > maxD1 * DEPTH[i] && b.di(c) > maxD1 * DEPTH[i]) {
					Pt p = Q.initPeripheralSysR(a, d, D);
					Pt q = Q.initPeripheralSysR(b, c, D);
					Quad quad0; quad0.setupPreservePts(a, p, q, b);
					temp.push_back(quad0);
				}
			}
			nestedPeriQuadCellVec.push_back(temp);
		}
	}
	else if (quadVec.size() > 0) {
		for (int i = 0; i < quadVec.size(); i++) {
			vector<Quad> temp;
			Quad q = quadVec[i];

			float dis = spaceiniquads;
			//construct the initial offset
			Pt a = q.pts[0]; Pt b = q.pts[1]; Pt c = q.pts[2]; Pt d = q.pts[3];
			Pt ua((b.x - a.x)*dis / a.di(b), (b.y - a.y)*dis / a.di(b));
			Pt va((d.x - a.x)*dis / a.di(d), (d.y - a.y)*dis / a.di(d));
			Pt a1(a.x + ua.x + va.x, a.y + ua.y + va.y);

			Pt ub((a.x - b.x)*dis / b.di(a), (a.y - b.y)*dis / b.di(a));
			Pt vb((c.x - b.x)*dis / b.di(c), (c.y - b.y)*dis / b.di(c));
			Pt b1(b.x + ub.x + vb.x, b.y + ub.y + vb.y);

			Pt uc((b.x - c.x)*dis / c.di(b), (b.y - c.y)*dis / c.di(b));
			Pt vc((d.x - c.x)*dis / c.di(d), (d.y - c.y)*dis / c.di(d));
			Pt c1(c.x + uc.x + vc.x, c.y + uc.y + vc.y);

			Pt ud((a.x - d.x)*dis / d.di(a), (a.y - d.y)*dis / d.di(a));
			Pt vd((c.x - d.x)*dis / d.di(c), (c.y - d.y)*dis / d.di(c));
			Pt d1(d.x + ud.x + vd.x, d.y + ud.y + vd.y);

			Quad Q; Q.setupPreservePts(a1, b1, c1, d1);

			float D = -2 * DEPTH[i];

			if (a.di(d) > maxD0*DEPTH[i] && b.di(c) > maxD0*DEPTH[i]) {
				Pt p = Q.initPeripheralSys(a, b, d, D);
				Pt q = Q.initPeripheralSys(b, c, a, D);
				Pt r = Q.initPeripheralSys(c, d, b, D);
				Pt s = Q.initPeripheralSys(d, a, c, D);
				Quad quad0; quad0.setupPreservePts(a, p, q, b);
				Quad quad1; quad1.setupPreservePts(b, q, r, c);
				Quad quad2; quad2.setupPreservePts(c, r, s, d);
				Quad quad3; quad3.setupPreservePts(d, s, p, a);
				
				if (PlotA[i] == true) { temp.push_back(quad0); }
				if (PlotB[i] == true) { temp.push_back(quad1); }
				if (PlotC[i] == true) { temp.push_back(quad2); }
				if (PlotD[i] == true) { temp.push_back(quad3); }
				
				nestedPeriQuadCellVec.push_back(temp);
			}
			else if (a.di(d) < maxD0 * DEPTH[i] && b.di(c) < maxD0*DEPTH[i] && a.di(d) > maxD1 * DEPTH[i] && b.di(c) > maxD1 * DEPTH[i]) {
				Pt p = Q.initPeripheralSysR(a, d, D);
				Pt q = Q.initPeripheralSysR(b, c, D);
				Quad quad0; quad0.setupPreservePts(a, p, q, b);
				temp.push_back(quad0);
			}
		}
	}
	periQuadCellVec.clear();
	vector<Quad> intxquad;
	for (int i = 0; i < nestedPeriQuadCellVec.size(); i++) {
		for (int j = 0; j < nestedPeriQuadCellVec[i].size(); j++) {
			Quad Q = nestedPeriQuadCellVec[i][j];
			Pt a = Q.pts[0];
			Pt b = Q.pts[1];
			Pt c = Q.pts[2];
			Pt d = Q.pts[3];
			vector<Seg> segvec;
			Pt u((c.x - b.x) / b.di(c), (c.y - b.y) / b.di(c));
			Pt u_(-u.y, u.x);
			float L = LENGTH[i];
			float n = b.di(c) / L;
			Seg seg0(b, a); segvec.push_back(seg0);
			for (float k = 0; k < b.di(c); k += L) {
				Pt X(b.x + (u.x)*k, b.y + (u.y)*k);
				Pt Y(X.x + u.y * 50, X.y - u.x * 50);
				Pt Z; Z = intx(X, Y, a, d);
				segvec.push_back(Seg(X, Z));
			}
			Seg seg1(c, d); segvec.push_back(seg1);
			for (int k = 1; k < segvec.size(); k++) {
				Pt a = segvec[k - 1].a; Pt b = segvec[k - 1].b;
				Pt c = segvec[k].a; Pt d = segvec[k].b;
				Quad quad; quad.setupPreservePts(a, b, d, c);
				periQuadCellVec.push_back(quad);
			}
		}
	}
	for (int i = 0; i < intxquad.size(); i++) {
		intxquad[i].display3();
	}
}

/*	genAdaptiveGrid	*/
void GuiApp::genAdaptiveGrid() {
	subdivVec.clear(); periQuadCellVec.clear();
	drawAdaptiveGrid = 1; adaptiveGridSegVec.clear();
	if (quadVec.size() > 0) {
		for (int i = 0; i<quadVec.size(); i++) {
			vector<Seg>segvec = quadVec[i].adaptiveGrids(LENGTH[i], DEPTH[i], 50);
			for (int j = 0; j < segvec.size(); j++) {
				adaptiveGridSegVec.push_back(segvec[j]);
			}
		}
	}
}

void GuiApp::setup()
{
	LENGTH[0] = Length0; DEPTH[0] = Depth0; SUBDIV[0] = Subdiv0; AREA[0] = Area0; 
	LENGTH[1] = Length1; DEPTH[1] = Depth1; SUBDIV[1] = Subdiv1; AREA[1] = Area1;
	LENGTH[2] = Length2; DEPTH[2] = Depth2; SUBDIV[2] = Subdiv2; AREA[2] = Area2;
	LENGTH[3] = Length3; DEPTH[3] = Depth3; SUBDIV[3] = Subdiv3; AREA[3] = Area3;
	LENGTH[4] = Length4; DEPTH[4] = Depth4; SUBDIV[4] = Subdiv4; AREA[4] = Area4;
	PlotA[0] = Plot0SegA; PlotB[0] = Plot0SegB; PlotC[0] = Plot0SegC; PlotD[0] = Plot0SegD;
	PlotA[1] = Plot1SegA; PlotB[1] = Plot1SegB; PlotC[1] = Plot1SegC; PlotD[1] = Plot1SegD;
	PlotA[2] = Plot2SegA; PlotB[2] = Plot2SegB; PlotC[2] = Plot2SegC; PlotD[2] = Plot2SegD;
	PlotA[3] = Plot3SegA; PlotB[3] = Plot3SegB; PlotC[3] = Plot3SegC; PlotD[3] = Plot3SegD;
	PlotA[4] = Plot4SegA; PlotB[4] = Plot4SegB; PlotC[4] = Plot4SegC; PlotD[4] = Plot4SegD;

	grp1params.setName("Controls");

	grp1params.add(blank0.set("object 0"));
	grp1params.add(Area0.set("(slider)Area 0", 35000.0, 0, 100000.0));
	grp1params.add(Length0.set("(slider)Length 0", 30, 1, 50));
	grp1params.add(Depth0.set("(slider)Depth 0", 10, 1, 50));
	grp1params.add(Plot0SegA.set("plot-0 Seg A?",true));
	grp1params.add(Plot0SegB.set("plot-0 Seg B?", true));
	grp1params.add(Plot0SegC.set("plot-0 Seg C?", true));
	grp1params.add(Plot0SegD.set("plot-0 Seg D?", true));
	grp1params.add(Subdiv0.set("(slider)Subdiv 0", 1, 0, 5));

	grp1params.add(blank1.set("object 1"));
	grp1params.add(Area1.set("(slider)Area 1", 35000.0, 0, 100000.0));
	grp1params.add(Length1.set("(slider)Length 1", 30, 1, 50));
	grp1params.add(Depth1.set("(slider)Depth 1", 10, 1, 50));
	grp1params.add(Plot1SegA.set("plot-1 Seg A?", true));
	grp1params.add(Plot1SegB.set("plot-1 Seg B?", true));
	grp1params.add(Plot1SegC.set("plot-1 Seg C?", true));
	grp1params.add(Plot1SegD.set("plot-1 Seg D?", true));
	grp1params.add(Subdiv1.set("(slider)Subdiv 1", 1, 0, 5));

	grp1params.add(blank2.set("object 2"));
	grp1params.add(Area2.set("(slider)Area 2", 35000.0, 0, 100000.0));
	grp1params.add(Length2.set("(slider)Length 2", 30, 1, 50));
	grp1params.add(Depth2.set("(slider)Depth 2", 10, 1, 50));
	grp1params.add(Plot2SegA.set("plot-2 Seg A?", true));
	grp1params.add(Plot2SegB.set("plot-2 Seg B?", true));
	grp1params.add(Plot2SegC.set("plot-2 Seg C?", true));
	grp1params.add(Plot2SegD.set("plot-2 Seg D?", true));
	grp1params.add(Subdiv2.set("(slider)Subdiv 2", 1, 0, 5));

	grp1params.add(blank3.set("object 3"));
	grp1params.add(Area3.set("(slider)Area 3", 35000.0, 0, 100000.0));
	grp1params.add(Length3.set("(slider)Length 3", 30, 1, 50));
	grp1params.add(Depth3.set("(slider)Depth 3", 10, 1, 50));
	grp1params.add(Plot3SegA.set("plot-3 Seg A?", true));
	grp1params.add(Plot3SegB.set("plot-3 Seg B?", true));
	grp1params.add(Plot3SegC.set("plot-3 Seg C?", true));
	grp1params.add(Plot3SegD.set("plot-3 Seg D?", true));
	grp1params.add(Subdiv3.set("(slider)Subdiv 3", 1, 0, 5));

	grp1params.add(blank4.set("object 4"));
	grp1params.add(Area4.set("(slider)Area 4", 35000.0, 0, 100000.0));
	grp1params.add(Length4.set("(slider)Length 4", 30, 1, 50));
	grp1params.add(Depth4.set("(slider)Depth 4", 10, 1, 50));
	grp1params.add(Plot4SegA.set("plot-4 Seg A?", true));
	grp1params.add(Plot4SegB.set("plot-4 Seg B?", true));
	grp1params.add(Plot4SegC.set("plot-4 Seg C?", true));
	grp1params.add(Plot4SegD.set("plot-4 Seg D?", true));
	grp1params.add(Subdiv4.set("(slider)Subdiv 4", 1, 0, 5));

	grp1params.add(blank5.set("object 5"));

	gui.setup(grp1params);
	gui.setPosition(0, 0);
	gui.setBackgroundColor(0);
	gui.setBorderColor(255);
	gui.setFillColor(ofColor(255, 0, 0));

	GlobalLoc.setup(300, 100);
	
	grp2params.add(spaceiniquads.set("inital quad spacing", 5,0,20));
	grp2params.add(transX.set("translate X", 0, 0, ofGetWidth()/2));
	grp2params.add(transY.set("translate Y", 0, 0, ofGetHeight()/2));
	grp2params.add(scalehull.set("scale Cx. hull", 1, 1, 5));
	grp2params.add(ICurvature.set("C. Hull Curvature", 0.75, 0.51, 0.99));	
	grp2params.add(manipulatequads.set("manipulate quads", false));
	grp2params.add(manipulatehull.set("manipulate hull", false));
	gui2.setup(grp2params);
	gui2.setPosition(ofGetWidth()-200, 0);

	MSG = "Keyboard Controls";
	MSG += "\n--------------------";
	MSG += "\n1. Press 'e' or 'E' for config generations";
	MSG += "\n2. Press 'r' or 'R' for quad subdivisons";
	MSG += "\n3. Press 't' or 'T' for peripheral cells";
	MSG += "\n4. Press 'h' or 'H' for adaptive grids";
	MSG += "\n5. Press 'c' or 'C' to generate convex hull";
	MSG += "\n6. Press 'x' or 'X' to delete convex hull";

}

void GuiApp::update()
{
	LENGTH[0] = Length0; DEPTH[0] = Depth0; SUBDIV[0] = Subdiv0; AREA[0] = Area0;
	LENGTH[1] = Length1; DEPTH[1] = Depth1; SUBDIV[1] = Subdiv1; AREA[1] = Area1;
	LENGTH[2] = Length2; DEPTH[2] = Depth2; SUBDIV[2] = Subdiv2; AREA[2] = Area2;
	LENGTH[3] = Length3; DEPTH[3] = Depth3; SUBDIV[3] = Subdiv3; AREA[3] = Area3;
	LENGTH[4] = Length4; DEPTH[4] = Depth4; SUBDIV[4] = Subdiv4; AREA[4] = Area4;
	PlotA[0] = Plot0SegA; PlotB[0] = Plot0SegB; PlotC[0] = Plot0SegC; PlotD[0] = Plot0SegD;
	PlotA[1] = Plot1SegA; PlotB[1] = Plot1SegB; PlotC[1] = Plot1SegC; PlotD[1] = Plot1SegD;
	PlotA[2] = Plot2SegA; PlotB[2] = Plot2SegB; PlotC[2] = Plot2SegC; PlotD[2] = Plot2SegD;
	PlotA[3] = Plot3SegA; PlotB[3] = Plot3SegB; PlotC[3] = Plot3SegC; PlotD[3] = Plot3SegD;
	PlotA[4] = Plot4SegA; PlotB[4] = Plot4SegB; PlotC[4] = Plot4SegC; PlotD[4] = Plot4SegD;

	if (SUBDIV[0] > 5) { SUBDIV[0] = 1; }
	else { SUBDIV[0] = Subdiv0; }
	
	if (drawAdaptiveGrid==1) {
		genAdaptiveGrid();
	}	

	//generate convex hull
	if (genconvexhull == 1 && manipulatehull==false) {
		vector<Pt> ptsforhull;
		for (int i = 0; i < quadVec.size(); i++) {
			Quad Q = quadVec[i];
			Pt a = Q.pts[0];
			Pt b = Q.pts[1];
			Pt c = Q.pts[2];
			Pt d = Q.pts[3];
			Pt m((a.x + c.x) / 2, (a.y + c.y) / 2);
			Pt a_(m.x + (a.x - m.x)*scalehull, m.y + (a.y - m.y)*scalehull);
			Pt b_(m.x + (b.x - m.x)*scalehull, m.y + (b.y - m.y)*scalehull);
			Pt c_(m.x + (c.x - m.x)*scalehull, m.y + (c.y - m.y)*scalehull);
			Pt d_(m.x + (d.x - m.x)*scalehull, m.y + (d.y - m.y)*scalehull);
			ptsforhull.push_back(a_);
			ptsforhull.push_back(b_);
			ptsforhull.push_back(c_);
			ptsforhull.push_back(d_);
		}	
		// update global vector of points and renders directly in draw
		genConvexHull(ptsforhull, ptsforhull.size());
		// use the global vector to generate curves	
		genconvexhullcrv();
	}
	if(genconvexhull==0){
		convexhullcrvpts.clear();
		convexhullpts.clear();
	}
	if (manipulatehull == true) {
		//if (convexhullpts.size() > 0) {
			genconvexhullcrv();
		//}
	}
	

}

void GuiApp::draw()
{
	ofBackground(255);
	ofSetColor(0); ofNoFill(); ofSetLineWidth(1);
	ofPushMatrix();
	ofTranslate(transX, transY);

	if (drawCmd == 1) {
		for (int i = 0; i < quadVec.size(); i++) { 
			quadVec[i].display(to_string(i)); 
		}
		for (int i = 0; i < quadVec.size(); i++) {
			Quad q = quadVec[i];
			vector<Pt> pts = q.getPts();
			for (int j = 0; j < pts.size(); j++) {
				//ofSetColor(0); ofNoFill(); ofEllipse(pts[j].x, pts[j].y, 15, 15);
			}
		}
	}
	if (drawSubdiv == 1) {
		for (int i = 0; i < subdivVec.size(); i++) {
			vector<Quad> div = subdivVec[i];
			for (int j = 0; j < div.size(); j++) { div[j].display2(); }
		}
	}
	if (drawPeri == 1) {
		for (int i = 0; i < periQuadCellVec.size(); i++) {
			periQuadCellVec[i].displayPath();
		}
	}
	if (drawAdaptiveGrid == 1) {
		for (int i = 0; i < adaptiveGridSegVec.size(); i++) {
			Seg S = adaptiveGridSegVec[i];
			Pt a = S.a; Pt b = S.b;
			ofSetColor(175); ofSetLineWidth(1);
			ofDrawLine(a.x, a.y, b.x, b.y);
		}
	}

	/* handle manual mode  */
	if (manipulatequads == true) {
		for (int i = 0; i < quadVec.size(); i++) {
			for (int j = 0; j < 4; j++) {
				if (quadVec[i].pts[j].getSelected() == 1) {
					ofFill(); ofSetColor(200, 0, 0, 150);
					ofEllipse(quadVec[i].pts[j].x, quadVec[i].pts[j].y, 15, 15);
				}
			}
		}
	}
	

	/* plot the convex hull - created in update */
	if (manipulatehull == true && convexhullpts.size()>0) {
		for (int i = 0; i < convexhullpts.size(); i++) {
			Pt p = convexhullpts[i];
			if (p.getSelected() == 1) {
				ofFill(); ofSetColor(200, 0, 0, 150);
			}
			else {
				ofNoFill(); 
			}
			 ofEllipse(p.x, p.y, 15, 15);
		}
	}
	for (int i = 1; i < convexhullpts.size(); i++) {
		Pt p = convexhullpts[i - 1]; Pt q = convexhullpts[i];
		ofSetLineWidth(1); ofSetColor(0, 0, 255, 50);
		ofDrawLine(p.x, p.y, q.x, q.y);
	}
		
	for (int i = 1; i < convexhullcrvpts.size(); i++) {
		Pt p = convexhullcrvpts[i-1]; Pt q = convexhullcrvpts[i];
		ofSetLineWidth(7); ofFill(); ofSetColor(250,0,0,150); ofDrawLine(p.x, p.y, q.x, q.y);
	}			
	

	ofPopMatrix();

	ofColor(150); ofSetLineWidth(1);
	ofSetColor(0);

	ofDrawBitmapString(MSG, 700, 20);

	gui.draw();
	gui2.draw();	

	if (manipulatequads == true) {
		manipulatehull.set(false);
		for (int i = 0; i < quadVec.size(); i++) {
			for (int j = 0; j < 4; j++) {
				Pt p = quadVec[i].pts[j];
				ofNoFill(); ofEllipse(p.x + transX, p.y + transY, 20, 20);
			}
		}
	}	

	for (int i = 0; i < convexhullpts.size(); i++) {
		Pt p = convexhullpts[i];
		if (p.getSelected() == 1) {
			ofFill(); ofSetColor(200, 0, 0, 150);
		}
		else {
			ofNoFill();
		}
		ofEllipse(p.x + transX, p.y + transY, 15, 15);
	}
}

void GuiApp::keyPressed(int key) {
	if (key == 'e' || key == 'E') {
		convexhullcrvpts.clear();
		convexhullpts.clear(); 
		initRW(); drawCmd = 1; 
	}
	if (key == 'r' || key == 'R') { initSubdiv(); drawSubdiv = 1;  drawAdaptiveGrid = 0; }
	if (key == 't' || key == 'T') { initPeri(); drawPeri = 1;  drawAdaptiveGrid = 0; initPeri(); }
	if (key == 'h' || key == 'H') { genAdaptiveGrid(); drawAdaptiveGrid = 1;}
	if (key == 'c' || key == 'C') { genconvexhull = 1; }
	if (key == 'x' || key == 'X') { genconvexhull = 0; }
	if (key == 's' || key == 'S') {
		global_image_counter++;
		ofImage screenshot;
		screenshot.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		screenshot.saveImage("screenshot" + to_string(global_image_counter) + ".png");
	}
}

void GuiApp::mouseMoved(int x, int y) {
	Pt m(x - transX, y - transY);
	if (manipulatequads == true) {
		for (int i = 0; i < quadVec.size(); i++) {
			for (int j = 0; j < 4; j++) {
				if (quadVec[i].pts[j].di(m) < 25) {
					quadVec[i].pts[j].setSelected(1);
				}
				else {
					quadVec[i].pts[j].setSelected(0);
				}
			}
		}
	}
	
	if (manipulatehull == true) {
		for (int i = 0; i < convexhullpts.size(); i++) {
			if (convexhullpts[i].di(m) < 25) {
				convexhullpts[i].setSelected(1);
				cout << "hull point selected" << endl;
			}
			else {
				convexhullpts[i].setSelected(0);
			}
		}
	}
	
	
}

void GuiApp::mousePressed(int x, int y, int button) {}

void GuiApp::mouseDragged(int x, int y, int button) {
	Pt m(x - transX, y - transY);
	if (manipulatequads == true) {
		for (int i = 0; i < quadVec.size(); i++) {
			for (int j = 0; j < 4; j++) {
				if (quadVec[i].pts[j].getSelected() == 1) {
					quadVec[i].pts[j].setup(m.x, m.y);
				}
			}
		}
	}
	if (manipulatehull == true) {
		for (int i = 0; i < convexhullpts.size(); i++) {
			if (convexhullpts[i].getSelected() == 1) {
				convexhullpts[i].setup(m.x, m.y);
			}
		}
	}
	
}

void GuiApp::mouseReleased(int x, int y, int button) {}

Pt GuiApp::intx(Pt p, Pt q, Pt r, Pt s) {
	float a1 = q.y - p.y; float b1 = p.x - q.x; float c1 = a1*q.x + b1*q.y;
	float a2 = s.y - r.y; float b2 = r.x - s.x; float c2 = a2*s.x + b2*s.y;
	float det = a1*b2 - a2*b1;
	float rx = (c1*b2 - c2*b1) / det; float ry = (c2*a1 - c1*a2) / det;
	return Pt(rx, ry);
}

int GuiApp::orientationPts(Pt p, Pt q, Pt r) {
	int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
	if (val == 0) return 0; // collinear 
	return (val > 0) ? 1 : 2; // clock or counterclock wise 
}

void GuiApp::genConvexHull(vector<Pt> pts, int n) {
	convexhullpts.clear();
	vector<Pt> hull;
	if (n < 3) { return; }
	int r = 0;
	for (int i = 1; i < n; i++) {
		if (pts[i].x < pts[r].x) { r = i; }
	}
	int p = r, q;
	do {
		hull.push_back(pts[p]); q = (p + 1) % n;
		for (int i = 0; i < n; i++) {
			if (orientationPts(pts[p], pts[i], pts[q]) == 2) { q = i; }
		}
		p = q;
	} while (p != r);
	
	hull.push_back(hull[0]);

	convexhullpts = hull;

	/*
	if (n < 3) { println("nope"); return; }
	Vector<Pt> hull = new Vector<Pt>();
	int r = 0;
	for (int i = 1; i < n; i++) {
	if (points[i].x < points[r].x) { r = i; }
	}
	int p = r, q;
	do
	{
	hull.add(points[p]);  q = (p + 1) % n;
	for (int i = 0; i < n; i++) {
	if (orientation(points[p], points[i], points[q]) == 2) { q = i; }
	}
	p = q;
	} while (p != r);

	*/
}
