// DTK2（dtk2widget）内部使用：引导程序与 DApplication 共享的环境变量还原接口。
//
// 引导程序（dtkwidget_init.cpp 的 constructor）为了阻止加载与 libdtk6widget
// ABI 不兼容的 chameleon 样式，会临时把 QT_STYLE_OVERRIDE 设为 dlight2。
// 这个改动只应影响本进程的 Qt 初始化，不能泄漏给子进程，否则 dtk5/dtk6 等
// 子程序会选错样式；因此 DApplication 构造完成（Qt 已读取完样式）后必须调用
// restoreStyleOverride() 把它还原成本进程启动前的值。

#ifndef DTK2WIDGET_INIT_P_H
#define DTK2WIDGET_INIT_P_H

namespace Dtk {
namespace Widget {
namespace Private {

void restoreStyleOverride();

}  // namespace Private
}  // namespace Widget
}  // namespace Dtk

#endif  // DTK2WIDGET_INIT_P_H
