#include "ClassSystem.h"

ClassSystem::ClassSystem(QWidget *parent)
    : QMainWindow{parent, Qt::WindowStaysOnTopHint | Qt::Tool |
                              Qt::FramelessWindowHint} {
  SET_WIDGET_TRANSPARENT;
  // show widgets
  clockWid->show();
}

ClassSystem::~ClassSystem() {}
void ClassSystem::paintEvent(QPaintEvent *ev) {
  clockWid->move((desktop->width() - clockWid->width()) / 2, 500);
}

void ClassSystem::mousePressEvent(QMouseEvent *ev) {
  if (ev->button() == Qt::LeftButton)
    mStartPoint = frameGeometry().topLeft() - ev->globalPos();
}
void ClassSystem::mouseMoveEvent(QMouseEvent *ev) {
  move(ev->globalPos() + mStartPoint);
}