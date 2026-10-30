/*
 * Copyright (C) 2026 GXDE OS Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 */

#ifndef DREORDERLISTWIDGET_H
#define DREORDERLISTWIDGET_H

#include <QListWidget>

#include "dtkwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

/**
 * QListWidget with internal drag reordering, extended selection and a
 * built-in rename/delete context menu.
 *
 * Items must carry a stable id in Qt::UserRole so callers can resynchronise
 * their own model after reordering.
 */
class DReorderListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit DReorderListWidget(QWidget *parent = nullptr);

signals:
    void reordered();
    void deleteRequested(const QList<int> &rows);
    void renameRequested(int row);
    void externalDropAt(int row, const QMimeData *mime);

protected:
    void dropEvent(QDropEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
};

DWIDGET_END_NAMESPACE

#endif // DREORDERLISTWIDGET_H
