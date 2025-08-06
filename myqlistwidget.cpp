#include "myqlistwidget.h"
#include "log_macros.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDragMoveEvent>
#include <QMimeData>

MyQListWidget::MyQListWidget(QWidget *parent)
{
}

void MyQListWidget::dropEvent(QDropEvent *event)
{
    LOG_MSG("dropEvent");
    QListWidget::dropEvent(event);
    emit itemsReordered();
//    if (!indexAt(event->pos()).isValid()) {
//         event->ignore();
//         return;
//     }
//     event->acceptProposedAction();
//     QListWidget::dropEvent(event);
}

void MyQListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    LOG_MSG("dragEnterEvent");
    QListWidget::dragEnterEvent(event);
    event->acceptProposedAction();
    emit reordering();
}

void MyQListWidget::dragMoveEvent(QDragMoveEvent *event)
{
      QModelIndex index = indexAt(event->pos());
      if (!index.isValid()) {
            // Drop is in empty space — reject it
            event->ignore();
            return;
        }
    QListWidget::dragMoveEvent(event);
    // LOG_MSG("dragMoveEvent");
    //QModelIndex index = indexAt(event->pos());
           if (event->source() == this && index.isValid()) {
               QListWidgetItem *targetItem = item(index.row());
               QListWidgetItem *draggedItem = currentItem();
               if (targetItem == draggedItem) {
                   event->ignore();  // Prevent dragging over the same item
                   return;
               }
           }
           event->accept();
           //event->acceptProposedAction();
}

void MyQListWidget::mouseMoveEvent(QMouseEvent *event) {
    QModelIndex index = indexAt(event->pos());
    if (!index.isValid()) {
        // Mouse is in empty space — do not start drag
        return;
    }

    // Otherwise, proceed with default behavior
    QListWidget::mouseMoveEvent(event);
}
