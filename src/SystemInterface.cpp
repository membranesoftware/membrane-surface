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
#include <stdlib.h>
#include "Config.h"
#include "SystemInterface.h"

const char *SystemInterface::version = "23-stable-c19b2321";
const char *SystemInterface::Command_ClearCache = "ClearCache";
const char *SystemInterface::Command_CommandResult = "CommandResult";
const char *SystemInterface::Command_EndSet = "EndSet";
const char *SystemInterface::Command_GetStatus = "GetStatus";
const char *SystemInterface::Command_PlayAnimation = "PlayAnimation";
const char *SystemInterface::Command_RemoveWindow = "RemoveWindow";
const char *SystemInterface::Command_ShowColorFillBackground = "ShowColorFillBackground";
const char *SystemInterface::Command_ShowCountdownWindow = "ShowCountdownWindow";
const char *SystemInterface::Command_ShowFileImageBackground = "ShowFileImageBackground";
const char *SystemInterface::Command_ShowIconLabelWindow = "ShowIconLabelWindow";
const char *SystemInterface::Command_ShowResourceImageBackground = "ShowResourceImageBackground";
const char *SystemInterface::Constant_AgentIdPrefixField = "b";
const char *SystemInterface::Constant_AuthorizationHashAlgorithm = "sha256";
const char *SystemInterface::Constant_AuthorizationHashPrefixField = "g";
const char *SystemInterface::Constant_AuthorizationTokenPrefixField = "h";
const char *SystemInterface::Constant_CreateTimePrefixField = "a";
const char *SystemInterface::Constant_DurationPrefixField = "f";
const char *SystemInterface::Constant_PriorityPrefixField = "d";
const char *SystemInterface::Constant_StartTimePrefixField = "e";
const char *SystemInterface::Constant_UserIdPrefixField = "c";
void SystemInterface::populate () {
  commandMap.insert (std::pair<StdString, SystemInterface::Command> (StdString ("ClearCache"), SystemInterface::Command (59, StdString ("ClearCache"), StdString ("EmptyObject"))));
  commandMap.insert (std::pair<StdString, SystemInterface::Command> (StdString ("CommandResult"), SystemInterface::Command (0, StdString ("CommandResult"), StdString ("CommandResult"))));
  commandMap.insert (std::pair<StdString, SystemInterface::Command> (StdString ("EndSet"), SystemInterface::Command (21, StdString ("EndSet"), StdString ("EmptyObject"))));
  commandMap.insert (std::pair<StdString, SystemInterface::Command> (StdString ("GetStatus"), SystemInterface::Command (8, StdString ("GetStatus"), StdString ("EmptyObject"))));
  commandMap.insert (std::pair<StdString, SystemInterface::Command> (StdString ("PlayAnimation"), SystemInterface::Command (215, StdString ("PlayAnimation"), StdString ("PlayAnimation"))));
  commandMap.insert (std::pair<StdString, SystemInterface::Command> (StdString ("RemoveWindow"), SystemInterface::Command (217, StdString ("RemoveWindow"), StdString ("RemoveWindow"))));
  commandMap.insert (std::pair<StdString, SystemInterface::Command> (StdString ("ShowColorFillBackground"), SystemInterface::Command (40, StdString ("ShowColorFillBackground"), StdString ("ShowColorFillBackground"))));
  commandMap.insert (std::pair<StdString, SystemInterface::Command> (StdString ("ShowCountdownWindow"), SystemInterface::Command (219, StdString ("ShowCountdownWindow"), StdString ("ShowCountdownWindow"))));
  commandMap.insert (std::pair<StdString, SystemInterface::Command> (StdString ("ShowFileImageBackground"), SystemInterface::Command (106, StdString ("ShowFileImageBackground"), StdString ("ShowFileImageBackground"))));
  commandMap.insert (std::pair<StdString, SystemInterface::Command> (StdString ("ShowIconLabelWindow"), SystemInterface::Command (216, StdString ("ShowIconLabelWindow"), StdString ("ShowIconLabelWindow"))));
  commandMap.insert (std::pair<StdString, SystemInterface::Command> (StdString ("ShowResourceImageBackground"), SystemInterface::Command (81, StdString ("ShowResourceImageBackground"), StdString ("ShowResourceImageBackground"))));
  getParamsMap.insert (std::pair<StdString, SystemInterface::GetParamsFunction> (StdString ("AnimationCommand"), SystemInterface::getParams_AnimationCommand));
  getParamsMap.insert (std::pair<StdString, SystemInterface::GetParamsFunction> (StdString ("CommandResult"), SystemInterface::getParams_CommandResult));
  getParamsMap.insert (std::pair<StdString, SystemInterface::GetParamsFunction> (StdString ("EmptyObject"), SystemInterface::getParams_EmptyObject));
  getParamsMap.insert (std::pair<StdString, SystemInterface::GetParamsFunction> (StdString ("PlayAnimation"), SystemInterface::getParams_PlayAnimation));
  getParamsMap.insert (std::pair<StdString, SystemInterface::GetParamsFunction> (StdString ("RemoveWindow"), SystemInterface::getParams_RemoveWindow));
  getParamsMap.insert (std::pair<StdString, SystemInterface::GetParamsFunction> (StdString ("ShowColorFillBackground"), SystemInterface::getParams_ShowColorFillBackground));
  getParamsMap.insert (std::pair<StdString, SystemInterface::GetParamsFunction> (StdString ("ShowCountdownWindow"), SystemInterface::getParams_ShowCountdownWindow));
  getParamsMap.insert (std::pair<StdString, SystemInterface::GetParamsFunction> (StdString ("ShowFileImageBackground"), SystemInterface::getParams_ShowFileImageBackground));
  getParamsMap.insert (std::pair<StdString, SystemInterface::GetParamsFunction> (StdString ("ShowIconLabelWindow"), SystemInterface::getParams_ShowIconLabelWindow));
  getParamsMap.insert (std::pair<StdString, SystemInterface::GetParamsFunction> (StdString ("ShowResourceImageBackground"), SystemInterface::getParams_ShowResourceImageBackground));
  populateDefaultFieldsMap.insert (std::pair<StdString, SystemInterface::PopulateDefaultFieldsFunction> (StdString ("AnimationCommand"), SystemInterface::populateDefaultFields_AnimationCommand));
  populateDefaultFieldsMap.insert (std::pair<StdString, SystemInterface::PopulateDefaultFieldsFunction> (StdString ("CommandResult"), SystemInterface::populateDefaultFields_CommandResult));
  populateDefaultFieldsMap.insert (std::pair<StdString, SystemInterface::PopulateDefaultFieldsFunction> (StdString ("EmptyObject"), SystemInterface::populateDefaultFields_EmptyObject));
  populateDefaultFieldsMap.insert (std::pair<StdString, SystemInterface::PopulateDefaultFieldsFunction> (StdString ("PlayAnimation"), SystemInterface::populateDefaultFields_PlayAnimation));
  populateDefaultFieldsMap.insert (std::pair<StdString, SystemInterface::PopulateDefaultFieldsFunction> (StdString ("RemoveWindow"), SystemInterface::populateDefaultFields_RemoveWindow));
  populateDefaultFieldsMap.insert (std::pair<StdString, SystemInterface::PopulateDefaultFieldsFunction> (StdString ("ShowColorFillBackground"), SystemInterface::populateDefaultFields_ShowColorFillBackground));
  populateDefaultFieldsMap.insert (std::pair<StdString, SystemInterface::PopulateDefaultFieldsFunction> (StdString ("ShowCountdownWindow"), SystemInterface::populateDefaultFields_ShowCountdownWindow));
  populateDefaultFieldsMap.insert (std::pair<StdString, SystemInterface::PopulateDefaultFieldsFunction> (StdString ("ShowFileImageBackground"), SystemInterface::populateDefaultFields_ShowFileImageBackground));
  populateDefaultFieldsMap.insert (std::pair<StdString, SystemInterface::PopulateDefaultFieldsFunction> (StdString ("ShowIconLabelWindow"), SystemInterface::populateDefaultFields_ShowIconLabelWindow));
  populateDefaultFieldsMap.insert (std::pair<StdString, SystemInterface::PopulateDefaultFieldsFunction> (StdString ("ShowResourceImageBackground"), SystemInterface::populateDefaultFields_ShowResourceImageBackground));
  hashFieldsMap.insert (std::pair<StdString, SystemInterface::HashFieldsFunction> (StdString ("AnimationCommand"), SystemInterface::hashFields_AnimationCommand));
  hashFieldsMap.insert (std::pair<StdString, SystemInterface::HashFieldsFunction> (StdString ("CommandResult"), SystemInterface::hashFields_CommandResult));
  hashFieldsMap.insert (std::pair<StdString, SystemInterface::HashFieldsFunction> (StdString ("EmptyObject"), SystemInterface::hashFields_EmptyObject));
  hashFieldsMap.insert (std::pair<StdString, SystemInterface::HashFieldsFunction> (StdString ("PlayAnimation"), SystemInterface::hashFields_PlayAnimation));
  hashFieldsMap.insert (std::pair<StdString, SystemInterface::HashFieldsFunction> (StdString ("RemoveWindow"), SystemInterface::hashFields_RemoveWindow));
  hashFieldsMap.insert (std::pair<StdString, SystemInterface::HashFieldsFunction> (StdString ("ShowColorFillBackground"), SystemInterface::hashFields_ShowColorFillBackground));
  hashFieldsMap.insert (std::pair<StdString, SystemInterface::HashFieldsFunction> (StdString ("ShowCountdownWindow"), SystemInterface::hashFields_ShowCountdownWindow));
  hashFieldsMap.insert (std::pair<StdString, SystemInterface::HashFieldsFunction> (StdString ("ShowFileImageBackground"), SystemInterface::hashFields_ShowFileImageBackground));
  hashFieldsMap.insert (std::pair<StdString, SystemInterface::HashFieldsFunction> (StdString ("ShowIconLabelWindow"), SystemInterface::hashFields_ShowIconLabelWindow));
  hashFieldsMap.insert (std::pair<StdString, SystemInterface::HashFieldsFunction> (StdString ("ShowResourceImageBackground"), SystemInterface::hashFields_ShowResourceImageBackground));
}

