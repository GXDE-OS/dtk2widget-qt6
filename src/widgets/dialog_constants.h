/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BUTTON_CONSTANTS_H
#define BUTTON_CONSTANTS_H

#include "dtkwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

/** 
 * CONTENT_INSERT_OFFSET定义值改动说明
 * 原来的值为2,本身在Qt5下是无害的，因为在Qt5上insertWidget越界会自动追加
 * QBoxLayout::insertWidget在Qt6的行为是调用者给它的索引超过实际数量会损坏内部的QList结构
 * 后续QLayout::activate()时直接段错误
 * 也请参阅ddialog中对越界保护的说明
 * 
 * 索引0是内部的一个textLayout,应该是标题和消息用的
 * 索引1开始才是用户通过addContent/insertContent添加的控件
 * 我也不知道为什么原版会定义为2...
 * ----------------------------------------------------------------------------
 * The change of CONTENT_INSERT_OFFSET value
 * The original value is 2, which is harmless in Qt5 because insertWidget will automatically
 * append when index out of bound/range. However, in Qt6, QBoxLayout::instertWidget
 * could DAMAGE its internal QList structure when index out of bound/range, causing SIGSEGV
 * when QLayout::activate() is called later.
 * Please also refer to the boundary check in ddialog.cpp.
 *
 * Index 0: Internal textLayout, should be used for title and message.
 * Index 1 and above: User-added widgets by calling addContent/insertContent.
 * I have absolutely no idea why the original code defines it as 2 either so don't ask me...
 */

/**
 * 真是怪了，放一个我当时用的调试指令和当时的输出在这里吧
 * I felt wired either, so here is the debug command I used and its output at that time:
 *
 * LD_LIBRARY_PATH=~/Desktop/Repository/GXDE/UI/dtk2widget-qt6/build-qt6/src:$LD_LIBRARY_PATH \
 * gdb -batch \
 * -ex "set pagination off" \
 * -ex "run" \
 * -ex "bt full" \
 * --args ~/Desktop/Repository/GXDE/UI/dtk2widget-qt6/build-qt6/examples/dwidget-examples/collections/collections
 * ----------------------------------------------------------------------------
 * Thread 1 "collections" received signal SIGSEGV, Segmentation fault.
 *   0x00007ffff762ed15 in QBoxLayout::itemAt(int) const () from /lib/x86_64-linux-gnu/libQt6Widgets.so.6
 *
 *   #0   QBoxLayout::itemAt(int) const  # 这就是上面说的QBoxLayout This is the QBoxLayout mentioned above
 *   #1   QLayout::activateRecursiveHelper(QLayoutItem*)  # 递归遍历QLayoutItem Recursively iterate those QLayoutItem
 *   #2   QLayout::activateRecursiveHelper(QLayoutItem*)  # 递归 recursion
 *   #3   QLayout::activateRecursiveHelper(QLayoutItem*)  # 递归 recursion
 *   #4   QLayout::activate()  # 调用QLayout::activate() Called QLayout::activate()
 *   #5   QWidgetPrivate::setVisible(bool)
 *   #6   QDialogPrivate::setVisible(bool)
 *   #7   QDialog::exec()
 *   #8   Dtk::Widget::DDialog::exec()
 *   #9   Dtk::Widget::DApplication::handleAboutAction()  #  打开「关于」对话框 Triggerred by opening the About dialog
 * ----------------------------------------------------------------------------
 * 段错误是第三次递归时触发的，内存访问非法，应该是野指针
 * Note that the SIGSEGV is triggered at the third recursion, which is an illegal memory access
 * and should be a wild pointer.
 */

namespace DIALOG {
    const int DEFAULT_WIDTH = 380;
    const int DEFAULT_HEIGHT = 120;
    const int BORDER_SHADOW_WIDTH = 0;
    const int BORDER_RADIUS = 12;
    const int CONTENT_INSERT_OFFSET = 1;
    const int BUTTON_HEIGHT = 28;
    const int CLOSE_BUTTON_WIDTH = 27;
    const int CLOSE_BUTTON_HEIGHT = 23;
    const int ICON_LAYOUT_TOP_MARGIN = 14;
    const int ICON_LAYOUT_BOTTOM_MARGIN = 14;
    const int ICON_LAYOUT_LEFT_MARGIN = 20;
    const int ICON_LAYOUT_RIGHT_MARGIN = 20;
    const int ICON_LAYOUT_SPACING = 20;
    const int BUTTON_LAYOUT_TOP_MARGIN = 0;
    const int BUTTON_LAYOUT_BOTTOM_MARGIN = 0;
    const int BUTTON_LAYOUT_LEFT_MARGIN = 0;
    const int BUTTON_LAYOUT_RIGHT_MARGIN = 0;
}

DWIDGET_END_NAMESPACE

#endif // BUTTON_CONSTANTS_H

