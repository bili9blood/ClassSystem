#pragma once

#include <qevent.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qscreen.h>
#include <qwidget.h>

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
  QPixmap m_capturedImg;
  QWidget *m_drawRectWidget = new QWidget(
      this, Qt::WindowStaysOnTopHint | Qt::Tool | Qt::FramelessWindowHint);

  const QSize kScreenSize = QApplication::primaryScreen()->size();

  // functions
  QRect absRect(const QRect &r);

 protected:
  bool eventFilter(QObject *obj, QEvent *ev) override;
  void closeEvent(QCloseEvent *) override;
};