/*
 * Copyright (C) 2026 CharOfString
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef CONTAINERWIDGET_H
#define CONTAINERWIDGET_H

#include <QWidget>

class ContainerWidget : public QWidget {
    Q_OBJECT

public:
    explicit ContainerWidget(QWidget *parent = nullptr);
};

#endif  // CONTAINERWIDGET_H
