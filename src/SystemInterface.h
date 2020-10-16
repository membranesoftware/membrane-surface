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
#ifndef SYSTEM_INTERFACE_H
#define SYSTEM_INTERFACE_H

#include <stdint.h>
#include <map>
#include <list>
#include <vector>
#include "StdString.h"
#include "Json.h"

class SystemInterface {
public:
  static const char *version;
  static const char *Command_AgentConfiguration;
  static const char *Command_AgentContact;
  static const char *Command_AgentStatus;
  static const char *Command_ClearCache;
  static const char *Command_CommandResult;
  static const char *Command_EndSet;
  static const char *Command_FindItems;
  static const char *Command_GetStatus;
  static const char *Command_ReportContact;
  static const char *Command_ReportStatus;
  static const char *Command_ShowColorFillBackground;
  static const char *Command_ShowFileImageBackground;
  static const char *Command_ShowResourceImageBackground;
  static const char *Command_ShutdownAgent;
  static const char *Command_TaskItem;
  static const int CommandId_AgentConfiguration = 45;
  static const int CommandId_AgentContact = 33;
  static const int CommandId_AgentStatus = 1;
  static const int CommandId_ClearCache = 59;
  static const int CommandId_CommandResult = 0;
  static const int CommandId_EndSet = 21;
  static const int CommandId_FindItems = 3;
  static const int CommandId_GetStatus = 8;
  static const int CommandId_ReportContact = 32;
  static const int CommandId_ReportStatus = 2;
  static const int CommandId_ShowColorFillBackground = 40;
  static const int CommandId_ShowFileImageBackground = 106;
  static const int CommandId_ShowResourceImageBackground = 81;
  static const int CommandId_ShutdownAgent = 43;
  static const int CommandId_TaskItem = 26;
  static const int ParamFlag_Required = 1;
  static const int ParamFlag_NotEmpty = 2;
  static const int ParamFlag_Hostname = 4;
  static const int ParamFlag_GreaterThanZero = 8;
  static const int ParamFlag_ZeroOrGreater = 16;
  static const int ParamFlag_Uuid = 32;
  static const int ParamFlag_Url = 64;
  static const int ParamFlag_RangedNumber = 128;
  static const int ParamFlag_Command = 256;
  static const int ParamFlag_EnumValue = 512;
  static const int Constant_MaxCommandPriority = 100;
  static const char *Constant_CreateTimePrefixField;
  static const char *Constant_AgentIdPrefixField;
  static const char *Constant_UserIdPrefixField;
  static const char *Constant_PriorityPrefixField;
  static const char *Constant_StartTimePrefixField;
  static const char *Constant_DurationPrefixField;
  static const char *Constant_AuthorizationHashPrefixField;
  static const char *Constant_AuthorizationTokenPrefixField;
  static const char *Constant_AuthorizationHashAlgorithm;
  static const char *Constant_WebSocketEvent;
  static const char *Constant_UrlQueryParameter;
  static const int Constant_DefaultTcpPort1 = 63738;
  static const int Constant_DefaultTcpPort2 = 63739;
  static const int Constant_DefaultUdpPort = 63738;
  static const char *Constant_DefaultInvokePath;
  static const char *Constant_DefaultAuthorizePath;
  static const char *Constant_DefaultLinkPath;
  static const int Constant_DefaultCommandType = 0;
  static const int Constant_Stream = 1;
  static const int Constant_Media = 2;
  static const int Constant_Monitor = 3;
  static const int Constant_Event = 4;
  static const int Constant_Master = 5;
  static const int Constant_Admin = 6;
  static const int Constant_Camera = 7;
  static const int Constant_CommandTypeCount = 8;
  static const int Constant_DefaultDisplayState = 0;
  static const int Constant_ShowUrlDisplayState = 1;
  static const int Constant_PlayMediaDisplayState = 2;
  static const int Constant_ShowImageDisplayState = 3;
  static const int Constant_PlayCameraStreamDisplayState = 4;
  static const int Constant_DefaultSortOrder = 0;
  static const int Constant_NameSort = 0;
  static const int Constant_NewestSort = 1;
  static const int Constant_DefaultStreamProfile = 0;
  static const int Constant_CompressedStreamProfile = 1;
  static const int Constant_LowQualityStreamProfile = 2;
  static const int Constant_LowestQualityStreamProfile = 3;
  static const int Constant_DefaultImageProfile = 0;
  static const int Constant_HighQualityImageProfile = 1;
  static const int Constant_LowQualityImageProfile = 2;
  static const int Constant_LowestQualityImageProfile = 3;
  static const int Constant_NoFlip = 0;
  static const int Constant_HorizontalFlip = 1;
  static const int Constant_VerticalFlip = 2;
  static const int Constant_HorizontalAndVerticalFlip = 3;
  static const char *Constant_DisplayCondition;
  void populate ();
	SystemInterface ();
	~SystemInterface ();

