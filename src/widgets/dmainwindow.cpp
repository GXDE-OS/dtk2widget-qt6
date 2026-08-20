/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include "dmainwindow.h"
#include "dplatformwindowhandle.h"
#include "dapplication.h"
#include "dtitlebar.h"

#include "private/dmainwindow_p.h"
#include "private/dapplication_p.h"
#include "private/ddeshellmanager.h"

#include <QKeySequence>
#include <QShortcut>
#include <QWindow>
#include <QMouseEvent>
#include <QDebug>
#include <QPlatformSurfaceEvent>

#ifdef Q_OS_MAC
#include "osxwindow.h"
#endif

/// shadow
#define SHADOW_COLOR_NORMAL QColor(0, 0, 0, 255 * 35/100)
#define SHADOW_COLOR_ACTIVE QColor(0, 0, 0, 255 * 60/100)

DWIDGET_BEGIN_NAMESPACE

namespace {

class DNoTitleBarFilter : public QObject {
public:
    explicit DNoTitleBarFilter(QWidget* window)
        : QObject(window), m_window(window) {}

protected:
    bool eventFilter(QObject* watched, QEvent* event) override {
        if (watched != m_window)
            return QObject::eventFilter(watched, event);

        const QEvent::Type type = event->type();

        if (type == QEvent::Show) {
            QWindow* handle = m_window->windowHandle();
            if (!handle) {
                // 窗口句柄已失效，需要重新应用。
                m_applied = false;
                m_appliedHandle = nullptr;
                return QObject::eventFilter(watched, event);
            }

            if (!m_applied || m_appliedHandle != handle) {
                // Show 事件仅做一次性（或窗口句柄变化后）的初始化应用。
                m_applied = true;
                m_appliedHandle = handle;
                DDdeShellManager::instance()->setNoTitleBar(handle, true);
            }
        } else if (type == QEvent::PlatformSurface) {
            auto* surf = static_cast<QPlatformSurfaceEvent*>(event);
            // 只对“surface 新建完成”事件重新应用 NoTitleBar。
            // Markdown 预览中的 QWebEngineView 会让顶层窗口的 wl_surface
            // 经历“销毁 -> 重建”的过程：SurfaceAboutToBeDestroyed 时旧
            // surface 即将失效，此时若仍去取 wl_surface 并向其绑定
            // dde_shell_surface，会拿到 stale 指针并在已销毁的 surface 上
            // 触发 Wayland 协议错误，导致 compositor 断开连接（程序崩溃）。
            if (surf->surfaceEventType()
                    == QPlatformSurfaceEvent::SurfaceAboutToBeDestroyed) {
                // surface 即将销毁：清理与旧 surface 绑定的缓存，避免后续
                // 在已失效的 stale 指针上调用 Wayland 协议接口而崩溃。
                if (QWindow* handle = m_window->windowHandle()) {
                    DDdeShellManager::instance()->resetSurface(handle);
                }
                m_applied = false;
                m_appliedHandle = nullptr;
            } else {
                QWindow* handle = m_window->windowHandle();
                if (!handle) {
                    m_applied = false;
                    m_appliedHandle = nullptr;
                    return QObject::eventFilter(watched, event);
                }

                // setNoTitleBar 内部每次都会重新获取当前 wl_surface，幂等。
                DDdeShellManager::instance()->setNoTitleBar(handle, true);
                m_applied = true;
                m_appliedHandle = handle;
            }
        }
        return QObject::eventFilter(watched, event);
    }

private:
    QWidget* m_window = nullptr;
    bool m_applied = false;
    QWindow* m_appliedHandle = nullptr;
};

}  // namespace

DMainWindowPrivate::DMainWindowPrivate(DMainWindow *qq)
    : DObjectPrivate(qq)
{
    titlebar = new DTitlebar(qq);
    background = new DMainWindowBackground(NULL);
    if (DApplication::isDXcbPlatform()) {
        handle = new DPlatformWindowHandle(qq, qq);
        qq->setMenuWidget(titlebar);
    } else {
        qq->setMenuWidget(titlebar);
#ifdef Q_OS_MAC
        OSX::HideWindowTitlebar(qq->winId());
#else
        // Wayland 下不设置 setEmbedMode 以便正确显示右上角的关闭按钮
        if (!DApplication::isWayland()) {
            titlebar->setEmbedMode(true);
        }

#endif
    }
}

