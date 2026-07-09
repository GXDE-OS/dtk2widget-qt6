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
 * The solution is from GXDE's fork of Deepin-Menu
 */

#include <QGuiApplication>
#include <QWidget>
#include <QWindow>
#include <QMargins>
#include <QHash>
#include <qpa/qplatformnativeinterface.h>

#include <LayerShellQt/Shell>
#include <LayerShellQt/Window>
#include <wayland-client.h>
#include <qpa/qplatformwindow.h>
#include <QtWaylandClient/private/qwaylandwindow_p.h>

#include "treeland-dde-shell-client-protocol.h"
#include "dmenuwayland.h"

DWIDGET_BEGIN_NAMESPACE

namespace DMenuWayland {
namespace {

bool isCurPlatWayland() {
    if (!qGuiApp) {
        return qgetenv("XDG_SESSION_TYPE") == "wayland";
    }

    return QGuiApplication::platformName().toLower()
        .contains(QLatin1String("wayland"));
}

wl_display* g_display = nullptr;
wl_registry* g_registry = nullptr;
treeland_dde_shell_manager_v1* g_ddeShellManager = nullptr;
bool g_tried = false;

QHash<QObject *, treeland_dde_shell_surface_v1 *> &shellSurfaces() {
    static QHash<QObject *, treeland_dde_shell_surface_v1 *> map;
    return map;
}

void registryGlobal(void*, wl_registry* reg, uint32_t name,
        const char* interface, uint32_t) {
    if (qstrcmp(interface, "treeland_dde_shell_manager_v1") == 0) {
        g_ddeShellManager = static_cast<treeland_dde_shell_manager_v1 *>(
            wl_registry_bind(reg, name,
                &treeland_dde_shell_manager_v1_interface, 1));
    }
}

void registryGlobalRemove(void*, wl_registry*, uint32_t) {}

const wl_registry_listener kRegistryListener = {
    registryGlobal,
    registryGlobalRemove
};

void* nativeRes(const QByteArray& name) {
    if (QPlatformNativeInterface* native_int =
            QGuiApplication::platformNativeInterface()) {
        return native_int->nativeResourceForIntegration(name);
    }

    return nullptr;
}

wl_surface* wlSurfaceOf(QWidget* widget) {
    if (!widget) {
        return nullptr;
    }

    widget->createWinId();
    QWindow* win = widget->windowHandle();
    if (!win) {
        return nullptr;
    }

    if (QPlatformNativeInterface* native_int =
            QGuiApplication::platformNativeInterface()) {
        return static_cast<wl_surface *>(native_int->nativeResourceForWindow(
            "surface", win));
    }

    return nullptr;
}

bool ensureManager() {
    if (g_tried) {
        return g_ddeShellManager != nullptr;
    }

    g_tried = true;

    g_display = static_cast<wl_display *>(nativeRes("display"));

    if (!g_display) {
        return false;
    }

    g_registry = wl_display_get_registry(g_display);
    if (!g_registry) {
        return false;
    }

    wl_registry_add_listener(g_registry, &kRegistryListener, nullptr);
    wl_display_roundtrip(g_display);
    return g_ddeShellManager != nullptr;
}

treeland_dde_shell_surface_v1* shellSurfaceFor(QWidget* widget) {
    if (!ensureManager() || !g_ddeShellManager) {
        return nullptr;
    }

    wl_surface* surface = wlSurfaceOf(widget);
    if (!surface) {
        return nullptr;
    }

    // Release old sub menu before rebuilding
    treeland_dde_shell_surface_v1* old = shellSurfaces().take(widget);
    if (old) {
        treeland_dde_shell_surface_v1_destroy(old);
    }

    treeland_dde_shell_surface_v1* ss =
        treeland_dde_shell_manager_v1_get_shell_surface(g_ddeShellManager,
            surface);

    if (!ss) {
        return nullptr;
    }

    shellSurfaces().insert(widget, ss);
    QObject::connect(widget, &QObject::destroyed, [widget]() {
        if (treeland_dde_shell_surface_v1 *s = shellSurfaces().take(widget)) {
            treeland_dde_shell_surface_v1_destroy(s);
        }
    });
    return ss;
}

}  // namespace

void initLayerShell() {
    // Aborted to use layer shell.
}

bool isLayerShellActive() {
    // Aborted to use layer shell.
    return false;
}

void setMenuLayerRole(QWidget* menu) {
    if (!menu || !isCurPlatWayland()) {
        return;
    }

    menu->createWinId();
    if (!isLayerShellActive()) {
        return;  // Now using normal xdg popup
    }

    QWindow* window = menu->windowHandle();
    if (!window) {
        return;
    }

    LayerShellQt::Window* layer = LayerShellQt::Window::get(window);
    if (!layer) {
        return;
    }

    LayerShellQt::Window::Anchors anchors;
    anchors |= LayerShellQt::Window::AnchorTop;
    anchors |= LayerShellQt::Window::AnchorLeft;
    layer->setAnchors(anchors);
    layer->setMargins(QMargins(0, 0, 0, 0));
    layer->setLayer(LayerShellQt::Window::LayerOverlay);
    layer->setExclusiveZone(0);
    layer->setKeyboardInteractivity(
        LayerShellQt::Window::KeyboardInteractivityOnDemand);
    layer->setScope(QStringLiteral("dde-shell/menu"));
}

bool placeAtCursor(QWidget* menu, int yOffset) {
    treeland_dde_shell_surface_v1* ss = shellSurfaceFor(menu);
    if (!ss) {
        return false;
    }

    // Lock the menu to the cursor position
    treeland_dde_shell_surface_v1_set_auto_placement(ss,
        static_cast<uint32_t>(yOffset));
    wl_display_flush(g_display);
    return true;
}

bool placeRelativeToWindow(QWidget* menu, int x, int y) {
    treeland_dde_shell_surface_v1* ss = shellSurfaceFor(menu);
    if (!ss) {
        return false;
    }

    treeland_dde_shell_surface_v1_set_surface_position(ss, x, y);
    wl_display_flush(g_display);
    return true;
}

void setShadowMargins(QWidget* menu, const QMargins& margins) {
    if (!menu || !isCurPlatWayland()) {
        return;
    }

    QWindow* win = menu->windowHandle();
    if (!win || !win->handle()) {
        return;
    }

    static_cast<QtWaylandClient::QWaylandWindow *>(win->handle())
        ->setCustomMargins(margins);
}

}  // namespace DMenuWayland

DWIDGET_END_NAMESPACE
