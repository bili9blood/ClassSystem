#pragma once
#include <QFontMetrics>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

class ClockWidget : public QWidget {
  Q_OBJECT
 public:
  explicit ClockWidget(QWidget *parent = nullptr);
  ~ClockWidget() override;

 private:
  QVBoxLayout *layout = new QVBoxLayout(this);
  QLabel *timeLabel = new QLabel("00:00", this);
  QLabel *dateLabel =
      new QLabel(QDate::currentDate().toString("yyyy年M月d日 ddd"), this);
  QTimer timerOneSec;
  static constexpr const char *timeFormat[2] = {"hh:mm", "hh mm"};
  time_t seconds = 0;

 private slots:
  void onSeconds();
};
