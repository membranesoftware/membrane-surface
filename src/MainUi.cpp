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
#include <unistd.h>
#include <sys/select.h>
#include <list>
#include "StdString.h"
#include "App.h"
#include "Label.h"
#include "Image.h"
#include "ImageWindow.h"
#include "Ui.h"
#include "Buffer.h"
#include "OsUtil.h"
#include "SystemInterface.h"
#include "Json.h"
#include "Color.h"
#include "Panel.h"
#include "IconLabelWindow.h"
#include "CountdownWindow.h"
#include "MainUi.h"

const int MainUi::MaxCommandSize = (256 * 1024); // bytes

MainUi::MainUi ()
: Ui ()
, backgroundPanel (NULL)
{

}

MainUi::~MainUi () {
	clearAnimation ();
	clearWindowIdMap ();
}

void MainUi::clearAnimation () {
	std::list<MainUi::AnimationCommand>::iterator i, end;

	i = animationList.begin ();
	end = animationList.end ();
	while (i != end) {
		if (i->cmdInv) {
			delete (i->cmdInv);
			i->cmdInv = NULL;
		}
		++i;
	}
	animationList.clear ();
}

void MainUi::clearWindowIdMap () {
	std::map<StdString, Widget *>::iterator i, end;

	i = windowIdMap.begin ();
	end = windowIdMap.end ();
	while (i != end) {
		if (i->second) {
			i->second->release ();
			i->second = NULL;
		}
		++i;
	}
	windowIdMap.clear ();
}

void MainUi::setWindowId (const StdString &windowId, Widget *widget) {
	std::map<StdString, Widget *>::iterator pos;

	if (windowId.empty () || (! widget)) {
		return;
	}
	widget->retain ();
	pos = windowIdMap.find (windowId);
	if (pos == windowIdMap.end ()) {
		windowIdMap.insert (std::pair<StdString, Widget *> (windowId, widget));
	}
	else {
		if (pos->second) {
			pos->second->isDestroyed = true;
			pos->second->release ();
		}
		pos->second = widget;
	}
}

OsUtil::Result MainUi::doLoad () {
	StdString path;
	ImageWindow *image;
	MainUi::BackgroundImageLoadedContext *ctx;

	backgroundPanel = (Panel *) addWidget (new Panel ());
	backgroundPanel->setFixedSize (true, App::instance->windowWidth, App::instance->windowHeight);
	backgroundPanel->zLevel = Widget::MinZLevel;

	path = OsUtil::getEnvValue ("BACKGROUND_IMAGE_PATH", "");
	if (! path.empty ()) {
		ctx = new MainUi::BackgroundImageLoadedContext ();
		ctx->ui = this;
		ctx->backgroundType = OsUtil::getEnvValue ("BACKGROUND_IMAGE_TYPE", (int) 0);
		retain ();

		image = (ImageWindow *) backgroundPanel->addWidget (new ImageWindow ());
		image->loadCallback = Widget::EventCallbackContext (showFileImageBackground_imageLoaded, ctx);
		switch (ctx->backgroundType) {
			case SystemInterface::Constant_FitStretchBackground: {
				image->onLoadFit ((float) App::instance->windowWidth, (float) App::instance->windowHeight);
				break;
			}
			case SystemInterface::Constant_FillStretchBackground: {
				image->onLoadScale ((float) App::instance->windowWidth, (float) App::instance->windowHeight);
				break;
			}
		}
		image->setImageFilePath (path, true);
	}

	return (OsUtil::Success);
}

void MainUi::doUnload () {
	backgroundPanel = NULL;
}

void MainUi::doUpdate (int msElapsed) {
	StdString s;
	Json *cmd;
	SystemInterface::Prefix prefix;
	fd_set rfds;
	struct timeval tv;
	char buf[8192];
	int len;
	bool result;

	while (true) {
		FD_ZERO (&rfds);
		FD_SET (0, &rfds);
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		len = select (1, &rfds, NULL, NULL, &tv);
		if (len != 1) {
			break;
		}
		len = read (0, buf, sizeof (buf));
		if (len <= 0) {
			break;
		}
		if (buf[len - 1] == '\n') {
			--len;
		}

		if (! commandBuffer.empty ()) {
			commandBuffer.add ((uint8_t *) buf, len);
			s.assignBuffer (&commandBuffer);
		}
		else {
			s.assign (buf, len);
		}
		if (!(SystemInterface::instance->parseCommand (s, &cmd))) {
			if (commandBuffer.empty ()) {
				commandBuffer.add ((uint8_t *) buf, len);
			}
			if (commandBuffer.length > MainUi::MaxCommandSize) {
				commandBuffer.reset ();
			}
			continue;
		}

		commandBuffer.reset ();
		result = executeCommand (cmd, true);
		delete (cmd);

		prefix.createTime = OsUtil::getTime ();
		cmd = SystemInterface::instance->createCommand (prefix, SystemInterface::CommandId_CommandResult, (new Json ())->set ("success", result));
		Log::printf ("%s", cmd->toString ().c_str ());
		delete (cmd);
	}

	updateAnimation (msElapsed);
}

