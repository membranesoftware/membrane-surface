/*
* Copyright 2018-2022 Membrane Software <author@membranesoftware.com> https://membranesoftware.com
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
#include "Log.h"
#include "StdString.h"
#include "App.h"
#include "Widget.h"
#include "Color.h"
#include "Panel.h"
#include "Label.h"
#include "CountdownWindow.h"

CountdownWindow::CountdownWindow (Sprite *iconSprite, const StdString &labelText, UiConfiguration::FontType labelTextFontType)
: Panel ()
, isRightAligned (false)
, label (NULL)
, image (NULL)
, progressBar (NULL)
, countdownTime (0)
, countdownClock (0)
{
	setFillBg (true, UiConfiguration::instance->lightBackgroundColor);
	setBorder (true, UiConfiguration::instance->mediumInverseBackgroundColor);

	label = (Label *) addWidget (new Label (labelText, labelTextFontType, UiConfiguration::instance->primaryTextColor));
	image = (Image *) addWidget (new Image (iconSprite));
	progressBar = (ProgressBar *) addWidget (new ProgressBar ());

	refreshLayout ();
}

CountdownWindow::~CountdownWindow () {

}

void CountdownWindow::reveal () {
	borderColor.translate (UiConfiguration::instance->lightBackgroundColor, UiConfiguration::instance->mediumInverseBackgroundColor, UiConfiguration::instance->longColorTranslateDuration);
	bgColor.translate (UiConfiguration::instance->mediumInverseBackgroundColor, UiConfiguration::instance->lightBackgroundColor, UiConfiguration::instance->longColorTranslateDuration);
	label->textColor.translate (UiConfiguration::instance->darkInverseTextColor, UiConfiguration::instance->primaryTextColor, UiConfiguration::instance->longColorTranslateDuration);
	image->translateAlpha (0.0f, 1.0f, UiConfiguration::instance->longColorTranslateDuration);
}

void CountdownWindow::countdown (int duration) {
	if (duration <= 0) {
		return;
	}
	countdownTime = duration;
	countdownClock = duration;
	progressBar->setProgress (0.0f, (float) countdownTime);
}

void CountdownWindow::doUpdate (int msElapsed) {
	Panel::doUpdate (msElapsed);
	if (countdownClock > 0) {
		countdownClock -= msElapsed;
		if (countdownClock < 0) {
			countdownClock = 0;
		}
		progressBar->setProgress ((float) (countdownTime - countdownClock));
	}
}

void CountdownWindow::refreshLayout () {
	float x, y, x2, y2;

	x = UiConfiguration::instance->paddingSize;
	y = UiConfiguration::instance->paddingSize;
	x2 = 0.0f;
	y2 = 0.0f;

	if (isRightAligned) {
		label->flowRight (&x, y, &x2, &y2);
		image->flowRight (&x, y, &x2, &y2);
	}
	else {
		image->flowRight (&x, y, &x2, &y2);
		label->flowRight (&x, y, &x2, &y2);
	}

	image->position.assignY (y + ((y2 - y) / 2.0f) - (image->height / 2.0f));
	label->position.assignY (y + ((y2 - y) / 2.0f) - (label->height / 2.0f));

	y = y2 + UiConfiguration::instance->marginSize;
	x2 += UiConfiguration::instance->paddingSize;
	progressBar->setSize (x2, UiConfiguration::instance->progressBarHeight);
	progressBar->flowDown (0.0f, &y, &x2, &y2);
	setFixedSize (true, x2, y2);
}
