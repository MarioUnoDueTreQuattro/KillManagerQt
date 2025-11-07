#include "clickable_label.h"

ClickableLabel::ClickableLabel(QWidget* parent)
    : QLabel(parent)
{
    // Nothing special needed in constructor
}

void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
    // Emit signal only on left button press
    if (event->button() == Qt::LeftButton)
    {
        emit leftClicked();
    }

    // Call base implementation to preserve default behavior
    QLabel::mousePressEvent(event);
}
