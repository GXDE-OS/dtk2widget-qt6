/*
 * Copyright (C) 2026 CharOfString <markus_verify@126.com>
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
 * ----------------------------------------------------------------------------
 * This class is exported from deepin-menu.
 */

#include <QMenu>
#include <QWindow>
#include <QSurfaceFormat>
#include <QPainter>
#include <QPainterPath>
#include <QRegion>
#include <QStyleFactory>
#include <QStyleOption>
#include <QApplication>
#include <QTimer>
#include <QPointer>
#include <QImage>
#include <QDebug>

#include "dmenueffect.h"
#include "dkwinblur.h"
#include "dapplication.h"

QT_BEGIN_NAMESPACE
void qt_blurImage(QPainter* p, QImage& blurImage, qreal radius, bool quality,
    bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

namespace {

const int kRadius = 8;
const int kShadowMargin = 30;
const int kShadowBlur = 10;
const int kShadowOffsetY = 4;

bool menuDebug() {
    static const bool on = qEnvironmentVariableIsSet("DTK_MENU_DEBUG");
    return on;
}

bool menuBlurEnabled() {
    return qgetenv("DTK_MENU_BLUR") != "0";
}

qreal menuAlpha() {
    bool ok = false;
    const double v = qEnvironmentVariable("DTK_MENU_ALPHA").toDouble(&ok);
    return ok ? v : 0.45;
}

bool menuBlurActive() {
    return menuBlurEnabled() && DKWinBlurManager::instance()->isValid();
}

QRegion roundedRegion(const QRect& rect, int radius) {
    if (radius <= 0 || rect.isEmpty()) {
        return QRegion(rect);
    }

    QRegion region(rect.adjusted(radius, 0, -radius, 0));
    region += QRegion(rect.adjusted(0, radius, 0, -radius));

    const int d = radius * 2;
    region += QRegion(rect.x(), rect.y(), d, d, QRegion::Ellipse)
        & QRect(rect.x(), rect.y(), radius, radius);
    region += QRegion(rect.right() - d + 1, rect.y(), d, d, QRegion::Ellipse)
        & QRect(rect.right() - radius + 1, rect.y(), radius, radius);
    region += QRegion(rect.x(), rect.bottom() - d + 1, d, d, QRegion::Ellipse)
        & QRect(rect.x(), rect.bottom() - radius + 1, radius, radius);
    region += QRegion(rect.right() - d + 1, rect.bottom() - d + 1, d, d,
        QRegion::Ellipse)
        & QRect(rect.right() - radius + 1, rect.bottom() - radius + 1,
            radius, radius);
    return region;
}

const char* kInstalledProperty = "_dtk_menu_effect_installed";

}  // namespace

DMenuProxyStyle::DMenuProxyStyle(QStyle* baseStyle) : QProxyStyle(baseStyle) {}

void DMenuProxyStyle::drawPrimitive(PrimitiveElement element,
        const QStyleOption* option, QPainter* painter,
        const QWidget* widget) const {
    if (element == PE_PanelMenu) {
        drawMenuDecoration(option, painter, widget);
        return;
    }

    if (element == PE_FrameMenu) {
        return;
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

// Non treeland: draw the style manually
void DMenuProxyStyle::drawMenuDecoration(const QStyleOption* option,
        QPainter* painter, const QWidget* widget) const {
    Q_UNUSED(option)

    if (!widget) {
        return;
    }

    const QRect content = widget->rect().adjusted(
        kShadowMargin, kShadowMargin, -kShadowMargin, -kShadowMargin);
    QPainterPath bgPath;
    bgPath.addRoundedRect(content, kRadius, kRadius);

    painter->setRenderHint(QPainter::Antialiasing);

    QImage shadow(widget->size(), QImage::Format_ARGB32_Premultiplied);
    shadow.fill(Qt::transparent);
    {
        QPainter sp(&shadow);
        sp.setRenderHint(QPainter::Antialiasing);
        sp.fillPath(bgPath.translated(0, kShadowOffsetY), QColor(0, 0, 0));
    }

    painter->save();
    QPainterPath outside;
    outside.addRect(widget->rect());
    outside = outside.subtracted(bgPath);
    painter->setClipPath(outside);
    painter->setOpacity(0.18);
    qt_blurImage(painter, shadow, kShadowBlur * 2.0, true, true);
    painter->restore();

    const bool blurOn = menuBlurActive();
    QColor bgColor = widget->palette().color(QPalette::Window);
    if (blurOn)
        bgColor.setAlphaF(menuAlpha());
    painter->fillPath(bgPath, bgColor);
    painter->strokePath(bgPath, QPen(QColor(0, 0, 0, 20), 1));

    painter->setClipPath(bgPath);
}

void DMenuEffect::install(QMenu* menu) {
    if (!menu || !DApplication::isWayland()) {
        return;
    }

    if (menu->property(kInstalledProperty).toBool()) {
        return;
    }

    menu->setProperty(kInstalledProperty, true);

    menu->setAttribute(Qt::WA_TranslucentBackground);
    menu->createWinId();
    if (QWindow* win = menu->windowHandle()) {
        QSurfaceFormat fmt = win->requestedFormat();
        if (fmt.alphaBufferSize() <= 0) {
            fmt.setAlphaBufferSize(8);
            win->setFormat(fmt);
        }
    }

    menu->setContentsMargins(kShadowMargin, kShadowMargin, kShadowMargin,
        kShadowMargin);

    QStyle* base = QStyleFactory::create(QStringLiteral("dlight2"));
    DMenuProxyStyle* proxy = new DMenuProxyStyle(base);
    proxy->setParent(menu);
    menu->setStyle(proxy);

    new DMenuEffect(menu);
}

DMenuEffect::DMenuEffect(QMenu* menu) : QObject(menu), m_menu(menu) {
    m_menu->installEventFilter(this);
}

bool DMenuEffect::eventFilter(QObject* watched, QEvent* event) {
    if (watched == m_menu) {
        switch (event->type()) {
        case QEvent::Show:
            compensatePosition();
            QTimer::singleShot(0, this, [this]() { updateBlur(); });
            break;
        case QEvent::Resize:
        case QEvent::Move:
            QTimer::singleShot(0, this, [this]() { updateBlur(); });
            break;
        case QEvent::Hide:
            m_posCompensated = false;
            clearBlur();
            break;
        default:
            break;
        }
    }

    return QObject::eventFilter(watched, event);
}

void DMenuEffect::compensatePosition() {
    if (m_posCompensated || !m_menu) {
        return;
    }

    m_posCompensated = true;
    m_menu->move(m_menu->pos() - QPoint(kShadowMargin, kShadowMargin));
}

void DMenuEffect::setupLayerShell() {}

void DMenuEffect::updateBlur() {
    if (!m_menu || !m_menu->isVisible()) {
        return;
    }

    QWindow* window = m_menu->windowHandle();
    if (!window) {
        return;
    }

    if (!menuBlurEnabled()) {
        clearBlur();
        return;
    }

    const QRect panel(kShadowMargin, kShadowMargin,
        m_menu->width() - 2 * kShadowMargin,
        m_menu->height() - 2 * kShadowMargin);

    if (panel.isEmpty()) {
        return;
    }

    const QRegion region = roundedRegion(panel, kRadius);

    if (menuDebug())
        qDebug() << "(DMenu) Blur updated: " << m_menu->size() << panel;

    DKWinBlurManager::instance()->setBlur(window, region);
}

void DMenuEffect::clearBlur() {
    if (!m_menu) {
        return;
    }

    if (QWindow* window = m_menu->windowHandle()) {
        DKWinBlurManager::instance()->clearBlur(window);
    }
}

DWIDGET_END_NAMESPACE
