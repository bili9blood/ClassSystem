#pragma once

#include <qdialogbuttonbox.h>
#include <qevent.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qscreen.h>
#include <qwidget.h>

#include "ImgPaster.h"
#include "TitleWidget.h"

class ScreenCapturer : public QWidget {
  Q_OBJECT
 public:
  explicit ScreenCapturer(QWidget *parent = nullptr);

  void capture();

 private:
  QRect m_captureRect;
  QDialogButtonBox *m_btnBox = new QDialogButtonBox(this);
  TitleWidget *m_title = new TitleWidget("截图", this);
  QPixmap m_capturedImg;

  const QSize kScreenSize = QApplication::primaryScreen()->size();

  QList<ImgPaster *> m_pasterPool;

  enum class Status { Preparing, Selecting, Finish } m_status;

  // functions
  QRect absRect(const QRect &r);

 private slots:
  void cropImg();

 protected:
  void paintEvent(QPaintEvent *) override;
  void closeEvent(QCloseEvent *) override;
  void keyPressEvent(QKeyEvent *) override;
  void mouseDoubleClickEvent(QMouseEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void mouseReleaseEvent(QMouseEvent *) override;
};