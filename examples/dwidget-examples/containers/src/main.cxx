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

#include <QApplication>
#include <QScrollArea>
#include <QVBoxLayout>
#include <DLog>
#include <DMainWindow>

#include "dapplication.h"
#include "dthememanager.h"
#include "container_widget/container_widget.hxx"
#include "main_window/main_window.hxx"

DCORE_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

int main(int argc, char* argv[]) {
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    DApplication app(argc, argv);
    app.loadTranslator();
    app.setApplicationName("DTK2 Container Widgets Example");
    app.setOrganizationName("CharOfString");
    app.setApplicationDisplayName("ContainerExample");
    app.setApplicationVersion("1.0.0");
    app.setProductIcon(QIcon::fromTheme("applications-development"));

    // 使用 gxde-qt6-integration 样式插件
    app.setStyle("dlight2");

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    MainWindow mainWindowHelper;
    mainWindowHelper.setWindowIcon(QIcon::fromTheme(
        "applications-development"));
    mainWindowHelper.show();

    return app.exec();
}
