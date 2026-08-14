#include "dmainwindowbackground.h"
#include <QFile>
#include <DThemeManager>
#include <QDebug>
#include <QDir>
#include "dapplication.h"

DWIDGET_USE_NAMESPACE

DMainWindowBackground::DMainWindowBackground(QMainWindow *window)
{
    setMainWindow(window);
    refresh();
    m_appName = DApplication::applicationName();
}

void DMainWindowBackground::drawInWidget(QPainter *painter)
{
    if (!m_dmainWindow || !painter) {
        // 防止空指针导致崩溃
        return;
    }
    // 只有在设置了壁纸才会渲染
    if (!m_isSetBackground) {
        return;
    }
    QString theme = DThemeManager::instance()->theme(m_dmainWindow);

    auto oldRenderHints = painter->renderHints();
    painter->setRenderHint(QPainter::Antialiasing);

    QList<BackgroundPlace> list = {
        BackgroundPlace::FullWindow,
        BackgroundPlace::Center,
        BackgroundPlace::TopCenter,
        BackgroundPlace::BottomCenter,
        BackgroundPlace::CenterLeft,
        BackgroundPlace::CenterRight,
        BackgroundPlace::TopLeft,
        BackgroundPlace::TopRight,
        BackgroundPlace::BottomLeft,
        BackgroundPlace::BottomRight
    };
    for (BackgroundPlace i: list) {
        QList<int> xy = getImageXY(DMainWindowBackground::BackgroundPlace(i));
        int x = xy[0];
        int y = xy[1];
        painter->drawImage(x, y, getImage(DMainWindowBackground::BackgroundPlace(i)));
    }

    painter->setRenderHints(oldRenderHints);

    //// 绘制半透明图层
    // 备份原来的 QPen 和 QBrush
    QPen oldPen = painter->pen();
    QBrush oldBrush = painter->brush();
    if (theme == "dark") {
        painter->setPen(QPen(QColor("#99252525")));
        painter->setBrush(QBrush(QColor("#99252525")));
    }
    else {
        painter->setPen(QPen(QColor("#99ffffff")));
        painter->setBrush(QBrush(QColor("#99ffffff")));
    }
    int windowWidth = m_dmainWindow->size().width();
    int windowHeight = m_dmainWindow->size().height();
    painter->drawRect(0, 0, windowWidth, windowHeight);
    // 还原原来的 QPen 和 QBrush
    painter->setPen(oldPen);
    painter->setBrush(oldBrush);

}

void DMainWindowBackground::setMainWindow(QMainWindow *window)
{
    m_dmainWindow = window;
}

void DMainWindowBackground::resizeImage()
{
    if (!m_dmainWindow) {
        // 防止空指针导致崩溃
        return;
    }
    // 使用逻辑尺寸进行缩放，devicePixelRatio 交由 QImage 携带，
    // 绘制时由 QPainter 在逻辑坐标系下自动完成高清缩放，
    // 避免在 Wayland 非整数缩放（如 1.25）下坐标体系不一致导致壁纸偏移。
    qreal scaleFactor = m_dmainWindow->devicePixelRatioF();
    // scaled 的目标必须是「设备像素」尺寸（逻辑尺寸 × dpr），
    // 之后再 setDevicePixelRatio(dpr)，这样 QPainter 在逻辑坐标系下
    // 绘制时实际渲染尺寸 = (width*dpr)/dpr = width，恰好铺满窗口。
    int imageWidth = m_dmainWindow->width() * scaleFactor;
    int imageHeight = m_dmainWindow->height() * scaleFactor;
    QImage image = m_imageVar[BackgroundPlace::FullWindow];
    m_backgroundResized = image.scaled(QSize(imageWidth, imageHeight),
                                       Qt::KeepAspectRatioByExpanding,
                                       Qt::SmoothTransformation);
    m_backgroundResized.setDevicePixelRatio(scaleFactor);
}

QImage DMainWindowBackground::getImage(BackgroundPlace place)
{
    if (place == BackgroundPlace::FullWindow) {
        // 拉伸图片
        return m_backgroundResized;
    }
    return m_imageVar[place];
}

QList<int> DMainWindowBackground::getImageSize(BackgroundPlace place)
{
    QList<int> size;
    QImage image = getImage(place);
    // QImage::size() 返回的是设备像素尺寸，需要除以 devicePixelRatio
    // 转换为逻辑尺寸，否则在缩放场景下尺寸会被放大。
    size << qRound(image.size().width() / image.devicePixelRatio())
         << qRound(image.size().height() / image.devicePixelRatio());
    return size;
}