void SystemInterface::getParams_AnimationCommand (std::list<SystemInterface::Param> *destList) {
  destList->clear ();
  destList->push_back (SystemInterface::Param (StdString ("executeTime"), StdString ("number"), StdString (""), 17));
  destList->push_back (SystemInterface::Param (StdString ("command"), StdString ("object"), StdString (""), 257));
}

void SystemInterface::getParams_CommandResult (std::list<SystemInterface::Param> *destList) {
  destList->clear ();
  destList->push_back (SystemInterface::Param (StdString ("success"), StdString ("boolean"), StdString (""), 1));
  destList->push_back (SystemInterface::Param (StdString ("error"), StdString ("string"), StdString (""), 0));
  destList->push_back (SystemInterface::Param (StdString ("itemId"), StdString ("string"), StdString (""), 32));
  destList->push_back (SystemInterface::Param (StdString ("item"), StdString ("object"), StdString (""), 256));
  destList->push_back (SystemInterface::Param (StdString ("taskId"), StdString ("string"), StdString (""), 32));
  destList->push_back (SystemInterface::Param (StdString ("stringResult"), StdString ("string"), StdString (""), 0));
}

void SystemInterface::getParams_EmptyObject (std::list<SystemInterface::Param> *destList) {
  destList->clear ();
}

