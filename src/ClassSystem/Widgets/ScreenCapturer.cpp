#include "ScreenCapturer.h"

#include <qdatetime.h>
#include <qdesktopwidget.h>
#include <qpainter.h>

ScreenCapturer::ScreenCapturer(QWidget *parent)
    : QWidget(parent,
              Qt::WindowStaysOnTopHint | Qt::Tool | Qt::FramelessWindowHint) {
  setWidgetTransparent(this);
  setWidgetTransparent(m_drawRectWidget);
  setWidgetTransparent(m_btnBox);
  m_drawRectWidget->setCursor(Qt::CrossCursor);
  m_imgLabel->setFixedSize(QApplication::primaryScreen()->size());
  m_drawRectWidget->setFixedSize(QApplication::primaryScreen()->size());
  m_tipLabel->setFont(qFont{.pointSize = 18, .weight = 300}());
  m_tipLabel->move(QApplication::primaryScreen()->size().width() / 2 -
                       m_tipLabel->width() / 2,
                   0);

  m_btnBox->setWindowFlags(windowFlags());
  m_btnBox->addButton("确认", QDialogButtonBox::AcceptRole);
  m_btnBox->addButton("取消", QDialogButtonBox::RejectRole);

  connect(m_btnBox, &QDialogButtonBox::accepted, this,
          &ScreenCapturer::cropImg);
  connect(m_btnBox, &QDialogButtonBox::rejected, this, &ScreenCapturer::close);
}

ScreenCapturer::~ScreenCapturer() {
  delete m_drawRectWidget;
  delete m_imgLabel;
  delete m_tipLabel;
  delete m_btnBox;
}

void ScreenCapturer::capture() {
  m_captureRect = {};
  m_capturedImg = QApplication::primaryScreen()->grabWindow(
      QApplication::desktop()->winId());

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

QRect ScreenCapturer::absRect(const QRect &r) {
  // clang-format off
      int top    = std::min(m_captureRect.top(), m_captureRect.bottom()),
          bottom = std::max(m_captureRect.top(), m_captureRect.bottom()),
          left   = std::min(m_captureRect.left(), m_captureRect.right()),
          right  = std::max(m_captureRect.left(), m_captureRect.right());
  // clang-format on
  return QRect(left, top, std::abs(r.width()), std::abs(r.height()));
}

void ScreenCapturer::cropImg() {
  auto fileName = "screenshots/" +
                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss") +
                  ".png";
  auto img = m_capturedImg.copy(absRect(m_captureRect));
  img.save(fileName);

  auto newPaster = new ImgPaster(nullptr, img);
  m_pasterPool << newPaster;
  newPaster->show();
  newPaster->move(cursor().pos());
  connect(newPaster, &ImgPaster::closed, [this] {
    m_pasterPool.removeOne(qobject_cast<ImgPaster *>(sender()));
  });
  close();
}

bool ScreenCapturer::eventFilter(QObject *obj, QEvent *ev) {
  if (obj == m_drawRectWidget) {
    if (ev->type() == QEvent::MouseButtonPress) {
      m_btnBox->close();
      auto e = static_cast<QMouseEvent *>(ev);
      m_captureRect.setTopLeft(e->pos());
      m_captureRect.setBottomRight(e->pos());
      return true;
    }
    if (ev->type() == QEvent::MouseMove) {
      auto e = static_cast<QMouseEvent *>(ev);
      m_captureRect.setBottomRight(e->pos());
      m_drawRectWidget->update();
      auto r = absRect(m_captureRect);
      m_btnBox->show();
      m_btnBox->move(
          std::max(r.right() - m_btnBox->width(), 0),
          std::min(r.bottom() + 5, kScreenSize.height() - m_btnBox->height()));
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
    if (ev->type() == QEvent::MouseButtonDblClick) {
      close();
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

      pen.setWidth(10);
      painter.setPen(pen);
      auto r = absRect(m_captureRect);
      painter.drawPoint(r.topLeft());
      painter.drawPoint(r.topRight());
      painter.drawPoint(r.bottomLeft());
      painter.drawPoint(r.bottomRight());
      painter.setCompositionMode(QPainter::CompositionMode_Clear);
      painter.eraseRect(m_captureRect);

      // 画一层不透明度为1的矩形，不让鼠标穿透到下方
      painter.setCompositionMode(QPainter::CompositionMode());
      painter.fillRect(m_captureRect, QColor(0, 0, 0, 1));
    }
  }
  return false;
}

void ScreenCapturer::closeEvent(QCloseEvent *) {
  m_drawRectWidget->close();
  m_btnBox->close();
  m_imgLabel->clear();
}

void ScreenCapturer::keyPressEvent(QKeyEvent *ev) {
  if (ev->key() == Qt::Key_Escape) close();
}
