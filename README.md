# helloworld-service
A binding example for AGL

## Setup 

```
git clone https://github.com/iotbzh/helloworld-service
cd helloworld-service
git submodule update --recursive --init
```

## Build & Install for AGL

```
conf.d/app-templates/autobuild/agl/autobuild.mk package
```

## Build & Install for 'native' Linux distros (Fedora, openSUSE, Debian, Ubuntu, ...)

```
TODO
```

## Deploy

### AGL

```
scp package/*.wgt root@<your-board-ip>:/tmp
ssh root@<your-board-ip> afm-util install /tmp/*.wgt
ssh root@<your-board-ip> afm-util start yourappname@x.y
```

### Native Linux

TODO


