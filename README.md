# Next KMail

[![GitHub action](https://img.shields.io/github/actions/workflow/status/chenph-dev/next-kmail/builds_desktop_cmake.yml?style=flat-square)](https://github.com/chenph-dev/next-kmail/actions/workflows/builds_desktop_cmake.yml)
[![GitHub action](https://img.shields.io/github/actions/workflow/status/chenph-dev/next-kmail/builds_mobile_cmake.yml?style=flat-square)](https://github.com/chenph-dev/next-kmail/actions/workflows/builds_mobile_cmake.yml)
[![GitHub issues](https://img.shields.io/github/issues/chenph-dev/next-kmail.svg?style=flat-square)](https://github.com/chenph-dev/next-kmail/issues)
[![MIT license](https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square)](LICENSE.md)

## 简介

Next KMail 是一款基于 Qt6 / QML 构建的新一代跨平台邮件客户端，旨在提供现代化、高效且美观的邮件体验。

项目处于早期开发阶段，目标功能包括：

- 多账户管理（支持 IMAP / SMTP 等主流协议）
- 桌面端与移动端统一体验
- 现代化 UI，支持亮色 / 暗色主题
- 邮件搜索与过滤
- 附件管理与预览

#### 技术栈

- C++17 / Qt 6.8+ / QML
- CMake 构建系统
- 支持 Linux / macOS / Windows / Android / iOS

## 构建

#### 环境要求

- C++17 编译器
- Qt 6.8+
- macOS / iOS 构建需要 Xcode 15+
- Windows 构建需要 MSVC 2022
- Android 构建需要 JDK 17、SDK 28+、NDK 28+

> https://doc.qt.io/qt-6.8/supported-platforms.html

#### 支持平台

- Linux (Ubuntu 22.04+)
- macOS 12+
- Windows 10+
- Android 9+
- iOS 16+

#### 编译步骤

```bash
git clone https://github.com/chenph-dev/next-kmail.git --recursive
cd next-kmail/
cmake -B build/
cmake --build build/
```

Android 和 iOS 构建需使用 Qt 安装目录下的 `qt-cmake`：

```bash
/path/to/Qt/bin/qt-cmake -B build/
cmake --build build/
```

## 部署

- [Linux](deploy_linux.sh) — 应用 ZIP / AppImage
- [macOS](deploy_macos.sh) — 应用 ZIP（支持代码签名）
- [Windows](deploy_windows.sh) — 应用 ZIP / NSIS 安装包

## CI

- [Desktop (cmake)](.github/workflows/builds_desktop_cmake.yml) — Linux / macOS（含公证）/ Windows
- [Mobile (cmake)](.github/workflows/builds_mobile_cmake.yml) — Android / iOS（含代码签名与应用商店发布）
- [Linux flatpak](.github/workflows/flatpak.yml) — 按需触发

## 许可证

Next KMail 基于 [MIT 许可证](LICENSE.md) 发布。

- [AppUtils](thirdparty/AppUtils/README.md) — MIT
- [MobileUI](thirdparty/MobileUI/README.md) — MIT
- [ComponentLibrary](thirdparty/ComponentLibrary/README.md) — MIT
- [IconLibrary](thirdparty/IconLibrary/) — 多种许可证，详见 [COPYING](thirdparty/IconLibrary/COPYING)
