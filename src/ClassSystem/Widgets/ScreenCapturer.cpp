#include "ScreenCapturer.h"

#include <qdatetime.h>
#include <qdesktopwidget.h>
#include <qpainter.h>

ScreenCapturer::ScreenCapturer(QWidget *parent)
    : QWidget(parent,
              Qt::WindowStaysOnTopHint | Qt::Tool | Qt::FramelessWindowHint) {
  move(0, 0);
  setWidgetTransparent(this);
  setWidgetTransparent(m_btnBox);
  setStyleSheet(".titleText{color: white;}");
  setCursor(Qt::CrossCursor);

  setFixedSize(QApplication::primaryScreen()->size());

  m_btnBox->setWindowFlags(windowFlags());
  m_btnBox->addButton("确认", QDialogButtonBox::AcceptRole);
  m_btnBox->addButton("取消", QDialogButtonBox::RejectRole);

  connect(m_btnBox, &QDialogButtonBox::accepted, this,
          &ScreenCapturer::cropImg);
  connect(m_btnBox, &QDialogButtonBox::rejected, this, &ScreenCapturer::close);
}

void ScreenCapturer::capture() {
  m_status = Status::Preparing;
  m_capturedImg = QApplication::primaryScreen()->grabWindow(
      QApplication::desktop()->winId());
  showFullScreen();
  repaint();
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
  newPaster->move(cursor().pos() - QPoint(img.width(), img.height()));
  connect(newPaster, &ImgPaster::closed, [this] {
    m_pasterPool.removeOne(qobject_cast<ImgPaster *>(sender()));
  });
  repaint();
  close();
}

void ScreenCapturer::paintEvent(QPaintEvent *) {
  const QColor kUnSelBgColor = {0, 0, 0, 170};
  QPainter painter(this);
  if (m_status == Status::Finish) {
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.eraseRect(rect());
    return;
  }
  painter.drawPixmap(0, 0, m_capturedImg);
  if (m_status == Status::Preparing) {
    painter.fillRect(rect(), kUnSelBgColor);
    return;
  }

  // Status::Painting
  QPen pen;
  pen.setColor({2, 185, 246});
  pen.setWidth(2);
  painter.setPen(pen);
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
  painter.drawPixmap(r.topLeft(), m_capturedImg.copy(r));
}

void ScreenCapturer::closeEvent(QCloseEvent *) {
  repaint();
  m_status = Status::Finish;
  m_captureRect = {0, 0, 0, 0};  // reset
  m_btnBox->close();
}

void ScreenCapturer::keyPressEvent(QKeyEvent *ev) {
  if (ev->key() == Qt::Key_Escape) close();
}

void ScreenCapturer::mouseDoubleClickEvent(QMouseEvent *) { close(); }

void ScreenCapturer::mousePressEvent(QMouseEvent *ev) {
  m_status = Status::Selecting;
  m_btnBox->close();
  m_captureRect.setTopLeft(ev->pos());
  m_captureRect.setBottomRight(ev->pos());
}
void ScreenCapturer::mouseMoveEvent(QMouseEvent *ev) {
  m_status = Status::Selecting;
  m_captureRect.setBottomRight(ev->pos());
  repaint();
  auto r = absRect(m_captureRect);
  m_btnBox->show();
  m_btnBox->move(
      std::max(r.right() - m_btnBox->width(), 0),
      std::min(r.bottom() + 5, kScreenSize.height() - m_btnBox->height()));

  m_title->move(r.left(), std::max(0, r.top() - m_title->height()));
}
void ScreenCapturer::mouseReleaseEvent(QMouseEvent *) {
  repaint();
  m_status = Status::Finish;
}