	struct Prefix {
		StdString agentId;
		StdString userId;
		int priority;
		int64_t createTime;
		int64_t startTime;
		int64_t duration;
		Prefix (): priority (0), createTime (0), startTime (0), duration (0) { }
	};

	struct Param {
		StdString name;
		StdString type;
		StdString containerType;
		int flags;
		double rangeMin, rangeMax;
		Param (): name (""), type (""), containerType (""), flags (0), rangeMin (0.0f), rangeMax (0.0f) { }
		Param (const StdString &name, const StdString &type, const StdString &containerType, int flags): name (name), type (type), containerType (containerType), flags (flags), rangeMin (0.0f), rangeMax (0.0f) { }
		Param (const StdString &name, const StdString &type, int flags, double rangeMin, double rangeMax): name (name), type (type), containerType (""), flags (flags), rangeMin (rangeMin), rangeMax (rangeMax) { }
	};

	struct Command {
		int id;
		StdString name;
		StdString paramType;
		Command (): id (0), name (""), paramType ("") { }
		Command (int id, const StdString &name, const StdString &paramType): id (id), name (name), paramType (paramType) { }
	};

	typedef void (*GetParamsFunction) (std::list<SystemInterface::Param> *destList);
	typedef void (*PopulateDefaultFieldsFunction) (Json *destObject);
	typedef void (*HashUpdateFunction) (void *contextPtr, unsigned char *data, int dataLength);
	typedef StdString (*HashDigestFunction) (void *contextPtr);
	typedef void (*HashFieldsFunction) (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr);

	StdString lastError;
	std::map<StdString, SystemInterface::Command> commandMap;
	std::map<StdString, SystemInterface::GetParamsFunction> getParamsMap;
	std::map<StdString, SystemInterface::PopulateDefaultFieldsFunction> populateDefaultFieldsMap;
	std::map<StdString, SystemInterface::HashFieldsFunction> hashFieldsMap;

	// Return a newly created Json object containing a command item, or NULL if the command could not be created. commandParams can be NULL if not needed, causing the resulting command to contain empty parameter fields. If commandParams is not NULL, this method becomes responsible for freeing the object when it's no longer needed.
	Json *createCommand (const SystemInterface::Prefix &prefix, const char *commandName, int commandType, Json *commandParams);

	// Populate a command's authorization prefix field using the provided values and hash functions. Returns a boolean value indicating if the field was successfully generated.
	bool setCommandAuthorization (Json *command, const StdString &authSecret, const StdString &authToken, SystemInterface::HashUpdateFunction hashUpdateFn, SystemInterface::HashDigestFunction hashDigestFn, void *hashContextPtr);

	// Return the authorization hash generated from the provided values and functions. If authToken is not provided, any available prefix auth token is used.
	StdString getCommandAuthorizationHash (Json *command, const StdString &authSecret, const StdString &authToken, SystemInterface::HashUpdateFunction hashUpdateFn, SystemInterface::HashDigestFunction hashDigestFn, void *hashContextPtr);

	// Find command data for the specified name and store fields into the provided struct. Returns a boolean value indicating if the command was found.
	bool getCommand (const StdString &name, SystemInterface::Command *command);

	// Find type data for the specified name and store Param structs into the provided list. Returns a boolean value indicating if the type was found.
	bool getType (const StdString &name, std::list<SystemInterface::Param> *destList);

	// Populate default fields in an object, as appropriate for the specified type name. Returns a boolean value indicating if the type was found.
	bool populateDefaultFields (const StdString &typeName, Json *destObject);

