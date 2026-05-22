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

#include <DLog>
#include <DTitlebar>
#include <DThemeManager>
#include <QIcon>
#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QMenu>
#include <QScrollArea>

#include "./main_window.hxx"
#include "../container_widget/container_widget.hxx"

DCORE_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

MainWindow::MainWindow(QWidget* parent) : DMainWindow(parent) {
    dInfo() << "Initializing main window...";
    this->setWindowTitle("DTK2Widget-Qt6 Example (Containers)");
    this->resize(800, 600);

    auto* centralWidgetGen = new QWidget(this);
    auto* layoutGen = new QHBoxLayout(centralWidgetGen);
    layoutGen->setSpacing(0);
    layoutGen->setContentsMargins(0, 0, 0, 0);
    centralWidgetGen->setLayout(layoutGen);

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    ContainerWidget* containerWidget = new ContainerWidget(scrollArea);
    scrollArea->setWidget(containerWidget);

    layoutGen->addWidget(scrollArea);
    setCentralWidget(centralWidgetGen);

    initTitleBar();
    initMenu();
    dInfo() << "Main window has been initialized!!";
}


void MainWindow::initTitleBar() {
    dInfo() << "Initializing title bar...";

    auto* windowLogo = new QLabel(this);
    windowLogo->setPixmap(QIcon::fromTheme("applications-development")
        .pixmap(20, 20));
    windowLogo->setFixedSize(QSize(20, 20));

    auto* windowTitle = new QLabel(this);
    windowTitle->setText("DTK2Widget-Qt6 Example (Containers)");
    windowTitle->setObjectName("app_window_title");
    windowTitle->setStyleSheet(
        "#app_window_title { font-size: 14px; }");

    auto* titleBarLayout = new QHBoxLayout();
    titleBarLayout->setContentsMargins(0, 0, 0, 0);
    titleBarLayout->setSpacing(8);
    titleBarLayout->addSpacing(8);
    titleBarLayout->addWidget(windowLogo);
    titleBarLayout->addSpacing(8);
    titleBarLayout->addWidget(windowTitle);
    titleBarLayout->addStretch();

    auto* customizedTitleBarGen = new QWidget(this);
    customizedTitleBarGen->setLayout(titleBarLayout);

    auto* titleBarPtr = this->titlebar();
    titleBarPtr->setCustomWidget(customizedTitleBarGen, Qt::AlignLeft);

    dInfo() << "Title bar has been initialized!!";
}

void MainWindow::initMenu() {
    dInfo() << "Initializing menu...";

    auto* menuGen = new QMenu(this);
    mThemeAction = new QAction("切换深色模式 · Toggle Dark Theme", menuGen);
    connect(mThemeAction, &QAction::triggered, this, &MainWindow::toggleTheme);
    menuGen->addAction(mThemeAction);

    auto* titleBarPtr = this->titlebar();
    titleBarPtr->setMenu(menuGen);

    dInfo() << "Title bar has been initialized!!";
}

void MainWindow::toggleTheme() {
    QString currentTheme = DThemeManager::instance()->theme(this);
    QString newTheme = (currentTheme == "dark") ? "light" : "dark";
    DThemeManager::instance()->setTheme(this, newTheme);
}

void MainWindow::onThemeChanged() {
    dInfo() << "Dark/light sheme has been changed!!";
}
