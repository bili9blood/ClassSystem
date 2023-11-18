#pragma once

#include <qabstractnativeeventfilter.h>
#include <qboxlayout.h>
#include <qevent.h>
#include <qlabel.h>
#include <qmessagebox.h>
#include <qscreen.h>
#include <qtextbrowser.h>
#include <qtimer.h>

#include "MenuButton.h"
#include "RollCallWindow.h"
#include "ScreenCapturer.h"
#include "TableWindow.h"

class PopupMenu : public QWidget {
  friend class MainPanel;

  Q_OBJECT
 public:
  explicit PopupMenu(QWidget *parent = nullptr);

  ~PopupMenu();

 public slots:
  void onBtnClicked();

 private:
  // buttons
  QWidget m_btnsWidget = QWidget(
      nullptr, Qt::WindowStaysOnTopHint | Qt::Tool | Qt::FramelessWindowHint);
  QVBoxLayout m_btnsLayout = QVBoxLayout(&m_btnsWidget);
  QList<MenuButton *> m_btnsList;

  // popup menu
  QLabel *m_popMenuLabel = new QLabel(this);

  QHBoxLayout *m_mainLayout = new QHBoxLayout(this);

  int m_mouseStartYPos;
  bool m_init = true;
  bool m_shouldUpdateBtnsVisible = true;

  QPixmap m_iconClosed =
      QPixmap(":/img/menu.png")
          .scaled(cs::settings::popupMenuSize, Qt::KeepAspectRatio,
                  Qt::SmoothTransformation);
  QPixmap m_iconOpened =
      QPixmap(":/img/menu-opened.png")
          .scaled(cs::settings::popupMenuSize, Qt::KeepAspectRatio,
                  Qt::SmoothTransformation);

  QColor m_bgColor = {31, 33, 34, 185};

  ScreenCapturer m_capturer;
  TableWindow m_tableWindow;
  RollCallWindow m_rollCallWindow;

  // functions
  void updateBtnsPosition();
  void setBtnsVisible(bool visible);

 protected:
  bool eventFilter(QObject *obj, QEvent *ev) override;
  void paintEvent(QPaintEvent *ev) override;
  void mousePressEvent(QMouseEvent *ev) override;
  void mouseMoveEvent(QMouseEvent *ev) override;
  void mouseReleaseEvent(QMouseEvent *ev) override;
  void enterEvent(QEvent *ev) override;
  void leaveEvent(QEvent *ev) override;
  void moveEvent(QMoveEvent *ev) override;
  void showEvent(QShowEvent *ev) override;
};
