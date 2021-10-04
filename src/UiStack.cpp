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
#include "SDL2/SDL.h"
#include "App.h"
#include "OsUtil.h"
#include "StdString.h"
#include "Log.h"
#include "Input.h"
#include "Ui.h"
#include "UiConfiguration.h"
#include "TooltipWindow.h"
#include "UiStack.h"

UiStack::UiStack ()
: activeUi (NULL)
, uiMutex (NULL)
, nextCommandType (-1)
, nextCommandUi (NULL)
, nextCommandMutex (NULL)
, mouseHoverClock (0)
, isMouseHoverActive (false)
, isMouseHoverSuspended (false)
{
	uiMutex = SDL_CreateMutex ();
	nextCommandMutex = SDL_CreateMutex ();
}

UiStack::~UiStack () {
	clear ();
	if (activeUi) {
		activeUi->release ();
		activeUi = NULL;
	}
	if (uiMutex) {
		SDL_DestroyMutex (uiMutex);
		uiMutex = NULL;
	}
	if (nextCommandMutex) {
		SDL_DestroyMutex (nextCommandMutex);
		nextCommandMutex = NULL;
	}
}

void UiStack::clear () {
	Ui *ui;

	tooltip.destroyAndClear ();
	keyFocusTarget.clear ();
	mouseHoverTarget.clear ();

	SDL_LockMutex (uiMutex);
	while (! uiList.empty ()) {
		ui = uiList.back ();
		ui->pause ();
		ui->unload ();
		ui->release ();
		uiList.pop_back ();
	}
	SDL_UnlockMutex (uiMutex);
}

Ui *UiStack::getActiveUi () {
	Ui *ui;

	ui = NULL;
	SDL_LockMutex (uiMutex);
	if (activeUi) {
		ui = activeUi;
		ui->retain ();
	}
	SDL_UnlockMutex (uiMutex);

	return (ui);
}

void UiStack::setUi (Ui *ui) {
	if (! ui) {
		return;
	}

	SDL_LockMutex (nextCommandMutex);
	nextCommandType = UiStack::SetUiCommand;
	if (nextCommandUi) {
		nextCommandUi->release ();
	}
	nextCommandUi = ui;
	nextCommandUi->retain ();
	SDL_UnlockMutex (nextCommandMutex);
}

void UiStack::pushUi (Ui *ui) {
	if (! ui) {
		return;
	}

	SDL_LockMutex (nextCommandMutex);
	nextCommandType = UiStack::PushUiCommand;
	if (nextCommandUi) {
		nextCommandUi->release ();
	}
	nextCommandUi = ui;
	nextCommandUi->retain ();
	SDL_UnlockMutex (nextCommandMutex);
}

void UiStack::popUi () {
	SDL_LockMutex (nextCommandMutex);
	nextCommandType = UiStack::PopUiCommand;
	if (nextCommandUi) {
		nextCommandUi->release ();
	}
	nextCommandUi = NULL;
	SDL_UnlockMutex (nextCommandMutex);
}

void UiStack::update (int msElapsed) {
	Ui *ui;
	Widget *keywidget, *mousewidget;

	SDL_LockMutex (uiMutex);
	if (! uiList.empty ()) {
		ui = uiList.back ();
		if (ui != activeUi) {
			if (activeUi) {
				activeUi->pause ();
				activeUi->release ();
			}
			activeUi = ui;
			activeUi->retain ();

			tooltip.destroyAndClear ();
			keyFocusTarget.clear ();
			mouseHoverTarget.clear ();
			activeUi->resume ();
		}
	}
	SDL_UnlockMutex (uiMutex);

	tooltip.compact ();
	keyFocusTarget.compact ();
	mouseHoverTarget.compact ();

	keywidget = keyFocusTarget.widget;
	if (keywidget && (! keywidget->isKeyFocused)) {
		keyFocusTarget.clear ();
	}

	mousewidget = App::instance->rootPanel->findWidget ((float) Input::instance->mouseX, (float) Input::instance->mouseY, true);
	if (! mousewidget) {
		mouseHoverTarget.clear ();
		deactivateMouseHover ();
	}
	else {
		if (! mouseHoverTarget.equals (mousewidget)) {
			mouseHoverTarget.assign (mousewidget);
			deactivateMouseHover ();
		}
		else {
			if ((! isMouseHoverActive) && (! isMouseHoverSuspended)) {
				mouseHoverClock -= msElapsed;
				if (mouseHoverClock <= 0) {
					activateMouseHover ();
				}
			}
		}
	}
}

