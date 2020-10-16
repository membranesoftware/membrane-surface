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
#include <vector>
#include <stack>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "Result.h"
#include "StdString.h"
#include "Log.h"
#include "OsUtil.h"
#include "MathUtil.h"
#include "Input.h"
#include "Network.h"
#include "Panel.h"
#include "MainUi.h"
#include "App.h"

const int App::DefaultMinFrameDelay = 20;
const int App::WindowWidths[] = { 768, 1024, 1280, 1600, 1920 };
const int App::WindowHeights[] = { 432, 576, 720, 900, 1080 };
const int App::WindowSizeCount = 5;
const float App::FontScales[] = { 0.66f, 0.8f, 1.0f, 1.25f, 1.5f };
const int App::FontScaleCount = 5;
const int App::MaxCornerRadius = 16;

const char *App::NetworkThreadsKey = "NetworkThreads";
const char *App::WindowWidthKey = "WindowWidth";
const char *App::WindowHeightKey = "WindowHeight";
const char *App::FontScaleKey = "FontScale";
const char *App::HttpsKey = "Https";
const char *App::ShowInterfaceAnimationsKey = "ShowInterfaceAnimations";

App *App::instance = NULL;

void App::createInstance () {
	if (App::instance) {
		delete (App::instance);
	}
	App::instance = new App ();
}

void App::freeInstance () {
	if (App::instance) {
		delete (App::instance);
		App::instance = NULL;
		IMG_Quit ();
		SDL_Quit ();
	}
}

App::App ()
: shouldRefreshUi (false)
, isInterfaceAnimationEnabled (false)
, nextFontScale (1.0f)
, nextWindowWidth (0)
, nextWindowHeight (0)
, isShuttingDown (false)
, isShutdown (false)
, startTime (0)
, isHttpsEnabled (false)
, window (NULL)
, render (NULL)
, isTextureRenderEnabled (false)
, rootPanel (NULL)
, displayDdpi (0.0f)
, displayHdpi (0.0f)
, displayVdpi (0.0f)
, windowWidth (0)
, windowHeight (0)
, minDrawFrameDelay (0)
, minUpdateFrameDelay (0)
, fontScale (1.0f)
, imageScale (0)
, drawCount (0)
, updateCount (0)
, isPrefsWriteDisabled (false)
, updateThread (NULL)
, uniqueIdMutex (NULL)
, nextUniqueId (1)
, prefsMapMutex (NULL)
, roundedCornerSprite (NULL)
, renderTaskMutex (NULL)
, isSuspendingUpdate (false)
, updateMutex (NULL)
, updateCond (NULL)
{
	uniqueIdMutex = SDL_CreateMutex ();
	prefsMapMutex = SDL_CreateMutex ();
	renderTaskMutex = SDL_CreateMutex ();
	updateMutex = SDL_CreateMutex ();
	updateCond = SDL_CreateCond ();
}

App::~App () {
	if (rootPanel) {
		rootPanel->release ();
		rootPanel = NULL;
	}

	if (roundedCornerSprite) {
		delete (roundedCornerSprite);
		roundedCornerSprite = NULL;
	}

	if (uniqueIdMutex) {
		SDL_DestroyMutex (uniqueIdMutex);
		uniqueIdMutex = NULL;
	}

	if (prefsMapMutex) {
		SDL_DestroyMutex (prefsMapMutex);
		prefsMapMutex = NULL;
	}

	if (renderTaskMutex) {
		SDL_DestroyMutex (renderTaskMutex);
		renderTaskMutex = NULL;
	}

	if (updateMutex) {
		SDL_DestroyMutex (updateMutex);
		updateMutex = NULL;
	}

	if (updateCond) {
		SDL_DestroyCond (updateCond);
		updateCond = NULL;
	}
}

int App::getImageScale (int w, int h) {
	int i, result;

	if ((w <= 0) || (h <= 0)) {
		return (-1);
	}

	result = 0;
	for (i = 0; i < App::WindowSizeCount; ++i) {
		if (w >= App::WindowWidths[i]) {
			result = i;
		}
	}

	return (result);
}

