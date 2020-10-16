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
#include <map>
#include <list>
#include "Result.h"
#include "Log.h"
#include "StdString.h"
#include "App.h"
#include "Widget.h"
#include "Color.h"
#include "Panel.h"
#include "Label.h"
#include "Image.h"
#include "Button.h"
#include "ImageWindow.h"
#include "LabelWindow.h"
#include "UiConfiguration.h"
#include "Toolbar.h"

Toolbar::Toolbar (float toolbarWidth)
: Panel ()
, barWidth (toolbarWidth)
{
	UiConfiguration *uiconfig;

	uiconfig = &(App::instance->uiConfig);
	setPadding (uiconfig->paddingSize, uiconfig->paddingSize);
	setFillBg (true, uiconfig->darkPrimaryColor);

	refreshLayout ();
}

Toolbar::~Toolbar () {
	clearLeftItems ();
	clearRightItems ();
	leftCorner.clear ();
	rightCorner.clear ();
	leftOverlay.clear ();
	rightOverlay.clear ();
}

void Toolbar::setWidth (float toolbarWidth) {
	if (FLOAT_EQUALS (toolbarWidth, barWidth)) {
		return;
	}
	barWidth = toolbarWidth;
	refreshLayout ();
}

bool Toolbar::empty () {
	if ((! leftItemList.empty ()) || (! rightItemList.empty ())) {
		return (false);
	}
	if (leftCorner.widget || rightCorner.widget || leftOverlay.widget || rightOverlay.widget) {
		return (false);
	}

	return (true);
}

void Toolbar::clearAll () {
	leftOverlay.destroyAndClear ();
	rightOverlay.destroyAndClear ();
	clearLeftItems ();
	clearRightItems ();
	leftCorner.destroyAndClear ();
	rightCorner.destroyAndClear ();
}

void Toolbar::clearLeftItems () {
	std::list<Widget *>::iterator i, end;
	Widget *widget;

	i = leftItemList.begin ();
	end = leftItemList.end ();
	while (i != end) {
		widget = *i;
		widget->isDestroyed = true;
		widget->release ();
		++i;
	}
	leftItemList.clear ();
}

void Toolbar::clearRightItems () {
	std::list<Widget *>::iterator i, end;
	Widget *widget;

	i = rightItemList.begin ();
	end = rightItemList.end ();
	while (i != end) {
		widget = *i;
		widget->isDestroyed = true;
		widget->release ();
		++i;
	}
	rightItemList.clear ();
}

void Toolbar::clearLeftOverlay () {
	leftOverlay.destroyAndClear ();
	refreshLayout ();
}

void Toolbar::addLeftItem (Widget *itemWidget) {
	addWidget (itemWidget);
	itemWidget->retain ();
	leftItemList.push_back (itemWidget);
	refreshLayout ();
}

void Toolbar::addRightItem (Widget *itemWidget) {
	addWidget (itemWidget);
	itemWidget->retain ();
	// TODO: Prevent left toolbar items from rendering into the right item area (currently increasing right item z-level as a workaround)
	itemWidget->zLevel = 1;
	rightItemList.push_back (itemWidget);
	refreshLayout ();
}

void Toolbar::addRightSpacer () {
	UiConfiguration *uiconfig;
	Panel *panel;

	uiconfig = &(App::instance->uiConfig);
	panel = new Panel ();
	panel->setFixedSize (true, uiconfig->marginSize, 2.0f);
	addRightItem (panel);
}

void Toolbar::setLeftCorner (Widget *itemWidget) {
	if (leftCorner.widget) {
		leftCorner.widget->isDestroyed = true;
	}
	addWidget (itemWidget);
	leftCorner.assign (itemWidget);
	refreshLayout ();
}

void Toolbar::setRightCorner (Widget *itemWidget) {
	if (rightCorner.widget) {
		rightCorner.widget->isDestroyed = true;
	}
	addWidget (itemWidget);
	rightCorner.assign (itemWidget);
	refreshLayout ();
}

void Toolbar::setLeftOverlay (Widget *itemWidget) {
	if (leftOverlay.widget) {
		leftOverlay.widget->isDestroyed = true;
	}
	addWidget (itemWidget);
	leftOverlay.assign (itemWidget);
	refreshLayout ();
}

void Toolbar::setRightOverlay (Widget *itemWidget) {
	if (rightOverlay.widget) {
		rightOverlay.widget->isDestroyed = true;
	}
	addWidget (itemWidget);
	rightOverlay.assign (itemWidget);
	refreshLayout ();
}