void DMainWindowPrivate::init()
{
    D_Q(DMainWindow);

    const DApplication *dapp = qobject_cast<DApplication *>(qApp);
    if (dapp) {
        q->setWindowTitle(dapp->productName());
    } else {
        q->setWindowTitle(qApp->applicationDisplayName());
    }

    if (handle) {
        q->connect(handle, &DPlatformWindowHandle::borderColorChanged, q, &DMainWindow::borderColorChanged);
        q->connect(handle, &DPlatformWindowHandle::borderWidthChanged, q, &DMainWindow::borderWidthChanged);
        q->connect(handle, &DPlatformWindowHandle::clipPathChanged, q, &DMainWindow::clipPathChanged);
        q->connect(handle, &DPlatformWindowHandle::frameMarginsChanged, q, &DMainWindow::frameMarginsChanged);
        q->connect(handle, &DPlatformWindowHandle::frameMaskChanged, q, &DMainWindow::frameMaskChanged);
        q->connect(handle, &DPlatformWindowHandle::shadowColorChanged, q, &DMainWindow::shadowColorChanged);
        q->connect(handle, &DPlatformWindowHandle::shadowOffsetChanged, q, &DMainWindow::shadowOffsetChanged);
        q->connect(handle, &DPlatformWindowHandle::shadowRadiusChanged, q, &DMainWindow::shadowRadiusChanged);
        q->connect(handle, &DPlatformWindowHandle::windowRadiusChanged, q, &DMainWindow::windowRadiusChanged);
        q->connect(handle, &DPlatformWindowHandle::translucentBackgroundChanged, q, &DMainWindow::translucentBackgroundChanged);
        q->connect(handle, &DPlatformWindowHandle::enableSystemMoveChanged, q, &DMainWindow::enableSystemMoveChanged);
        q->connect(handle, &DPlatformWindowHandle::enableSystemResizeChanged, q, &DMainWindow::enableSystemResizeChanged);
        q->connect(handle, &DPlatformWindowHandle::enableBlurWindowChanged, q, &DMainWindow::enableBlurWindowChanged);
        q->connect(handle, &DPlatformWindowHandle::autoInputMaskByClipPathChanged, q, &DMainWindow::autoInputMaskByClipPathChanged);
        q->connect(handle, &DPlatformWindowHandle::enableWindowBackgroundChanged, q, &DMainWindow::enableWindowBackgroundChanged);

        if (!handle->isEnableNoTitlebar(q->windowHandle())) {
            q->connect(qApp, &QGuiApplication::focusWindowChanged, q, [q] {
                if (q->isActiveWindow())
                {
                    q->setShadowColor(SHADOW_COLOR_ACTIVE);
                } else
                {
                    q->setShadowColor(SHADOW_COLOR_NORMAL);
                }
            });
        }
    }

    if (!help && DApplicationPrivate::isUserManualExists()) {
        help = new QShortcut(QKeySequence(Qt::Key_F1), q);
        help->setContext(Qt::ApplicationShortcut);
        QObject::connect(help, &QShortcut::activated,
        q, [ = ]() {
            DApplication *dapp = qobject_cast<DApplication *>(qApp);
            if (dapp) {
                dapp->handleHelpAction();
            }
        });
    }

    // 仅在 Wayland 下使用
    // TODO: mouseReleaseEvent 事件依旧存在问题: https://bbs.deepin.org.cn/zh/post/279273
    if (DApplication::isWayland()) {
        titlebar->setDMainWindow(q);
        // 通过事件过滤器设置 NoTitleBar，这样用旧头文件编译的子类也一样生效
        q->installEventFilter(new DNoTitleBarFilter(q));
    }
    background->setMainWindow(q);
}

/*!
 * \class DMainWindow
 * \brief The DMainWindow class provides a main application window.
 *
 * A main window provides a framework for building an application's user
 * interface. DMainWindow has its own layout compared to QMainWindow,
 * it has only title bar and content area, simpler and cleaner.
 *
 * Developers can provide customized title bar and content to make the
 * application rich functional.
 */

/*!
 * \brief DMainWindow::DMainWindow constructs an instance of DMainWindow
 * \param parent is passed to QMainWindow construtor.
 */