	// Update a hash digest using fields in an object, as appropriate for the specified type name
	void hashFields (const StdString &typeName, Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr);

	// Return a boolean value indicating if the provided fields are valid according to rules appearing in a Param list. If the fields are found to be invalid, this method sets the lastError value.
	bool fieldsValid (Json *fields, std::list<SystemInterface::Param> *paramList);

	// Parse a command JSON string and store the resulting Json object using the provided pointer. Returns a boolean value indicating if the parse was successful. If the parse fails, this method sets the lastError value.
	bool parseCommand (const StdString &commandString, Json **commandJson);

	// Return the command ID value appearing in the provided command object, or -1 if no such ID was found
	int getCommandId (Json *command);

	// Return the command name value appearing in the provided command object, or an empty string if no such name was found
	StdString getCommandName (Json *command);

	// Return the prefix.agentId value appearing in the provided command object, or an empty string if no such value was found
	StdString getCommandAgentId (Json *command);

	// Return the params.id value appearing in the provided command object, or an empty string if no such value was found
	StdString getCommandRecordId (Json *command);

	// Return the agent name value appearing in the provided AgentStatus command object, or an empty string if no such value was found
	StdString getCommandAgentName (Json *command);

	// Return the agent address value appearing in the provided AgentStatus command object, or an empty string if no such value was found
	StdString getCommandAgentAddress (Json *command);

	// Return a boolean value indicating if the provided command object is a record that holds the closed state
	bool isRecordClosed (Json *command);

	// Return a boolean value indicating if the provided string matches a Windows platform identifier
	bool isWindowsPlatform (const StdString &platform);

	// Return a SystemInterface::Prefix structure containing prefix fields from the provided command
	SystemInterface::Prefix getCommandPrefix (Json *command);

	// Get the provided command's params object and store it using the provided Json pointer. Returns a boolean value indicating if the params object was found.
	bool getCommandParams (Json *command, Json *params);

	// Return a string value from params in the provided command, or the default value if the named field wasn't found
	StdString getCommandStringParam (Json *command, const StdString &paramName, const StdString &defaultValue);
	StdString getCommandStringParam (Json *command, const char *paramName, const char *defaultValue);

	// Return a bool value from params in the provided command, or the default value if the named field wasn't found
	bool getCommandBooleanParam (Json *command, const StdString &paramName, bool defaultValue);
	bool getCommandBooleanParam (Json *command, const char *paramName, bool defaultValue);

	// Return an int number value from params in the provided command, or the default value if the named field wasn't found
	int getCommandNumberParam (Json *command, const StdString &paramName, const int defaultValue);
	int getCommandNumberParam (Json *command, const char *paramName, const int defaultValue);
	int64_t getCommandNumberParam (Json *command, const StdString &paramName, const int64_t defaultValue);
	int64_t getCommandNumberParam (Json *command, const char *paramName, const int64_t defaultValue);
	double getCommandNumberParam (Json *command, const StdString &paramName, const double defaultValue);
	double getCommandNumberParam (Json *command, const char *paramName, const double defaultValue);
	float getCommandNumberParam (Json *command, const StdString &paramName, const float defaultValue);
	float getCommandNumberParam (Json *command, const char *paramName, const float defaultValue);

	// Find the specified object item and store it in the provided Json object. Returns a boolean value indicating if the item was found.
	bool getCommandObjectParam (Json *command, const StdString &paramName, Json *destJson);
	bool getCommandObjectParam (Json *command, const char *paramName, Json *destJson);

	// Fill the provided vector with items from the specified number array, optionally clearing the list before doing so. Returns a boolean value indicating if the array was found.
	bool getCommandNumberArrayParam (Json *command, const StdString &paramName, std::vector<int> *destList, bool shouldClear = false);
	bool getCommandNumberArrayParam (Json *command, const char *paramName, std::vector<int> *destList, bool shouldClear = false);
	bool getCommandNumberArrayParam (Json *command, const StdString &paramName, std::vector<int64_t> *destList, bool shouldClear = false);
	bool getCommandNumberArrayParam (Json *command, const char *paramName, std::vector<int64_t> *destList, bool shouldClear = false);
	bool getCommandNumberArrayParam (Json *command, const StdString &paramName, std::vector<double> *destList, bool shouldClear = false);
	bool getCommandNumberArrayParam (Json *command, const char *paramName, std::vector<double> *destList, bool shouldClear = false);
	bool getCommandNumberArrayParam (Json *command, const StdString &paramName, std::vector<float> *destList, bool shouldClear = false);
	bool getCommandNumberArrayParam (Json *command, const char *paramName, std::vector<float> *destList, bool shouldClear = false);

