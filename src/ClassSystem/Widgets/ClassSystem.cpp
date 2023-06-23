#include "ClassSystem.h"

ClassSystem::ClassSystem(QWidget *parent)
    : QMainWindow{parent, Qt::WindowStaysOnTopHint | Qt::Tool |
                              Qt::FramelessWindowHint} {
  SET_WIDGET_TRANSPARENT;
  // show widgets
  clockWid->show();
  popMenuLabel->installEventFilter(this);
  popMenuLabel->setFixedSize(100, 100);
  popMenuLabel->setPixmap(QPixmap(":/img/logo.png"));
  actionsWid->installEventFilter(this);
  actionsWid->lower();
}

ClassSystem::~ClassSystem() {}
void ClassSystem::paintEvent(QPaintEvent *ev) {
  clockWid->move((desktop->width() - clockWid->width()) / 2, 500);
}

bool ClassSystem::eventFilter(QObject *obj, QEvent *ev) {
  if (obj == popMenuLabel) {
    if (ev->type() == QEvent::MouseButtonPress) {
      QMouseEvent *e = static_cast<QMouseEvent *>(ev);
      if (e->button() == Qt::LeftButton)
        mStartPoint = frameGeometry().topLeft() - e->globalPos();
      return true;
    }
    if (ev->type() == QEvent::MouseMove) {
      QMouseEvent *e = static_cast<QMouseEvent *>(ev);
      if (e->buttons() & Qt::LeftButton)
        move(e->globalPos() + mStartPoint);
      return true;
    }
    if (ev->type() == QEvent::Enter) {
      return true;
    }
    if (ev->type() == QEvent::Leave) {
      if (!actionsFixedVisible) {
      }
      return true;
    }
  }
  return false;
}