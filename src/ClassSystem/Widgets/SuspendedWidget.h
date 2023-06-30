#pragma once
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QScreen>
#include <QWidget>

#include "Widgets/MenuWidget.h"

class SuspendedWidget : public QWidget {
  Q_OBJECT
 public:
  explicit SuspendedWidget(QWidget *parent = nullptr);
  ~SuspendedWidget() override;

 private:
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