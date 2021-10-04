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
// Panel that shows a countdown indicator

#ifndef COUNTDOWN_WINDOW_H
#define COUNTDOWN_WINDOW_H

#include "UiConfiguration.h"
#include "Image.h"
#include "Label.h"
#include "ProgressBar.h"
#include "Panel.h"

class CountdownWindow : public Panel {
public:
	CountdownWindow (Sprite *iconSprite, const StdString &labelText = StdString (""), UiConfiguration::FontType labelTextFontType = UiConfiguration::BodyFont);
 	virtual ~CountdownWindow ();

	// Read-only data members
	bool isRightAligned;

	// Begin the window's reveal animation
	void reveal ();

	// Begin the window's countdown animation with a timeout of duration milliseconds
	void countdown (int duration);

protected:
	// Reset the panel's widget layout as appropriate for its content and configuration
	void refreshLayout ();

	// Execute operations to update object state as appropriate for an elapsed millisecond time period
	void doUpdate (int msElapsed);

private:
	Label *label;
	Image *image;
	ProgressBar *progressBar;
	int countdownTime;
	int countdownClock;
};

#endif
