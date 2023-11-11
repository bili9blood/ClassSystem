#pragma once

#include <qboxlayout.h>
#include <qmainwindow.h>

// tcp socket
#include <qtcpsocket.h>

// other
#include <qevent.h>
#include <qsettings.h>

#include "PopupMenu.h"

class MainPanel : public QWidget {
  Q_OBJECT

 public:
  explicit MainPanel(QWidget *parent = nullptr);

 signals:
  void updatesAvailable();

 private:
  bool m_init = true;
  QPoint m_mouseStartPoint;

  QVBoxLayout *layout_ = new QVBoxLayout(this);

  // server
  QTcpSocket *m_socket = new QTcpSocket(this);
  void initSocket();

  PopupMenu *m_menu = new PopupMenu;

  void saveGeometry();

 private slots:
  void onConnected();

 protected:
  bool nativeEvent(const QByteArray &, void *message, long *result) override;
  void paintEvent(QPaintEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void resizeEvent(QResizeEvent *ev) override;
  void moveEvent(QMoveEvent *ev) override;
  void closeEvent(QCloseEvent *ev) override;
};
