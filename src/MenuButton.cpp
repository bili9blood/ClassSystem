#include "MenuButton.h"

#include <qevent.h>
#include <qpainter.h>
#include <qtimer.h>

MenuButton::MenuButton(QPixmap icon, QString text, QWidget *parent)
    : QWidget(parent) {
  setFixedWidth(cs::settings::menuButtonWidth);
  // init labels
  m_iconLabel->setAlignment(Qt::AlignCenter);
  m_iconLabel->setPixmap(
      icon.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
  m_textLabel->setStyleSheet("color: white");
  m_textLabel->setText(text);
  m_textLabel->setFont(cs::font{.pointSize = cs::settings::smallFontSize}());
  m_textLabel->setAlignment(Qt::AlignCenter);

  // install event filter
  m_iconLabel->installEventFilter(this);
  m_textLabel->installEventFilter(this);

  // init layouts
  m_mainLayout->setMargin(5);

  m_mainLayout->addWidget(m_iconLabel);
  m_mainLayout->setStretchFactor(m_iconLabel, 6);
  m_mainLayout->addWidget(m_textLabel);
  m_mainLayout->setStretchFactor(m_textLabel, 4);
}

bool MenuButton::event(QEvent *ev) {
  if (ev->type() == QEvent::Paint) {
    QPainter painter(this);
    if (m_state == State::Clicked) {
      painter.setBrush(QColor(65, 66, 68, 245));
      painter.setPen(Qt::transparent);
      painter.drawRoundedRect(rect(), 10, 10);
    }
    if (m_state == State::Hovered) {
      painter.setBrush(QColor(43, 44, 46, 245));
      painter.setPen(Qt::transparent);
      painter.drawRoundedRect(rect(), 10, 10);
    }
    return true;
  }
  if (ev->type() == QEvent::Enter || ev->type() == QEvent::MouseButtonRelease) {
    m_state = State::Hovered;
    update();
    return true;
  }
  if (ev->type() == QEvent::MouseButtonPress ||
      ev->type() == QEvent::MouseButtonDblClick) {
    m_state = State::Clicked;
    emit clicked();
    update();
    return true;
  }
  if (ev->type() == QEvent::Leave) {
    m_state = State::Normal;
    update();
    return true;
  }
  return false;
}
