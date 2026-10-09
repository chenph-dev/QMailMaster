# QMailMaster

[![GitHub action](https://img.shields.io/github/actions/workflow/status/chenph/QMailMaster/builds_desktop_cmake.yml?style=flat-square)](https://github.com/chenph/QMailMaster/actions/workflows/builds_desktop_cmake.yml)
[![GitHub action](https://img.shields.io/github/actions/workflow/status/chenph/QMailMaster/builds_mobile_cmake.yml?style=flat-square)](https://github.com/chenph/QMailMaster/actions/workflows/builds_mobile_cmake.yml)
[![GitHub issues](https://img.shields.io/github/issues/chenph/QMailMaster.svg?style=flat-square)](https://github.com/chenph/QMailMaster/issues)
[![MIT license](https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square)](LICENSE.md)

## About

A cross-platform email client built with Qt6 / QML, supporting NetEase, Google, and QQ mail.

#### Prerequisites

You will need a C++17 compiler and Qt 6.8+.  

For macOS and iOS builds, you'll need Xcode (15+) installed.  
For Windows builds, you'll need MSVC 2022 installed.  
For Android builds, you'll need the appropriates JDK (17) SDK (28+) and NDK (28+).

> https://doc.qt.io/qt-6.8/supported-platforms.html

#### Supported platforms

- Linux (Ubuntu 22.04+)
- macOS 12+
- Windows 10+
- Android 9+
- iOS 16+

#### Building QMailMaster

```bash
$ git clone https://github.com/chenph/QMailMaster.git --recursive
$ cd QMailMaster/
$ cmake -B build/
$ cmake --build build/
```

To build for Android and iOS, you should use the qt-cmake script from your Qt installation.

```bash
$ /path/to/Qt/bin/qt-cmake -B build/
$ cmake --build build/
```

## Content

#### C++ modules

> [AppUtils](thirdparty/AppUtils/README.md) Various general purpose helpers

> [MobileUI](https://github.com/emericg/MobileUI) Interact with Android and iOS UI

#### QML component library

> [ComponentLibrary](thirdparty/ComponentLibrary/README.md) Many themed or entirely new QML components

#### Icon library

> [IconLibrary](thirdparty/IconLibrary/) Material Design icons

#### Deploy scripts

> [Linux](deploy_linux.sh) application ZIP and AppImage

> [macOS](deploy_macos.sh) application ZIP (with code signing available)

> [Windows](deploy_windows.sh) application ZIP and NSIS installer

#### GitHub CI workflows

> [Desktop (cmake)](.github/workflows/builds_desktop_cmake.yml) Linux, macOS (with notarization) and Windows workflow

> [Mobile (cmake)](.github/workflows/builds_mobile_cmake.yml) Android and iOS workflow (with code signing & store deployment)

> [Linux flatpak](.github/workflows/flatpak.yml) "on demand" workflow

## Licensing

QMailMaster is released under the terms of the [MIT license](LICENSE.md).

* [AppUtils](thirdparty/AppUtils/README.md) uses MIT license

* [MobileUI](thirdparty/MobileUI/README.md) uses MIT license

* [ComponentLibrary](thirdparty/ComponentLibrary/README.md) uses MIT license

* [IconLibrary](thirdparty/IconLibrary/) uses a combination of licenses, see [COPYING](thirdparty/IconLibrary/COPYING)
