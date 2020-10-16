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
#include "Config.h"
#include <stdlib.h>
#include <math.h>
#include <list>
#include "Result.h"
#include "Log.h"
#include "StdString.h"
#include "App.h"
#include "Widget.h"
#include "Color.h"
#include "Panel.h"
#include "Sprite.h"
#include "Label.h"
#include "Image.h"
#include "Button.h"
#include "Ui.h"
#include "UiConfiguration.h"
#include "ScrollBar.h"

ScrollBar::ScrollBar (float maxScrollTrackLength)
: Panel ()
, scrollPosition (0.0f)
, maxScrollPosition (0.0f)
, maxTrackLength (maxScrollTrackLength)
, trackLength (0.0f)
, trackWidth (0.0f)
, arrowPanel (NULL)
, upArrowImage (NULL)
, downArrowImage (NULL)
, isFollowingMouse (false)
{
	UiConfiguration *uiconfig;
	float w, h;

	uiconfig = &(App::instance->uiConfig);

	setFillBg (true, Color (0.0f, 0.0f, 0.0f, uiconfig->overlayWindowAlpha));
	setBorder (true, Color (uiconfig->darkBackgroundColor.r, uiconfig->darkBackgroundColor.g, uiconfig->darkBackgroundColor.b, uiconfig->overlayWindowAlpha));

	// Only vertical orientation is supported in this implementation

	upArrowImage = (Image *) addWidget (new Image (uiconfig->coreSprites.getSprite (UiConfiguration::ScrollUpArrowSprite)));
	upArrowImage->zLevel = 1;
	upArrowImage->isInputSuspended = true;
	w = upArrowImage->width;
	h = upArrowImage->height;

	downArrowImage = (Image *) addWidget (new Image (uiconfig->coreSprites.getSprite (UiConfiguration::ScrollDownArrowSprite)));
	downArrowImage->zLevel = 1;
	downArrowImage->isInputSuspended = true;
	if (downArrowImage->width > w) {
		w = downArrowImage->width;
	}
	if (downArrowImage->height > h) {
		h = downArrowImage->height;
	}

	trackWidth = w;
	arrowPanel = (Panel *) addWidget (new Panel ());
	arrowPanel->setFillBg (true, uiconfig->mediumPrimaryColor);
	arrowPanel->setFixedSize (true, trackWidth, upArrowImage->height + downArrowImage->height + (h * 0.25f));
	if (maxTrackLength < arrowPanel->height) {
		maxTrackLength = arrowPanel->height;
	}

	refreshLayout ();
}

ScrollBar::~ScrollBar () {

}

void ScrollBar::setPosition (float positionValue, bool shouldSkipCallback) {
	float pos;

	pos = positionValue;
	if (pos < 0.0f) {
		pos = 0.0f;
	}
	if (pos > maxScrollPosition) {
		pos = maxScrollPosition;
	}
	if (FLOAT_EQUALS (scrollPosition, pos)) {
		return;
	}

	scrollPosition = pos;
	refreshLayout ();
	if (positionChangeCallback.callback && (! shouldSkipCallback)) {
		positionChangeCallback.callback (positionChangeCallback.callbackData, this);
	}
}

void ScrollBar::setScrollBounds (float scrollViewHeight, float scrollAreaHeight) {
	float ratio;

	if ((scrollViewHeight <= 0.0f) || (scrollAreaHeight <= 0.0f)) {
		return;
	}

	ratio = (scrollAreaHeight / scrollViewHeight);
	trackLength = (1.0f + ratio) * arrowPanel->height;
	if (trackLength > maxTrackLength) {
		trackLength = maxTrackLength;
	}

	maxScrollPosition = scrollAreaHeight - scrollViewHeight;
	if (maxScrollPosition < 0.0f) {
		maxScrollPosition = 0.0f;
	}
	if (scrollPosition > maxScrollPosition) {
		scrollPosition = maxScrollPosition;
	}

	refreshLayout ();
}

void ScrollBar::setMaxTrackLength (float maxScrollTrackLength) {
	float len;

	len = maxScrollTrackLength;
	if (len < arrowPanel->height) {
		len = arrowPanel->height;
	}
	if (FLOAT_EQUALS (len, maxTrackLength)) {
		return;
	}

	maxTrackLength = len;
	refreshLayout ();
}

void ScrollBar::refreshLayout () {
	float y;

	setFixedSize (true, trackWidth, trackLength);
	if (maxScrollPosition <= 0.0f) {
		y = 0.0f;
	}
	else {
		y = scrollPosition / maxScrollPosition;
		y *= trackLength;
		y -= (arrowPanel->height / 2.0f);
		if (y < 0.0f) {
			y = 0.0f;
		}
		if (y > (height - arrowPanel->height)) {
			y = height - arrowPanel->height;
		}
	}
	arrowPanel->position.assign (0.0f, y);

	upArrowImage->position.assign (0.0f, arrowPanel->position.y);
	downArrowImage->position.assign (0.0f, arrowPanel->position.y + arrowPanel->height - downArrowImage->height);
}

void ScrollBar::doUpdate (int msElapsed) {
	Input *input;
	float dy, val;

	Panel::doUpdate (msElapsed);
	if (isFollowingMouse) {
		input = &(App::instance->input);
		if (! input->isMouseLeftButtonDown) {
			isFollowingMouse = false;
		}
		else {
			dy = ((float) input->mouseY) - screenY;
			if (dy < 0.0f) {
				dy = 0.0f;
			}
			if (dy > trackLength) {
				dy = trackLength;
			}
			val = (dy / trackLength);
			val *= maxScrollPosition;
			setPosition (val);
		}
	}
}

bool ScrollBar::doProcessMouseState (const Widget::MouseState &mouseState) {
	Input *input;
	UiConfiguration *uiconfig;
	float val, dy;

	input = &(App::instance->input);
	uiconfig = &(App::instance->uiConfig);

	if (! isFollowingMouse) {
		if (mouseState.isEntered && mouseState.isLeftClicked) {
			isFollowingMouse = true;
			dy = ((float) input->mouseY) - screenY;
			if (dy < 0.0f) {
				dy = 0.0f;
			}
			if (dy > trackLength) {
				dy = trackLength;
			}
			val = (dy / trackLength);
			val *= maxScrollPosition;
			setPosition (val);
		}
	}
	else {
		if (! input->isMouseLeftButtonDown) {
			isFollowingMouse = false;
		}
	}

	if (mouseState.isEntered || isFollowingMouse) {
		arrowPanel->bgColor.translate (uiconfig->mediumPrimaryColor, uiconfig->shortColorTranslateDuration);
	}
	else {
		arrowPanel->bgColor.translate (uiconfig->lightPrimaryColor, uiconfig->shortColorTranslateDuration);
	}

	return (false);
}

void ScrollBar::doRefresh () {
	float w, h;

	Panel::doRefresh ();
	w = upArrowImage->width;
	h = upArrowImage->height;
	if (downArrowImage->width > w) {
		w = downArrowImage->width;
	}
	if (downArrowImage->height > h) {
		h = downArrowImage->height;
	}

	trackWidth = w;
	arrowPanel->setFixedSize (true, trackWidth, upArrowImage->height + downArrowImage->height + (h * 0.25f));
	if (maxTrackLength < arrowPanel->height) {
		maxTrackLength = arrowPanel->height;
	}
	refreshLayout ();
}

void ScrollBar::doResetInputState () {
	isFollowingMouse = false;
}
