#ifndef MYQLISTWIDGET_H
#define MYQLISTWIDGET_H

#include <QListWidget>

class MyQListWidget : public QListWidget
{
Q_OBJECT
public:
    explicit MyQListWidget(QWidget* =nullptr);
    bool containsItemText(const QString& );
    // void addItem(QListWidgetItem* ) ;
protected:
    void dropEvent(QDropEvent* ) override;
    void dragMoveEvent(QDragMoveEvent* ) override;
    void dragEnterEvent(QDragEnterEvent* ) override;
    void mouseMoveEvent(QMouseEvent *) override;
    bool event(QEvent* event) override;
signals:
    void itemsReordered();
    void reordering();
};

#endif // MYQLISTWIDGET_H