void SystemInterface::getParams_PlayAnimation (std::list<SystemInterface::Param> *destList) {
  destList->clear ();
  destList->push_back (SystemInterface::Param (StdString ("commands"), StdString ("array"), StdString ("AnimationCommand"), 1));
}

void SystemInterface::getParams_RemoveWindow (std::list<SystemInterface::Param> *destList) {
  destList->clear ();
  destList->push_back (SystemInterface::Param (StdString ("windowId"), StdString ("string"), StdString (""), 3));
}

void SystemInterface::getParams_ShowColorFillBackground (std::list<SystemInterface::Param> *destList) {
  destList->clear ();
  destList->push_back (SystemInterface::Param (StdString ("fillColorR"), StdString ("number"), 129, (double) 0, (double) 255));
  destList->push_back (SystemInterface::Param (StdString ("fillColorG"), StdString ("number"), 129, (double) 0, (double) 255));
  destList->push_back (SystemInterface::Param (StdString ("fillColorB"), StdString ("number"), 129, (double) 0, (double) 255));
}

void SystemInterface::getParams_ShowCountdownWindow (std::list<SystemInterface::Param> *destList) {
  destList->clear ();
  destList->push_back (SystemInterface::Param (StdString ("windowId"), StdString ("string"), StdString (""), 1));
  destList->push_back (SystemInterface::Param (StdString ("icon"), StdString ("number"), StdString (""), 1));
  destList->push_back (SystemInterface::Param (StdString ("positionX"), StdString ("number"), StdString (""), 1));
  destList->push_back (SystemInterface::Param (StdString ("positionY"), StdString ("number"), StdString (""), 1));
  destList->push_back (SystemInterface::Param (StdString ("labelText"), StdString ("string"), StdString (""), 3));
  destList->push_back (SystemInterface::Param (StdString ("countdownTime"), StdString ("number"), StdString (""), 1));
}

void SystemInterface::getParams_ShowFileImageBackground (std::list<SystemInterface::Param> *destList) {
  destList->clear ();
  destList->push_back (SystemInterface::Param (StdString ("imagePath"), StdString ("string"), StdString (""), 3));
  destList->push_back (SystemInterface::Param (StdString ("background"), StdString ("number"), StdString (""), 1));
}

void SystemInterface::getParams_ShowIconLabelWindow (std::list<SystemInterface::Param> *destList) {
  destList->clear ();
  destList->push_back (SystemInterface::Param (StdString ("windowId"), StdString ("string"), StdString (""), 1));
  destList->push_back (SystemInterface::Param (StdString ("icon"), StdString ("number"), StdString (""), 1));
  destList->push_back (SystemInterface::Param (StdString ("positionX"), StdString ("number"), StdString (""), 1));
  destList->push_back (SystemInterface::Param (StdString ("positionY"), StdString ("number"), StdString (""), 1));
  destList->push_back (SystemInterface::Param (StdString ("labelText"), StdString ("string"), StdString (""), 3));
}

void SystemInterface::getParams_ShowResourceImageBackground (std::list<SystemInterface::Param> *destList) {
  destList->clear ();
  destList->push_back (SystemInterface::Param (StdString ("imagePath"), StdString ("string"), StdString (""), 3));
}

void SystemInterface::populateDefaultFields_AnimationCommand (Json *destObject) {
  if (! destObject->exists ("executeTime")) {
    destObject->set ("executeTime", 0);
  }
}

void SystemInterface::populateDefaultFields_CommandResult (Json *destObject) {
}

void SystemInterface::populateDefaultFields_EmptyObject (Json *destObject) {
}

void SystemInterface::populateDefaultFields_PlayAnimation (Json *destObject) {
}

void SystemInterface::populateDefaultFields_RemoveWindow (Json *destObject) {
}

void SystemInterface::populateDefaultFields_ShowColorFillBackground (Json *destObject) {
}

void SystemInterface::populateDefaultFields_ShowCountdownWindow (Json *destObject) {
  if (! destObject->exists ("windowId")) {
    destObject->set ("windowId", "");
  }
  if (! destObject->exists ("icon")) {
    destObject->set ("icon", 0);
  }
  if (! destObject->exists ("countdownTime")) {
    destObject->set ("countdownTime", 20000);
  }
}

void SystemInterface::populateDefaultFields_ShowFileImageBackground (Json *destObject) {
  if (! destObject->exists ("background")) {
    destObject->set ("background", 0);
  }
}