bool MainUi::inputCommand (Json *cmdInv) {
	return (executeCommand (cmdInv, true));
}

bool MainUi::executeCommand (Json *cmdInv, bool allowPlayAnimation) {
	switch (SystemInterface::instance->getCommandId (cmdInv)) {
		case SystemInterface::CommandId_RemoveWindow: {
			removeWindow (cmdInv);
			return (true);
		}
		case SystemInterface::CommandId_PlayAnimation: {
			if (allowPlayAnimation) {
				playAnimation (cmdInv);
				return (true);
			}
			break;
		}
		case SystemInterface::CommandId_ShowColorFillBackground: {
			showColorFillBackground (cmdInv);
			return (true);
		}
		case SystemInterface::CommandId_ShowResourceImageBackground: {
			showResourceImageBackground (cmdInv);
			return (true);
		}
		case SystemInterface::CommandId_ShowFileImageBackground: {
			showFileImageBackground (cmdInv);
			return (true);
		}
		case SystemInterface::CommandId_ShowIconLabelWindow: {
			showIconLabelWindow (cmdInv);
			return (true);
		}
		case SystemInterface::CommandId_ShowCountdownWindow: {
			showCountdownWindow (cmdInv);
			return (true);
		}
	}
	return (false);
}

void MainUi::removeWindow (Json *cmdInv) {
	StdString id;
	std::map<StdString, Widget *>::iterator pos;

	id = SystemInterface::instance->getCommandStringParam (cmdInv, "windowId", StdString (""));
	if (id.empty ()) {
		return;
	}
	pos = windowIdMap.find (id);
	if (pos == windowIdMap.end ()) {
		return;
	}
	if (pos->second) {
		pos->second->isDestroyed = true;
		pos->second->release ();
		pos->second = NULL;
	}
	windowIdMap.erase (pos);
}

void MainUi::playAnimation (Json *cmdInv) {
	Json item, execcmd;
	MainUi::AnimationCommand listcmd;
	int i, count;

	clearAnimation ();
	count = SystemInterface::instance->getCommandArrayLength (cmdInv, "commands");
	for (i = 0; i < count; ++i) {
		if (SystemInterface::instance->getCommandObjectArrayItem (cmdInv, "commands", i, &item)) {
			listcmd.executeTime = item.getNumber ("executeTime", (int) 0);
			if (item.getObject ("command", &execcmd)) {
				listcmd.cmdInv = execcmd.copy ();
				animationList.push_back (listcmd);
			}
		}
	}
}

void MainUi::updateAnimation (int msElapsed) {
	std::list<MainUi::AnimationCommand>::iterator i, end, pos;
	int mintime;

	if (animationList.size () <= 0) {
		return;
	}

	while (true) {
		i = animationList.begin ();
		end = animationList.end ();
		mintime = -1;
		pos = end;
		while (i != end) {
			if ((mintime < 0) || (i->executeTime < mintime)) {
				mintime = i->executeTime;
				pos = i;
			}
			++i;
		}
		if ((mintime > msElapsed) || (pos == end)) {
			break;
		}

		if (pos->cmdInv) {
			executeCommand (pos->cmdInv);
			delete (pos->cmdInv);
		}
		animationList.erase (pos);
	}

	i = animationList.begin ();
	end = animationList.end ();
	while (i != end) {
		i->executeTime -= msElapsed;
		++i;
	}
}

int MainUi::getIconType (int icon) {
	switch (icon) {
		case SystemInterface::Constant_InfoIcon: {
			return (UiConfiguration::InfoIconSprite);
		}
		case SystemInterface::Constant_ErrorIcon: {
			return (UiConfiguration::ErrorIconSprite);
		}
		case SystemInterface::Constant_CountdownIcon: {
			return (UiConfiguration::CountdownIconSprite);
		}
		case SystemInterface::Constant_DateIcon: {
			return (UiConfiguration::DateIconSprite);
		}
	}
	return (-1);
}

void MainUi::setWidgetPosition (Widget *widget, float x, float y) {
	if (x < 0) {
		x = ((float) App::instance->windowWidth) - widget->width + x;
	}
	if (y < 0) {
		y = ((float) App::instance->windowHeight) - widget->height + y;
	}
	widget->position.assign (x, y);
}

void MainUi::showColorFillBackground (Json *cmdInv) {
	uint8_t r, g, b;

	backgroundPanel->clear ();
	clearWindowIdMap ();
	r = (uint8_t) SystemInterface::instance->getCommandNumberParam (cmdInv, "fillColorR", (int) 0);
	g = (uint8_t) SystemInterface::instance->getCommandNumberParam (cmdInv, "fillColorG", (int) 0);
	b = (uint8_t) SystemInterface::instance->getCommandNumberParam (cmdInv, "fillColorB", (int) 0);
	backgroundPanel->setFillBg (true, Color::fromByteValues (r, g, b));
}

