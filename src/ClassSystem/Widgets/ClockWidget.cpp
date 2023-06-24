#include "ClockWidget.h"

ClockWidget::ClockWidget(QWidget *parent)
    : QWidget{parent, Qt::FramelessWindowHint} {
  setParentToDesktop((HWND)winId());
  SET_WIDGET_TRANSPARENT;
  // set timer
  timerOneSec.setInterval(1000);
  connect(&timerOneSec, &QTimer::timeout, this, &ClockWidget::onSeconds);
  timerOneSec.start();
  dateLabel->setAlignment(Qt::AlignCenter);
  // setup time label
  timeLabel->setAlignment(Qt::AlignCenter);
  timeLabel->setFont(QFont("Inconsolata", 100, QFont::DemiBold));
  timeLabel->show();
  // set layout
  layout->addWidget(timeLabel);
  layout->addWidget(dateLabel);
  layout->setSpacing(0);
}

ClockWidget::~ClockWidget() = default;

void ClockWidget::onSeconds() {
  // set date label
  QFont dateFont1("微软雅黑", 10, QFont::Medium), dateFont2 = dateFont1;
  QFontMetrics metrics1(dateFont1), metrics2 = metrics1;
  while (metrics2.horizontalAdvance(dateLabel->text()) < timeLabel->width()) {
    metrics1 = metrics2;
    dateFont1 = dateFont2;
    dateFont2.setPointSize(dateFont2.pointSize() + 1);
    metrics2 = QFontMetrics(dateFont2);
  }
  dateFont1.setPointSize(dateFont1.pointSize() - 2);
  dateLabel->setFont(dateFont1);
  ++seconds;
  // update label
  timeLabel->setText(QTime::currentTime().toString(timeFormat[seconds % 2]));
}