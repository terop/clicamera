# clicamera

clicamera is a small CLI camera application. It is targeted for x86 based
computers and the Jolla smartphone. It uses the Qt framework and its multimedia
features for taking an image.

## Dependencies

* Qt framework version 5.2 (version limitation set by Sailfish OS)
* GStreamer and its good and bad plugins
* A C++ compiler supporting C++11

## Compilation

* x86 computers
  * Qt Creator: open the `clicamera.pro` file and use its build tools
  * non Qt Creator based workflow: run `qmake -o Makefile` and then execute `make`
* Sailfish OS
  * Only Qt Creator from the Sailfish OS SDK is supported

Only GCC has been tested to work but Clang may also work.

## License
See the MIT license in the LICENSE file.

Copyright (c) 2018 Tero Paloheimo