void SystemInterface::populateDefaultFields_ShowIconLabelWindow (Json *destObject) {
  if (! destObject->exists ("windowId")) {
    destObject->set ("windowId", "");
  }
  if (! destObject->exists ("icon")) {
    destObject->set ("icon", 0);
  }
}

void SystemInterface::populateDefaultFields_ShowResourceImageBackground (Json *destObject) {
}

void SystemInterface::hashFields_AnimationCommand (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr) {
  StdString s;

  s.sprintf ("%lli", (long long int) commandParams->getNumber ("executeTime", (int64_t) 0));
  hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
}

void SystemInterface::hashFields_CommandResult (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr) {
  StdString s;

  if (commandParams->exists ("error")) {
    s = commandParams->getString ("error", "");
    if (! s.empty ()) {
      hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
    }
  }
  if (commandParams->exists ("itemId")) {
    s = commandParams->getString ("itemId", "");
    if (! s.empty ()) {
      hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
    }
  }
  if (commandParams->exists ("stringResult")) {
    s = commandParams->getString ("stringResult", "");
    if (! s.empty ()) {
      hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
    }
  }
  s.sprintf ("%s", commandParams->getBoolean ("success", false) ? "true" : "false");
  hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
  if (commandParams->exists ("taskId")) {
    s = commandParams->getString ("taskId", "");
    if (! s.empty ()) {
      hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
    }
  }
}

void SystemInterface::hashFields_EmptyObject (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr) {

}

void SystemInterface::hashFields_PlayAnimation (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr) {
  Json obj;
  int i, len;

  len = commandParams->getArrayLength ("commands");
  for (i = 0; i < len; ++i) {
    if (commandParams->getArrayObject ("commands", i, &obj)) {
      SystemInterface::hashFields_AnimationCommand (&obj, hashUpdateFn, hashContextPtr);
    }
  }
}

void SystemInterface::hashFields_RemoveWindow (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr) {
  StdString s;

  s = commandParams->getString ("windowId", "");
  if (! s.empty ()) {
    hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
  }
}

void SystemInterface::hashFields_ShowColorFillBackground (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr) {
  StdString s;

  s.sprintf ("%lli", (long long int) commandParams->getNumber ("fillColorB", (int64_t) 0));
  hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
  s.sprintf ("%lli", (long long int) commandParams->getNumber ("fillColorG", (int64_t) 0));
  hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
  s.sprintf ("%lli", (long long int) commandParams->getNumber ("fillColorR", (int64_t) 0));
  hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
}

void SystemInterface::hashFields_ShowCountdownWindow (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr) {
  StdString s;

  s.sprintf ("%lli", (long long int) commandParams->getNumber ("countdownTime", (int64_t) 0));
  hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
  s.sprintf ("%lli", (long long int) commandParams->getNumber ("icon", (int64_t) 0));
  hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
  s = commandParams->getString ("labelText", "");
  if (! s.empty ()) {
    hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
  }
  s.sprintf ("%lli", (long long int) commandParams->getNumber ("positionX", (int64_t) 0));
  hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
  s.sprintf ("%lli", (long long int) commandParams->getNumber ("positionY", (int64_t) 0));
  hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
  s = commandParams->getString ("windowId", "");
  if (! s.empty ()) {
    hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
  }
}

void SystemInterface::hashFields_ShowFileImageBackground (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr) {
  StdString s;

  s.sprintf ("%lli", (long long int) commandParams->getNumber ("background", (int64_t) 0));
  hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
  s = commandParams->getString ("imagePath", "");
  if (! s.empty ()) {
    hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
  }
}

void SystemInterface::hashFields_ShowIconLabelWindow (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr) {
  StdString s;

  s.sprintf ("%lli", (long long int) commandParams->getNumber ("icon", (int64_t) 0));
  hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
  s = commandParams->getString ("labelText", "");
  if (! s.empty ()) {
    hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
  }
  s.sprintf ("%lli", (long long int) commandParams->getNumber ("positionX", (int64_t) 0));
  hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
  s.sprintf ("%lli", (long long int) commandParams->getNumber ("positionY", (int64_t) 0));
  hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
  s = commandParams->getString ("windowId", "");
  if (! s.empty ()) {
    hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
  }
}

void SystemInterface::hashFields_ShowResourceImageBackground (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr) {
  StdString s;

  s = commandParams->getString ("imagePath", "");
  if (! s.empty ()) {
    hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
  }
}
SystemInterface *SystemInterface::instance = NULL;

SystemInterface::SystemInterface ()
: lastError ("")
{
	populate ();
}

SystemInterface::~SystemInterface () {

}

