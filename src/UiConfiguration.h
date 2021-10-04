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
// Class that holds configuration values for UI elements

#ifndef UI_CONFIGURATION_H
#define UI_CONFIGURATION_H

#include "SDL2/SDL.h"
#include "StdString.h"
#include "Color.h"
#include "Font.h"
#include "SpriteGroup.h"

class UiConfiguration {
public:
	// Indexes in the coreSprites group
	enum {
		OkButtonSprite = 0,
		CancelButtonSprite = 1,
		EnterTextButtonSprite = 2,
		RandomizeButtonSprite = 3,
		SettingsButtonSprite = 4,
		ExitButtonSprite = 5,
		MainMenuButtonSprite = 6,
		BackButtonSprite = 7,
		NewsButtonSprite = 8,
		DeleteButtonSprite = 9,
		HelpButtonSprite = 10,
		PasteButtonSprite = 11,
		ImageButtonSprite = 12,
		NetworkConnectIconSprite = 13,
		NetworkDisconnectIconSprite = 14,
		CheckmarkSprite = 15,
		ScrollUpArrowSprite = 16,
		ScrollDownArrowSprite = 17,
		TaskInProgressIconSprite = 18,
		TaskCompleteIconSprite = 19,
		InfoIconSprite = 20,
		WebLinkIconSprite = 21,
		ToggleCheckboxSprite = 22,
		ToggleCheckboxOutlineSprite = 23,
		ToggleCheckboxIndeterminateSprite = 24,
		UpdateButtonSprite = 25,
		FeedbackButtonSprite = 26,
		StarButtonSprite = 27,
		StarOutlineButtonSprite = 28,
		StarHalfButtonSprite = 29,
		ClearButtonSprite = 30,
		ReloadButtonSprite = 31,
		RenameButtonSprite = 32,
		ErrorIconSprite = 33,
		AboutButtonSprite = 34,
		ExpandLessButtonSprite = 35,
		ExpandMoreButtonSprite = 36,
		ConfigurationIconSprite = 37,
		LoadingImageIconSprite = 38,
		KeyIconSprite = 39,
		AddButtonSprite = 40,
		VisibilityOnButtonSprite = 41,
		VisibilityOffButtonSprite = 42,
		ConsoleIconSprite = 43,
		CountdownIconSprite = 44
	};

	// Sprite frame indexes
	enum {
		WhiteButtonFrame = 0,
		WhiteLargeButtonFrame = 1,
		BlackButtonFrame = 2,
		BlackLargeButtonFrame = 3,
		ChipIconFrame = 0
	};

	enum FontType {
		CaptionFont = 0,
		BodyFont = 1,
		ButtonFont = 2,
		TitleFont = 3,
		HeadlineFont = 4,
		ConsoleFont = 5,
		NoFont = -1
	};
	enum {
		FontCount = 6
	};

	UiConfiguration ();
	~UiConfiguration ();
	static UiConfiguration *instance;

	// Load resources referenced by the UiConfiguration and return a result value
	int load (float fontScale = 1.0f);

	// Free resources allocated by any previous load operation
	void unload ();

	// Reset configuration values as appropriate for the current application image scale
	void resetScale ();

	// Free any loaded font resources and replace them with new ones at the specified scale
	int reloadFonts (float fontScale);

	// Read-only data members; read-write access is permissible by the Ui class and its subclasses
	float paddingSize;
	float marginSize;
	int shortColorTranslateDuration; // ms
	int longColorTranslateDuration; // ms
	int shortColorAnimateDuration; // ms
	int longColorAnimateDuration; // ms
	int mouseHoverThreshold;
	int blinkDuration; // ms
	int backgroundCrossFadeDuration; // ms
	StdString fontNames[UiConfiguration::FontCount];
	int fontBaseSizes[UiConfiguration::FontCount];
	int fontSizes[UiConfiguration::FontCount];
	Font *fonts[UiConfiguration::FontCount];
	Color lightPrimaryColor;
	Color mediumPrimaryColor;
	Color darkPrimaryColor;
	Color lightSecondaryColor;
	Color mediumSecondaryColor;
	Color darkSecondaryColor;
	Color lightBackgroundColor;
	Color mediumBackgroundColor;
	Color darkBackgroundColor;
	Color lightInverseBackgroundColor;
	Color mediumInverseBackgroundColor;
	Color darkInverseBackgroundColor;
	Color primaryTextColor;
	Color lightPrimaryTextColor;
	Color inverseTextColor;
	Color darkInverseTextColor;
	Color flatButtonTextColor;
	Color linkTextColor;
	Color errorTextColor;
	Color statusOkTextColor;
	Color raisedButtonTextColor;
	Color raisedButtonInverseTextColor;
	Color raisedButtonBackgroundColor;
	Color dropShadowColor;
	float dropShadowWidth;
	Color dividerColor;
	int cornerRadius;
	float buttonFocusedShadeAlpha;
	float buttonPressedShadeAlpha;
	float buttonDisabledShadeAlpha;
	Color mouseoverBgColor;
	float mouseoverBgAlpha;
	float activeFocusedIconAlpha;
	float activeUnfocusedIconAlpha;
	float inactiveIconAlpha;
	float scrimBackgroundAlpha;
	float overlayWindowAlpha;
	float waitingShadeAlpha;
	float progressBarHeight;
	float mouseWheelScrollSpeed; // portion of view height per wheel scroll event, from 0.0f to 1.0f
	float textLineHeightMargin;
	float textUnderlineMargin;
	float menuDividerLineWidth;
	float headlineDividerLineWidth;
	float selectionBorderWidth;
	float selectionBorderAlpha;
	float sliderThumbSize;
	float sliderTrackWidth;
	float sliderTrackHeight;
	int textAreaSmallLineCount;
	int textAreaMediumLineCount;
	int textAreaLargeLineCount;
	int textFieldShortLineLength;
	int textFieldMediumLineLength;
	int textFieldLongLineLength;
	float textFieldInsertCursorWidth;
	float textFieldOvertypeCursorScale; // portion of text font max glyph width, from 0.0f to 1.0f
	int comboBoxLineLength;
	int comboBoxExpandViewItems;
	float timelineMarkerWidth;
	float rightNavWidthScale; // portion of total window width, from 0.0f to 1.0f
	int snackbarTimeout; // ms
	int snackbarScrollDuration; // ms
	int recordSyncDelayDuration; // ms
	float smallThumbnailImageScale; // portion of total window width, from 0.0f to 1.0f
	float mediumThumbnailImageScale; // portion of total window width, from 0.0f to 1.0f
	float largeThumbnailImageScale; // portion of total window width, from 0.0f to 1.0f
	StdString coreSpritesPath;
	bool isLoaded;
	SpriteGroup coreSprites;
};

#endif
