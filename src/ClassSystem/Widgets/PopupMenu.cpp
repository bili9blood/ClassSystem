#include "PopupMenu.h"

#include <qpainter.h>

PopupMenu::PopupMenu(QWidget *parent)
    : QWidget{parent,
              Qt::WindowStaysOnTopHint | Qt::Tool | Qt::FramelessWindowHint} {
  setWidgetTransparent(this);

  // append buttons
  m_btnsList << new MenuButton({":/img/capture.png"}, "截图", m_btnsWidget);
  setWidgetTransparent(m_btnsWidget);
  m_btnsLayout->setMargin(0);
  m_btnsLayout->setSpacing(0);
  foreach (MenuButton *btn, m_btnsList) {
    m_btnsLayout->addWidget(btn);
    connect(btn, &MenuButton::clicked, this, &PopupMenu::onBtnClicked);
  }

  m_btnsWidget->installEventFilter(this);

  // init layouts
  m_mainLayout->addWidget(m_popMenuLabel);

  m_mainLayout->setMargin(5);
  setBtnsVisible(true);

  connect(&m_timerClose, &QTimer::timeout, [this] { setBtnsVisible(false); });
  m_timerClose.setSingleShot(true);
}

void PopupMenu::updateBtnsPosition() {
  m_btnsWidget->move(x() - 10 - m_btnsWidget->width(),
                     y() - m_btnsWidget->height() / 2 + height() / 2);
}
void PopupMenu::setBtnsVisible(bool visible) {
  if (visible) {
    m_btnsWidget->show();
    m_popMenuLabel->setPixmap(m_iconOpened);
  } else {
    m_btnsWidget->hide();
    m_popMenuLabel->setPixmap(m_iconClosed);
  }
}

void PopupMenu::onBtnClicked() {
  auto btn = qobject_cast<MenuButton *>(sender());
  for (int i = 0; i < m_btnsList.size(); ++i) {
    if (btn == m_btnsList.at(i)) {
      switch (i) {
        case 0:  // capture screen
          m_capturer.capture();
          break;
      }
      break;
    }
  }
}

bool PopupMenu::eventFilter(QObject *obj, QEvent *ev) {
  if (obj == m_btnsWidget) {
    if (ev->type() == QEvent::Paint) {
      QPainter painter(m_btnsWidget);
      painter.setBrush(QColor(31, 33, 34, 185));
      painter.setPen(Qt::transparent);
      painter.drawRoundedRect(m_btnsWidget->rect(), 10, 10);
      return true;
    }
    if (ev->type() == QEvent::Enter) {
      m_timerClose.stop();
      return true;
    }
    if (ev->type() == QEvent::Leave) {
      m_timerClose.start(3000);
      return true;
    }
  }
  return false;
}

void PopupMenu::paintEvent(QPaintEvent *) {
  if (m_init) {
    move(QApplication::primaryScreen()->size().width() - width(), y());
    setBtnsVisible(false);
    m_init = false;
  }

  QPainter painter(this);
  painter.setBrush(m_bgColor);
  painter.setPen(Qt::transparent);
  painter.drawRoundedRect(QRect(rect().topLeft(), rect().size() + QSize(10, 0)),
                          10, 10);
}
void PopupMenu::mousePressEvent(QMouseEvent *ev) {
  m_shouldUpdateBtnsVisible = true;
  m_mouseStartYPos = y() - ev->globalY();
}
void PopupMenu::mouseMoveEvent(QMouseEvent *ev) {
  m_shouldUpdateBtnsVisible = false;
  move(x(), ev->globalY() + m_mouseStartYPos);
}

void PopupMenu::mouseReleaseEvent(QMouseEvent *ev) {
  m_timerClose.start(10000);
  if (m_shouldUpdateBtnsVisible) setBtnsVisible(!m_btnsWidget->isVisible());
}

void PopupMenu::enterEvent(QEvent *ev) {
  m_bgColor = {43, 44, 46, 245};
  update();
}

void PopupMenu::leaveEvent(QEvent *ev) {
  m_bgColor = {31, 33, 34, 185};
  update();
}

void PopupMenu::moveEvent(QMoveEvent *ev) { updateBtnsPosition(); }