Json *SystemInterface::createCommand (const SystemInterface::Prefix &prefix, const char *commandName, Json *commandParams) {
	Json *cmd, *cmdprefix;
	SystemInterface::Command command;

	if (! commandParams) {
		commandParams = new Json ();
		commandParams->setEmpty ();
	}
	if (! getCommand (commandName, &command)) {
		lastError.sprintf ("Unknown command name \"%s\"", commandName);
		delete (commandParams);
		return (NULL);
	}
	if (! populateDefaultFields (command.paramType, commandParams)) {
		lastError.sprintf ("Command \"%s\" failed to populate fields for param type \"%s\"", commandName, command.paramType.c_str ());
		delete (commandParams);
		return (NULL);
	}

	cmd = new Json ();
	cmd->set ("command", command.id);
	cmd->set ("commandName", commandName);

	cmdprefix = new Json ();
	if (prefix.createTime > 0) {
		cmdprefix->set (SystemInterface::Constant_CreateTimePrefixField, prefix.createTime);
	}
	if (! prefix.agentId.empty ()) {
		cmdprefix->set (SystemInterface::Constant_AgentIdPrefixField, prefix.agentId);
	}
	if (! prefix.userId.empty ()) {
		cmdprefix->set (SystemInterface::Constant_UserIdPrefixField, prefix.userId);
	}
	if (prefix.priority > 0) {
		cmdprefix->set (SystemInterface::Constant_PriorityPrefixField, prefix.priority);
	}
	if (prefix.startTime > 0) {
		cmdprefix->set (SystemInterface::Constant_StartTimePrefixField, prefix.startTime);
	}
	if (prefix.duration > 0) {
		cmdprefix->set (SystemInterface::Constant_DurationPrefixField, prefix.duration);
	}
	cmd->set ("prefix", cmdprefix);

	cmd->set ("params", commandParams);

	return (cmd);
}

bool SystemInterface::setCommandAuthorization (Json *command, const StdString &authSecret, const StdString &authToken, SystemInterface::HashUpdateFunction hashUpdateFn, SystemInterface::HashDigestFunction hashDigestFn, void *hashContextPtr) {
	StdString hash;
	Json prefix;
	bool result;

	result = false;
	hash = getCommandAuthorizationHash (command, authSecret, authToken, hashUpdateFn, hashDigestFn, hashContextPtr);
	if (! hash.empty ()) {
		if (command->getObject ("prefix", &prefix)) {
			prefix.set (SystemInterface::Constant_AuthorizationHashPrefixField, hash);
			if ((! authToken.empty ()) && (! prefix.exists (SystemInterface::Constant_AuthorizationTokenPrefixField))) {
				prefix.set (SystemInterface::Constant_AuthorizationTokenPrefixField, authToken);
			}
			result = true;
		}
	}

	return (result);
}

StdString SystemInterface::getCommandAuthorizationHash (Json *command, const StdString &authSecret, const StdString &authToken, SystemInterface::HashUpdateFunction hashUpdateFn, SystemInterface::HashDigestFunction hashDigestFn, void *hashContextPtr) {
	SystemInterface::Command cmd;
	StdString token, cmdname, s;
	Json prefix, params;

	if (! (hashUpdateFn && hashDigestFn)) {
		return (StdString (""));
	}

	cmdname = command->getString ("commandName", "");
	if (! getCommand (cmdname, &cmd)) {
		return (StdString (""));
	}

	if (! command->getObject ("prefix", &prefix)) {
		return (StdString (""));
	}

	if (! authToken.empty ()) {
		token.assign (authToken);
	}
	else {
		token = prefix.getString (SystemInterface::Constant_AuthorizationTokenPrefixField, "");
	}

	hashUpdateFn (hashContextPtr, (unsigned char *) authSecret.c_str (), authSecret.length ());
	hashUpdateFn (hashContextPtr, (unsigned char *) token.c_str (), token.length ());
	hashUpdateFn (hashContextPtr, (unsigned char *) cmdname.c_str (), cmdname.length ());

	if (prefix.isNumber (SystemInterface::Constant_CreateTimePrefixField)) {
		s.sprintf ("%lli", (long long int) prefix.getNumber (SystemInterface::Constant_CreateTimePrefixField, (int64_t) 0));
		hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
	}

	s = prefix.getString (SystemInterface::Constant_AgentIdPrefixField, "");
	if (! s.empty ()) {
		hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
	}

	s = prefix.getString (SystemInterface::Constant_UserIdPrefixField, "");
	if (! s.empty ()) {
		hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
	}

	if (prefix.isNumber (SystemInterface::Constant_PriorityPrefixField)) {
		s.sprintf ("%lli", (long long int) prefix.getNumber (SystemInterface::Constant_PriorityPrefixField, (int64_t) 0));
		hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
	}

	if (prefix.isNumber (SystemInterface::Constant_StartTimePrefixField)) {
		s.sprintf ("%lli", (long long int) prefix.getNumber (SystemInterface::Constant_StartTimePrefixField, (int64_t) 0));
		hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
	}

	if (prefix.isNumber (SystemInterface::Constant_DurationPrefixField)) {
		s.sprintf ("%lli", (long long int) prefix.getNumber (SystemInterface::Constant_DurationPrefixField, (int64_t) 0));
		hashUpdateFn (hashContextPtr, (unsigned char *) s.c_str (), s.length ());
	}

	if (command->getObject ("params", &params)) {
		hashFields (cmd.paramType, &params, hashUpdateFn, hashContextPtr);
	}

	return (hashDigestFn (hashContextPtr));
}

bool SystemInterface::getCommand (const StdString &name, SystemInterface::Command *command) {
	std::map<StdString, SystemInterface::Command>::iterator i;

	i = commandMap.find (name);
	if (i == commandMap.end ()) {
		return (false);
	}

	*command = i->second;
	return (true);
}

bool SystemInterface::getType (const StdString &name, std::list<SystemInterface::Param> *destList) {
	std::map<StdString, SystemInterface::GetParamsFunction>::iterator i;

	i = getParamsMap.find (name);
	if (i == getParamsMap.end ()) {
		return (false);
	}

	i->second (destList);
	return (true);
}