int App::run () {
	SDL_version version1, version2;
	SDL_RendererInfo renderinfo;
	SDL_Rect rect;
	int result, delay, i;
	int64_t endtime, elapsed, t1, t2;
	double fps;
	Ui *ui;

	if (minDrawFrameDelay <= 0) {
		minDrawFrameDelay = App::DefaultMinFrameDelay;
	}
	if (minUpdateFrameDelay <= 0) {
		minUpdateFrameDelay = App::DefaultMinFrameDelay;
	}
	prng.seed ((uint32_t) (OsUtil::getTime () & 0xFFFFFFFF));
	prefsMap.clear ();
	if (prefsPath.empty ()) {
		isPrefsWriteDisabled = true;
	}
	else {
		result = prefsMap.read (prefsPath, true);
		if (result != Result::Success) {
			Log::debug ("Failed to read preferences file; prefsPath=\"%s\" err=%i", prefsPath.c_str (), result);
			prefsMap.clear ();
		}
	}

	isHttpsEnabled = prefsMap.find (App::HttpsKey, true);

	if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		Log::err ("Failed to start SDL: %s", SDL_GetError ());
		return (Result::SdlOperationFailedError);
	}

	if (IMG_Init (IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG)) {
		Log::err ("Failed to start SDL_image: %s", IMG_GetError ());
		return (Result::SdlOperationFailedError);
	}

	result = resource.open ();
	if (result != Result::Success) {
		Log::err ("Failed to open application resources; err=%i", result);
		return (result);
	}

	result = uiText.load (OsUtil::getEnvLanguage (UiText::DefaultLanguage));
	if (result != Result::Success) {
		Log::err ("Failed to load text resources; err=%i", result);
		return (result);
	}

	result = input.start ();
	if (result != Result::Success) {
		Log::err ("Failed to acquire application input devices; err=%i", result);
		return (result);
	}

	result = network.start (prefsMap.find (App::NetworkThreadsKey, Network::DefaultRequestThreadCount));
	if (result != Result::Success) {
		Log::err ("Failed to acquire application network resources; err=%i", result);
		return (result);
	}
	network.addDatagramCallback (App::datagramReceived, NULL);

	result = SDL_GetDisplayDPI (0, &displayDdpi, &displayHdpi, &displayVdpi);
	if (result != 0) {
		Log::warning ("Failed to determine display DPI: %s", SDL_GetError ());
		displayHdpi = 72.0f;
		displayVdpi = 72.0f;
	}

	if ((windowWidth <= 0) || (windowHeight <= 0)) {
		windowWidth = prefsMap.find (App::WindowWidthKey, 0);
		windowHeight = prefsMap.find (App::WindowHeightKey, 0);
	}
	imageScale = getImageScale (windowWidth, windowHeight);
	if (imageScale < 0) {
		imageScale = 0;
		windowWidth = App::WindowWidths[0];
		windowHeight = App::WindowHeights[0];
		result = SDL_GetDisplayUsableBounds (0, &rect);
		if (result != 0) {
			Log::warning ("Failed to determine display usable bounds: %s", SDL_GetError ());
		}
		else {
			windowWidth = rect.w;
			windowHeight = rect.h;
			imageScale = getImageScale (windowWidth, windowHeight);
			Log::debug ("Set window size from display usable bounds; boundsRect=x%i,y%i,w%i,h%i windowWidth=%i windowHeight=%i imageScale=%i", rect.x, rect.y, rect.w, rect.h, windowWidth, windowHeight, imageScale);
		}
	}

	result = SDL_CreateWindowAndRenderer (windowWidth, windowHeight, 0, &window, &render);
	if (result != 0) {
		Log::err ("Failed to create application window: %s", SDL_GetError ());
		return (Result::SdlOperationFailedError);
	}

	result = SDL_GetRendererInfo (render, &renderinfo);
	if (result != 0) {
		Log::err ("Failed to create application renderer: %s", SDL_GetError ());
		return (Result::SdlOperationFailedError);
	}
	if ((renderinfo.flags & (SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE)) == (SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE)) {
		isTextureRenderEnabled = true;
	}
	if (isTextureRenderEnabled) {
		isInterfaceAnimationEnabled = prefsMap.find (App::ShowInterfaceAnimationsKey, true);
	}

	clipRect.x = 0;
	clipRect.y = 0;
	clipRect.w = windowWidth;
	clipRect.h = windowHeight;
	nextWindowWidth = windowWidth;
	nextWindowHeight = windowHeight;

	i = prefsMap.find (App::FontScaleKey, App::FontScaleCount / 2);
	if ((i >= 0) && (i < App::FontScaleCount)) {
		fontScale = App::FontScales[i];
		nextFontScale = fontScale;
	}

	SDL_SetWindowTitle (window, uiText.getText (UiTextString::WindowTitle).c_str ());

	startTime = OsUtil::getTime ();
	uiConfig.resetScale ();
	result = uiConfig.load (fontScale);
	if (result != Result::Success) {
		Log::err ("Failed to load application resources; err=%i", result);
		return (result);
	}

	populateRoundedCornerSprite ();
	populateWidgets ();

	ui = new MainUi ();
	uiStack.setUi (ui);

	updateThread = SDL_CreateThread (App::runUpdateThread, "runUpdateThread", (void *) this);

	SDL_VERSION (&version1);
	SDL_GetVersion (&version2);
	Log::info ("Application started; buildId=\"%s\" sdlBuildVersion=%i.%i.%i sdlLinkVersion=%i.%i.%i windowSize=%ix%i windowFlags=0x%x renderFlags=0x%x isTextureRenderEnabled=%s diagonalDpi=%.2f horizontalDpi=%.2f verticalDpi=%.2f imageScale=%i minDrawFrameDelay=%i minUpdateFrameDelay=%i lang=%s pid=%i", BUILD_ID, version1.major, version1.minor, version1.patch, version2.major, version2.minor, version2.patch, windowWidth, windowHeight, (unsigned int) SDL_GetWindowFlags (window), (unsigned int) renderinfo.flags, BOOL_STRING (isTextureRenderEnabled), displayDdpi, displayHdpi, displayVdpi, imageScale, minDrawFrameDelay, minUpdateFrameDelay, OsUtil::getEnvLanguage ("").c_str (), OsUtil::getProcessId ());

	while (true) {
		if (isShutdown) {
			break;
		}

		t1 = OsUtil::getTime ();
		input.pollEvents ();

		if (! FLOAT_EQUALS (fontScale, nextFontScale)) {
			if (uiConfig.reloadFonts (nextFontScale) != Result::Success) {
				nextFontScale = fontScale;
			}
			else {
				shouldRefreshUi = true;
				fontScale = nextFontScale;
				for (i = 0; i < App::FontScaleCount; ++i) {
					if (FLOAT_EQUALS (fontScale, App::FontScales[i])) {
						SDL_LockMutex (prefsMapMutex);
						prefsMap.insert (App::FontScaleKey, i);
						SDL_UnlockMutex (prefsMapMutex);
						break;
					}
				}
			}
		}

		executeRenderTasks ();
		draw ();
		if ((windowWidth != nextWindowWidth) || (windowHeight != nextWindowHeight)) {
			resizeWindow ();
		}
		uiStack.executeStackCommands ();
		resource.compact ();

		t2 = OsUtil::getTime ();
		delay = (int) (minDrawFrameDelay - (t2 - t1));
		if (delay < 1) {
			delay = 1;
		}
		SDL_Delay (delay);
	}
	SDL_WaitThread (updateThread, &result);

	uiStack.clear ();
	if (rootPanel) {
		rootPanel->release ();
		rootPanel = NULL;
	}
	uiConfig.unload ();
	if (roundedCornerSprite) {
		roundedCornerSprite->unload ();
	}
	resource.compact ();
	resource.close ();

	writePrefs ();

	SDL_DestroyRenderer (render);
	render = NULL;
	SDL_DestroyWindow (window);
	window = NULL;

	endtime = OsUtil::getTime ();
	elapsed = endtime - startTime;
	fps = (double) drawCount;
	if (elapsed > 1000) {
		fps /= ((double) elapsed) / 1000.0f;
	}
	Log::info ("Application ended; updateCount=%lli drawCount=%lli runtime=%.3fs FPS=%f pid=%i", (long long) updateCount, (long long) drawCount, ((double) elapsed) / 1000.0f, fps, OsUtil::getProcessId ());

	return (Result::Success);
}

