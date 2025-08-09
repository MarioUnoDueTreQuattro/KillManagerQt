#include "myqlistwidget.h"
#include "utility.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDragMoveEvent>
#include <QMimeData>

MyQListWidget::MyQListWidget(QWidget *)
{
    this->setDragDropMode(QAbstractItemView::InternalMove);
    this->setDefaultDropAction(Qt::MoveAction);
    this->setDragEnabled(true);
    //this->setAcceptDrops(true);
    this->setDropIndicatorShown(true);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
}

void MyQListWidget::dropEvent(QDropEvent *event)
{
    LOG_MSG("dropEvent");
    // Defensive: ensure drop is internal and flat
    if (event->source() != this)
    {
        qWarning("Rejected external drop: only internal flat moves allowed.");
        event->ignore();
        return;
    }
    //QModelIndex index = indexAt(event->pos());
    QListWidget::dropEvent(event);
    //event->acceptProposedAction ();
    emit itemsReordered();
    // if (!indexAt(event->pos()).isValid()) {
    // event->ignore();
    // return;
    // }
    // event->acceptProposedAction();
    // QListWidget::dropEvent(event);
}

void MyQListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    LOG_MSG("dragEnterEvent");
    if (event->source() != this)
    {
        event->ignore();
        return;
    }
    event->acceptProposedAction();
    emit reordering();
    QListWidget::dragEnterEvent(event);
}

void MyQListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    // if (!index.isValid())
    // {
    //        // Drop is in empty space — reject it
    // LOG_MSG("Drop is in empty space — reject it");
    // event->ignore();
    // return;
    // }
    // LOG_MSG("dragMoveEvent");
    //QModelIndex index = indexAt(event->pos());
    if (event->source() == this && index.isValid())
    {
        QListWidgetItem *targetItem = item(index.row());
        QListWidgetItem *draggedItem = currentItem();
        //LOG_MSG (targetItem->text () +" -> " + currentItem ()->text ());
        if (targetItem->text () == currentItem ()->text ())
        {
            //LOG_MSG("Prevent dragging same text");
            event->ignore();
            return;
        }
        if (targetItem == draggedItem)
        {
            //LOG_MSG("Prevent dragging over the same item");
            event->ignore();  // Prevent dragging over the same item
            return;
        }
        if (row(targetItem) == this->row (draggedItem))
        {
            LOG_MSG("Prevent dragging over the NEXT item");
            event->ignore();  // Prevent dragging over the same item
            return;
        }
    }
    event->accept();
    //event->acceptProposedAction();
    QListWidget::dragMoveEvent(event);
}

void MyQListWidget::mouseMoveEvent(QMouseEvent *event)
{
//    QModelIndex index = indexAt(event->pos());
//    if (index.isValid ())
//    {
//        //LOG_MSG("index.isValid=true");
//        QListWidgetItem *targetItem = item(index.row());
//        targetItem-> setFlags( targetItem->flags() | Qt::ItemIsDragEnabled);
//        currentItem()-> setFlags( currentItem ()->flags() | Qt::ItemIsDragEnabled);
//    }
//    else  LOG_MSG("index.isValid=false!!!!!!!!!");
    QListWidgetItem* item = itemAt(event->pos());
    if (!item) {
       LOG_MSG("index.isValid=false!!!!!!!!!");
    }
    QListWidget::mouseMoveEvent(event);
    //LOG_MSG("");
    // QModelIndex index = indexAt(event->pos());
    // if (!index.isValid())
    // {
    //        // Mouse is in empty space — do not start drag
    // return;
    // }
    // Otherwise, proceed with default behavior
}

//void MyQListWidget::addItem(QListWidgetItem* item) {
// if (!item) {
// throw std::invalid_argument("Null QListWidgetItem passed");
// }

//        // QListWidget doesn't support hierarchy, so we just add it
// QListWidget::addItem(item);
// }

bool MyQListWidget::containsItemText(const QString& text)
{
    for (int i = 0; i < this->count(); ++i)
    {
        QListWidgetItem* item = this->item(i);
        if (item && item->text() == text)
        {
            return true;
        }
    }
    return false;
}
