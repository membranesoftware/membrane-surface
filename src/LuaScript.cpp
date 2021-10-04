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
#include <string.h>
#include <list>
extern "C" {
#include "lua.h"
#include "lauxlib.h"
}
#include "SDL2/SDL.h"
#include "App.h"
#include "OsUtil.h"
#include "Log.h"
#include "UiText.h"
#include "LuaScript.h"

const LuaScript::Function LuaFunctions[] = {
	{
		LuaScript::help,
		"help",
		"()",
		UiTextString::LuaScriptHelpHelpText
	},
	{
		LuaScript::quit,
		"quit",
		"()",
		UiTextString::LuaScriptQuitHelpText
	},
	{
		LuaScript::print,
		"print",
		"(...)",
		UiTextString::LuaScriptPrintHelpText
	},
	{
		LuaScript::sleep,
		"sleep",
		"(milliseconds)",
		UiTextString::LuaScriptSleepHelpText
	},
	{
		LuaScript::dofile,
		"dofile",
		"(filename)",
		UiTextString::LuaScriptDofileHelpText
	}
};
const int LuaFunctionCount = sizeof (LuaFunctions) / sizeof (LuaScript::Function);

LuaScript::LuaScript (const StdString &script)
: script (script)
, runResult (-1)
, state (NULL)
{
	int i;

	state = luaL_newstate ();
	for (i = 0; i < LuaFunctionCount; ++i) {
		lua_register (state, LuaFunctions[i].name, LuaFunctions[i].fn);
	}
}

LuaScript::~LuaScript () {
	if (state) {
		lua_close (state);
		state = NULL;
	}
}

void LuaScript::run (void *luaScriptPtr) {
	LuaScript *lua;
	int result;

	lua = (LuaScript *) luaScriptPtr;
	result = luaL_loadstring (lua->state, lua->script.c_str ());
	if (result == LUA_ERRSYNTAX) {
		lua->runResult = OsUtil::Result::MalformedDataError;
		Log::printf ("%s", UiText::instance->getText (UiTextString::LuaSyntaxErrorText).c_str ());
	}
	else if (result != LUA_OK) {
		lua->runResult = OsUtil::Result::LuaOperationFailedError;
		Log::printf ("%s", UiText::instance->getText (UiTextString::LuaParseErrorText).c_str ());
	}
	else {
		result = lua_pcall (lua->state, 0, 0, 0);
		if (result != 0) {
			lua->runErrorText.assign (lua_tostring (lua->state, -1));
			lua->runResult = OsUtil::Result::LuaOperationFailedError;
			Log::printf ("%s; %s", UiText::instance->getText (UiTextString::LuaScriptExecutionErrorText).c_str (), lua->runErrorText.c_str ());
		}
		else {
			lua->runResult = OsUtil::Result::Success;
		}
	}

	delete (lua);
}

int LuaScript::help (lua_State *L) {
	std::list<LuaScript::Function> fns;
	std::list<LuaScript::Function>::iterator fi, end;
	StdString format, s;
	int i, w, maxw;

	maxw = 0;
	for (i = 0; i < LuaFunctionCount; ++i) {
		fns.push_back (LuaFunctions[i]);
		w = strlen (LuaFunctions[i].name) + strlen (LuaFunctions[i].parameters);
		if (w > maxw) {
			maxw = w;
		}
	}
	fns.sort (LuaScript::compareFunctions);

	maxw += 4;
	format.sprintf ("%%-%is%%s", maxw);
	fi = fns.begin ();
	end = fns.end ();
	while (fi != end) {
		s.sprintf ("%s%s", fi->name, fi->parameters);
		Log::printf (format.c_str (), s.c_str (), UiText::instance->getText (fi->helpText).c_str ());
		++fi;
	}
	Log::printf (" ");
	Log::printf ("* %s", UiText::instance->getText (UiTextString::EnvironmentVariables).capitalized ().c_str ());
	format.sprintf ("%%-%is%%s", maxw);
	Log::printf (format.c_str (), "RUN_SCRIPT", UiText::instance->getText (UiTextString::LuaScriptRunScriptHelpText).c_str ());
	return (0);
}

