# helloworld-service

A binding example for AGL

## Setup 

```bash
git clone --recursive https://github.com/iotbzh/helloworld-service
cd helloworld-service
```

## Build  for AGL

```bash
#setup your build environement
. /xdt/sdk/environment-setup-aarch64-agl-linux
#build your application
./conf.d/autobuild/agl/autobuild package
```

## Build for 'native' Linux distros (Fedora, openSUSE, Debian, Ubuntu, ...)

```bash
./conf.d/autobuild/agl/autobuild package
```

You can also use binary package from OBS: [opensuse.org/LinuxAutomotive][opensuse.org/LinuxAutomotive]

## Deploy

### AGL

```bash
export YOUR_BOARD_IP=192.168.1.X
export APP_NAME=helloworld-service
scp build/${APP_NAME}.wgt root@${YOUR_BOARD_IP}:/tmp
ssh root@${YOUR_BOARD_IP} afm-util install /tmp/${APP_NAME}.wgt
APP_VERSION=$(ssh root@${YOUR_BOARD_IP} afm-util list | grep ${APP_NAME}@ | cut -d"\"" -f4| cut -d"@" -f2)
ssh root@${YOUR_BOARD_IP} afm-util start ${APP_NAME}@${APP_VERSION}
```

## TEST

### AGL

```bash
export YOUR_BOARD_IP=192.168.1.X
export PORT=8000
ssh root@${YOUR_BOARD_IP} afb-daemon --ws-client=unix:/run/user/0/apis/ws/helloworld --port=${PORT} --token='x' -v

#On an other terminal
ssh root@${YOUR_BOARD_IP} afb-client-demo -H 127.0.0.1:${PORT}/api?token=x helloworld ping true
#or
curl http://${YOUR_BOARD_IP}:${PORT}/api/helloworld/ping?token=x
#For a nice display
curl http://${YOUR_BOARD_IP}:${PORT}/api/helloworld/ping?token=x 2>/dev/null | python -m json.tool
```

### Native Linux

For native build you need to have tools **afb-daemon**.
You can build it by your self [app-framework-binder][app-framework-binder], or use binary package from OBS: [opensuse.org/LinuxAutomotive][opensuse.org/LinuxAutomotive]

```bash
export PORT=8000
afb-daemon --port=${PORT}  --ldpaths=/opt/AGL/helloworld-service/lib/

curl http://localhost:${PORT}/api/helloworld/ping
#For a nice display
curl http://localhost:${PORT}/api/helloworld/ping 2>/dev/null | python -m json.tool

```

# Activate security

To test auth just switch the line:

```diff
 static const struct afb_verb_v2 verbs[]= {
   /*Without security*/
-  { .verb = "ping"     , .session = AFB_SESSION_NONE, .callback = pingSample  , .auth = NULL},
+  /*{ .verb = "ping"     , .session = AFB_SESSION_NONE, .callback = pingSample  , .auth = NULL},*/
   /*With security "urn:AGL:permission:monitor:public:get"*/
-  /*{ .verb = "ping"     , .session = AFB_SESSION_NONE, .callback = pingSample  , .auth = &_afb_auths_v2_monitor[1]},*/
+  { .verb = "ping"     , .session = AFB_SESSION_NONE, .callback = pingSample  , .auth = &_afb_auths_v2_monitor[1]},
   {NULL}
 };
```

And rebuild your application

[opensuse.org/LinuxAutomotive]:https://en.opensuse.org/LinuxAutomotive
[app-framework-binder]:https://gerrit.automotivelinux.org/gerrit/#/admin/projects/src/app-framework-binder