/*
 * Copyright (C) 2026 GXDE OS Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 */

#include "dreorderlistwidget.h"

#include <QContextMenuEvent>
#include <QDropEvent>
#include <QMenu>
#include <QMimeData>

DWIDGET_BEGIN_NAMESPACE

DReorderListWidget::DReorderListWidget(QWidget *parent)
    : QListWidget(parent)
{
    setAcceptDrops(true);
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    setDefaultDropAction(Qt::MoveAction);
    setDropIndicatorShown(true);
    setDragDropOverwriteMode(false);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void DReorderListWidget::dropEvent(QDropEvent *event)
{
    if (event->source() == this) {
        const QModelIndex dropIndex = indexAt(event->position().toPoint());
        const int to = dropIndex.isValid() ? dropIndex.row() : count();
        const int from = currentRow();
        if (from >= 0 && to >= 0 && to != from && to != from + 1) {
            QListWidgetItem *item = takeItem(from);
            const int target = to > from ? to - 1 : to;
            insertItem(target, item);
            setCurrentRow(target);
            Q_EMIT reordered();
        }
        event->acceptProposedAction();
        return;
    }

    const QModelIndex index = indexAt(event->position().toPoint());
    Q_EMIT externalDropAt(index.isValid() ? index.row() : -1, event->mimeData());
    event->acceptProposedAction();
}

void DReorderListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    const QModelIndex index = indexAt(event->pos());
    QMenu menu(this);
    QAction *rename = index.isValid() ? menu.addAction(tr("Rename")) : nullptr;
    QAction *remove = menu.addAction(tr("Delete selected"));
    QAction *chosen = menu.exec(event->globalPos());
    if (!chosen) {
        return;
    }
    if (chosen == rename) {
        Q_EMIT renameRequested(index.row());
        return;
    }
    if (chosen == remove) {
        QList<int> rows;
        for (const QModelIndex &selected : selectionModel()->selectedRows()) {
            rows.append(selected.row());
        }
        if (rows.isEmpty() && index.isValid()) {
            rows.append(index.row());
        }
        if (!rows.isEmpty()) {
            Q_EMIT deleteRequested(rows);
        }
    }
}

DWIDGET_END_NAMESPACE
