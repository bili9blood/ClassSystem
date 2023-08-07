#pragma once

#include <qboxlayout.h>
#include <qevent.h>
#include <qlabel.h>
#include <qtimer.h>

class ImgPaster : public QWidget {
  Q_OBJECT

 public:
  ImgPaster(QWidget *parent, QPixmap pixmap);
  // ~ImgPaster();

 private:
  QLabel *m_labelPixmap = new QLabel(this);
  QLabel *m_labelSize = new QLabel(this);
  int m_size = 0;
  QPixmap m_pixmap;
  QPoint m_mouseStartPos;
  QTimer m_timer;
  QSize m_initalSize;
  QHBoxLayout *m_mainLayout = new QHBoxLayout(this);

 signals:
  void closed();

 private slots:
  void hideSizeLabel();

 protected:
  void mousePressEvent(QMouseEvent *ev);
  void mouseDoubleClickEvent(QMouseEvent *ev);
  void mouseMoveEvent(QMouseEvent *ev);
  void wheelEvent(QWheelEvent *ev);
  void keyPressEvent(QKeyEvent *ev);
  void closeEvent(QCloseEvent *ev);
};
