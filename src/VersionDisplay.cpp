#include "VersionDisplay.h"

#include <qpainter.h>

VersionDisplay::VersionDisplay(QWidget *parent) : QWidget(parent) {
  m_layout->addWidget(m_label);

  m_label->setFont(cs::font{.family = "华文中宋",
                            .pointSize = cs::settings::smallFontSize,
                            .weight = QFont::Thin}());
  m_label->setAlignment(Qt::AlignCenter);

  setFixedSize(270, 70);
}

void VersionDisplay::onProgress(ushort progress) {
  m_progress = progress;
  m_label->setText(QString::asprintf("%u%%", progress));
  repaint();
}

void VersionDisplay::paintEvent(QPaintEvent *) {
  if (m_progress == 101) [[likely]] {
    m_label->setText(m_versionText);
    return;
  }

  QPainter painter(this);

  const QRect progressBarRect = {6, 6, 260, 50};
  const auto progressRect = [](ushort progress) -> QRect {
    return {9, 9, qBound(0, int(254.0 * (progress * 0.01)), 254), 44};
  };

  painter.setRenderHint(QPainter::Antialiasing);

  painter.setPen(QPen(QColor(104, 148, 92, 150), 6));
  painter.setBrush(Qt::transparent);
  painter.drawRoundedRect(progressBarRect, 25, 25);

  painter.setPen(Qt::transparent);
  painter.setBrush(QColor(46, 213, 115, 180));
  painter.drawRoundedRect(progressRect(m_progress), 22, 22);
}