void App::populateWidgets () {
	if (! rootPanel) {
		rootPanel = new Panel ();
		rootPanel->retain ();
		rootPanel->id = getUniqueId ();
		rootPanel->setFixedSize (true, windowWidth, windowHeight);
		rootPanel->keyEventCallback = Widget::KeyEventCallbackContext (App::keyEvent, NULL);
	}
}

void App::populateRoundedCornerSprite () {
	Uint32 *pixels, *dest, color, rmask, gmask, bmask, amask;
	SDL_Surface *surface;
	SDL_Texture *texture;
	StdString path;
	float dist, targetalpha, minalpha, opacity;
	int radius, x, y, w, h;
	uint8_t alpha;

	if (roundedCornerSprite) {
		delete (roundedCornerSprite);
	}
	roundedCornerSprite = new Sprite ();

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xFF000000;
	gmask = 0x00FF0000;
	bmask = 0x0000FF00;
	amask = 0x000000FF;
#else
	rmask = 0x000000FF;
	gmask = 0x0000FF00;
	bmask = 0x00FF0000;
	amask = 0xFF000000;
#endif
	minalpha = 0.1f;
	opacity = 8.0f;
	radius = 1;
	while (radius <= App::MaxCornerRadius) {
		w = (radius * 2) + 1;
		h = (radius * 2) + 1;
		pixels = (Uint32 *) malloc (w * h * sizeof (Uint32));
		if (! pixels) {
			Log::warning ("Failed to create texture; err=\"Out of memory, dimensions %ix%i\"", radius, radius);
			delete (roundedCornerSprite);
			roundedCornerSprite = NULL;
			return;
		}

		dest = pixels;
		y = 0;
		while (y < h) {
			x = 0;
			while (x < w) {
				dist = MathUtil::getDistance (((float) x) + 0.5f, ((float) y) + 0.5f, (float) radius + 0.5f, (float) radius + 0.5f);
				targetalpha = 1.0f - ((1.0f - minalpha) * (dist / (float) radius));
				if (targetalpha <= 0.0f) {
					targetalpha = 0.0f;
				}
				else {
					targetalpha *= opacity;
					if (targetalpha > 1.0f) {
						targetalpha = 1.0f;
					}
				}
				alpha = (uint8_t) (targetalpha * 255.0f);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
				color = 0xFFFFFF00 | (alpha & 0xFF);
#else
				color = 0x00FFFFFF | (((Uint32) (alpha & 0xFF)) << 24);
#endif
				*dest = color;
				++dest;
				++x;
			}

			++y;
		}

		surface = SDL_CreateRGBSurfaceFrom (pixels, w, h, 32, w * sizeof (Uint32), rmask, gmask, bmask, amask);
		if (! surface) {
			free (pixels);
			Log::warning ("Failed to create texture; err=\"SDL_CreateRGBSurfaceFrom, %s\"", SDL_GetError ());
			delete (roundedCornerSprite);
			roundedCornerSprite = NULL;
			return;
		}

		path.sprintf ("*_App::roundedCornerSprite_%llx", (long long int) App::instance->getUniqueId ());
		texture = App::instance->resource.createTexture (path, surface);
		SDL_FreeSurface (surface);
		free (pixels);
		if (! texture) {
			delete (roundedCornerSprite);
			roundedCornerSprite = NULL;
			return;
		}

		roundedCornerSprite->addTexture (texture, path);
		++radius;
	}
}

