#include "ClockWidget.h"

ClockWidget::ClockWidget(QWidget *parent)
    : QWidget{parent, Qt::FramelessWindowHint} {
  setParentToDesktop((HWND)winId());
  SET_WIDGET_TRANSPARENT;
  // set timer
  timerOneSec.setInterval(1000);
  connect(&timerOneSec, &QTimer::timeout, this, &ClockWidget::onSeconds);
  timerOneSec.start();
  // set label
  timeLabel->setAlignment(Qt::AlignCenter);
  timeLabel->setFont(QFont("Inconsolata", 100, QFont::Medium));
  timeLabel->show();
  // set layout
  layout->addWidget(timeLabel);
}

ClockWidget::~ClockWidget() {}

void ClockWidget::onSeconds() {
  ++seconds;
  // update label
  timeLabel->setText(QTime::currentTime().toString(timeFormat[seconds % 2]));
}