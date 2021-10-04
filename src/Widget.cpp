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
#include <math.h>
#include "SDL2/SDL.h"
#include "App.h"
#include "Log.h"
#include "StdString.h"
#include "Input.h"
#include "UiConfiguration.h"
#include "Widget.h"

const int Widget::MinZLevel = -10;

Widget::Widget ()
: id (0)
, isDestroyed (false)
, isVisible (true)
, isTextureTargetDrawEnabled (true)
, isInputSuspended (false)
, isPanelSizeClipEnabled (false)
, zLevel (0)
, isMouseHoverEnabled (false)
, classId (-1)
, hasScreenPosition (false)
, screenX (0.0f)
, screenY (0.0f)
, isKeyFocused (false)
, tooltipAlignment (Widget::BottomAlignment)
, width (0.0f)
, height (0.0f)
, destroyClock (0)
, isFixedCenter (false)
, isMouseEntered (false)
, isMousePressed (false)
, refcount (0)
, refcountMutex (NULL)
{
	refcountMutex = SDL_CreateMutex ();
}

Widget::~Widget () {
	if (refcountMutex) {
		SDL_DestroyMutex (refcountMutex);
		refcountMutex = NULL;
	}
}

void Widget::retain () {

	SDL_LockMutex (refcountMutex);
	++refcount;
	if (refcount < 1) {
		refcount = 1;
	}
	SDL_UnlockMutex (refcountMutex);
}

