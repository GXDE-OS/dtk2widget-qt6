[简体中文 | [English](./README.en.md)]

<a id="readme-top"></a>

![Static Badge](https://img.shields.io/badge/DTK%E7%89%88%E6%9C%AC-2.3.6-%23007CFF?style=plastic)
![Static Badge](https://img.shields.io/badge/%E7%9B%AE%E6%A0%87Qt%E7%89%88%E6%9C%AC-6.8.2-%2341CD52?style=plastic)
![Static Badge](https://img.shields.io/badge/%E7%9B%AE%E6%A0%87%E7%B3%BB%E7%BB%9F%E7%89%88%E6%9C%AC-GXDE%2025.4-%23A81D33?style=plastic)
![Static Badge](https://img.shields.io/badge/%E6%9C%80%E6%96%B0%E7%89%88%E6%9C%AC-6.0.1-green?style=plastic)


<!-- PROJECT LOGO -->
<br />
<div align="center">
  <h3 align="center">DTK2Widget-Qt6</h3>

  <p align="center">
    <sup>（实验性）</sup> 对DTK2 Widget的Qt6移植
    <br />
    <a href="https://charofstring.cc/assets/dtk2widget-qt6_doxygen_zh/index.html"><strong>查看WIKI »</strong></a>
    <br />
    <br />
    <a href="https://gitee.com/GXDE-OS/gxde-qt6-integration">查看对应绑定</a>
    &middot;
    <a href="https://gitee.com/GXDE-OS/dtk2widget-qt6/issues">报告问题</a>
    &middot;
    <a href="https://gitee.com/GXDE-OS/dtk2widget-qt6/issues">请求新控件</a>
  </p>
</div>

> **注意**: 这不是深度（Deepin）官方的仓库，我们与Deepin无关。

<!-- TABLE OF CONTENTS -->
<details>
  <summary>目录</summary>
  <ol>
    <li>
      <a href="#关于项目">关于项目</a>
      <ul>
        <li><a href="#构建依赖">构建依赖</a></li>
      </ul>
    </li>
    <li>
      <a href="#开始使用">开始使用</a>
      <ul>
        <li><a href="#安装依赖">安装依赖</a></li>
        <li><a href="#从源码编译">从源码编译</a></li>
        <li><a href="#打包为-deb-安装包">打包为 .deb 安装包</a></li>
      </ul>
    </li>
    <li><a href="#如何使用">如何使用</a></li>
    <li><a href="#里程碑">里程碑</a></li>
    <li><a href="#贡献者">贡献者</a></li>
    <li><a href="#许可证">许可证</a></li>
    <li><a href="#联系我们">联系我们</a></li>
    <li><a href="#原README">原README</a></li>
    <li><a href="#感谢">感谢</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## 关于项目

![Containers的截图，使用此移植库](./doc/screenshots/containers.png)

喜欢DTK2的外观又想要Qt6？没问题！

这个仓库是对深度经典的DTK2的Qt6移植，我们翻译了Qt5与Qt6之间API的不同并且开发了[GXDE-Qt6-Integration](https://gitee.com/GXDE-OS/gxde-qt6-integration)扩展，使您可以同时使用Qt6与DTK2.

<p align="right">(<a href="#readme-top">回到顶部</a>)</p>



### 构建依赖
* `.deb`包构建依赖：
  * debhelper <sub>(>= 9)</sub>
  * pkg-config
* Qt6 依赖：<sub>(推荐Qt版本：6.8.2)</sub>
  * qt6-base-dev
  * qt6-base-private-dev
  * qt6-tools-dev-tools
  * qt6-multimedia-dev
  * qt6-svg-dev
  * qt6-l10n-tools
  * qt6-scxml-dev
* DTK 依赖：
  * libdtk6core-dev
  * libdtk6log-dev
  * libgsettings-qt6-dev
* X11与系统依赖：
  * libudev-dev
  * libxext-dev
  * x11proto-xext-dev
  * libxcb-util-dev
  * libxrender-dev
  * libxi-dev
  * libstartup-notification0-dev
  * libmtdev-dev
* 图形依赖：
  * libegl1-mesa-dev
  * libfontconfig1-dev
  * libfreetype6-dev
  * libglib2.0-dev
  * librsvg2-dev
* 运行时依赖：
  * [gxde-qt6integration_6.0.1-1](https://gitee.com/GXDE-OS/gxde-qt6-integration)

<p align="right">(<a href="#readme-top">回到顶部</a>)</p>



<!-- GETTING STARTED -->
## 开始使用
本项目基于 `qmake6` 构建，并通过 Debian 标准的 `debuild` 流程打包为 `.deb`。

以下步骤在 GXDE 25.3 上验证通过。

### 安装依赖

1. 安装打包工具：
   ```bash
   sudo apt update
   sudo apt install build-essential devscripts debhelper pkg-config git
   ```

2. 安装项目所需的开发依赖<sup>（详细列表见上文 [构建依赖](#构建依赖)）</sup>：
   ```bash
   sudo apt install \
       qt6-base-dev qt6-base-private-dev qt6-tools-dev-tools \
       qt6-multimedia-dev qt6-svg-dev qt6-l10n-tools qt6-scxml-dev \
       libdtk6core-dev libdtk6log-dev libgsettings-qt6-dev \
       libudev-dev libxext-dev x11proto-xext-dev libxcb-util-dev \
       libxrender-dev libxi-dev libstartup-notification0-dev libmtdev-dev \
       libegl1-mesa-dev libfontconfig1-dev libfreetype6-dev \
       libglib2.0-dev librsvg2-dev
   ```

   > **提示**：进入项目目录后，可以使用 `sudo apt build-dep .` 让 apt 根据 `debian/control` 自动解析并安装全部构建依赖。

### 从源码编译

如果你只想得到可用的库文件而**不需要**生成 `.deb` 包，可按以下步骤手动编译：

1. 克隆仓库：
   ```bash
   git clone https://gitee.com/GXDE-OS/dtk2widget-qt6.git
   cd dtk2widget-qt6
   ```

2. 创建独立的构建目录, 使用 `qmake6` 配置项目：
   ```bash
   mkdir build-qt6
   cd build-qt6
   qmake6 ../dtkwidget.pro PREFIX=/usr LIB_INSTALL_DIR=/usr/lib/$(dpkg-architecture -qDEB_HOST_MULTIARCH)
   ```

3. 编译项目：
   ```bash
   make -j$(nproc)
   ```

4. 安装到系统（可选，会写入 `/usr` 下的系统路径）：
   ```bash
   sudo make install
   ```

### 打包为 .deb 安装包

推荐使用此方式，便于安装、卸载与分发。

```bash
sudo apt install cmake fakeroot
chmod a+x ./build-deb
./build-deb -d
```

编译完成后可以使用`./build-deb -c`清理中间产物，`.deb`包可以在项目根目录的上级目录找到。

<p align="right">(<a href="#readme-top">回到顶部</a>)</p>



<!-- USAGE EXAMPLES -->
## 如何使用

要使用本库，请同时安装依赖的运行时 [gxde-qt6integration](https://gitee.com/GXDE-OS/gxde-qt6-integration)。

我们目前正在制作一个全新的示例程序。

_对于使用文档，请参阅我们的[WIKI](https://gitee.com/GXDE-OS/dtk2widget-qt6/wikis)_

<p align="right">(<a href="#readme-top">回到顶部</a>)</p>



<!-- ROADMAP -->
## 里程碑

**关联的issue**: [(Gitee) #IJJXNG](https://gitee.com/GXDE-OS/dtk2widget/issues/IJJXNG)

- [x] 修复由依赖Qt5 D-Bus引发的崩溃
- [x] 翻译Qt5/Qt6之间API差异
- [x] 编写新的Qt集成扩展
- [ ] 新增文档
- [ ] 新增示例

<p align="right">(<a href="#readme-top">返回顶部</a>)</p>



<!-- CONTRIBUTING -->
## 贡献者

如果您有兴趣对本项目做出贡献，欢迎fork本仓库并且提交PR。

如果您需要请求新控件，请提交一个新Issue。

### 本仓库的贡献者

<a href="https://github.com/GXDE-OS/dtk2widget-qt6/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=GXDE-OS/dtk2widget-qt6" />
</a>

<p align="right">(<a href="#readme-top">回到顶部</a>)</p>



<!-- LICENSE -->
## 许可证

本项目采取GNU LESSER GENERAL PUBLIC LICENSE Version 3许可。详细请阅读[LICENSE](./LICENSE)。

<p align="right">(<a href="#readme-top">返回顶部</a>)</p>



<!-- CONTACT -->
## 联系我们

要联系我们，推荐新建一个Issue并且描述遇到的问题。

<p align="right">(<a href="#readme-top">返回顶部</a>)</p>


<!-- ORIGINAL README -->
## 原README

本仓库是对深度的DTK2的fork，原README可以在[这里](./README.original.md)找到。

<p align="right">(<a href="#readme-top">返回顶部</a>)</p>


<!-- ACKNOWLEDGMENTS -->
## 感谢

感谢所有依赖到的第三方库、Best-README-Template，以及对本项目感兴趣的你。

<p align="right">(<a href="#readme-top">返回顶部</a>)</p>
