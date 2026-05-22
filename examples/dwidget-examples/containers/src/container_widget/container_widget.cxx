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

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "dbackgroundgroup.h"
#include "dblursurface.h"
#include "dcardwidget.h"
#include "ddocumenttabbar.h"
#include "dfloatingmessage.h"
#include "dfloatingwidget.h"
#include "dframe.h"
#include "dbuttonlist.h"
#include "dhoverbutton.h"
#include "dicontextbutton.h"
#include "dswitchbutton.h"
#include "dtabbedstackwidget.h"
#include "dtypographylabel.h"

#include "container_widget.hxx"

DWIDGET_USE_NAMESPACE

ContainerWidget::ContainerWidget(QWidget* parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(16);

    // 标题
    DTypographyLabel* title = new DTypographyLabel("DTK2 Container Widgets",
        DTypographyLabel::Title, this);
    title->setEmphasis(true);
    mainLayout->addWidget(title);

    // DFrame示例
    DFrame* frame = new DFrame(this);
    frame->setMinimumHeight(60);
    QVBoxLayout* frameLayout = new QVBoxLayout(frame);
    frameLayout->addWidget(new QLabel("DFrame", frame));
    mainLayout->addWidget(frame);

    // DCardWidget示例
    DCardWidget* card = new DCardWidget(this);
    card->setMinimumHeight(60);
    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->addWidget(new QLabel("DCardWidget", card));
    mainLayout->addWidget(card);

    // DBackgroundGroup示例
    DBackgroundGroup* group = new DBackgroundGroup(new QVBoxLayout, this);
    group->setMinimumHeight(100);
    group->layout()->addWidget(new QLabel("DBackgroundGroup #1", group));
    group->layout()->addWidget(new QLabel("DBackgroundGroup #2", group));
    mainLayout->addWidget(group);

    // DTypographyLabel示例
    DCardWidget* typoCard = new DCardWidget(this);
    QVBoxLayout* typoLayout = new QVBoxLayout(typoCard);
    typoLayout->addWidget(new DSectionTitle("DTypographyLabel", typoCard));
    typoLayout->addWidget(new DTypographyLabel("LargeTitle",
        DTypographyLabel::LargeTitle, typoCard));
    typoLayout->addWidget(new DTypographyLabel("Title",
        DTypographyLabel::Title, typoCard));
    typoLayout->addWidget(new DTypographyLabel("Body", DTypographyLabel::Body,
        typoCard));
    DTypographyLabel *caption = new DTypographyLabel("Caption",
        DTypographyLabel::Caption, typoCard);
    caption->setSecondary(true);
    typoLayout->addWidget(caption);
    mainLayout->addWidget(typoCard);

    // Buttons示例
    DCardWidget* btnCard = new DCardWidget(this);
    QVBoxLayout* btnLayout = new QVBoxLayout(btnCard);
    btnLayout->addWidget(new DSectionTitle("Buttons", btnCard));
    QHBoxLayout* btnRow = new QHBoxLayout;

    DHoverButton* hoverBtn = new DHoverButton(btnCard);
    hoverBtn->setNormalIcon(QIcon::fromTheme("go-previous"));
    hoverBtn->setHoverIcon(QIcon::fromTheme("go-next"));
    hoverBtn->setFixedSize(32, 32);

    DIconTextButton* iconTextBtn = new DIconTextButton(QIcon::fromTheme(
        "document-open"), "Open", btnCard);
    DIconButton* iconBtn = new DIconButton(QIcon::fromTheme(
        "view-refresh-symbolic"), btnCard);
    iconBtn->setCircleEnabled(true);
    iconBtn->setFixedSize(32, 32);
    iconBtn->setIconSize(QSize(16, 16));

    btnRow->addWidget(hoverBtn);
    btnRow->addWidget(iconTextBtn);
    btnRow->addWidget(iconBtn);
    btnRow->addStretch();
    btnLayout->addLayout(btnRow);
    mainLayout->addWidget(btnCard);

    // DTabbedStackWidget示例
    DCardWidget* tabCard = new DCardWidget(this);
    QVBoxLayout* tabLayout = new QVBoxLayout(tabCard);
    tabLayout->addWidget(new DSectionTitle("DTabbedStackWidget", tabCard));

    DTabbedStackWidget* tabbedStack = new DTabbedStackWidget(tabCard);
    tabbedStack->addPage(new QLabel("Page 1", tabCard), "Home");
    tabbedStack->addPage(new QLabel("Page 2", tabCard), "Tools");
    tabbedStack->addPage(new QLabel("Page 3", tabCard), "Settings");
    tabLayout->addWidget(tabbedStack);
    mainLayout->addWidget(tabCard);

    // DDocumentTabBar示例
    DCardWidget* docCard = new DCardWidget(this);
    QVBoxLayout* docLayout = new QVBoxLayout(docCard);
    docLayout->addWidget(new DSectionTitle("DDocumentTabBar", docCard));

    DDocumentTabBar* docTabBar = new DDocumentTabBar(docCard);
    docTabBar->addDocument("doc1", "notes.txt");
    docTabBar->addDocument("doc2", "report.md");
    docTabBar->setDocumentModified(1, true);
    docTabBar->setFixedHeight(36);
    docLayout->addWidget(docTabBar);

    // 文档计数器，利用 DDocumentTabBar 自带的 + 按钮添加标签页
    int* docCounter = new int(2);
    static const QStringList docNames = {
        "readme.md", "config.ini", "script.js",
        "style.css", "data.json", "index.html"
    };

    QObject::connect(docTabBar, &DTabBar::tabAddRequested, docCard,
        [docTabBar, docCounter]() {
            const QString name = docNames[(*docCounter) % docNames.size()];
            const QString id = QString("doc%1").arg((*docCounter) + 1);
            docTabBar->addDocument(id, name);
            ++(*docCounter);
        });

    QObject::connect(docTabBar, &DTabBar::tabCloseRequested, docCard,
        [docTabBar](int index) {
            docTabBar->closeDocument(index);
        });

    mainLayout->addWidget(docCard);

    // DFloatingMessage示例
    DCardWidget* msgCard = new DCardWidget(this);
    QVBoxLayout* msgLayout = new QVBoxLayout(msgCard);
    msgLayout->addWidget(new DSectionTitle("DFloatingMessage", msgCard));

    DFloatingMessage* msg = new DFloatingMessage(
        DFloatingMessage::ResidentType, msgCard);
    msg->setIcon(QIcon::fromTheme("dialog-information"));
    msg->setMessage("Resident message example");
    msg->setActionWidget(new QPushButton("OK", msg));
    msgLayout->addWidget(msg);
    mainLayout->addWidget(msgCard);

    // DBlurSurface示例
    DCardWidget* blurCard = new DCardWidget(this);
    QVBoxLayout* blurLayout = new QVBoxLayout(blurCard);
    blurLayout->addWidget(new DSectionTitle("DBlurSurface", blurCard));

    DBlurSurface* blur = new DBlurSurface(blurCard);
    blur->setMinimumHeight(60);
    QVBoxLayout* blurInner = new QVBoxLayout(blur);
    blurInner->addWidget(new QLabel("Blur surface panel", blur));
    blurLayout->addWidget(blur);
    mainLayout->addWidget(blurCard);

    // DFloatingWidget示例
    DCardWidget* floatCard = new DCardWidget(this);
    QVBoxLayout* floatLayout = new QVBoxLayout(floatCard);
    floatLayout->addWidget(new DSectionTitle("DFloatingWidget", floatCard));

    DFloatingWidget* floating = new DFloatingWidget(floatCard);
    QWidget* floatContent = new QWidget(floating);
    QVBoxLayout* floatInner = new QVBoxLayout(floatContent);
    floatInner->addWidget(new QLabel("Floating widget content", floatContent));
    floating->setWidget(floatContent);
    floating->setFixedHeight(50);
    floatLayout->addWidget(floating);
    mainLayout->addWidget(floatCard);

    mainLayout->addStretch();
}