SDL_Texture *App::getRoundedCornerTexture (int radius, int *textureWidth, int *textureHeight) {
	if ((radius <= 0) || (radius > App::MaxCornerRadius)) {
		return (NULL);
	}

	return (roundedCornerSprite->getTexture ((radius - 1), textureWidth, textureHeight));
}

void App::shutdown () {
	Ui *ui;

	if (isShuttingDown) {
		return;
	}

	isShuttingDown = true;
	ui = uiStack.getActiveUi ();
	if (ui) {
		ui->showShutdownWindow ();
		ui->release ();
	}
	network.stop ();
	input.stop ();
}

HashMap *App::lockPrefs () {
	SDL_LockMutex (prefsMapMutex);
	return (&prefsMap);
}

void App::unlockPrefs () {
	SDL_UnlockMutex (prefsMapMutex);
}

void App::datagramReceived (void *callbackData, const char *messageData, int messageLength, const char *sourceAddress, int sourcePort) {

}

void App::executeRenderTasks () {
	std::vector<App::RenderTaskContext>::iterator i, end;

	renderTaskList.clear ();
	SDL_LockMutex (renderTaskMutex);
	renderTaskList.swap (renderTaskAddList);
	SDL_UnlockMutex (renderTaskMutex);

	i = renderTaskList.begin ();
	end = renderTaskList.end ();
	while (i != end) {
		i->callback (i->callbackData);
		++i;
	}
	renderTaskList.clear ();
}

