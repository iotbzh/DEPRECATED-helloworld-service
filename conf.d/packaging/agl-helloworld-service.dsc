Format: 1.0
Source: agl-helloworld-service
Binary: agl-helloworld-service-bin
Architecture: any
Version: 2.0-0
Maintainer: Stephane Desneux <sdx@iot.bzh>
Standards-Version: 3.8.2
Homepage: https://github.com/iotbzh/helloworld-service
Build-Depends: debhelper (>= 5),
 cmake,
 gcc,
 g++,
 libjson-c-dev ,
 libsystemd-dev  (>= 222),
 agl-app-framework-binder-dev ,
 libmicrohttpd-dev  (>= 0.9.55)
Debtransform-Tar: agl-helloworld-service_1.0.orig.tar.gz