QList<int> DMainWindowBackground::getImageXY(BackgroundPlace place)
{
    QList<int> xy;
    if (!m_dmainWindow) {
        // 防止空指针导致崩溃
        return xy;
    }
    QList<int> image = getImageSize(place);
    int imageWidth = image[0];
    int imageHeight = image[1];
    int windowWidth = 0;
    int windowHeight = 0;
    if (m_dmainWindow) {
        // 使用逻辑尺寸（与 getImageSize 返回的逻辑尺寸保持一致），
        // QPainter::drawImage 在逻辑坐标系下绘制，由 QImage 的
        // devicePixelRatio 处理高清缩放。
        windowWidth = m_dmainWindow->width();
        windowHeight = m_dmainWindow->height();
    }
    int x = 0, y = 0;

    //// 确定 X 轴
    // 如果为 Left（0,3,6）,则可以直接确定 X = 0
    if (place % 3 == 0) {
        x = 0;
    }
    // 如果为 Center（1,4,7）,则可以直接确定 X = (width - image.width) / 2
    if (place % 3 == 1 || place == BackgroundPlace::FullWindow) {
        x = (windowWidth - imageWidth) / 2;
    }
    // 如果为 Bottom（2,5,8）,则可以直接确定 X = width - image.width
    if (place % 3 == 2) {
        x = windowWidth - imageWidth;
    }
    //// 确定 Y 轴
    // 如果为 Top（0<=place<=2）,则可以直接确定 Y = 0
    if (place >= BackgroundPlace::TopLeft && place <= BackgroundPlace::TopRight) {
        y = 0;
    }
    // 如果为 Center（3<=place<=5）,则可以直接确定 Y = (height - image.height) / 2
    if ((place >= BackgroundPlace::CenterLeft && place <= BackgroundPlace::CenterRight) || place == 9) {
        y = (windowHeight - imageHeight) / 2;
    }
    // 如果为 Bottom（6<=place<=8）,则可以直接确定 Y = height - image.width
    if (place >= BackgroundPlace::BottomLeft && place <= BackgroundPlace::BottomRight) {
        y = windowHeight - imageHeight;
    }
    xy << x << y;
    return xy;
}

void DMainWindowBackground::setAppName(QString name)
{
    m_appName = name;
}

void DMainWindowBackground::setUseGlobalBackground(bool use)
{
    m_useGlobalBackground = use;
}

void DMainWindowBackground::setCustomBackgroundPath(QStringList pathList)
{
    m_customBackgroundPath = pathList;
}

QStringList DMainWindowBackground::customBackgroundPath()
{
    return m_customBackgroundPath;
}

bool DMainWindowBackground::isSetBackground()
{
    return m_isSetBackground;
}

void DMainWindowBackground::refresh()
{
    if (!m_dmainWindow) {
        // 防止空指针导致崩溃
        return;
    }
    m_isSetBackground = false;
    QString theme = DThemeManager::instance()->theme(m_dmainWindow);

    QStringList imageName = m_imageList;
    QStringList imagePath = {};
    // 清空列表
    m_imageVar.clear();
    // 获取图片路径
    //QString basePath = QDir::homePath() + "/.config/GXDE/" + m_appName + "/background-" + theme;
    QStringList basePathList;
    QStringList globalPathList = QStringList() << "/usr/share/backgrounds/GXDE/dtk2/global/background-" + theme
                             << QDir::homePath() + "/.config/GXDE/dtk2/global/background-" + theme;
    if (m_useGlobalBackground) {
        basePathList = globalPathList;
    }
    basePathList << "/usr/share/backgrounds/GXDE/dtk2/" + m_appName + "/background-" + theme
                 << QDir::homePath() + "/.config/GXDE/" + m_appName + "/background-" + theme;
    for (QString i: m_customBackgroundPath) {
        basePathList.append(i);
    }
    for (QString i: imageName) {
        QString image = basePathList[0] + "-" + i + ".png";
        for (QString j: basePathList) {
            QString tempPath = j + "-" + i + ".png";
            if (QFile::exists(tempPath)) {
                image = tempPath;
            }
        }
        imagePath.append(image);
    }
    // 加载 QImage 对象
    qreal scaleFactor = m_dmainWindow ? m_dmainWindow->devicePixelRatioF() : 1.0;
    for (int i = 0; i < imageName.count(); ++i) {
        QImage image;
        if (QFile::exists(imagePath[i])) {
            m_isSetBackground = true;
            image.load(imagePath[i]);
            if (i == 8) {
                // 如果已经设置了右下角 logo,则不重复显示
                m_showFMLogo = false;
            }
            // 为非全屏角标图片设置 devicePixelRatio 并放大，
            // 使其在逻辑坐标系下绘制时保持高清（FullWindow 由 resizeImage 处理）。
            if (i != static_cast<int>(BackgroundPlace::FullWindow) && scaleFactor != 1.0) {
                QImage scaled = image.scaled(image.size() * scaleFactor,
                                            Qt::KeepAspectRatio,
                                            Qt::SmoothTransformation);
                scaled.setDevicePixelRatio(scaleFactor);
                image = scaled;
            }
        }
        m_imageVar.append(image);
    }
    resizeImage();
    // 读取默认 logo
    //m_fmLogo = QImage(":/images/images/fm-logo.png");
}

void DMainWindowBackground::setUserBackground(ThemesType themetype, QString imagePath, BackgroundPlace place)
{
    QStringList themesTypeStr = {"light", "dark"};
    QDir dir(QDir::homePath() + "/.config/GXDE/" + m_appName);
    if (!dir.exists()) {
        dir.mkpath(dir.path());
    }
    QString backgroundPath = dir.path() + "/background-" +
            themesTypeStr[ThemesType(themetype)] + "-" + m_imageList[BackgroundPlace(place)] + ".png";
    if (QFile::exists(backgroundPath)) {
        QFile::remove(backgroundPath);
    }
    QFile::copy(imagePath, backgroundPath);
}

void DMainWindowBackground::removeUserBackground(ThemesType themetype, BackgroundPlace place)
{
    QStringList themesTypeStr = {"light", "dark"};
    QDir dir(QDir::homePath() + "/.config/GXDE/" + m_appName);
    QString backgroundPath = dir.path() + "/background-" +
            themesTypeStr[ThemesType(themetype)] + "-" + m_imageList[BackgroundPlace(place)] + ".png";
    if (!QFile::exists(backgroundPath)) {
        return;
    }
    QFile::remove(backgroundPath);
}
