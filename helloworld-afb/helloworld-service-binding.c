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

static void ping(struct afb_req request, json_object *jresp, const char *tag)
{
	static int pingcount = 0;
	json_object *query = afb_req_json(request);
	afb_req_success_f(request, jresp, "Ping Binder Daemon tag=%s count=%d query=%s", tag, ++pingcount, json_object_to_json_string(query));
}

static void pingSample (struct afb_req request)
{
	ping(request, json_object_new_string ("Make the World great again!"), "pingSample");
	AFB_NOTICE("Verbosity macro at level notice invoked at ping invocation");
}

static const struct afb_auth _afb_auths_v2_monitor[] = {
    { .type = afb_auth_Permission, .text = "urn:AGL:permission:monitor:public:set" },
    { .type = afb_auth_Permission, .text = "urn:AGL:permission:monitor:public:get" },
    { .type = afb_auth_Or, .first = &_afb_auths_v2_monitor[1], .next = &_afb_auths_v2_monitor[0] }
};

static const struct afb_verb_v2 verbs[]= {
  /*Without security*/
  { .verb = "ping"     , .session = AFB_SESSION_NONE, .callback = pingSample  , .auth = NULL},
  /*With security "urn:AGL:permission:monitor:public:get"*/
  /*{ .verb = "ping"     , .session = AFB_SESSION_NONE, .callback = pingSample  , .auth = &_afb_auths_v2_monitor[1]},*/
  {NULL}
};

const struct afb_binding_v2 afbBindingV2 = {
	.api = "helloworld",
	.specification = NULL,
	.verbs = verbs,
	.preinit = NULL,
	.init = NULL,
	.onevent = NULL,
	.noconcurrency = 0
};