bool SystemInterface::populateDefaultFields (const StdString &typeName, Json *destObject) {
	std::map<StdString, SystemInterface::PopulateDefaultFieldsFunction>::iterator i;

	i = populateDefaultFieldsMap.find (typeName);
	if (i == populateDefaultFieldsMap.end ()) {
		return (false);
	}

	i->second (destObject);
	return (true);
}

void SystemInterface::hashFields (const StdString &typeName, Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr) {
	std::map<StdString, SystemInterface::HashFieldsFunction>::iterator i;

	if (! hashUpdateFn) {
		return;
	}

	i = hashFieldsMap.find (typeName);
	if (i == hashFieldsMap.end ()) {
		return;
	}

	i->second (commandParams, hashUpdateFn, hashContextPtr);
}

bool SystemInterface::fieldsValid (Json *fields, std::list<SystemInterface::Param> *paramList) {
	std::list<SystemInterface::Param>::iterator i, end;
	double numbervalue;
	StdString stringvalue;

	// TODO: Check for unknown field keys
	// TODO: Possibly allow validation to succeed if unknown field keys are present

	i = paramList->begin ();
	end = paramList->end ();
	while (i != end) {
		if (i->flags & SystemInterface::ParamFlag_Required) {
			if (! fields->exists (i->name)) {
				lastError.sprintf ("Missing required parameter field \"%s\"", i->name.c_str ());
				return (false);
			}
		}

		if (i->type.equals ("number")) {
			if (! fields->isNumber (i->name)) {
				lastError.sprintf ("Parameter field \"%s\" has incorrect type (expecting number)", i->name.c_str ());
				return (false);
			}

			numbervalue = fields->getNumber (i->name, 0);
			if (i->flags & SystemInterface::ParamFlag_GreaterThanZero) {
				if (numbervalue <= 0.0f) {
					lastError.sprintf ("Parameter field \"%s\" must be a number greater than zero", i->name.c_str ());
					return (false);
				}
			}

			if (i->flags & SystemInterface::ParamFlag_ZeroOrGreater) {
				if (numbervalue < 0.0f) {
					lastError.sprintf ("Parameter field \"%s\" must be a number greater than or equal to zero", i->name.c_str ());
					return (false);
				}
			}

			if (i->flags & SystemInterface::ParamFlag_RangedNumber) {
				if ((numbervalue < i->rangeMin) || (numbervalue > i->rangeMax)) {
					lastError.sprintf ("Parameter field \"%s\" must be a number in the range [%f..%f]", i->name.c_str (), i->rangeMin, i->rangeMax);
					return (false);
				}
			}

			if (i->flags & SystemInterface::ParamFlag_EnumValue) {
				// TODO: Implement this
			}
		}
		else if (i->type.equals ("boolean")) {
			if (! fields->isBoolean (i->name)) {
				lastError.sprintf ("Parameter field \"%s\" has incorrect type (expecting boolean)", i->name.c_str ());
				return (false);
			}
		}
		else if (i->type.equals ("string")) {
			if (! fields->isString (i->name)) {
				lastError.sprintf ("Parameter field \"%s\" has incorrect type (expecting string)", i->name.c_str ());
				return (false);
			}

			stringvalue = fields->getString (i->name, "");
			if (i->flags & SystemInterface::ParamFlag_NotEmpty) {
				if (stringvalue.empty ()) {
					lastError.sprintf ("Parameter field \"%s\" cannot contain an empty string", i->name.c_str ());
					return (false);
				}
			}

			if ((i->flags & SystemInterface::ParamFlag_Hostname) && (! stringvalue.empty ())) {
				// TODO: Implement this
				/*
				if (value.search (/^[a-zA-Z][a-zA-Z0-9-\.]*(:[0-9]+){0,1}$/) != 0) {
					return ("Parameter field \"" + param.name + "\" must contain a hostname string");
				}
				*/
			}

			if ((i->flags & SystemInterface::ParamFlag_Uuid) && (! stringvalue.empty ())) {
				// TODO: Implement this
				/*
				if (value.search (/^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}$/) != 0) {
					return ("Parameter field \"" + param.name + "\" must contain a UUID string");
				}
				*/
			}

			if ((i->flags & SystemInterface::ParamFlag_Url) && (! stringvalue.empty ())) {
				// TODO: Implement this
				/*
				if (value.search (/[^A-Za-z0-9\$\-_\.\+\!\*\?\(\),\/:;=&]/) != -1) {
					return ("Parameter field \"" + param.name + "\" must contain a URL string");
				}
				*/
			}

			if (i->flags & SystemInterface::ParamFlag_EnumValue) {
				// TODO: Implement this
			}
		}
		else if (i->type.equals ("array")) {
			// TODO: Implement this
		}
		else if (i->type.equals ("map")) {
			// TODO: Implement this
		}
		else if (i->type.equals ("object")) {
			// TODO: Implement this
		}

		++i;
	}

	return (true);
}

bool SystemInterface::parseCommand (const StdString &commandString, Json **commandJson) {
	Json *json;

	json = new Json ();
	if (! json->parse (commandString.c_str (), commandString.length ())) {
		lastError.assign ("JSON parse failed");
		delete (json);
		return (false);
	}

	// TODO: Validate fields here

	if (commandJson) {
		*commandJson = json;
	}
	else {
		delete (json);
	}
	return (true);
}

