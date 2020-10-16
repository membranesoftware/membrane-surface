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
#include <unistd.h>
#include <sys/select.h>
#include "Result.h"
#include "StdString.h"
#include "Log.h"
#include "App.h"
#include "Label.h"
#include "Image.h"
#include "ImageWindow.h"
#include "Ui.h"
#include "Buffer.h"
#include "Json.h"
#include "Color.h"
#include "Panel.h"
#include "MainUi.h"

const int MainUi::MaxCommandSize = (256 * 1024); // bytes

MainUi::MainUi ()
: Ui ()
, backgroundPanel (NULL)
{

}

MainUi::~MainUi () {

}

int MainUi::doLoad () {
	return (Result::Success);
}

void MainUi::doUnload () {

}

void MainUi::doClearPopupWidgets () {

}

void MainUi::doResume () {

}

void MainUi::doRefresh () {

}

void MainUi::doPause () {

}

void MainUi::doUpdate (int msElapsed) {
	SystemInterface *interface;
	StdString s;
	Json *cmd;
	fd_set rfds;
	struct timeval tv;
	char buf[8192];
	int len, commandid;

	interface = &(App::instance->systemInterface);
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

		if (! interface->parseCommand (s, &cmd)) {
			if (commandBuffer.empty ()) {
				commandBuffer.add ((uint8_t *) buf, len);
			}
			if (commandBuffer.length > MainUi::MaxCommandSize) {
				commandBuffer.reset ();
			}

			continue;
		}

		commandBuffer.reset ();
		commandid = interface->getCommandId (cmd);
		switch (commandid) {
			case SystemInterface::CommandId_ShutdownAgent: {
				App::instance->shutdown ();
				break;
			}
			case SystemInterface::CommandId_ShowColorFillBackground: {
				showColorFillBackground (interface->getCommandNumberParam (cmd, "fillColorR", (int) 0), interface->getCommandNumberParam (cmd, "fillColorG", (int) 0), interface->getCommandNumberParam (cmd, "fillColorB", (int) 0));
				break;
			}
			case SystemInterface::CommandId_ShowResourceImageBackground: {
				showResourceImageBackground (interface->getCommandStringParam (cmd, "imagePath", StdString ("")));
				break;
			}
			case SystemInterface::CommandId_ShowFileImageBackground: {
				showFileImageBackground (interface->getCommandStringParam (cmd, "imagePath", StdString ("")));
				break;
			}
		}
		delete (cmd);
	}
}

void MainUi::doResize () {

}

bool MainUi::doProcessKeyEvent (SDL_Keycode keycode, bool isShiftDown, bool isControlDown) {
	return (false);
}

void MainUi::populateBackgroundPanel () {
	if (! backgroundPanel) {
		backgroundPanel = (Panel *) addWidget (new Panel ());
		backgroundPanel->setFixedSize (true, App::instance->windowWidth, App::instance->windowHeight);
		backgroundPanel->zLevel = Widget::MinZLevel;
	}
}

void MainUi::showColorFillBackground (int fillColorR, int fillColorG, int fillColorB) {
	populateBackgroundPanel ();
	backgroundPanel->clear ();
	backgroundPanel->setFillBg (true, Color::fromByteValues ((uint8_t) fillColorR, (uint8_t) fillColorG, (uint8_t) fillColorB));
}

void MainUi::showResourceImageBackground (const StdString &imagePath) {
	ImageWindow *image;

	if (imagePath.empty ()) {
		return;
	}

	populateBackgroundPanel ();
	backgroundPanel->clear ();
	image = (ImageWindow *) backgroundPanel->addWidget (new ImageWindow ());
	image->setLoadResize (true, (float) App::instance->windowWidth);
	image->setImageFilePath (imagePath);
}

void MainUi::showFileImageBackground (const StdString &imagePath) {
	ImageWindow *image;

	if (imagePath.empty ()) {
		return;
	}

	populateBackgroundPanel ();
	backgroundPanel->clear ();
	image = (ImageWindow *) backgroundPanel->addWidget (new ImageWindow ());
	image->setLoadResize (true, (float) App::instance->windowWidth);
	image->setImageFilePath (imagePath, true);
}
