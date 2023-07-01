//
// Created by 35012 on 2023/6/30.
//

#ifndef CLASSSYSTEM_MAINPANEL_H
#define CLASSSYSTEM_MAINPANEL_H

#include <QDateTime>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QWidget>

class MainPanel : public QWidget {
  Q_OBJECT

 public:
  explicit MainPanel(QWidget *parent = nullptr);
  ~MainPanel() override;

 private:
  QGridLayout *layoutMain = new QGridLayout(this);
  QGridLayout *layoutDateTime = new QGridLayout;
  QLabel *labelDate = new QLabel("00-00", this);
  QLabel *labelTime = new QLabel("00:00:00", this);
  QLabel *labelDDDD = new QLabel("星期八", this);
  QTimer timerHalfSeconds;
  constexpr static const char *timeFormat[2] = {"hh:mm:ss", "hh mm ss"};
  uint lastSecond;
 private slots:
  void onHalfSecs();

 protected:
  void paintEvent(QPaintEvent *) override;
};

#endif  // CLASSSYSTEM_MAINPANEL_H
