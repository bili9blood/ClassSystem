#pragma once
#include "Widgets/ClockWidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QScreen>

class ClassSystem : public QMainWindow {
  Q_OBJECT
public:
  explicit ClassSystem(QWidget *parent = nullptr);
  ~ClassSystem();

private:
  // widgets
  ClockWidget *clockWid = new ClockWidget;
  QDesktopWidget *desktop = qApp->desktop();
  QPoint mStartPoint;

protected:
  void paintEvent(QPaintEvent *ev);
  void mousePressEvent(QMouseEvent *ev);
  void mouseMoveEvent(QMouseEvent *ev);
};