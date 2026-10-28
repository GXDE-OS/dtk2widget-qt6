// DTK2的Qt6移植的「运行时引导程序」

// 当libdtk2widget.so.6被映射到进程中，例如使用此库的程序的main开始初始化时，阻止Qt加载系统的chameleon样式插件（此时系统会加载基于libdtk6widget的chameleon，与我们ABI不兼容），阻止后我们加载我们自己修改的dstyleplugin-gxde-qt6（暂定名称，即自己的样式插件，基于旧版chameleon移植）

// 这里的环境变量设置为overwrite=0，允许外部的环境变量或者-style flag覆盖设置

// 由于环境变量会遗传给子进程，引导程序在改写 QT_STYLE_OVERRIDE 前会把原值记录下来，
// 由 DApplication 构造函数在 Qt 初始化完成后调用 Private::restoreStyleOverride() 还原，
// 避免 dtk5/dtk6 等子程序被强制套上 dlight2。

// TODO: 不优雅，后期需要改为更优雅的实现，目前只保证能运行

#include "dtkwidget_init_p.h"

#include <stdlib.h>

#include <string>

namespace {

struct OriginalStyleOverride
{
    bool isSet = false;
    std::string value;
};

OriginalStyleOverride g_originalStyleOverride;
bool g_styleCaptured = false;

__attribute__((constructor(101)))

void dtk2widget_qt6_bootstrap()
{
    // 记录进入本库之前的值（可能未设置，也可能是用户/应用显式设置的）
    if (const char *value = getenv("QT_STYLE_OVERRIDE")) {
        g_originalStyleOverride.isSet = true;
        g_originalStyleOverride.value = value;
    }
    g_styleCaptured = true;

    setenv("QT_STYLE_OVERRIDE", "dlight2", 0);
}

}  // namespace

namespace Dtk {
namespace Widget {
namespace Private {

void restoreStyleOverride()
{
    if (!g_styleCaptured) {
        return;
    }

    // 仅当当前值仍是引导程序写入的 dlight2 时才还原，避免覆盖应用自己后来的设置
    const char *current = getenv("QT_STYLE_OVERRIDE");
    if (!current || std::string(current) != "dlight2") {
        return;
    }

    if (g_originalStyleOverride.isSet) {
        setenv("QT_STYLE_OVERRIDE", g_originalStyleOverride.value.c_str(), 1);
    } else {
        unsetenv("QT_STYLE_OVERRIDE");
    }
}

}  // namespace Private
}  // namespace Widget
}  // namespace Dtk