	// Return the length of the specified array, or 0 if the array was empty or non-existent
	int getCommandArrayLength (Json *command, const StdString &paramName);
	int getCommandArrayLength (Json *command, const char *paramName);

	// Return the specified number array item, or the default value if the item wasn't found
	int getCommandNumberArrayItem (Json *command, const StdString &paramName, int index, int defaultValue);
	int64_t getCommandNumberArrayItem (Json *command, const StdString &paramName, int index, int64_t defaultValue);
	double getCommandNumberArrayItem (Json *command, const StdString &paramName, int index, double defaultValue);
	float getCommandNumberArrayItem (Json *command, const StdString &paramName, int index, float defaultValue);

	// Return the specified string array item, or the default value if the item wasn't found
	StdString getCommandStringArrayItem (Json *command, const StdString &paramName, int index, const StdString &defaultValue);
	StdString getCommandStringArrayItem (Json *command, const char *paramName, int index, const StdString &defaultValue);

	// Find the specified object array item and store it in the provided Json object. Returns a boolean value indicating if the item was found.
	bool getCommandObjectArrayItem (Json *command, const StdString &paramName, int index, Json *destJson);
	bool getCommandObjectArrayItem (Json *command, const char *paramName, int index, Json *destJson);

  static void getParams_AgentConfiguration (std::list<SystemInterface::Param> *destList);
  static void getParams_AgentContact (std::list<SystemInterface::Param> *destList);
  static void getParams_AgentStatus (std::list<SystemInterface::Param> *destList);
  static void getParams_CommandResult (std::list<SystemInterface::Param> *destList);
  static void getParams_EmptyObject (std::list<SystemInterface::Param> *destList);
  static void getParams_FindItems (std::list<SystemInterface::Param> *destList);
  static void getParams_ReportContact (std::list<SystemInterface::Param> *destList);
  static void getParams_ReportStatus (std::list<SystemInterface::Param> *destList);
  static void getParams_ShowColorFillBackground (std::list<SystemInterface::Param> *destList);
  static void getParams_ShowFileImageBackground (std::list<SystemInterface::Param> *destList);
  static void getParams_ShowResourceImageBackground (std::list<SystemInterface::Param> *destList);
  static void getParams_TaskItem (std::list<SystemInterface::Param> *destList);
  static void populateDefaultFields_AgentConfiguration (Json *destObject);
  static void populateDefaultFields_AgentContact (Json *destObject);
  static void populateDefaultFields_AgentStatus (Json *destObject);
  static void populateDefaultFields_CommandResult (Json *destObject);
  static void populateDefaultFields_EmptyObject (Json *destObject);
  static void populateDefaultFields_FindItems (Json *destObject);
  static void populateDefaultFields_ReportContact (Json *destObject);
  static void populateDefaultFields_ReportStatus (Json *destObject);
  static void populateDefaultFields_ShowColorFillBackground (Json *destObject);
  static void populateDefaultFields_ShowFileImageBackground (Json *destObject);
  static void populateDefaultFields_ShowResourceImageBackground (Json *destObject);
  static void populateDefaultFields_TaskItem (Json *destObject);
  static void hashFields_AgentConfiguration (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr);
  static void hashFields_AgentContact (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr);
  static void hashFields_AgentStatus (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr);
  static void hashFields_CommandResult (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr);
  static void hashFields_EmptyObject (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr);
  static void hashFields_FindItems (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr);
  static void hashFields_ReportContact (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr);
  static void hashFields_ReportStatus (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr);
  static void hashFields_ShowColorFillBackground (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr);
  static void hashFields_ShowFileImageBackground (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr);
  static void hashFields_ShowResourceImageBackground (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr);
  static void hashFields_TaskItem (Json *commandParams, SystemInterface::HashUpdateFunction hashUpdateFn, void *hashContextPtr);
};
#endif
