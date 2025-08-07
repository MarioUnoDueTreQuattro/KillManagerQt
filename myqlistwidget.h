#ifndef MYQLISTWIDGET_H
#define MYQLISTWIDGET_H

#include <QListWidget>

class MyQListWidget : public QListWidget
{
Q_OBJECT
public:
    explicit MyQListWidget(QWidget* parent = nullptr);
    void dropEvent(QDropEvent* ) override;
    void dragMoveEvent(QDragMoveEvent* );
    void dragEnterEvent(QDragEnterEvent* ) override;
    void mouseMoveEvent(QMouseEvent *) override;
    bool containsItemText(const QString& );
    // void addItem(QListWidgetItem* ) ;
signals:
    void itemsReordered();
    void reordering();
};

#endif // MYQLISTWIDGET_H
