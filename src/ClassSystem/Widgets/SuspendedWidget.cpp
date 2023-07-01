#include "SuspendedWidget.h"

SuspendedWidget::SuspendedWidget(QWidget *parent)
    : QWidget{parent,
              Qt::WindowStaysOnTopHint | Qt::Tool | Qt::FramelessWindowHint} {
  SET_WIDGET_TRANSPARENT;
  popMenuLabel->installEventFilter(this);
  popMenuLabel->setFixedSize(100, 100);
  popMenuLabel->setPixmap(QPixmap(":/img/logo.png"));
  popMenuLabel->setPixmap(
      popMenuLabel->pixmap(Qt::ReturnByValue)
          .scaledToHeight(popMenuLabel->height(), Qt::SmoothTransformation));
  menuWid->addToMenu("退出", {":/img/exit.png"});
  connect(menuWid, &MenuWidget::clicked, this, &SuspendedWidget::onMenuClicked);
}

SuspendedWidget::~SuspendedWidget() = default;
void SuspendedWidget::paintEvent(QPaintEvent *ev) { raise(); }

bool SuspendedWidget::eventFilter(QObject *obj, QEvent *ev) {
  if (obj == popMenuLabel) {
    auto moveMenu = [this]() {
      menuWid->move(geometry().x() - menuWid->width() + 40,
                    geometry().y() + 10);
      update();
    };
    if (ev->type() == QEvent::MouseButtonPress ||
        ev->type() == QEvent::MouseButtonDblClick) {
      auto e = dynamic_cast<QMouseEvent *>(ev);
      if (e->button() == Qt::LeftButton)
        mStartPoint = frameGeometry().topLeft() - e->globalPos();
      moveMenu();
      menuWid->setVisible(!menuWid->isVisible());
      return true;
    }
    if (ev->type() == QEvent::MouseMove) {
      auto e = dynamic_cast<QMouseEvent *>(ev);
      if (e->buttons() & Qt::LeftButton) move(e->globalPos() + mStartPoint);
      menuWid->setVisible(false);
      moveMenu();
      return true;
    }
  }
  return false;
}

void SuspendedWidget::onMenuClicked(size_t menuID) {
  switch (menuID) {
    case 0:  // exit
      qApp->exit();
  }
}