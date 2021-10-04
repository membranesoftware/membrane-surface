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
// Index values that reference UiText strings

#ifndef UI_TEXT_STRING_H
#define UI_TEXT_STRING_H

#if PLATFORM_WINDOWS
#ifdef small
#undef small
#endif
#endif

class UiTextString {
public:
	static const int ConsoleStartText1 = 1;
	static const int ConsoleStartText2 = 2;
	static const int ConsoleStartText3 = 3;
	static const int ConsoleStartText4 = 4;
	static const int LuaScriptHelpHelpText = 5;
	static const int LuaScriptQuitHelpText = 6;
	static const int LuaScriptPrintHelpText = 7;
	static const int LuaScriptDofileHelpText = 8;
	static const int LuaScriptSleepHelpText = 9;
	static const int LuaSyntaxErrorText = 10;
	static const int LuaParseErrorText = 11;
	static const int LuaScriptExecutionErrorText = 12;
	static const int EnvironmentVariables = 13;
	static const int LuaScriptRunScriptHelpText = 14;
	static const int LuaScriptMissingFunctionArgumentErrorText = 15;
	static const int LuaScriptEmptyStringArgumentErrorText = 16;
	static const int LuaScriptLoadScriptErrorText = 17;
	static const int LuaScriptLoadScriptFileErrorText = 18;
	static const int LuaScriptLoadScriptSyntaxErrorText = 19;
	static const int LuaScriptLoadScriptExecutionErrorText = 20;
	static const int Ok = 21;
	static const int Yes = 22;
	static const int No = 23;
	static const int ShuttingDown = 24;
	static const int Confirm = 25;
	static const int Cancel = 26;
	static const int Remove = 27;
	static const int Console = 28;
};

#endif
