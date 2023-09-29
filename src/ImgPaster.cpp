#include "ImgPaster.h"

ImgPaster::ImgPaster(QWidget *parent, QPixmap pixmap)
    : QWidget{parent, Qt::FramelessWindowHint}, m_pixmap(pixmap) {
  setWindowTitle("贴图");
  setParentToDesktop(this);

  // init layout
  m_mainLayout->addWidget(m_labelPixmap);
  m_mainLayout->setMargin(0);

  // init labels
  m_labelPixmap->setStyleSheet("border: 2px solid black;");
  m_labelPixmap->setScaledContents(true);
  m_labelPixmap->setPixmap(m_pixmap);
  resize(m_pixmap.size());
  m_labelSize->resize(100, 40);
  m_labelSize->setAlignment(Qt::AlignCenter);
  m_labelSize->setStyleSheet("background-color:white");
  m_labelSize->setVisible(false);

  // init timer
  connect(&m_timer, &QTimer::timeout, this, &ImgPaster::hideSizeLabel);
  m_timer.setSingleShot(true);
  m_initalSize = size();
}

void ImgPaster::hideSizeLabel() {
  m_labelSize->setVisible(false);
  m_timer.stop();
}

void ImgPaster::mousePressEvent(QMouseEvent *ev) {
  if (ev->button() == Qt::LeftButton) m_mouseStartPos = ev->pos();

  if (ev->button() == Qt::MiddleButton) close();
}

void ImgPaster::mouseDoubleClickEvent(QMouseEvent *) { close(); }

void ImgPaster::mouseMoveEvent(QMouseEvent *ev) {
  if (ev->buttons() & Qt::LeftButton) move(ev->pos() + pos() - m_mouseStartPos);
}

void ImgPaster::wheelEvent(QWheelEvent *ev) {
  m_size += (ev->angleDelta().y() > 0 && m_size < 20);
  m_size -= (ev->angleDelta().y() < 0 && m_size > -5);
  QSize newSize = m_initalSize * (1.0 + m_size * 0.1);
  m_labelPixmap->setPixmap(
      m_pixmap.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
  resize(newSize);
  m_labelSize->setText(QString::number(100 + m_size * 10) + '%');
  m_labelSize->show();
  m_timer.start(1500);
}

void ImgPaster::keyPressEvent(QKeyEvent *ev) {
  int x = pos().x(), y = pos().y();
  switch (ev->key()) {
    case Qt::Key_Left:
      move(QPoint(x - 1, y));
      ev->accept();
      return;
    case Qt::Key_Right:
      move(QPoint(x + 1, y));
      ev->accept();
      return;
    case Qt::Key_Up:
      move(QPoint(x, y + 1));
      ev->accept();
      return;
    case Qt::Key_Down:
      move(QPoint(x, y - 1));
      ev->accept();
      return;
    case Qt::Key_Delete:
    case Qt::Key_Backspace:
      close();
  }
}

void ImgPaster::closeEvent(QCloseEvent *) {
  emit closed();
  deleteLater();
}
