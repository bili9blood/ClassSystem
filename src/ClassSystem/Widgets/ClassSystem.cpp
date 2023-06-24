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
  menuWid->addToMenu("test", QPixmap(":/img/logo.png"))
      .addToMenu("test2", QPixmap(R"(D:\Pictures\Default.jpg)"));
  menuWid->lower();
  connect(menuWid, &MenuWidget::clicked, this, &ClassSystem::onMenuClicked);
}

ClassSystem::~ClassSystem() = default;
void ClassSystem::paintEvent(QPaintEvent *ev) {
  clockWid->move((desktop->width() - clockWid->width()) / 2,
                 desktop->height() - 400 - clockWid->height());
}

bool ClassSystem::eventFilter(QObject *obj, QEvent *ev) {
  if (obj == popMenuLabel) {
    auto moveMenu = [this]() {
      menuWid->move(geometry().x() - menuWid->width(), geometry().y() + 10);
    };
    if (ev->type() == QEvent::MouseButtonPress) {
      QMouseEvent *e = static_cast<QMouseEvent *>(ev);
      if (e->button() == Qt::LeftButton)
        mStartPoint = frameGeometry().topLeft() - e->globalPos();
      menuFixedVisible = !menuFixedVisible;
      menuWid->setVisible(menuFixedVisible);
      return true;
    }
    if (ev->type() == QEvent::MouseMove) {
      QMouseEvent *e = static_cast<QMouseEvent *>(ev);
      if (e->buttons() & Qt::LeftButton)
        move(e->globalPos() + mStartPoint);
      moveMenu();
      return true;
    }
    if (ev->type() == QEvent::Enter) {
      menuWid->setVisible(true);
      moveMenu();
      return true;
    }
    if (ev->type() == QEvent::Leave) {
      if (!menuFixedVisible) {
        menuWid->setVisible(false);
      }
      return true;
    }
  }
  return false;
}

void ClassSystem::onMenuClicked(size_t menuID) { qDebug() << menuID; }