void App::draw () {
	Ui *ui;

	SDL_RenderClear (render);

	ui = uiStack.getActiveUi ();
	if (ui) {
		ui->draw ();
		rootPanel->draw ();
		ui->release ();
	}
	SDL_RenderPresent (render);
	++drawCount;
}

int App::runUpdateThread (void *appPtr) {
	App *app;
	int64_t t1, t2, last;
	int delay;

	app = (App *) appPtr;
	last = OsUtil::getTime ();
	while (true) {
		if (app->isShutdown) {
			break;
		}

		t1 = OsUtil::getTime ();
		app->update ((int) (t1 - last));
		t2 = OsUtil::getTime ();
		last = t1;

		delay = (int) (app->minUpdateFrameDelay - (t2 - t1));
		if (delay < 1) {
			delay = 1;
		}
		SDL_Delay (delay);
	}

	return (0);
}

void App::update (int msElapsed) {
	Ui *ui;

	uiStack.update (msElapsed);
	if (shouldRefreshUi) {
		uiStack.refresh ();
		rootPanel->refresh ();
		shouldRefreshUi = false;
	}

	rootPanel->processInput ();
	ui = uiStack.getActiveUi ();
	if (ui) {
		ui->update (msElapsed);
		ui->release ();
	}
	rootPanel->update (msElapsed, 0.0f, 0.0f);

	writePrefs ();
	++updateCount;

	SDL_LockMutex (updateMutex);
	while (isSuspendingUpdate) {
		SDL_CondBroadcast (updateCond);
		SDL_CondWait (updateCond, updateMutex);
	}
	SDL_UnlockMutex (updateMutex);

	if (isShuttingDown) {
		if (network.isStopComplete ()) {
			network.waitThreads ();
			isShutdown = true;
		}
	}
}

bool App::keyEvent (void *ptr, SDL_Keycode keycode, bool isShiftDown, bool isControlDown) {
	if (isControlDown) {
		switch (keycode) {
			case SDLK_q: {
				App::instance->shutdown ();
				return (true);
			}
		}
	}

	if (App::instance->uiStack.processKeyEvent (keycode, isShiftDown, isControlDown)) {
		return (true);
	}

	return (false);
}

int64_t App::getUniqueId () {
	int64_t id;

	SDL_LockMutex (uniqueIdMutex);
	id = nextUniqueId;
	++nextUniqueId;
	SDL_UnlockMutex (uniqueIdMutex);

	return (id);
}

void App::suspendUpdate () {
	SDL_LockMutex (updateMutex);
	isSuspendingUpdate = true;
	SDL_CondWait (updateCond, updateMutex);
	SDL_UnlockMutex (updateMutex);
}

void App::unsuspendUpdate () {
	SDL_LockMutex (updateMutex);
	isSuspendingUpdate = false;
	SDL_CondBroadcast (updateCond);
	SDL_UnlockMutex (updateMutex);
}