bool LuaScript::compareFunctions (const Function &a, const Function &b) {
	return (strcmp (a.name, b.name) < 0);
}

void LuaScript::argvInteger (lua_State *L, int position, int *value) {
	int argc;
	char buf[512];

	argc = lua_gettop (L);
	if (argc < position) {
		snprintf (buf, sizeof (buf), "%s", UiText::instance->getText (UiTextString::LuaScriptMissingFunctionArgumentErrorText).c_str ());
		luaL_error (L, "%s", buf);
		return;
	}
	if (value) {
		*value = lua_tointeger (L, position);
	}
}

void LuaScript::argvString (lua_State *L, int position, char **value) {
	int argc;
	char buf[512], *s;

	argc = lua_gettop (L);
	if (argc < position) {
		snprintf (buf, sizeof (buf), "%s", UiText::instance->getText (UiTextString::LuaScriptMissingFunctionArgumentErrorText).c_str ());
		luaL_error (L, "%s", buf);
		return;
	}
	s = (char *) lua_tostring (L, position);
	if ((! s) || (s[0] == '\0')) {
		snprintf (buf, sizeof (buf), "%s", UiText::instance->getText (UiTextString::LuaScriptEmptyStringArgumentErrorText).c_str ());
		luaL_error (L, "%s", buf);
		return;
	}
	if (value) {
		*value = s;
	}
}

void LuaScript::argvBoolean (lua_State *L, int position, bool *value) {
	int argc;
	char buf[512];

	argc = lua_gettop (L);
	if (argc < position) {
		snprintf (buf, sizeof (buf), "%s", UiText::instance->getText (UiTextString::LuaScriptMissingFunctionArgumentErrorText).c_str ());
		luaL_error (L, "%s", buf);
		return;
	}
	if (value) {
		*value = lua_toboolean (L, position);
	}
}

int LuaScript::print (lua_State *L) {
	StdString s;
	int argc, i;

	argc = lua_gettop (L);
	for (i = 1; i <= argc; ++i) {
		s.append (lua_tostring (L, i));
	}
	Log::printf ("%s", s.c_str ());
	return (0);
}

int LuaScript::dofile (lua_State *L) {
	LuaScript *lua;
	char *path, buf[4096];
	int result;

	LuaScript::argvString (L, 1, &path);
	buf[0] = '\0';
	lua = new LuaScript ();
	result = luaL_loadfile (lua->state, path);
	if (result == LUA_ERRFILE) {
		snprintf (buf, sizeof (buf), "%s: %s", UiText::instance->getText (UiTextString::LuaScriptLoadScriptFileErrorText).c_str (), path);
	}
	else if (result == LUA_ERRSYNTAX) {
		snprintf (buf, sizeof (buf), "%s: %s", UiText::instance->getText (UiTextString::LuaScriptLoadScriptSyntaxErrorText).c_str (), path);
	}
	else if (result != 0) {
		snprintf (buf, sizeof (buf), "%s: %s", UiText::instance->getText (UiTextString::LuaScriptLoadScriptErrorText).c_str (), path);
	}
	else {
		result = lua_pcall (lua->state, 0, 0, 0);
		if (result != 0) {
			snprintf (buf, sizeof (buf), "%s: %s", UiText::instance->getText (UiTextString::LuaScriptLoadScriptExecutionErrorText).c_str (), lua_tostring (lua->state, -1));
		}
	}

	delete (lua);
	if (buf[0] != '\0') {
		return (luaL_error (L, "%s", buf));
	}
	return (0);
}

int LuaScript::quit (lua_State *L) {
	App::instance->shutdown ();
	return (0);
}

int LuaScript::sleep (lua_State *L) {
	int ms;
	char buf[1024];

	LuaScript::argvInteger (L, 1, &ms);
	if (ms <= 0) {
		return (0);
	}
	if (App::instance->isShutdown) {
		snprintf (buf, sizeof (buf), "%s", UiText::instance->getText (UiTextString::ShuttingDown).c_str ());
		return (luaL_error (L, "%s", buf));
	}
	SDL_Delay (ms);
	return (0);
}
