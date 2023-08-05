#include "SuspendedWidget.h"

SuspendedWidget::SuspendedWidget(QWidget *parent)
    : QWidget{parent,
              Qt::WindowStaysOnTopHint | Qt::Tool | Qt::FramelessWindowHint} {
  setWidgetTransparent(this);
}

SuspendedWidget::~SuspendedWidget() = default;
void SuspendedWidget::paintEvent(QPaintEvent *ev) { raise(); }

bool SuspendedWidget::eventFilter(QObject *obj, QEvent *ev) { return false; }

void SuspendedWidget::onMenuClicked(size_t menuID) {}