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
  // widgets on desktop
  ClockWidget *clockWid = new ClockWidget;
  QDesktopWidget *desktop = qApp->desktop();
  // other widgets
  QLabel *popMenuLabel = new QLabel(this);
  QWidget *actionsWid = new QWidget(this);
  bool actionsFixedVisible = false;
  QPoint mStartPoint;

protected:
  bool eventFilter(QObject *obj, QEvent *ev) override;
  void paintEvent(QPaintEvent *ev) override;
};