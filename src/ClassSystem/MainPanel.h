#pragma once

// layouts
#include <qboxlayout.h>
#include <qlayoutitem.h>

// widgets
#include <QAnimationStackedWidget/QAnimationStackedWidget.h>
#include <qframe.h>
#include <qheaderview.h>
#include <qlabel.h>
#include <qlcdnumber.h>
#include <qpropertyanimation.h>
#include <qtablewidget.h>
#include <qtextbrowser.h>

// local socket
#include <qlocalsocket.h>

// other
#include <qaction.h>
#include <qevent.h>
#include <qsettings.h>
#include <qtimer.h>

#include "ClassData.h"
#include "PopupMenu.h"
#include "TitleWidget.h"

class MainPanel : public QWidget {
  Q_OBJECT

 public:
  explicit MainPanel(QWidget *parent = nullptr);

  void loadData();

 private:
  // layouts
  QGridLayout *m_mainLayout = new QGridLayout(this);
  QGridLayout *m_headerLayout = new QGridLayout();
  QGridLayout *m_noticesLayout = new QGridLayout();
  QHBoxLayout *m_studentsLayout = new QHBoxLayout();
  QVBoxLayout *m_mealStuLayout = new QVBoxLayout();
  QVBoxLayout *m_stuOnDutyLayout = new QVBoxLayout();

  // lines
  QFrame *m_sentenceLine = new QFrame(this);
  QFrame *m_titleLine = new QFrame(this);
  QFrame *m_topNoticeLine = new QFrame(this);
  QFrame *m_bottomNoticeLine = new QFrame(this);
  QFrame *m_daysLeftLine = new QFrame(this);
  QFrame *m_lessonsLine = new QFrame(this);
  QFrame *m_stuLine = new QFrame(this);

  // header
  QLabel *m_labelDate = new QLabel("00-00", this);
  QLabel *m_labelTime = new QLabel("00:00:00", this);
  QLabel *m_labelDDDD = new QLabel("星期八", this);
  QLabel *m_sentenceLabel = new QLabel(this);
  TitleWidget *m_title = new TitleWidget("ClassSystem", this);

  ClassData::Data m_data;

  // lessons
  QTableWidget *m_lessons = new QTableWidget(9, 1, this);

  // notices
  QAnimationStackedWidget *m_noticesWid = new QAnimationStackedWidget(this);
  QList<QTextBrowser *> m_noticesTextBrowsers;
  QLabel *m_noticesTitle = new QLabel("公告", this);

  // days left
  QLabel *m_eventNameLabel = new QLabel(this);
  QLCDNumber *m_daysLeftDisplay = new QLCDNumber(this);
  QSpacerItem *m_daysLeftSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum);

  // students carry meals
  QLabel *m_mealStuTitle = new QLabel("抬饭生", this);
  QTableWidget *m_mealStuTable = new QTableWidget(this);

  // students on duty
  QLabel *m_stuOnDutyTitle = new QLabel("值日生", this);
  QTableWidget *m_stuOnDutyTable = new QTableWidget(this);

  // timers
  int m_clockTimerId = 0;
  int m_noticeTimerId = 0;
  int m_curLessonUpdateTimerId = 0;
  constexpr static const char *kTimeFormat[2] = {"hh:mm:ss", "hh mm ss"};
  bool m_formatWithColons = true;

  bool m_init = true;

  QPoint m_mouseStartPoint;
  constexpr static int kPadding = 8;

  // local server
  QLocalSocket *m_socket = new QLocalSocket(this);
  static constexpr const char kServerName[] = "CLASS-ADMIN-SERVER";
  void initLocalSocket();

  PopupMenu *m_menu = new PopupMenu;

 private slots:
  void onReadyRead();
  void onConnected();

 protected:
  bool nativeEvent(const QByteArray &, void *message, long *result) override;
  void paintEvent(QPaintEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void resizeEvent(QResizeEvent *ev) override;
  void moveEvent(QMoveEvent *ev) override;
  void timerEvent(QTimerEvent *ev) override;
};