void Toolbar::refreshLayout () {
	UiConfiguration *uiconfig;
	std::list<Widget *>::iterator i, end;
	Widget *widget;
	float x, y, h;

	uiconfig = &(App::instance->uiConfig);
	x = widthPadding;
	y = heightPadding;
	if (leftCorner.widget && (! leftCorner.widget->isDestroyed)) {
		leftCorner.widget->position.assign (x, y);
		x += leftCorner.widget->width + uiconfig->marginSize;
	}

	if (leftOverlay.widget && (! leftOverlay.widget->isDestroyed)) {
		leftOverlay.widget->position.assign (x, 0.0f);
	}
	i = leftItemList.begin ();
	end = leftItemList.end ();
	while (i != end) {
		widget = *i;
		++i;

		if (! widget->isDestroyed) {
			if (leftOverlay.widget && (! leftOverlay.widget->isDestroyed)) {
				widget->isVisible = false;
			}
			else {
				widget->position.assign (x, y);
				widget->isVisible = true;
				x += widget->width + uiconfig->marginSize;
			}
		}
	}

	x = barWidth - widthPadding;
	if (rightCorner.widget && (! rightCorner.widget->isDestroyed)) {
		x -= rightCorner.widget->width;
		rightCorner.widget->position.assign (x, y);
		x -= uiconfig->marginSize;
	}

	if (rightOverlay.widget && (! rightOverlay.widget->isDestroyed)) {
		x -= rightOverlay.widget->width;
		rightOverlay.widget->position.assign (x, 0.0f);
		x -= uiconfig->marginSize;
	}
	i = rightItemList.begin ();
	end = rightItemList.end ();
	while (i != end) {
		widget = *i;
		++i;

		if (! widget->isDestroyed) {
			if (rightOverlay.widget) {
				widget->isVisible = false;
			}
			else {
				x -= widget->width;
				widget->position.assign (x, y);
				widget->isVisible = true;
				x -= uiconfig->marginSize;
			}
		}
	}

	resetSize ();
	h = maxWidgetY;
	if (!(leftOverlay.widget || rightOverlay.widget)) {
		h += heightPadding;
	}
	setFixedSize (true, barWidth, h);

	if (leftCorner.widget) {
		leftCorner.widget->position.assign (leftCorner.widget->position.x, (h / 2.0f) - (leftCorner.widget->height / 2.0f));
	}
	if (rightCorner.widget) {
		rightCorner.widget->position.assign (rightCorner.widget->position.x, (h / 2.0f) - (rightCorner.widget->height / 2.0f));
	}
	i = leftItemList.begin ();
	end = leftItemList.end ();
	while (i != end) {
		widget = *i;
		widget->position.assignY ((h / 2.0f) - (widget->height / 2.0f));
		++i;
	}
	i = rightItemList.begin ();
	end = rightItemList.end ();
	while (i != end) {
		widget = *i;
		widget->position.assignY ((h / 2.0f) - (widget->height / 2.0f));
		++i;
	}
}

void Toolbar::doUpdate (int msElapsed) {
	std::list<Widget *>::iterator i, end;
	Widget *widget;
	bool shouldrefresh, found;

	Panel::doUpdate (msElapsed);
	shouldrefresh = false;
	while (true) {
		found = false;
		i = leftItemList.begin ();
		end = leftItemList.end ();
		while (i != end) {
			widget = *i;
			if (widget->isDestroyed) {
				found = true;
				shouldrefresh = true;
				leftItemList.erase (i);
				widget->release ();
				break;
			}
			++i;
		}

		if (! found) {
			break;
		}
	}

	while (true) {
		found = false;
		i = rightItemList.begin ();
		end = rightItemList.end ();
		while (i != end) {
			widget = *i;
			if (widget->isDestroyed) {
				found = true;
				shouldrefresh = true;
				rightItemList.erase (i);
				widget->release ();
				break;
			}
			++i;
		}

		if (! found) {
			break;
		}
	}

	if (leftCorner.widget && leftCorner.widget->isDestroyed) {
		leftCorner.clear ();
		shouldrefresh = true;
	}
	if (rightCorner.widget && rightCorner.widget->isDestroyed) {
		rightCorner.clear ();
		shouldrefresh = true;
	}
	if (leftOverlay.widget && leftOverlay.widget->isDestroyed) {
		leftOverlay.clear ();
		shouldrefresh = true;
	}
	if (rightOverlay.widget && rightOverlay.widget->isDestroyed) {
		rightOverlay.clear ();
		shouldrefresh = true;
	}

	if (shouldrefresh) {
		refreshLayout ();
	}
}

float Toolbar::getLeftWidth () {
	UiConfiguration *uiconfig;
	std::list<Widget *>::reverse_iterator i, end;
	float w;

	uiconfig = &(App::instance->uiConfig);
	w = barWidth;
	if (! rightItemList.empty ()) {
		i = rightItemList.rbegin ();
		end = rightItemList.rend ();
		while (i != end) {
			if (! (*i)->isDestroyed) {
				w = (*i)->position.x;
				break;
			}
			++i;
		}
	}

	if (leftCorner.widget) {
		w -= (leftCorner.widget->width + uiconfig->marginSize);
	}
	w -= widthPadding;
	return (w);
}
