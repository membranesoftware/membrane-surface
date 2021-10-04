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
// Class that presents a set of Ui objects as a stack

#ifndef UI_STACK_H
#define UI_STACK_H

#include <list>
#include "SDL2/SDL.h"
#include "WidgetHandle.h"
#include "Ui.h"

class UiStack {
public:
	UiStack ();
	~UiStack ();

	// Remove all items from the stack
	void clear ();

	// Return the topmost item in the Ui stack, or NULL if no such item was found. If a Ui object is returned by this method, it has been retained and must be released by the caller when no longer needed.
	Ui *getActiveUi ();

	// Remove all Ui objects and add the provided one as the top item in the stack
	void setUi (Ui *ui);

	// Push the provided Ui object to the top of the stack
	void pushUi (Ui *ui);

	// Remove and unload the top item in the Ui stack
	void popUi ();

	// Execute operations to update state as appropriate for an elapsed millisecond time period
	void update (int msElapsed);

	// Execute operations to change the top item of the Ui stack, as specified by previously received commands. This method must be invoked only from the application's main thread.
	void executeStackCommands ();

	// Refresh all Ui items in the stack
	void refresh ();

	// Resize all Ui items in the stack as appropriate for the current window dimensions
	void resize ();

	// Execute actions appropriate for a received keypress event and return a boolean value indicating if the event was consumed and should no longer be processed
	bool processKeyEvent (SDL_Keycode keycode, bool isShiftDown, bool isControlDown);

	// Set the widget that should be the target of keypress text edit operations
	void setKeyFocusTarget (Widget *widget);

	// Deactivate the mouse hover widget and prevent reactivation until a new mouse hover widget is acquired
	void suspendMouseHover ();

private:
	// Stack command types
	enum {
		SetUiCommand = 0,
		PushUiCommand = 1,
		PopUiCommand = 2
	};

	// Execute operations appropriate when mouseHoverTarget has held its current value beyond the hover threshold
	void activateMouseHover ();

	// Deactivate any previously activated mouse hover widgets
	void deactivateMouseHover ();

	std::list<Ui *> uiList;
	Ui *activeUi;
	SDL_mutex *uiMutex;
	int nextCommandType;
	Ui *nextCommandUi;
	SDL_mutex *nextCommandMutex;
	WidgetHandle tooltip;
	WidgetHandle keyFocusTarget;
	WidgetHandle mouseHoverTarget;
	int mouseHoverClock;
	bool isMouseHoverActive;
	bool isMouseHoverSuspended;
};

#endif
