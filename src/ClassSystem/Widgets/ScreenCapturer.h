#pragma once

#include <qdialogbuttonbox.h>
#include <qevent.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qscreen.h>
#include <qwidget.h>

#include "ImgPaster.h"

class ScreenCapturer : public QWidget {
  Q_OBJECT
 public:
  explicit ScreenCapturer(QWidget *parent = nullptr);
  ~ScreenCapturer();

  void capture();

 private:
  QRect m_captureRect;
  QLabel *m_imgLabel = new QLabel(this);
  QLabel *m_tipLabel = new QLabel("正在截图", this);
  QDialogButtonBox *m_btnBox = new QDialogButtonBox();
  QPixmap m_capturedImg;
  QWidget *m_drawRectWidget = new QWidget(
      this, Qt::WindowStaysOnTopHint | Qt::Tool | Qt::FramelessWindowHint);

  const QSize kScreenSize = QApplication::primaryScreen()->size();

  QList<ImgPaster *> m_pasterPool;

  // functions
  QRect absRect(const QRect &r);

 private slots:
  void cropImg();

 protected:
  bool eventFilter(QObject *obj, QEvent *ev) override;
  void closeEvent(QCloseEvent *) override;
  void keyPressEvent(QKeyEvent *) override;
};