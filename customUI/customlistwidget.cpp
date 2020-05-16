#include "customlistwidget.h"

CustomListWidget::CustomListWidget(QWidget *parent) : QListWidget(parent)
{
    // 设置垂直滚动条按像素滚动，如果使用ScrollPerItem按项滚动会感觉到卡顿
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

void CustomListWidget::mouseMoveEvent(QMouseEvent *event)
{
    // 只有点击时才触发
    if (isMousePressed)
    {
        // 根据实际需要的效果设置qt4Delta
        int qt4Delta = (event->y() - m_y) * 2;

        QWheelEvent e = QWheelEvent(event->pos(), event->globalPos(), QPoint(0, 0), QPoint(0, 0),
                                    qt4Delta, Qt::Vertical, Qt::NoButton, Qt::NoModifier);
        wheelEvent(&e);
        m_y = event->y();
    }
}

void CustomListWidget::mousePressEvent(QMouseEvent *event)
{
    m_y = event->y();
    isMousePressed = true;
    QListWidget::mousePressEvent(event);
}

void CustomListWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    isMousePressed = false;
}