void UiStack::executeStackCommands () {
	Ui *ui, *item;
	int cmd, result;

	SDL_LockMutex (nextCommandMutex);
	cmd = nextCommandType;
	ui = nextCommandUi;
	nextCommandType = -1;
	nextCommandUi = NULL;
	SDL_UnlockMutex (nextCommandMutex);

	if (cmd < 0) {
		if (ui) {
			ui->release ();
		}
		return;
	}

	switch (cmd) {
		case UiStack::SetUiCommand: {
			if (! ui) {
				break;
			}
			result = ui->load ();
			if (result != OsUtil::Result::Success) {
				Log::err ("Failed to load UI resources; err=%i", result);
				ui->release ();
				break;
			}

			App::instance->suspendUpdate ();
			SDL_LockMutex (uiMutex);
			while (! uiList.empty ()) {
				item = uiList.back ();
				item->pause ();
				item->unload ();
				item->release ();
				uiList.pop_back ();
			}
			uiList.push_back (ui);
			SDL_UnlockMutex (uiMutex);
			App::instance->unsuspendUpdate ();
			break;
		}
		case UiStack::PushUiCommand: {
			if (! ui) {
				break;
			}
			result = ui->load ();
			if (result != OsUtil::Result::Success) {
				Log::err ("Failed to load UI resources; err=%i", result);
				ui->release ();
				break;
			}

			App::instance->suspendUpdate ();
			SDL_LockMutex (uiMutex);
			uiList.push_back (ui);
			SDL_UnlockMutex (uiMutex);
			App::instance->unsuspendUpdate ();
			break;
		}
		case UiStack::PopUiCommand: {
			if (ui) {
				ui->release ();
			}
			ui = NULL;

			App::instance->suspendUpdate ();
			SDL_LockMutex (uiMutex);
			if (! uiList.empty ()) {
				ui = uiList.back ();
				uiList.pop_back ();

				if (activeUi == ui) {
					activeUi->release ();
					activeUi = NULL;
				}
			}
			SDL_UnlockMutex (uiMutex);

			if (ui) {
				ui->pause ();
				ui->unload ();
				ui->release ();
			}
			App::instance->unsuspendUpdate ();
			break;
		}
		default: {
			if (ui) {
				ui->release ();
			}
			break;
		}
	}
}

void UiStack::refresh () {
	std::list<Ui *>::iterator i, end;

	SDL_LockMutex (uiMutex);
	i = uiList.begin ();
	end = uiList.end ();
	while (i != end) {
		(*i)->refresh ();
		++i;
	}
	SDL_UnlockMutex (uiMutex);
}

void UiStack::resize () {
	std::list<Ui *>::iterator i, end;

	SDL_LockMutex (uiMutex);
	i = uiList.begin ();
	end = uiList.end ();
	while (i != end) {
		(*i)->resize ();
		++i;
	}
	SDL_UnlockMutex (uiMutex);
}

bool UiStack::processKeyEvent (SDL_Keycode keycode, bool isShiftDown, bool isControlDown) {
	return (false);
}

void UiStack::setKeyFocusTarget (Widget *widget) {
	Widget *target;

	target = keyFocusTarget.widget;
	if (target == widget) {
		return;
	}
	if (target) {
		target->setKeyFocus (false);
	}

	keyFocusTarget.assign (widget);
	target = keyFocusTarget.widget;
	if (target) {
		target->setKeyFocus (true);
	}
}

