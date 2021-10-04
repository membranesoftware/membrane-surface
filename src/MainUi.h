/*
* Copyright 2018-2021 Membrane Software <author@membranesoftware.com> https://membranesoftware.com
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
// UI that shows the main display

#ifndef MAIN_UI_H
#define MAIN_UI_H

#include <list>
#include <map>
#include "StdString.h"
#include "Buffer.h"
#include "Panel.h"
#include "Json.h"
#include "Ui.h"

class MainUi : public Ui {
public:
	static const int MaxCommandSize;

	MainUi ();
	~MainUi ();

protected:
	// Load subclass-specific resources and return a result value
	int doLoad ();

	// Unload subclass-specific resources
	void doUnload ();

	// Update subclass-specific interface state as appropriate for an elapsed millisecond time period
	void doUpdate (int msElapsed);

private:
	// Execute cmdInv as a surface command. If the command is PlayAnimation, execute it only if allowPlayAnimation is true.
	void executeCommand (Json *cmdInv, bool allowPlayAnimation = false);
	void removeWindow (Json *cmdInv);
	void playAnimation (Json *cmdInv);
	void showColorFillBackground (Json *cmdInv);
	void showResourceImageBackground (Json *cmdInv);
	void showFileImageBackground (Json *cmdInv);
	void showIconLabelWindow (Json *cmdInv);
	void showCountdownWindow (Json *cmdInv);

	// Callback functions
	struct BackgroundImageLoadedContext {
		MainUi *ui;
		int backgroundType;
	};
	static void showFileImageBackground_imageLoaded (void *ctxPtr, Widget *widgetPtr);

	Buffer commandBuffer;
	Panel *backgroundPanel;

	struct AnimationCommand {
		int executeTime;
		Json *cmdInv;
		AnimationCommand (): executeTime (0), cmdInv (NULL) { }
	};
	std::list<MainUi::AnimationCommand> animationList;
	std::map<StdString, Widget *> windowIdMap;

	// Remove all items from animationList
	void clearAnimation ();

	// Remove all items from windowIdMap
	void clearWindowIdMap ();

	// Set an entry in windowIdMap
	void setWindowId (const StdString &windowId, Widget *widget);

	// Update animation state for an elapsed time period
	void updateAnimation (int msElapsed);

	// Return the UiConfiguration core sprites index associated with a SystemInterface Icon constant, or -1 if no sprite index matched
	int getIconType (int icon);

	// Set a widget's position to x/y, with negative values indicating a position aligned from the interface right or top
	void setWidgetPosition (Widget *widget, float x, float y);
};

#endif
