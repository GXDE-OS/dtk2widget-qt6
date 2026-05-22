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

#ifndef CURLY_MAIN_WINDOW_H
#define CURLY_MAIN_WINDOW_H

#include <DMainWindow>
#include <QStackedWidget>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

class MainWindow : public DMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void onThemeChanged();

private:
    void initTitleBar();
    void initMenu();
    void toggleTheme();

    QStackedWidget* mStackedWidget = nullptr;
    QAction* mThemeAction = nullptr;
};


#endif //CURLY_MAIN_WINDOW_H
