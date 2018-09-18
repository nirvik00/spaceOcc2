#include "ofMain.h"
#include "ofApp.h"
#include "GuiApp.h"

#include "ofAppGLFWWindow.h"

int main() {

	ofGLFWWindowSettings settings;

	//gui window
	settings.setSize(1350, 1000);
	settings.setPosition(ofVec2f(0, 0));
	settings.resizable = true;
	shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);

	//ofApp window
	settings.setSize(500, 1000);
	settings.setPosition(ofVec2f(1350, 0));
	settings.resizable = true;
	shared_ptr<ofAppBaseWindow> appWindow = ofCreateWindow(settings);

	shared_ptr<GuiApp> guiApp(new GuiApp);
	shared_ptr<ofApp> viewerApp(new ofApp);

	viewerApp->gui = guiApp;

	ofRunApp(appWindow, viewerApp);
	ofRunApp(guiWindow, guiApp);

	ofRunMainLoop();
}