void MainUi::showResourceImageBackground (Json *cmdInv) {
	ImageWindow *image;
	StdString path;

	path = SystemInterface::instance->getCommandStringParam (cmdInv, "imagePath", StdString (""));
	if (path.empty ()) {
		return;
	}
	backgroundPanel->clear ();
	clearWindowIdMap ();

	image = (ImageWindow *) backgroundPanel->addWidget (new ImageWindow ());
	image->onLoadFit ((float) App::instance->windowWidth, (float) App::instance->windowHeight);
	image->setImageFilePath (path);
}

void MainUi::showFileImageBackground_imageLoaded (void *ctxPtr, Widget *widgetPtr) {
	MainUi::BackgroundImageLoadedContext *ctx;
	ImageWindow *image;
	float x, y;

	ctx = (BackgroundImageLoadedContext *) ctxPtr;
	image = (ImageWindow *) widgetPtr;
	if (! image->isLoaded ()) {
		image->isDestroyed = true;
	}
	else {
		x = 0.0f;
		y = 0.0f;
		switch (ctx->backgroundType) {
			case SystemInterface::Constant_CenterBackground:
			case SystemInterface::Constant_FitStretchBackground:
			case SystemInterface::Constant_FillStretchBackground: {
				x = ((float) App::instance->windowWidth - image->width) / 2.0f;
				y = ((float) App::instance->windowHeight - image->height) / 2.0f;
				break;
			}
		}
		image->position.assign (x, y);
	}

	ctx->ui->release ();
	delete (ctx);
}

void MainUi::showFileImageBackground (Json *cmdInv) {
	ImageWindow *image;
	StdString path;
	MainUi::BackgroundImageLoadedContext *ctx;

	path = SystemInterface::instance->getCommandStringParam (cmdInv, "imagePath", StdString (""));
	if (path.empty ()) {
		return;
	}
	backgroundPanel->clear ();
	clearWindowIdMap ();

	ctx = new MainUi::BackgroundImageLoadedContext ();
	ctx->ui = this;
	ctx->backgroundType = SystemInterface::instance->getCommandNumberParam (cmdInv, "background", (int) 0);
	retain ();
	image = (ImageWindow *) backgroundPanel->addWidget (new ImageWindow ());
	image->loadCallback = Widget::EventCallbackContext (showFileImageBackground_imageLoaded, ctx);
	switch (ctx->backgroundType) {
		case SystemInterface::Constant_FitStretchBackground: {
			image->onLoadFit ((float) App::instance->windowWidth, (float) App::instance->windowHeight);
			break;
		}
		case SystemInterface::Constant_FillStretchBackground: {
			image->onLoadScale ((float) App::instance->windowWidth, (float) App::instance->windowHeight);
			break;
		}
	}
	image->setImageFilePath (path, true);
}

void MainUi::showIconLabelWindow (Json *cmdInv) {
	IconLabelWindow *window;
	StdString id;
	int icon;

	icon = getIconType (SystemInterface::instance->getCommandNumberParam (cmdInv, "icon", (int) 0));
	if (icon < 0) {
		return;
	}
	window = (IconLabelWindow *) backgroundPanel->addWidget (new IconLabelWindow (UiConfiguration::instance->coreSprites.getSprite (icon), SystemInterface::instance->getCommandStringParam (cmdInv, "labelText", StdString (""))));
	setWidgetPosition (window, SystemInterface::instance->getCommandNumberParam (cmdInv, "positionX", (float) 0.0f), SystemInterface::instance->getCommandNumberParam (cmdInv, "positionY", (float) 0.0f));
	window->setFillBg (true, Color (1.0f, 1.0f, 1.0f));

	id = SystemInterface::instance->getCommandStringParam (cmdInv, "windowId", StdString (""));
	if (! id.empty ()) {
		setWindowId (id, window);
	}
}

void MainUi::showCountdownWindow (Json *cmdInv) {
	CountdownWindow *window;
	StdString id;
	int icon;

	icon = getIconType (SystemInterface::instance->getCommandNumberParam (cmdInv, "icon", (int) 0));
	if (icon < 0) {
		return;
	}
	window = (CountdownWindow *) backgroundPanel->addWidget (new CountdownWindow (UiConfiguration::instance->coreSprites.getSprite (icon), SystemInterface::instance->getCommandStringParam (cmdInv, "labelText", StdString (""))));
	window->setDropShadow (true, UiConfiguration::instance->dropShadowColor, UiConfiguration::instance->dropShadowWidth);
	setWidgetPosition (window, SystemInterface::instance->getCommandNumberParam (cmdInv, "positionX", (float) 0.0f), SystemInterface::instance->getCommandNumberParam (cmdInv, "positionY", (float) 0.0f));
	window->countdown (SystemInterface::instance->getCommandNumberParam (cmdInv, "countdownTime", (int) 0));
	window->reveal ();

	id = SystemInterface::instance->getCommandStringParam (cmdInv, "windowId", StdString (""));
	if (! id.empty ()) {
		setWindowId (id, window);
	}
}
