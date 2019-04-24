/*
 * Copyright (C) 2016-2018 "IoT.bzh"
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>

#define AFB_BINDING_VERSION 3
#include <afb/afb-binding.h>

static afb_event_t event;

static void pingSample(afb_req_t request)
{
	static int pingcount = 0;
	int listeners = 0;

	if(afb_event_is_valid(event))
		listeners = afb_event_push(event, json_object_new_int(pingcount));

	afb_req_success_f(request, json_object_new_int(pingcount), "Ping count = %d, Event listeners = %d", pingcount, listeners);

	AFB_API_NOTICE(afbBindingV3root, "Verbosity macro at level notice invoked at ping invocation, count = %d, event listeners = %d", pingcount, listeners);

	pingcount++;
}

static void subscribeSample(afb_req_t request)
{
	// Event not valid ? then creating one.
	if(! afb_event_is_valid(event))
		event = afb_api_make_event(request->api, "PingCount");


	if(afb_req_subscribe(request, event) < 0) {
		afb_req_fail(request, "Error", "Subscription operation failed");
		return;
	}

	afb_req_success(request, NULL, NULL);

	AFB_API_NOTICE(request->api, "Verbosity macro at level notice invoked at subscribe invocation");
}

static void unsubscribeSample(afb_req_t request)
{
	if(! afb_event_is_valid(event)) {
		afb_req_fail(request, "Invalid", "Event not valid");
		return;
	}

	if(afb_req_unsubscribe(request, event) < 0) {
		afb_req_fail(request, "Error", "Unsubscription operation failed");
		return;
	}

	afb_req_success(request, NULL, NULL);
	AFB_API_NOTICE(request->api, "Verbosity macro at level notice invoked at unsubscribe invocation");
}

// testArgsSample - return success only if argument is set to {"cezam": "open"}
static void testArgsSample(afb_req_t request)
{
	json_object *tmpJ;
	json_object *res = json_object_new_object();
	json_object *queryJ = afb_req_json(request);

	json_bool success = json_object_object_get_ex(queryJ, "cezam", &tmpJ);
	if (!success) {
		afb_req_fail_f(request, "ERROR", "key cezam not found in '%s'", json_object_get_string(queryJ));
		return;
	}

	if (json_object_get_type(tmpJ) != json_type_string) {
		afb_req_fail(request, "ERROR", "key cezam not a string");
		return;
	}

	if (strncmp(json_object_get_string(tmpJ), "open", 4) == 0) {
		json_object_object_add(res, "code", json_object_new_int(123456789));
		afb_req_success(request, res, NULL);
		return;
	}

	afb_req_fail_f(request, "ERROR", "value of cezam (%s) is not the expected one.",
				   json_object_get_string(queryJ));
}

static const struct afb_auth _afb_auths_monitor[] = {
	{.type = afb_auth_Permission, .text = "urn:AGL:permission:monitor:public:set"},
	{.type = afb_auth_Permission, .text = "urn:AGL:permission:monitor:public:get"},
	{.type = afb_auth_Or, .first = &_afb_auths_monitor[1], .next = &_afb_auths_monitor[0]}
};

static const afb_verb_t verbs[] = {
	/*Without security*/
	{.verb = "ping", .session = AFB_SESSION_NONE, .callback = pingSample, .auth = NULL},

	/*With security "urn:AGL:permission:monitor:public:get"*/
	/*{ .verb = "ping"     , .session = AFB_SESSION_NONE, .callback = pingSample  , .auth = &_afb_auths_monitor[1]},*/

	{.verb = "subscribe", .session = AFB_SESSION_NONE, .callback = subscribeSample, .auth = NULL},
	{.verb = "unsubscribe", .session = AFB_SESSION_NONE, .callback = unsubscribeSample, .auth = NULL},
	{.verb = "testargs", .session = AFB_SESSION_NONE, .callback = testArgsSample, .auth = NULL},
	{NULL}
};

const afb_binding_t afbBindingExport = {
	.api = "helloworld-event",
	.specification = NULL,
	.verbs = verbs,
	.preinit = NULL,
	.init = NULL,
	.onevent = NULL,
	.userdata = NULL,
	.provide_class = NULL,
	.require_class = NULL,
	.require_api = NULL,
	.noconcurrency = 0
};
