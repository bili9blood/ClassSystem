#pragma once

#include <qfontdatabase.h>
#include <qtimer.h>
#include <ui_RollCallWindow.h>

#include <random>

#include "ClassData.h"

class RollCallWindow : public QWidget {
  Q_OBJECT
 public:
  explicit RollCallWindow(QWidget *parent = nullptr);
  void loadData();

 private slots:
  void reset();
  void toggleStartStop(bool checked);
  void changeFontSize(int index);

  void moveToAnother(QListWidget *src, int index, QListWidget *dst);
  // only for signal-slots
  void moveToAnother();

  void onTimeout();

 private:
  Ui::RollCallWindow ui;

  ClassData::Data m_data;

  QTimer m_timer;

  int m_calledIndex;

  // random
  std::mt19937 m_rng;

  const QStringList kStartStopText = {"开始", "停止"};

  QFontDatabase m_fontDb;
  const QList<int> kPointSizeList = m_fontDb.pointSizes(qApp->font().family());
};