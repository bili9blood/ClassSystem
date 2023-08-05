#pragma once
#include <qapplication.h>
#include <qlabel.h>

class SuspendedWidget : public QWidget {
  Q_OBJECT
 public:
  explicit SuspendedWidget(QWidget *parent = nullptr);
  ~SuspendedWidget() override;

 private:
  // other widgets
  QLabel *popMenuLabel = new QLabel(this);
  QPoint mStartPoint;
 private slots:
  void onMenuClicked(size_t menuID);

 protected:
  bool eventFilter(QObject *obj, QEvent *ev) override;
  void paintEvent(QPaintEvent *ev) override;
};