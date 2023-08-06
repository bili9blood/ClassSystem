#include "PopupMenu.h"

#include <qpainter.h>

PopupMenu::PopupMenu(QWidget *parent)
    : QWidget{parent,
              Qt::WindowStaysOnTopHint | Qt::Tool | Qt::FramelessWindowHint} {
  setWidgetTransparent(this);

  m_popMenuLabel->setPixmap(QPixmap(":/img/menu.png"));
  // init layouts
  m_mainLayout->addWidget(m_popMenuLabel);
  m_mainLayout->addLayout(m_btnLayout);

  m_mainLayout->setMargin(5);
}

void PopupMenu::paintEvent(QPaintEvent *) {
  if (m_init) {
    move(QApplication::primaryScreen()->size().width() - width(), y());
    m_init = false;
  }

  QPainter painter(this);
  painter.setBrush(QColor(31, 33, 34, 185));
  painter.setPen(Qt::transparent);
  painter.drawRoundedRect(QRect(rect().topLeft(), rect().size() + QSize(10, 0)),
                          10, 10);
}

bool PopupMenu::eventFilter(QObject *obj, QEvent *ev) { return false; }

void PopupMenu::mousePressEvent(QMouseEvent *ev) {
  m_mouseStartYPos = frameGeometry().y() - ev->globalY();
}
void PopupMenu::mouseMoveEvent(QMouseEvent *ev) {
  if (ev->buttons() & Qt::LeftButton)
    move(x(), ev->globalY() + m_mouseStartYPos);
}