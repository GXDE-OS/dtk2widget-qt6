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
 */

#include <DApplication>
#include <QDebug>
#include <QMenu>
#include <QTimer>
#include <QWindow>

int main(int argc, char **argv) {
    Dtk::Widget::DApplication app(argc, argv);
    if (!QGuiApplication::platformName().startsWith(QLatin1String("wayland"))) {
        qCritical() << "Run this regression test with QT_QPA_PLATFORM=wayland";
        return 77;
    }

    QMenu polished;
    polished.addAction(QStringLiteral("Polish must not create a native popup"));
    polished.ensurePolished();
    if (polished.testAttribute(Qt::WA_WState_Created)) {
        qCritical() << "Polish created a native menu before sizing:" << polished.size();
        return 1;
    }

    QWidget parent;
    parent.setWindowTitle(QStringLiteral("DTK Wayland menu regression"));
    parent.resize(400, 200);
    parent.show();

    // Titlebar menus are unparented and begin with QWidget's 640x480 default.
    QMenu menu;
    menu.addAction(QStringLiteral("New window"));
    menu.addAction(QStringLiteral("Dark theme"));
    menu.addSeparator();
    menu.addAction(QStringLiteral("Settings"));
    menu.addAction(QStringLiteral("About"));
    menu.addAction(QStringLiteral("Exit"));
    int result = 0;

    QTimer::singleShot(400, &app, [&] {
        for (int attempt = 1; attempt <= 3; ++attempt) {
            if (attempt == 3) {
                menu.addAction(QStringLiteral("A longer action added after the first popup"));
            }
            QTimer::singleShot(250, &menu, [&] {
                const QSize expected = menu.sizeHint();
                QWindow *window = menu.windowHandle();
                qInfo() << "attempt" << attempt << "widget" << menu.size()
                        << "expected" << expected
                        << "window" << (window ? window->size() : QSize());
                if (menu.size() != expected || !window || window->size() != expected) {
                    result = 1;
                }
                menu.hide();
            });
            menu.exec(parent.mapToGlobal(QPoint(20, 30)));
        }
        app.exit(result);
    });
    return app.exec();
}
