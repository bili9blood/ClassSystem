#pragma once

#include <qboxlayout.h>
#include <qevent.h>
#include <qlabel.h>
#include <qtimer.h>

class ImgPaster : public QWidget {
  Q_OBJECT

 public:
  ImgPaster(QWidget *parent, QPixmap pixmap);

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
  void mousePressEvent(QMouseEvent *ev) override;
  void mouseDoubleClickEvent(QMouseEvent *ev) override;
  void mouseMoveEvent(QMouseEvent *ev) override;
  void wheelEvent(QWheelEvent *ev) override;
  void keyPressEvent(QKeyEvent *ev) override;
  void closeEvent(QCloseEvent *ev) override;
};
