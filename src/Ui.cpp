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
#include "Config.h"
#include <stdlib.h>
#include <list>
#include "Log.h"
#include "App.h"
#include "Input.h"
#include "OsUtil.h"
#include "StdString.h"
#include "StringList.h"
#include "Json.h"
#include "Widget.h"
#include "Resource.h"
#include "UiConfiguration.h"
#include "Panel.h"
#include "Button.h"
#include "LabelWindow.h"
#include "ProgressBar.h"
#include "Ui.h"

Ui::Ui ()
: rootPanel (NULL)
, isLoaded (false)
, isFirstResumeComplete (false)
, refcount (0)
, refcountMutex (NULL)
, lastWindowCloseCount (0)
{
	refcountMutex = SDL_CreateMutex ();

	rootPanel = new Panel ();
	rootPanel->keyEventCallback = Widget::KeyEventCallbackContext (Ui::keyEvent, this);
	rootPanel->retain ();
}

Ui::~Ui () {
	if (rootPanel) {
		rootPanel->release ();
		rootPanel = NULL;
	}
	if (refcountMutex) {
		SDL_DestroyMutex (refcountMutex);
		refcountMutex = NULL;
	}
}

void Ui::retain () {

	SDL_LockMutex (refcountMutex);
	++refcount;
	if (refcount < 1) {
		refcount = 1;
	}
	SDL_UnlockMutex (refcountMutex);
}

void Ui::release () {
	bool isdestroyed;

	isdestroyed = false;
	SDL_LockMutex (refcountMutex);
	--refcount;
	if (refcount <= 0) {
		refcount = 0;
		isdestroyed = true;
	}
	SDL_UnlockMutex (refcountMutex);
	if (isdestroyed) {
		delete (this);
	}
}

int Ui::load () {
	StdString path;
	int result;

	if (isLoaded) {
		return (OsUtil::Result::Success);
	}
	path = getSpritePath ();
	if (! path.empty ()) {
		result = sprites.load (path);
		if (result != OsUtil::Result::Success) {
			Log::err ("Failed to load sprite resources");
			return (result);
		}
	}

	result = doLoad ();
	if (result != OsUtil::Result::Success) {
		return (result);
	}

	isLoaded = true;
	return (OsUtil::Result::Success);
}

void Ui::unload () {
	if (! isLoaded) {
		return;
	}
	clearPopupWidgets ();

	rootPanel->clear ();
	doUnload ();

	sprites.unload ();
	isLoaded = false;
}

StdString Ui::getSpritePath () {
	// Default implementation returns an empty path
	return (StdString (""));
}

int Ui::doLoad () {
	// Default implementation does nothing
	return (OsUtil::Result::Success);
}

void Ui::doUnload () {
	// Default implementation does nothing
}

void Ui::showShutdownWindow () {
	LabelWindow *label;
	ProgressBar *bar;
	Panel *panel;

	clearPopupWidgets ();

	panel = new Panel ();
	panel->setFillBg (true, Color (0.0f, 0.0f, 0.0f, 0.0f));
	panel->bgColor.translate (0.0f, 0.0f, 0.0f, UiConfiguration::instance->overlayWindowAlpha, UiConfiguration::instance->backgroundCrossFadeDuration);
	panel->setFixedSize (true, App::instance->rootPanel->width, App::instance->rootPanel->height);

	label = new LabelWindow (new Label (StdString::createSprintf ("%s %s", UiText::instance->getText (UiTextString::ShuttingDown).capitalized ().c_str (), APPLICATION_NAME), UiConfiguration::CaptionFont, UiConfiguration::instance->primaryTextColor));
	label->setFillBg (true, UiConfiguration::instance->lightBackgroundColor);

	bar = new ProgressBar (label->width, UiConfiguration::instance->progressBarHeight);
	bar->setIndeterminate (true);

	App::instance->rootPanel->addWidget (panel);
	App::instance->rootPanel->addWidget (label);
	App::instance->rootPanel->addWidget (bar);
	panel->zLevel = App::instance->rootPanel->maxWidgetZLevel + 1;
	label->zLevel = App::instance->rootPanel->maxWidgetZLevel + 2;
	bar->zLevel = App::instance->rootPanel->maxWidgetZLevel + 2;
	label->position.assign ((App::instance->rootPanel->width / 2.0f) - (label->width / 2.0f), (App::instance->rootPanel->height / 2.0f) - (label->height / 2.0f));
	bar->position.assign (label->position.x, label->position.y + label->height);
}

bool Ui::keyEvent (void *uiPtr, SDL_Keycode keycode, bool isShiftDown, bool isControlDown) {
	return (((Ui *) uiPtr)->processKeyEvent (keycode, isShiftDown, isControlDown));
}

bool Ui::processKeyEvent (SDL_Keycode keycode, bool isShiftDown, bool isControlDown) {
	// Base class method takes no action

	return (doProcessKeyEvent (keycode, isShiftDown, isControlDown));
}

bool Ui::doProcessKeyEvent (SDL_Keycode keycode, bool isShiftDown, bool isControlDown) {
	// Default implementation does nothing
	return (false);
}

bool Ui::doProcessWindowCloseEvent () {
	// Default implementation does nothing
	return (false);
}

void Ui::resume () {
	StdString title;

	if (! isLoaded) {
		return;
	}
	if (rootPanel->id <= 0) {
		rootPanel->id = App::instance->getUniqueId ();
	}
	rootPanel->position.assign (0.0f, 0.0f);
	rootPanel->setFixedSize (true, App::instance->windowWidth, App::instance->windowHeight);
	rootPanel->resetInputState ();
	rootPanel->isInputSuspended = false;
	App::instance->rootPanel->addWidget (rootPanel);
	lastWindowCloseCount = Input::instance->windowCloseCount;

	doResume ();
	isFirstResumeComplete = true;
}

void Ui::pause () {
	if (! isLoaded) {
		return;
	}
	App::instance->rootPanel->removeWidget (rootPanel);
	lastWindowCloseCount = Input::instance->windowCloseCount;
	clearPopupWidgets ();
	doPause ();
}

void Ui::update (int msElapsed) {
	int count;

	count = Input::instance->windowCloseCount;
	if (lastWindowCloseCount != count) {
		lastWindowCloseCount = count;
		if (! doProcessWindowCloseEvent ()) {
			App::instance->shutdown ();
		}
	}
	doUpdate (msElapsed);
}

void Ui::draw () {
	// Base class method takes no action
	doDraw ();
}

void Ui::refresh () {
	rootPanel->refresh ();
	doRefresh ();
}

void Ui::doRefresh () {
	// Default implementation does nothing
}

void Ui::resize () {
	rootPanel->position.assign (0.0f, 0.0f);
	rootPanel->setFixedSize (true, App::instance->windowWidth, App::instance->windowHeight);

	sprites.resize ();

	doResize ();
	rootPanel->resetInputState ();
}

void Ui::doResize () {
	// Default implementation does nothing
}

void Ui::doClearPopupWidgets () {
	// Default implementation does nothing
}

void Ui::doResume () {
	// Default implementation does nothing
}

void Ui::doPause () {
	// Default implementation does nothing
}

void Ui::doUpdate (int msElapsed) {
	// Default implementation does nothing
}

void Ui::doDraw () {
	// Default implementation does nothing
}

void Ui::clearPopupWidgets () {
	doClearPopupWidgets ();
}

Widget *Ui::addWidget (Widget *widget, float positionX, float positionY, int zLevel) {
	return (rootPanel->addWidget (widget, positionX, positionY, zLevel));
}
