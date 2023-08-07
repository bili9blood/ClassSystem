#include "MainPanel.h"

#include <qdatetime.h>
#include <qpainter.h>

MainPanel::MainPanel(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint) {
  setStyleSheet(R"(
#m_labelDDDD,#m_labelDate,#l{
  color:#9c9c9c;
}
#m_labelTime{
  color:white;
}
)");
  m_layoutMain->setMargin(10);
  // init datetime
  m_labelDDDD->setObjectName("m_labelDDDD");
  m_labelDate->setObjectName("m_labelDate");
  m_labelTime->setObjectName("m_labelTime");
  m_labelDDDD->setText(QDate::currentDate().toString("dddd"));
  m_labelDate->setText(QDate::currentDate().toString("MM-dd"));
  m_labelTime->setFont(qFont{"华文中宋", 36, QFont::Bold}());
  m_labelDate->setFont(qFont{.pointSize = 14}());
  m_labelDDDD->setFont(qFont{.pointSize = 14}());
  m_layoutDateTime->addWidget(m_labelTime, 0, 0, 2, 1, Qt::AlignBottom);
  m_layoutDateTime->addWidget(m_labelDate, 0, 1, Qt::AlignBottom);
  m_layoutDateTime->addWidget(m_labelDDDD, 1, 1, Qt::AlignBottom);
  m_layoutMain->addLayout(m_layoutDateTime, 0, 0, 1, 3, Qt::AlignLeft);
  // show students carry meals
  auto l = new QLabel("# 你好\n## 我也好");
  l->setObjectName("l");
  l->setTextFormat(Qt::MarkdownText);
  m_layoutStudentsCarryMeals->addWidget(l);
  m_layoutMain->addLayout(m_layoutStudentsCarryMeals, 1, 0);
  //  init timer
  m_timerHalfSeconds.setInterval(500);  // 0.5 secs
  connect(&m_timerHalfSeconds, &QTimer::timeout, this, &MainPanel::onHalfSecs);
  m_timerHalfSeconds.start();
  setWidgetTransparent(this);
  setParentToDesktop(this);
  m_lastSecond = QTime::currentTime().second();
}
void MainPanel::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setBrush(QColor(31, 33, 34, 235));
  painter.setPen(Qt::transparent);
  painter.drawRoundedRect(rect(), 20, 20);
}
void MainPanel::onHalfSecs() {
  m_labelTime->setText(QTime::currentTime().toString(
      m_kTimeFormat[m_lastSecond == QTime::currentTime().second()]));
  m_lastSecond = QTime::currentTime().second();
}

void MainPanel::mousePressEvent(QMouseEvent *ev) {
  m_mouseStartPoint = pos() - ev->globalPos();
}

void MainPanel::mouseMoveEvent(QMouseEvent *ev) {
  move(ev->globalPos() + m_mouseStartPoint);
}