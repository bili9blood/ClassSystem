#include "PopupMenu.h"

#include <qdesktopservices.h>
#include <qpainter.h>
#include <qtextbrowser.h>

inline PopupMenu *leftMenu;
inline PopupMenu *rightMenu;

PopupMenu::PopupMenu(bool isOnLeft, QWidget *parent)
    : QWidget(parent,
              Qt::WindowStaysOnTopHint | Qt::Tool | Qt::FramelessWindowHint),
      m_isLeftSide(isOnLeft) {
  setWidgetTransparent(this);

  // append buttons
  m_btnsList << new MenuButton({":/img/capture.png"}, "截图", &m_btnsWidget)
             << new MenuButton({":img/table.png"}, "表格", &m_btnsWidget)
             << new MenuButton({":/img/roll-call.png"}, "随机点名",
                               &m_btnsWidget)
             << new MenuButton({":/img/help.png"}, "帮助", &m_btnsWidget);
  setWidgetTransparent(&m_btnsWidget);
  m_btnsLayout.setMargin(0);
  m_btnsLayout.setSpacing(0);
  foreach (MenuButton *btn, m_btnsList) {
    m_btnsLayout.addWidget(btn);
    connect(btn, &MenuButton::clicked, this, &PopupMenu::onBtnClicked);
  }

  m_btnsWidget.installEventFilter(this);

  // init layouts
  m_mainLayout->addWidget(m_popMenuLabel);

  m_mainLayout->setMargin(5);
  setBtnsVisible(true);

  // 左侧
  if (isOnLeft) {
    // 旋转
    QTransform trans;
    trans.rotate(180);
    m_iconClosed = m_iconClosed.transformed(trans, Qt::SmoothTransformation);
    m_iconOpened = m_iconOpened.transformed(trans, Qt::SmoothTransformation);
  } else {  // 右侧
    rightMenu = this;
    leftMenu = new PopupMenu(true);
  }
}

PopupMenu::~PopupMenu() {
  if (this == rightMenu) delete leftMenu;
}

void PopupMenu::updateBtnsPosition() {
  const int btnsYPos = [this](int y) {
    // bound y-pos in screen
    int maxHeight =
        QApplication::primaryScreen()->size().height() - m_btnsWidget.height();
    return qBound(0, y, maxHeight);
  }(y() - m_btnsWidget.height() / 2 + height() / 2);

  if (m_isLeftSide) {
    m_btnsWidget.move(x() + width() + 10, btnsYPos);
  } else {
    m_btnsWidget.move(x() - 10 - m_btnsWidget.width(), btnsYPos);
  }
}

void PopupMenu::setBtnsVisible(bool visible) {
  if (visible) {
    QApplication::setActiveWindow(&m_btnsWidget);
    m_btnsWidget.show();
    m_popMenuLabel->setPixmap(m_iconOpened);
  } else {
    m_btnsWidget.hide();
    m_popMenuLabel->setPixmap(m_iconClosed);
  }
}

void PopupMenu::onBtnClicked() {
  auto btn = qobject_cast<MenuButton *>(sender());
  for (int i = 0; i < m_btnsList.size(); ++i) {
    if (btn == m_btnsList.at(i)) {
      switch (i) {
        case 0:  // 截图
          m_capturer.capture();
          break;

        case 1:  // 表格
          m_tableWindow.showNormal();
          m_tableWindow.activateWindow();
          break;

        case 2:  // 随机点名
          m_rollCallWindow.showNormal();
          m_rollCallWindow.activateWindow();
          break;

        case 3:  // 帮助
          QDesktopServices::openUrl({"https://class-system-docs.gitee.io/"});
          break;
      }
      break;
    }
  }
}

bool PopupMenu::eventFilter(QObject *obj, QEvent *ev) {
  if (obj == &m_btnsWidget) {
    if (ev->type() == QEvent::ActivationChange &&
        QApplication::activeWindow() != &m_btnsWidget) {
      setBtnsVisible(false);
    }
    if (ev->type() == QEvent::Paint) {
      QPainter painter(&m_btnsWidget);
      painter.setBrush(QColor(31, 33, 34, 185));
      painter.setPen(Qt::transparent);
      painter.drawRoundedRect(m_btnsWidget.rect(), 10, 10);
      return true;
    }
  }
  return false;
}

void PopupMenu::paintEvent(QPaintEvent *) {
  if (m_init) {
    const int xPos =
        m_isLeftSide ? 0
                     : QApplication::primaryScreen()->size().width() - width();
    move(xPos, y());
    setBtnsVisible(false);
    m_init = false;
  }

  QPainter painter(this);
  painter.setBrush(m_bgColor);
  painter.setPen(Qt::transparent);
  if (m_isLeftSide) {
    painter.drawRoundedRect(
        QRect(rect().topLeft() - QPoint(10, 0), rect().size() + QSize(10, 0)),
        10, 10);
  } else {
    painter.drawRoundedRect(
        QRect(rect().topLeft(), rect().size() + QSize(10, 0)), 10, 10);
  }
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
  if (m_shouldUpdateBtnsVisible) setBtnsVisible(!m_btnsWidget.isVisible());
}

void PopupMenu::enterEvent(QEvent *ev) {
  m_bgColor = {43, 44, 46, 245};
  update();
}

void PopupMenu::leaveEvent(QEvent *ev) {
  m_bgColor = {31, 33, 34, 185};
  update();
}

void PopupMenu::moveEvent(QMoveEvent *ev) {
  updateBtnsPosition();
  if (m_isLeftSide) {
    rightMenu->move(rightMenu->x(), y());
  } else {
    leftMenu->move(leftMenu->x(), y());
  }
}

void PopupMenu::showEvent(QShowEvent *ev) {
  if (!m_isLeftSide) {
    leftMenu->show();
  }
}