void UiStack::activateMouseHover () {
	Widget *widget;
	TooltipWindow *tooltipwindow;
	StdString text;
	float x, y, max;

	tooltip.destroyAndClear ();
	widget = mouseHoverTarget.widget;
	if (widget) {
		text.assign (widget->tooltipText);
	}
	if (widget && (! text.empty ())) {
		tooltipwindow = (TooltipWindow *) App::instance->rootPanel->addWidget (new TooltipWindow (text));
		tooltipwindow->zLevel = App::instance->rootPanel->maxWidgetZLevel + 1;
		tooltip.assign (tooltipwindow);

		x = widget->screenX;
		y = widget->screenY;
		switch (widget->tooltipAlignment) {
			case Widget::TopAlignment: {
				x += ((widget->width / 2.0f) - (tooltipwindow->width / 2.0f));
				y -= (tooltipwindow->height + UiConfiguration::instance->marginSize);

				if (x < UiConfiguration::instance->paddingSize) {
					x = UiConfiguration::instance->paddingSize;
				}
				max = App::instance->rootPanel->width - tooltipwindow->width - UiConfiguration::instance->paddingSize;
				if (x > max) {
					x = max;
				}

				if (y < UiConfiguration::instance->paddingSize) {
					y = widget->screenY + widget->height + UiConfiguration::instance->marginSize;
				}
				break;
			}
			case Widget::LeftAlignment: {
				x -= (tooltipwindow->width + UiConfiguration::instance->marginSize);
				y += ((widget->height / 2.0f) - (tooltipwindow->height / 2.0f));

				if (y < UiConfiguration::instance->paddingSize) {
					y = UiConfiguration::instance->paddingSize;
				}
				max = App::instance->rootPanel->height - tooltipwindow->height - UiConfiguration::instance->paddingSize;
				if (y > max) {
					y = max;
				}

				if (x < UiConfiguration::instance->paddingSize) {
					x = widget->screenX + widget->width + UiConfiguration::instance->marginSize;
				}
				break;
			}
			case Widget::RightAlignment: {
				x += (widget->width + UiConfiguration::instance->marginSize);
				y += ((widget->height / 2.0f) - (tooltipwindow->height / 2.0f));

				if (y < UiConfiguration::instance->paddingSize) {
					y = UiConfiguration::instance->paddingSize;
				}
				max = App::instance->rootPanel->height - tooltipwindow->height - UiConfiguration::instance->paddingSize;
				if (y > max) {
					y = max;
				}

				if ((x + tooltipwindow->width) >= (App::instance->rootPanel->width - UiConfiguration::instance->paddingSize)) {
					x = widget->screenX - (tooltipwindow->width + UiConfiguration::instance->marginSize);
				}
				break;
			}
			case Widget::BottomAlignment:
			default: {
				x += ((widget->width / 2.0f) - (tooltipwindow->width / 2.0f));
				y += (widget->height + UiConfiguration::instance->marginSize);

				if (x < UiConfiguration::instance->paddingSize) {
					x = UiConfiguration::instance->paddingSize;
				}
				max = App::instance->rootPanel->width - tooltipwindow->width - UiConfiguration::instance->paddingSize;
				if (x > max) {
					x = max;
				}

				if ((y + tooltipwindow->height) >= (App::instance->rootPanel->height - UiConfiguration::instance->paddingSize)) {
					y = widget->screenY - (tooltipwindow->height + UiConfiguration::instance->marginSize);
				}
				break;
			}
		}

		tooltipwindow->position.assign (x, y);
	}

	isMouseHoverSuspended = false;
	isMouseHoverActive = true;
}

void UiStack::deactivateMouseHover () {
	tooltip.destroyAndClear ();
	mouseHoverClock = UiConfiguration::instance->mouseHoverThreshold;
	isMouseHoverSuspended = false;
	isMouseHoverActive = false;
}

void UiStack::suspendMouseHover () {
	tooltip.destroyAndClear ();
	mouseHoverClock = UiConfiguration::instance->mouseHoverThreshold;
	isMouseHoverActive = false;
	isMouseHoverSuspended = true;
}