DMainWindow::DMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , DObject(*new DMainWindowPrivate(this))
{
    d_func()->init();

    // 默认启用背景
    setEnableWindowBackground(0);
    background()->setUseGlobalBackground(1);

    background()->setMainWindow(this);
    background()->refresh();

    titlebar()->setDMainWindow(this);
}

/**/
DMainWindowBackground *DMainWindow::background() const
{
    D_DC(DMainWindow);

    return d->background;
}

/*!
 * \brief DMainWindow::titlebar
 * \return a DTitlebar instance used by the main window.
 */
DTitlebar *DMainWindow::titlebar() const
{
    D_DC(DMainWindow);

    return d->titlebar;
}

/*!
 * \brief DMainWindow::isDXcbWindow
 * \return Whether this window is dxcb backended.
 *
 * Many features like blurred background and window clipping are supported
 * only if the window is using the dxcb Qt platform plugin.
 */
bool DMainWindow::isDXcbWindow() const
{
    D_DC(DMainWindow);

    return d->handle;
}

/*!
 * \property DMainWindow::windowRadius
 * \brief This property holds the radius of the main window.
 */
int DMainWindow::windowRadius() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return 0;
    }

    return d->handle->windowRadius();
}

/*!
 * \property DMainWindow::borderWidth
 * \brief This property holds the width of the main window's border.
 */
int DMainWindow::borderWidth() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return 0;
    }

    return d->handle->borderWidth();
}

/*!
 * \property DMainWindow::borderColor
 * \brief This property holds the color of the main window's border.
 */
QColor DMainWindow::borderColor() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return QColor();
    }

    return d->handle->borderColor();
}

/*!
 * \property DMainWindow::shadowRadius
 * \brief This property holds the shadow radius of the main widnow.
 */
int DMainWindow::shadowRadius() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return 0;
    }

    return d->handle->shadowRadius();
}

/*!
 * \property DMainWindow::shadowOffset
 * \brief This property holds the offset applied on the window shadow.
 */
QPoint DMainWindow::shadowOffset() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return QPoint();
    }

    return d->handle->shadowOffset();
}

/*!
 * \property DMainWindow::shadowColor
 * \brief This property holds the color of the window shadow.
 */
QColor DMainWindow::shadowColor() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return QColor();
    }

    return d->handle->shadowColor();
}

/*!
 * \property DMainWindow::clipPath
 * \brief This property holds the custom QPainterPath to be used to clip the window.
 *
 * By default DMainWindow is clipped as a corner-rounded rectangle, but you can
 * supply a custom QPainterPath to do custom shaped window.
 *
 * \see DMainWindow::frameMask
 */
QPainterPath DMainWindow::clipPath() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return QPainterPath();
    }

    return d->handle->clipPath();
}

/*!
 * \property DMainWindow::frameMask
 * \brief This property holds the mask to be applied on the window.
 *
 * For better clip quality, for example antialiasing, use property
 * DMainWindow::clipPath instead.
 */
QRegion DMainWindow::frameMask() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return QRegion();
    }

    return d->handle->frameMask();
}

QMargins DMainWindow::frameMargins() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return QMargins();
    }

    return d->handle->frameMargins();
}

/*!
 * \property DMainWindow::translucentBackground
 * \brief This property holds whether the window has translucent background.
 */
bool DMainWindow::translucentBackground() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return false;
    }

    return d->handle->translucentBackground();
}

/*!
 * \brief DMainWindow::enableSystemResize
 * \return This property holds whether the window can be resized by the user.
 *
 * The default value of this property is true.
 *
 * You can set this property to false and implement the resize polizy of this
 * window by you self.
 */
bool DMainWindow::enableSystemResize() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return false;
    }

    return d->handle->enableSystemResize();
}

/*!
 * \property DMainWindow::enableSystemMove
 * \brief This property holds whether the window can be moved by the user.
 *
 * The default value of this property is true.
 *
 * You can set this property to false and choose the effective area to drag and move.
 */
bool DMainWindow::enableSystemMove() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return false;
    }

    return d->handle->enableSystemResize();
}

void DMainWindow::refreshBackground()
{
    D_DC(DMainWindow);

    // 重新载入背景图跟 dxcb 无关，别再拿 handle 当门槛
    if (!d->background) {
        return;
    }
    d->background->refresh();
    this->update();
}