int SystemInterface::getCommandId (Json *command) {
	return (command->getNumber ("command", -1));
}

StdString SystemInterface::getCommandName (Json *command) {
	return (command->getString ("commandName", ""));
}

StdString SystemInterface::getCommandAgentId (Json *command) {
	Json prefix;

	if (! command->getObject ("prefix", &prefix)) {
		return (StdString (""));
	}

	return (prefix.getString (SystemInterface::Constant_AgentIdPrefixField, ""));
}

StdString SystemInterface::getCommandRecordId (Json *command) {
	return (getCommandStringParam (command, "id", ""));
}

bool SystemInterface::isWindowsPlatform (const StdString &platform) {
	return (platform.startsWith ("windows") || platform.equals ("win32") || platform.equals ("win64"));
}

SystemInterface::Prefix SystemInterface::getCommandPrefix (Json *command) {
	SystemInterface::Prefix result;
	Json prefix;

	if (! command->getObject ("prefix", &prefix)) {
		return (result);
	}

	result.agentId = prefix.getString (SystemInterface::Constant_AgentIdPrefixField, "");
	result.userId = prefix.getString (SystemInterface::Constant_UserIdPrefixField, "");
	result.priority = prefix.getNumber (SystemInterface::Constant_PriorityPrefixField, (int) 0);
	result.createTime = prefix.getNumber (SystemInterface::Constant_CreateTimePrefixField, (int64_t) 0);
	result.startTime = prefix.getNumber (SystemInterface::Constant_StartTimePrefixField, (int64_t) 0);
	result.duration = prefix.getNumber (SystemInterface::Constant_DurationPrefixField, (int64_t) 0);

	return (result);
}

bool SystemInterface::getCommandParams (Json *command, Json *params) {
	return (command->getObject ("params", params));
}

StdString SystemInterface::getCommandStringParam (Json *command, const StdString &paramName, const StdString &defaultValue) {
	Json params;

	if (! command->getObject ("params", &params)) {
		return (defaultValue);
	}

	return (params.getString (paramName, defaultValue));
}

StdString SystemInterface::getCommandStringParam (Json *command, const char *paramName, const char *defaultValue) {
	return (getCommandStringParam (command, StdString (paramName), StdString (defaultValue)));
}

bool SystemInterface::getCommandBooleanParam (Json *command, const StdString &paramName, bool defaultValue) {
	Json params;

	if (! command->getObject ("params", &params)) {
		return (defaultValue);
	}

	return (params.getBoolean (paramName, defaultValue));
}

bool SystemInterface::getCommandBooleanParam (Json *command, const char *paramName, bool defaultValue) {
	return (getCommandBooleanParam (command, StdString (paramName), defaultValue));
}

int SystemInterface::getCommandNumberParam (Json *command, const StdString &paramName, const int defaultValue) {
	Json params;

	if (! command->getObject ("params", &params)) {
		return (defaultValue);
	}

	return (params.getNumber (paramName, defaultValue));
}

int SystemInterface::getCommandNumberParam (Json *command, const char *paramName, const int defaultValue) {
	return (getCommandNumberParam (command, StdString (paramName), defaultValue));
}

int64_t SystemInterface::getCommandNumberParam (Json *command, const StdString &paramName, const int64_t defaultValue) {
	Json params;

	if (! command->getObject ("params", &params)) {
		return (defaultValue);
	}

	return (params.getNumber (paramName, defaultValue));
}

int64_t SystemInterface::getCommandNumberParam (Json *command, const char *paramName, const int64_t defaultValue) {
	return (getCommandNumberParam (command, StdString (paramName), defaultValue));
}

double SystemInterface::getCommandNumberParam (Json *command, const StdString &paramName, const double defaultValue) {
	Json params;

	if (! command->getObject ("params", &params)) {
		return (defaultValue);
	}

	return (params.getNumber (paramName, defaultValue));
}

double SystemInterface::getCommandNumberParam (Json *command, const char *paramName, const double defaultValue) {
	return (getCommandNumberParam (command, StdString (paramName), defaultValue));
}

float SystemInterface::getCommandNumberParam (Json *command, const StdString &paramName, const float defaultValue) {
	Json params;

	if (! command->getObject ("params", &params)) {
		return (defaultValue);
	}

	return (params.getNumber (paramName, defaultValue));
}

float SystemInterface::getCommandNumberParam (Json *command, const char *paramName, const float defaultValue) {
	return (getCommandNumberParam (command, StdString (paramName), defaultValue));
}

bool SystemInterface::getCommandObjectParam (Json *command, const StdString &paramName, Json *destJson) {
	Json params;

	if (! command->getObject ("params", &params)) {
		return (false);
	}

	return (params.getObject (paramName, destJson));
}

bool SystemInterface::getCommandObjectParam (Json *command, const char *paramName, Json *destJson) {
	return (getCommandObjectParam (command, StdString (paramName), destJson));
}

bool SystemInterface::getCommandNumberArrayParam (Json *command, const StdString &paramName, std::vector<int> *destList, bool shouldClear) {
	Json params;
	int i, len;

	if (! command->getObject ("params", &params)) {
		return (false);
	}
	if (! params.isArray (paramName)) {
		return (false);
	}

	if (shouldClear) {
		destList->clear ();
	}
	len = params.getArrayLength (paramName);
	for (i = 0; i < len; ++i) {
		destList->push_back (params.getArrayNumber (paramName, i, (int) 0));
	}

	return (true);
}

