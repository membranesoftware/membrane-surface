/*
* Copyright 2018-2020 Membrane Software <author@membranesoftware.com> https://membranesoftware.com
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* 3. Neither the name of the copyright holder nor the names of its contributors
* may be used to endorse or promote products derived from this software without
* specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/
// Class that runs the application and holds global state

#ifndef APP_H
#define APP_H

#include <vector>
#include <stack>
#include "SDL2/SDL.h"
#include "SystemInterface.h"
#include "StdString.h"
#include "Log.h"
#include "Input.h"
#include "Resource.h"
#include "Network.h"
#include "HashMap.h"
#include "Prng.h"
#include "UiStack.h"
#include "UiText.h"
#include "UiConfiguration.h"
#include "Sprite.h"
#include "Widget.h"
#include "Panel.h"

class App {
public:
	App ();
	~App ();

	static App *instance;

	// Populate the App instance pointer with a newly created object
	static void createInstance ();

	// Destroy and clear the App instance pointer
	static void freeInstance ();

	static const int DefaultMinFrameDelay;
	static const int WindowWidths[];
	static const int WindowHeights[];
	static const int WindowSizeCount;
	static const float FontScales[];
	static const int FontScaleCount;
	static const int MaxCornerRadius;

	// Key values for the prefs map
	static const char *NetworkThreadsKey;
	static const char *WindowWidthKey;
	static const char *WindowHeightKey;
	static const char *FontScaleKey;
	static const char *HttpsKey;
	static const char *ShowInterfaceAnimationsKey;

	// Read-write data members
	Log log;
	Prng prng;
	Input input;
	UiStack uiStack;
	UiText uiText;
	UiConfiguration uiConfig;
	Resource resource;
	Network network;
	SystemInterface systemInterface;
	bool shouldRefreshUi;
	bool isInterfaceAnimationEnabled;
	float nextFontScale;
	int nextWindowWidth;
	int nextWindowHeight;
	StdString prefsPath;

	// Read-only data members
	bool isShuttingDown;
	bool isShutdown;
	int64_t startTime;
	bool isHttpsEnabled;
	SDL_Window *window;
	SDL_Renderer *render; // The renderer must be accessed only from the application's main thread
	bool isTextureRenderEnabled;
	Panel *rootPanel;
	float displayDdpi;
	float displayHdpi;
	float displayVdpi;
	int windowWidth;
	int windowHeight;
	int minDrawFrameDelay; // milliseconds
	int minUpdateFrameDelay; // milliseconds
	float fontScale;
	int imageScale;
	int64_t drawCount;
	int64_t updateCount;
	SDL_Rect clipRect;
	bool isPrefsWriteDisabled;

	// Run the application, returning only after the application exits
	int run ();

	// Begin the application's shutdown process and halt execution when complete
	void shutdown ();

	// Return an int64_t value for use as a unique ID
	int64_t getUniqueId ();

	// Lock the application prefs map and return a pointer to its HashMap object
	HashMap *lockPrefs ();

	// Unlock the application prefs map
	void unlockPrefs ();

	// Suspend the application's update thread and block until the current update cycle completes
	void suspendUpdate ();

	// Unsuspend the application's update thread after a previous call to suspendUpdate
	void unsuspendUpdate ();

	// Push the provided rectangle onto the clip stack and apply it to the application's renderer. Apply the new clip rectangle as an intersection of any existing clip rectangle unless disableIntersection is true.
	void pushClipRect (const SDL_Rect *rect, bool disableIntersection = false);

	// Pop the clip stack
	void popClipRect ();

	// Disable any active clip rectangle, to be restored by a call to unsuspendClipRect
	void suspendClipRect ();

	// Restore a previously suspended clip rectangle
	void unsuspendClipRect ();

	// Return a texture containing a rounded corner of the specified radius, or NULL if no such texture is available. If a texture is found and width and height pointers are provided, those values are filled in with texture attributes.
	SDL_Texture *getRoundedCornerTexture (int radius, int *textureWidth = NULL, int *textureHeight = NULL);

	typedef void (*RenderTaskFunction) (void *fnData);
	struct RenderTaskContext {
		RenderTaskFunction callback;
		void *callbackData;
		RenderTaskContext (): callback (NULL), callbackData (NULL) { }
	};
	// Schedule a task function to execute at the top of the next render loop
	void addRenderTask (RenderTaskFunction fn, void *fnData);

	// Return a pseudorandom int value, chosen from within the specified inclusive range
	int getRandomInt (int i1, int i2);

	// Return a pseudorandomly chosen string value of the specified length
	StdString getRandomString (int stringLength);

	// Callback functions
	static bool keyEvent (void *ptr, SDL_Keycode keycode, bool isShiftDown, bool isControlDown);

private:
	// Create the root panel and other top-level widgets
	void populateWidgets ();

	// Create roundedCornerSprite and load it with textures
	void populateRoundedCornerSprite ();

	// Execute draw operations to update the application window
	void draw ();

	// Execute all operations in renderTaskList
	void executeRenderTasks ();

	// Execute operations to update application state as appropriate for an elapsed millisecond time period
	void update (int msElapsed);

	// Close the application window and reopen it at the size indicated by nextWindowWidth and nextWindowHeight
	void resizeWindow ();

	// Return the image scale index associated with the specified window size, or a negative value if no such scale is known
	int getImageScale (int w, int h);

	// Run the application's state update thread
	static int runUpdateThread (void *appPtr);

	// Callback function for use with Network::addDatagramCallback
	static void datagramReceived (void *callbackData, const char *messageData, int messageLength, const char *sourceAddress, int sourcePort);

	// Write the prefs file if any prefsMap keys have changed since the last write
	void writePrefs ();

	SDL_Thread *updateThread;
	SDL_mutex *uniqueIdMutex;
	int64_t nextUniqueId;
	HashMap prefsMap;
	SDL_mutex *prefsMapMutex;
	std::vector<SDL_Keycode> keyPressList;
	std::stack<SDL_Rect> clipRectStack;
	Sprite *roundedCornerSprite;
	SDL_mutex *renderTaskMutex;
	std::vector<App::RenderTaskContext> renderTaskList;
	std::vector<App::RenderTaskContext> renderTaskAddList;
	bool isSuspendingUpdate;
	SDL_mutex *updateMutex;
	SDL_cond *updateCond;
};

#endif