void Widget::release () {
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

void Widget::resetInputState () {
	isMouseEntered = false;
	isMousePressed = false;

	doResetInputState ();
}

void Widget::doResetInputState () {
	// Default implementation does nothing
}

void Widget::setMouseHoverTooltip (const StdString &text, Widget::Alignment alignment) {
	tooltipText.assign (text);
	tooltipAlignment = alignment;
	isMouseHoverEnabled = true;
}

void Widget::update (int msElapsed, float originX, float originY) {
	float x, y;


	if (destroyClock > 0) {
		destroyClock -= msElapsed;
		if (destroyClock <= 0) {
			isDestroyed = true;
		}
	}
	if (isDestroyed) {
		return;
	}

	position.update (msElapsed);
	screenX = position.x + originX;
	screenY = position.y + originY;
	hasScreenPosition = true;

	doUpdate (msElapsed);

	if (isFixedCenter) {
		fixedCenterPosition.update (msElapsed);
		x = fixedCenterPosition.x - (width / 2.0f);
		y = fixedCenterPosition.y - (height / 2.0f);

		// TODO: Possibly use a smooth translation here
		if (! position.equals (x, y)) {
			position.assign (x, y);
			screenX = position.x + originX;
			screenY = position.y + originY;
		}
	}

	if (updateCallback.callback) {
		updateCallback.callback (updateCallback.callbackData, msElapsed, this);
	}
}

void Widget::doUpdate (int msElapsed) {
	// Default implementation does nothing
}

void Widget::draw (SDL_Texture *targetTexture, float originX, float originY) {
	if (isDestroyed) {
		return;
	}
	if (targetTexture) {
		if (! isTextureTargetDrawEnabled) {
			return;
		}
		SDL_SetRenderTarget (App::instance->render, targetTexture);
	}
	doDraw (targetTexture, originX, originY);
	if (targetTexture) {
		SDL_SetRenderTarget (App::instance->render, NULL);
	}
}

void Widget::doDraw (SDL_Texture *targetTexture, float originX, float originY) {
	// Default implementation does nothing
}

void Widget::refresh () {
	// Superclass method takes no action
	doRefresh ();
}

void Widget::doRefresh () {
	// Default implementation does nothing
}

StdString Widget::toString () {
	return (StdString::createSprintf ("<#%llu / %i%s>", (unsigned long long) id, classId, toStringDetail ().c_str ()));
}

StdString Widget::toStringDetail () {
  return (StdString (""));
}

void Widget::setDestroyDelay (int delayMs) {
	if (delayMs <= 0) {
		isDestroyed = true;
		return;
	}
	destroyClock = delayMs;
}

void Widget::setFixedCenter (bool enable) {
	isFixedCenter = enable;
	if (isFixedCenter) {
		fixedCenterPosition.assign (position.x + (width / 2.0f), position.y + (height / 2.0f));
	}
}

void Widget::setKeyFocus (bool enable) {
	// Default implementation does nothing
}

bool Widget::processKeyEvent (SDL_Keycode keycode, bool isShiftDown, bool isControlDown) {

	if (isInputSuspended) {
		return (false);
	}

	if (keyEventCallback.callback && keyEventCallback.callback (keyEventCallback.callbackData, keycode, isShiftDown, isControlDown)) {
		return (true);
	}

	return (doProcessKeyEvent (keycode, isShiftDown, isControlDown));
}

bool Widget::doProcessKeyEvent (SDL_Keycode keycode, bool isShiftDown, bool isControlDown) {
	// Default implementation does nothing
	return (false);
}

Widget *Widget::findWidget (float screenPositionX, float screenPositionY, bool requireMouseHoverEnabled) {
	// Default implementation returns NULL
	return (NULL);
}

bool Widget::processMouseState (const Widget::MouseState &mouseState) {
	if (isInputSuspended) {
		return (false);
	}

	if (mouseState.isEntered) {
		if (! isMouseEntered) {
			isMouseEntered = true;
			eventCallback (mouseEnterCallback);
		}

		if (mouseState.isLeftClicked) {
			if (! isMousePressed) {
				isMousePressed = true;
				eventCallback (mousePressCallback);
			}
		}

		if (isMousePressed && mouseState.isLeftClickReleased) {
			isMousePressed = false;
			eventCallback (mouseReleaseCallback);
			if (mouseState.isLeftClickEntered) {
				eventCallback (mouseClickCallback);
			}
		}

		if (isMousePressed && mouseState.isLongPressed) {
			if (mouseLongPressCallback.callback) {
				isMousePressed = false;
				mouseLongPressCallback.callback (mouseLongPressCallback.callbackData, this);
			}
		}
	}
	else {
		if (isMousePressed) {
			isMousePressed = false;
			eventCallback (mouseReleaseCallback);
		}
		if (isMouseEntered) {
			isMouseEntered = false;
			eventCallback (mouseExitCallback);
		}
	}

	return (doProcessMouseState (mouseState));
}

bool Widget::doProcessMouseState (const Widget::MouseState &mouseState) {
	// Default implementation does nothing
	return (false);
}

bool Widget::compareZLevel (Widget *first, Widget *second) {
	return (first->zLevel < second->zLevel);
}

void Widget::mouseEnter () {
	eventCallback (mouseEnterCallback);
}

void Widget::mouseExit () {
	eventCallback (mouseExitCallback);
}

void Widget::mousePress () {
	eventCallback (mousePressCallback);
}

void Widget::mouseRelease () {
	eventCallback (mouseReleaseCallback);
}

void Widget::mouseClick () {
	eventCallback (mouseClickCallback);
}

bool Widget::eventCallback (const Widget::EventCallbackContext &callback) {
	if (! callback.callback) {
		return (false);
	}
	callback.callback (callback.callbackData, this);
	return (true);
}

void Widget::flowRight (float *positionX, float positionY, float *rightExtent, float *bottomExtent) {
	float pos;

	position.assign (*positionX, positionY);
	*positionX += width + UiConfiguration::instance->marginSize;
	if (rightExtent) {
		pos = position.x + width;
		if (pos > *rightExtent) {
			*rightExtent = pos;
		}
	}
	if (bottomExtent) {
		pos = position.y + height;
		if (pos > *bottomExtent) {
			*bottomExtent = pos;
		}
	}
}

void Widget::flowDown (float positionX, float *positionY, float *rightExtent, float *bottomExtent) {
	float pos;

	position.assign (positionX, *positionY);
	*positionY += height + UiConfiguration::instance->marginSize;
	if (rightExtent) {
		pos = position.x + width;
		if (pos > *rightExtent) {
			*rightExtent = pos;
		}
	}
	if (bottomExtent) {
		pos = position.y + height;
		if (pos > *bottomExtent) {
			*bottomExtent = pos;
		}
	}
}

void Widget::flowLeft (float *positionX) {
	*positionX -= width;
	position.assignX (*positionX);
	*positionX -= UiConfiguration::instance->marginSize;
}

void Widget::flowLeft (float *positionX, float positionY) {
	flowLeft (positionX);
	position.assignY (positionY);
}

void Widget::flowUp (float *positionY) {
	*positionY -= height;
	position.assignY (*positionY);
	*positionY -= UiConfiguration::instance->marginSize;
}

void Widget::flowUp (float positionX, float *positionY) {
	flowUp (positionY);
	position.assignX (positionX);
}

void Widget::centerHorizontal (float leftExtent, float rightExtent) {
	position.assignX (leftExtent + ((rightExtent - leftExtent) / 2.0f) - (width / 2.0f));
}

void Widget::centerVertical (float topExtent, float bottomExtent) {
	position.assignY (topExtent + ((bottomExtent - topExtent) / 2.0f) - (height / 2.0f));
}

Widget::Rectangle Widget::getScreenRect () {
	Widget::Rectangle rect;

	if (hasScreenPosition) {
		rect.x = screenX;
		rect.y = screenY;
		rect.w = width;
		rect.h = height;
	}

	return (rect);
}
