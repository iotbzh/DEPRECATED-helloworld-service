/*
 * Copyright (C) 2016, 2017 "IoT.bzh"
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

#define AFB_BINDING_VERSION 2
#include <afb/afb-binding.h>
#include <wrap-json.h>

static afb_event helloEvt;

static void pingSample(struct afb_req request)
{
	static int pingcount = 0;

	afb_req_success_f(request, json_object_new_int(pingcount), "Ping count = %d", pingcount);

	AFB_DEBUG("Helloworld Ping Count = %d", pingcount);

	if (!(pingcount % 2)) AFB_NOTICE("Helloworld Ping Count = %d", pingcount);
	if (!(pingcount % 3)) AFB_WARNING("Helloworld Ping Count = %d", pingcount);
	if (!(pingcount % 4)) AFB_ERROR("Helloworld Ping Count = %d", pingcount);

	pingcount++;
}

// testArgsSample - return success only if argument is set to {"cezam": "open"}
static void testArgsSample(struct afb_req request)
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

static void subscribe(afb_req request)
{
	int err;
	json_object *queryJ = NULL, *resultJ;

	// make query to connect to alsacore
	err = wrap_json_pack(&queryJ, "{s:s,s:i}", "devid", "default", "mode", 1);
	if (err) {
		afb_req_fail(request, "Fail-Event", "Fail to Pack Query");
		goto OnErrorExit;
	}

	// call remote API
	err = afb_service_call_sync("alsacore", "subscribe", queryJ, &resultJ);
	if (err) {
		afb_req_fail_f(request, "Fail-Event", "Fail Connect to alsacore, err=%s", json_object_get_string(resultJ));
		goto OnErrorExit;
	}

	// subscribe hello client to hello event
	afb_req_subscribe(request, helloEvt);
	if (err) {
		afb_req_fail(request, "Fail-Event", "Fail to subscribe to helloEvt");
		goto OnErrorExit;
	}

	afb_req_success(request, NULL, "Subscribe-Event");
	return;

OnErrorExit:
	return;
}

static void HelloEvt(const char *evtLabel, json_object *alsaEvtJ)
{

	// Just push Alsa event to hello client
	int nbClients = afb_event_push(helloEvt, alsaEvtJ);
	if (nbClients == 0) {
		AFB_WARNING("fail to push helloEvt (no subscriber)");
	}
}

static int HelloInit(void)
{
	helloEvt = afb_daemon_make_event("hello-Evt");

	int err = afb_daemon_require_api("alsacore", 1);
	if (err) {
		AFB_WARNING("DISPATCH-LOAD-CONFIG:REQUIRE 'alsacore' Fail");
		return 1;
	}
	return 0;
}

static const struct afb_auth _afb_auths_v2_monitor[] = {
	{.type = afb_auth_Permission, .text = "urn:AGL:permission:monitor:public:set"},
	{.type = afb_auth_Permission, .text = "urn:AGL:permission:monitor:public:get"},
	{.type = afb_auth_Or, .first = &_afb_auths_v2_monitor[1], .next = &_afb_auths_v2_monitor[0]}
};

static const struct afb_verb_v2 verbs[] = {
	/*Without security*/
	{.verb = "ping", .session = AFB_SESSION_NONE, .callback = pingSample, .auth = NULL},
	{.verb = "subscribe", .session = AFB_SESSION_NONE, .callback = subscribe, .auth = NULL},

	/*With security "urn:AGL:permission:monitor:public:get"*/
	/*{ .verb = "ping"     , .session = AFB_SESSION_NONE, .callback = pingSample  , .auth = &_afb_auths_v2_monitor[1]},*/

	{.verb = "testargs", .session = AFB_SESSION_NONE, .callback = testArgsSample, .auth = NULL},
	{NULL}
};

const struct afb_binding_v2 afbBindingV2 = {
	.api = "helloworld",
	.specification = NULL,
	.verbs = verbs,
	.preinit = NULL,
	.init = HelloInit,
	.onevent = HelloEvt,
	.noconcurrency = 0
};
