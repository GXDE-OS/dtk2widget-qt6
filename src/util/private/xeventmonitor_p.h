#ifndef XEVENTMONITOR_P_H
#define XEVENTMONITOR_P_H

// 请阅读doc/DBus.md

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtDBus/QDBusAbstractInterface>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusPendingReply>

class __XEventMonitor : public QDBusAbstractInterface
{
    Q_OBJECT

public:
    static inline const char *staticInterfaceName()
    { return "com.deepin.api.XEventMonitor"; }

    explicit __XEventMonitor(const QString &service, const QString &path,
                             const QDBusConnection &connection, QObject *parent = nullptr)
        : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
    {
    }

    ~__XEventMonitor() override = default;

public Q_SLOTS:
    inline QDBusPendingReply<QString> RegisterArea(int x1, int y1, int x2, int y2, int flags)
    {
        QList<QVariant> args;
        args << QVariant::fromValue(x1) << QVariant::fromValue(y1)
             << QVariant::fromValue(x2) << QVariant::fromValue(y2)
             << QVariant::fromValue(flags);
        return asyncCallWithArgumentList(QStringLiteral("RegisterArea"), args);
    }

    inline QDBusPendingReply<QString> RegisterFullScreen()
    {
        return asyncCallWithArgumentList(QStringLiteral("RegisterFullScreen"), {});
    }

    inline QDBusPendingReply<bool> UnregisterArea(const QString &key)
    {
        QList<QVariant> args;
        args << QVariant::fromValue(key);
        return asyncCallWithArgumentList(QStringLiteral("UnregisterArea"), args);
    }

Q_SIGNALS:
    void ButtonPress(int flag, int x, int y, const QString &key);
    void ButtonRelease(int flag, int x, int y, const QString &key);
    void CursorMove(int x, int y, const QString &key);
    void KeyPress(const QString &keyname, int x, int y, const QString &key);
    void KeyRelease(const QString &keyname, int x, int y, const QString &key);
};

namespace com {
namespace deepin {
namespace api {
typedef ::__XEventMonitor XEventMonitor;
}
}
}

#endif // XEVENTMONITOR_P_H