void App::pushClipRect (const SDL_Rect *rect, bool disableIntersection) {
	int x, y, w, h, diff;

	x = rect->x;
	y = rect->y;
	w = rect->w;
	h = rect->h;
	if ((! clipRectStack.empty ()) && (! disableIntersection)) {
		diff = x - clipRect.x;
		if (diff < 0) {
			w += diff;
			x = clipRect.x;
		}
		diff = y - clipRect.y;
		if (diff < 0) {
			h += diff;
			y = clipRect.y;
		}
		diff = (x + w) - (clipRect.x + clipRect.w);
		if (diff > 0) {
			w -= diff;
		}
		diff = (y + h) - (clipRect.y + clipRect.h);
		if (diff > 0) {
			h -= diff;
		}

		if (w < 0) {
			w = 0;
		}
		if (h < 0) {
			h = 0;
		}
	}

	clipRect.x = x;
	clipRect.y = y;
	clipRect.w = w;
	clipRect.h = h;
	SDL_RenderSetClipRect (render, &clipRect);
	clipRectStack.push (clipRect);
}

void App::popClipRect () {
	if (clipRectStack.empty ()) {
		return;
	}

	clipRectStack.pop ();
	if (clipRectStack.empty ()) {
		clipRect.x = 0;
		clipRect.y = 0;
		clipRect.w = windowWidth;
		clipRect.h = windowHeight;
		SDL_RenderSetClipRect (render, NULL);
	}
	else {
		clipRect = clipRectStack.top ();
		SDL_RenderSetClipRect (render, &clipRect);
	}
}

void App::suspendClipRect () {
	SDL_RenderSetClipRect (render, NULL);
}

void App::unsuspendClipRect () {
	SDL_RenderSetClipRect (render, &clipRect);
}

void App::addRenderTask (RenderTaskFunction fn, void *fnData) {
	App::RenderTaskContext ctx;

	if (! fn) {
		return;
	}

	ctx.callback = fn;
	ctx.callbackData = fnData;
	SDL_LockMutex (renderTaskMutex);
	renderTaskAddList.push_back (ctx);
	SDL_UnlockMutex (renderTaskMutex);
}

void App::writePrefs () {
	int result;

	if (isPrefsWriteDisabled) {
		return;
	}

	SDL_LockMutex (prefsMapMutex);
	if (prefsMap.isWriteDirty) {
		result = prefsMap.write (prefsPath);
		if (result != Result::Success) {
			Log::err ("Failed to write prefs file; prefsPath=\"%s\" err=%i", prefsPath.c_str (), result);
			isPrefsWriteDisabled = true;
		}
	}
	SDL_UnlockMutex (prefsMapMutex);
}

void App::resizeWindow () {
	std::vector<Ui *>::iterator i, end;
	int scale, result;

	scale = getImageScale (nextWindowWidth, nextWindowHeight);
	if (scale < 0) {
		nextWindowWidth = windowWidth;
		nextWindowHeight = windowHeight;
		return;
	}

	suspendUpdate ();
	SDL_SetWindowSize (window, nextWindowWidth, nextWindowHeight);
	windowWidth = nextWindowWidth;
	windowHeight = nextWindowHeight;
	imageScale = scale;
	uiConfig.resetScale ();

	clipRect.x = 0;
	clipRect.y = 0;
	clipRect.w = windowWidth;
	clipRect.h = windowHeight;
	rootPanel->setFixedSize (true, windowWidth, windowHeight);

	uiConfig.coreSprites.resize ();
	result = uiConfig.reloadFonts (fontScale);
	if (result != Result::Success) {
		Log::err ("Failed to reload fonts; fontScale=%.2f err=%i", fontScale, result);
	}

	uiStack.resize ();
	rootPanel->resetInputState ();
	shouldRefreshUi = true;
	unsuspendUpdate ();

	SDL_LockMutex (prefsMapMutex);
	prefsMap.insert (App::WindowWidthKey, windowWidth);
	prefsMap.insert (App::WindowHeightKey, windowHeight);
	SDL_UnlockMutex (prefsMapMutex);
}

int App::getRandomInt (int i1, int i2) {
	return (prng.getRandomValue (i1, i2));
}

static const char getRandomString_chars[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
StdString App::getRandomString (int stringLength) {
	StdString s;
	int i;

	for (i = 0; i < stringLength; ++i) {
		s.append (1, getRandomString_chars[getRandomInt (0, sizeof (getRandomString_chars) - 1)]);
	}

	return (s);
}
