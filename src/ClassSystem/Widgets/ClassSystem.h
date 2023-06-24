#pragma once
#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QScreen>

#include "Widgets/ClockWidget.h"
#include "Widgets/MenuWidget.h"

class ClassSystem : public QMainWindow {
  Q_OBJECT
 public:
  explicit ClassSystem(QWidget *parent = nullptr);
  ~ClassSystem() override;

 private:
  // widgets on desktop
  ClockWidget *clockWid = new ClockWidget;
  QDesktopWidget *desktop = qApp->desktop();
  // other widgets
  QLabel *popMenuLabel = new QLabel(this);
  MenuWidget *menuWid = new MenuWidget(nullptr);
  bool menuFixedVisible = false;
  QPoint mStartPoint;
 private slots:
  void onMenuClicked(size_t menuID);

 protected:
  bool eventFilter(QObject *obj, QEvent *ev) override;
  void paintEvent(QPaintEvent *ev) override;
};