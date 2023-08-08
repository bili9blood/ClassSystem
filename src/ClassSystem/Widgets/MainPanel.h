#pragma once

#include <qaction.h>
#include <qboxlayout.h>
#include <qevent.h>
#include <qframe.h>
#include <qlabel.h>
#include <qlistwidget.h>
#include <qmenu.h>
#include <qsystemtrayicon.h>
#include <qtimer.h>

#include "ClassData.h"
class MainPanel : public QWidget {
  Q_OBJECT

 public:
  explicit MainPanel(QWidget *parent = nullptr);

 private:
  // layouts
  QGridLayout *m_mainLayout = new QGridLayout(this);
  QGridLayout *m_headerLayout = new QGridLayout();
  QVBoxLayout *m_mealStuLayout = new QVBoxLayout();
  QVBoxLayout *m_stuOnDutyLayout = new QVBoxLayout();

  // lines
  QFrame *m_sentenceLine = new QFrame(this);
  QFrame *m_stuLine = new QFrame(this);

  // header
  QLabel *m_labelDate = new QLabel("00-00", this);
  QLabel *m_labelTime = new QLabel("00:00:00", this);
  QLabel *m_labelDDDD = new QLabel("星期八", this);
  QLabel *m_sentenceLabel = new QLabel(this);

  ClassData::Data m_data = ClassData::readFrom(new QFile("data.stm"));

  // students carry meals
  QLabel *m_mealStuTitle = new QLabel("抬饭生", this);
  QLabel *m_mealStuLabel = new QLabel(this);

  // students on duty
  QLabel *m_stuOnDutyTitle = new QLabel("值日生", this);
  QLabel *m_stuOnDutyLabel = new QLabel(this);

  // timers
  QTimer m_timer;
  constexpr static const char *kTimeFormat[2] = {"hh:mm:ss", "hh mm ss"};
  bool m_formatWithColons = true;

  QPoint m_mouseStartPoint;

 private slots:
  void onHalfSecs();

 protected:
  void paintEvent(QPaintEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
};
