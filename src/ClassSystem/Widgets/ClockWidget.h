#pragma once
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

class ClockWidget : public QWidget {
  Q_OBJECT
public:
  explicit ClockWidget(QWidget *parent = nullptr);
  ~ClockWidget();

private:
  QVBoxLayout *layout = new QVBoxLayout(this);
  QLabel *labelTime = new QLabel("00:00", this);
  QTimer timerOneSec;
  static constexpr const char *timeFormat[2] = {"hh:mm", "hh mm"};
  time_t seconds = 0;
private slots:
  void onSeconds();
};