/*!
 * \property DMainWindow::enableBlurWindow
 * \brief This property holds whether the window background is blurred.
 */
bool DMainWindow::enableBlurWindow() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return false;
    }

    return d->handle->enableBlurWindow();
}

/*!
 * \property DMainWindow::autoInputMaskByClipPath
 * \brief This property holds whether the user input is masked by the clip path.
 *
 * Sometimes you may want to handle events happening in the areas that are
 * visually clipped by the setting DMainWindow::clipPath.
 *
 * The default value of this property is true.
 */
bool DMainWindow::autoInputMaskByClipPath() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return false;
    }

    return d->handle->autoInputMaskByClipPath();
}

bool DMainWindow::enableWindowBackground() const
{
    D_DC(DMainWindow);

    // 以 dxcb 的窗口属性为准（那边可能被平台插件改过），没有 handle 就用本地的值
    if (d->handle) {
        return d->handle->enableWindowBackground();
    }

    return d->enableWindowBackground;
}

void DMainWindow::setWindowRadius(int windowRadius)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setWindowRadius(windowRadius);
}

void DMainWindow::setBorderWidth(int borderWidth)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setBorderWidth(borderWidth);
}

void DMainWindow::setBorderColor(const QColor &borderColor)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setBorderColor(borderColor);
}

void DMainWindow::setShadowRadius(int shadowRadius)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setShadowRadius(shadowRadius);
}

void DMainWindow::setShadowOffset(const QPoint &shadowOffset)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setShadowOffset(shadowOffset);
}

void DMainWindow::setShadowColor(const QColor &shadowColor)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setShadowColor(shadowColor);
}

void DMainWindow::setClipPath(const QPainterPath &clipPath)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setClipPath(clipPath);
}

void DMainWindow::setFrameMask(const QRegion &frameMask)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setFrameMask(frameMask);
}

void DMainWindow::setTranslucentBackground(bool translucentBackground)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setTranslucentBackground(translucentBackground);
}

void DMainWindow::setEnableSystemResize(bool enableSystemResize)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setEnableSystemResize(enableSystemResize);
}

void DMainWindow::setEnableSystemMove(bool enableSystemMove)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setEnableSystemMove(enableSystemMove);
}

void DMainWindow::setEnableBlurWindow(bool enableBlurWindow)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setEnableBlurWindow(enableBlurWindow);
}

void DMainWindow::setAutoInputMaskByClipPath(bool autoInputMaskByClipPath)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setAutoInputMaskByClipPath(autoInputMaskByClipPath);
}

void DMainWindow::setEnableWindowBackground(bool background)
{
    D_D(DMainWindow);

    d->enableWindowBackground = background;

    // handle 只有 dxcb 下才有；这个窗口属性也只有 dxcb 平台插件会读。
    // 其余的事情（标题栏菜单项、重新载入背景图）在 Wayland 下同样要做。
    if (d->handle) {
        d->handle->setEnableWindowBackground(background);
    }

    titlebar()->setDMainWindow(this);
    if (d->background) {
        d->background->refresh();
    }
    update();
}

#ifdef Q_OS_MAC
void DMainWindow::setWindowFlags(Qt::WindowFlags type)
{
    QMainWindow::setWindowFlags(type);
    OSX::HideWindowTitlebar(winId());
}
#endif

DMainWindow::DMainWindow(DMainWindowPrivate &dd, QWidget *parent)
    : QMainWindow(parent)
    , DObject(dd)
{
    d_func()->init();

    // 默认启用背景
    setEnableWindowBackground(0);
    background()->setUseGlobalBackground(1);

    background()->setMainWindow(this);
    background()->refresh();

    titlebar()->setDMainWindow(this);
}

void DMainWindow::showEvent(QShowEvent* event) {
    // NoTitleBar 的设置已移到 DNoTitleBarFilter（见文件开头）：子类若是用旧头文件
    // 编译的，这里的重写根本不会被调用到。
    QMainWindow::showEvent(event);
}

void DMainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    background()->resizeImage();
}

void DMainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    QPainter painter;
    painter.begin(this);
    background()->drawInWidget(&painter);
    painter.end();
}

DWIDGET_END_NAMESPACE
