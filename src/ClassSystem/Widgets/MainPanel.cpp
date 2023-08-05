#include "MainPanel.h"

#include <qdatetime.h>
#include <qpainter.h>

MainPanel::MainPanel(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint) {
  setStyleSheet(R"(
#labelDDDD,#labelDate,#l{
  color:#9c9c9c;
}
#labelTime{
  color:white;
}
)");
  layoutMain->setMargin(10);
  // init datetime
  labelDDDD->setObjectName("labelDDDD");
  labelDate->setObjectName("labelDate");
  labelTime->setObjectName("labelTime");
  labelDDDD->setText(QDate::currentDate().toString("dddd"));
  labelDate->setText(QDate::currentDate().toString("MM-dd"));
  labelTime->setFont(qFont{"华文中宋", 36, QFont::Bold}());
  labelDate->setFont(qFont{.pointSize = 14}());
  labelDDDD->setFont(qFont{.pointSize = 14}());
  layoutDateTime->addWidget(labelTime, 0, 0, 2, 1, Qt::AlignBottom);
  layoutDateTime->addWidget(labelDate, 0, 1, Qt::AlignBottom);
  layoutDateTime->addWidget(labelDDDD, 1, 1, Qt::AlignBottom);
  layoutMain->addLayout(layoutDateTime, 0, 0, 1, 3, Qt::AlignLeft);
  // show students carry meals
  auto l = new QLabel("# 你好\n## 我也好");
  l->setObjectName("l");
  l->setTextFormat(Qt::MarkdownText);
  layoutStudentsCarryMeals->addWidget(l);
  layoutMain->addLayout(layoutStudentsCarryMeals, 1, 0);
  //  init timer
  timerHalfSeconds.setInterval(500);  // 0.5 secs
  connect(&timerHalfSeconds, &QTimer::timeout, this, &MainPanel::onHalfSecs);
  timerHalfSeconds.start();
  setWidgetTransparent(this);
  setParentToDesktop((HWND)winId());
  lastSecond = QTime::currentTime().second();
}
void MainPanel::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setBrush(QColor(31, 33, 34, 235));
  painter.setPen(Qt::transparent);
  painter.drawRoundedRect(rect(), 30, 30);
}
void MainPanel::onHalfSecs() {
  labelTime->setText(QTime::currentTime().toString(
      timeFormat[lastSecond == QTime::currentTime().second()]));
  lastSecond = QTime::currentTime().second();
}

MainPanel::~MainPanel() = default;