bool SystemInterface::getCommandNumberArrayParam (Json *command, const char *paramName, std::vector<int> *destList, bool shouldClear) {
	return (getCommandNumberArrayParam (command, StdString (paramName), destList, shouldClear));
}

bool SystemInterface::getCommandNumberArrayParam (Json *command, const StdString &paramName, std::vector<int64_t> *destList, bool shouldClear) {
	Json params;
	int i, len;

	if (! command->getObject ("params", &params)) {
		return (false);
	}
	if (! params.isArray (paramName)) {
		return (false);
	}

	if (shouldClear) {
		destList->clear ();
	}
	len = params.getArrayLength (paramName);
	for (i = 0; i < len; ++i) {
		destList->push_back (params.getArrayNumber (paramName, i, (int64_t) 0));
	}

	return (true);
}

bool SystemInterface::getCommandNumberArrayParam (Json *command, const char *paramName, std::vector<int64_t> *destList, bool shouldClear) {
	return (getCommandNumberArrayParam (command, StdString (paramName), destList, shouldClear));
}

bool SystemInterface::getCommandNumberArrayParam (Json *command, const StdString &paramName, std::vector<double> *destList, bool shouldClear) {
	Json params;
	int i, len;

	if (! command->getObject ("params", &params)) {
		return (false);
	}
	if (! params.isArray (paramName)) {
		return (false);
	}

	if (shouldClear) {
		destList->clear ();
	}
	len = params.getArrayLength (paramName);
	for (i = 0; i < len; ++i) {
		destList->push_back (params.getArrayNumber (paramName, i, (double) 0.0f));
	}

	return (true);
}

bool SystemInterface::getCommandNumberArrayParam (Json *command, const char *paramName, std::vector<double> *destList, bool shouldClear) {
	return (getCommandNumberArrayParam (command, StdString (paramName), destList, shouldClear));
}

bool SystemInterface::getCommandNumberArrayParam (Json *command, const StdString &paramName, std::vector<float> *destList, bool shouldClear) {
	Json params;
	int i, len;

	if (! command->getObject ("params", &params)) {
		return (false);
	}
	if (! params.isArray (paramName)) {
		return (false);
	}

	if (shouldClear) {
		destList->clear ();
	}
	len = params.getArrayLength (paramName);
	for (i = 0; i < len; ++i) {
		destList->push_back (params.getArrayNumber (paramName, i, (float) 0.0f));
	}

	return (true);
}

bool SystemInterface::getCommandNumberArrayParam (Json *command, const char *paramName, std::vector<float> *destList, bool shouldClear) {
	return (getCommandNumberArrayParam (command, StdString (paramName), destList, shouldClear));
}

int SystemInterface::getCommandArrayLength (Json *command, const StdString &paramName) {
	Json params;

	if (! command->getObject ("params", &params)) {
		return (0);
	}

	return (params.getArrayLength (paramName));
}

int SystemInterface::getCommandArrayLength (Json *command, const char *paramName) {
	return (getCommandArrayLength (command, StdString (paramName)));
}

int SystemInterface::getCommandNumberArrayItem (Json *command, const StdString &paramName, int index, int defaultValue) {
	Json params;

	if (! command->getObject ("params", &params)) {
		return (0);
	}

	return (params.getArrayNumber (paramName, index, defaultValue));
}

int64_t SystemInterface::getCommandNumberArrayItem (Json *command, const StdString &paramName, int index, int64_t defaultValue) {
	Json params;

	if (! command->getObject ("params", &params)) {
		return (0);
	}

	return (params.getArrayNumber (paramName, index, defaultValue));
}

double SystemInterface::getCommandNumberArrayItem (Json *command, const StdString &paramName, int index, double defaultValue) {
	Json params;

	if (! command->getObject ("params", &params)) {
		return (0);
	}

	return (params.getArrayNumber (paramName, index, defaultValue));
}

float SystemInterface::getCommandNumberArrayItem (Json *command, const StdString &paramName, int index, float defaultValue) {
	Json params;

	if (! command->getObject ("params", &params)) {
		return (0);
	}

	return (params.getArrayNumber (paramName, index, defaultValue));
}

StdString SystemInterface::getCommandStringArrayItem (Json *command, const StdString &paramName, int index, const StdString &defaultValue) {
	Json params;

	if (! command->getObject ("params", &params)) {
		return (0);
	}

	return (params.getArrayString (paramName, index, defaultValue));
}

StdString SystemInterface::getCommandStringArrayItem (Json *command, const char *paramName, int index, const StdString &defaultValue) {
	return (getCommandStringArrayItem (command, StdString (paramName), index, defaultValue));
}

bool SystemInterface::getCommandObjectArrayItem (Json *command, const StdString &paramName, int index, Json *destJson) {
	Json params;

	if (! command->getObject ("params", &params)) {
		return (0);
	}

	return (params.getArrayObject (paramName, index, destJson));
}

bool SystemInterface::getCommandObjectArrayItem (Json *command, const char *paramName, int index, Json *destJson) {
	return (getCommandObjectArrayItem (command, StdString (paramName), index, destJson));
}
