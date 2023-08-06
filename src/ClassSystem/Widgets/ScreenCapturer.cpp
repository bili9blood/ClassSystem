#include "ScreenCapturer.h"

#include <qdatetime.h>
#include <qdesktopwidget.h>
#include <qpainter.h>

ScreenCapturer::ScreenCapturer(QWidget *parent)
    : QWidget(parent,
              Qt::WindowStaysOnTopHint | Qt::Tool | Qt::FramelessWindowHint) {
  setWidgetTransparent(this);
  setWidgetTransparent(m_drawRectWidget);
  setCursor(Qt::CrossCursor);
  m_imgLabel->setFixedSize(QApplication::primaryScreen()->size());
  m_drawRectWidget->setFixedSize(QApplication::primaryScreen()->size());
  m_tipLabel->setFont(qFont{.pointSize = 18, .weight = 300}());
  m_tipLabel->move(QApplication::primaryScreen()->size().width() / 2 -
                       m_tipLabel->width() / 2,
                   0);
}

void ScreenCapturer::capture() {
  m_captureRect = {};
  m_capturedImg = QApplication::primaryScreen()->grabWindow(
      QApplication::desktop()->winId());
  auto fileName = "screenshots/" +
                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss") +
                  ".png";

  // show tip
  m_tipLabel->setStyleSheet(
      "color: white;\n"
      "background-color: black;\n");
  m_tipLabel->show();

  // show captured picture
  m_imgLabel->setPixmap(m_capturedImg);
  m_imgLabel->show();

  m_tipLabel->raise();

  showFullScreen();

  m_drawRectWidget->update();
  m_drawRectWidget->showFullScreen();
  m_drawRectWidget->raise();

  m_drawRectWidget->installEventFilter(this);
}

bool ScreenCapturer::eventFilter(QObject *obj, QEvent *ev) {
  if (obj == m_drawRectWidget) {
    if (ev->type() == QEvent::MouseButtonPress) {
      auto e = static_cast<QMouseEvent *>(ev);
      m_captureRect.setTopLeft(e->pos());
      m_captureRect.setBottomRight(e->pos());
      return true;
    }
    if (ev->type() == QEvent::MouseMove) {
      auto e = static_cast<QMouseEvent *>(ev);
      m_captureRect.setBottomRight(e->pos());
      m_drawRectWidget->update();
      return true;
    }
    if (ev->type() == QEvent::MouseButtonRelease) {
      auto e = static_cast<QMouseEvent *>(ev);
      m_captureRect.setBottomRight(e->pos());
      if (m_captureRect.isNull()) {
        close();
        return true;
      }
      return true;
    }
    if (ev->type() == QEvent::KeyPress) {
      auto e = static_cast<QKeyEvent *>(ev);
      if (e->key() == Qt::Key_Escape) close();
      return true;
    }
    if (ev->type() == QEvent::Paint) {
      QPainter painter(m_drawRectWidget);
      QPen pen;
      pen.setColor({2, 185, 246});
      pen.setWidth(2);
      painter.setPen(pen);
      const QColor kUnSelBgColor = {0, 0, 0, 170};
      painter.fillRect(rect(), kUnSelBgColor);
      painter.drawRect(m_captureRect);

      // clang-format off
      int top    = std::min(m_captureRect.top(), m_captureRect.bottom()) - 1,
          bottom = std::max(m_captureRect.top(), m_captureRect.bottom()) + 2,
          left   = std::min(m_captureRect.left(), m_captureRect.right()) - 1,
          right  = std::max(m_captureRect.left(), m_captureRect.right()) + 2;
      // clang-format on

      pen.setWidth(10);
      painter.setPen(pen);
      painter.drawPoint(left, top);
      painter.drawPoint(left, bottom);
      painter.drawPoint(right, top);
      painter.drawPoint(right, bottom);
      painter.setCompositionMode(QPainter::CompositionMode_Clear);
      painter.eraseRect(m_captureRect);
      // qDebug() << m_captureRect;
    }
  }
  return false;
}
void ScreenCapturer::closeEvent(QCloseEvent *) {
  m_drawRectWidget->close();
  delete m_imgLabel;
  m_imgLabel = new QLabel(this);
}