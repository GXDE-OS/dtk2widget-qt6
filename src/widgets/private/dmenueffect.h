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

#ifndef DMENUEFFECT_H
#define DMENUEFFECT_H

#include <QObject>
#include <QProxyStyle>

#include "dtkwidget_global.h"

QT_BEGIN_NAMESPACE
class QMenu;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DMenuProxyStyle : public QProxyStyle {
    Q_OBJECT

public:
    explicit DMenuProxyStyle(QStyle* baseStyle);
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option,
        QPainter* painter, const QWidget* widget = nullptr) const override;

private:
    void drawMenuDecoration(const QStyleOption* option, QPainter* painter,
        const QWidget* widget) const;
};

class DMenuEffect : public QObject {
    Q_OBJECT

public:
    static void install(QMenu* menu);

    explicit DMenuEffect(QMenu* menu);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    void setupLayerShell();
    void placeMenu();
    void updateBlur();
    void clearBlur();

    QMenu* m_menu = nullptr;
};

DWIDGET_END_NAMESPACE

#endif  // DMENUEFFECT_H
