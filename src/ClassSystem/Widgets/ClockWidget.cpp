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
  labelTime->setAlignment(Qt::AlignCenter);
  labelTime->setFont(QFont("Inconsolata", 100, QFont::Medium));
  labelTime->show();
  // set layout
  layout->addWidget(labelTime);
}

ClockWidget::~ClockWidget() {}

void ClockWidget::onSeconds() {
  ++seconds;
  // update label
  labelTime->setText(QTime::currentTime().toString(timeFormat[seconds % 2]));
}