#include "MainPanel.h"

#include <qbrush.h>
#include <qdatetime.h>
#include <qpainter.h>
#include <qsizepolicy.h>

MainPanel::MainPanel(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint) {
  setWidgetTransparent(this);
  setParentToDesktop(this);

  setStyleSheet(R"(

#labelDDDD, #labelDate,#mealStuListWid, #stuOnDutyListWid, #stuLine {
  color: #d2d0ce;
}
#mealStuTitle, #stuOnDutyTitle{
  color: #edebe9;
}
#mealStuListWid, #stuOnDutyListWid {
  background-color: transparent;
}
#labelTime {
  color:#d9d9d9;
}
)");

  // show datetime
  m_labelDDDD->setObjectName("labelDDDD");
  m_labelDate->setObjectName("labelDate");
  m_labelTime->setObjectName("labelTime");
  m_labelDDDD->setText(QDate::currentDate().toString("dddd"));
  m_labelDate->setText(QDate::currentDate().toString("MM-dd"));
  m_labelTime->setFont(qFont{"华文中宋", 36, QFont::Bold}());
  m_labelDate->setFont(qFont{.pointSize = 16}());
  m_labelDDDD->setFont(qFont{.pointSize = 16}());

  // show students carry meals
  m_mealStuLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
  m_mealStuLabel->setFrameShape(QFrame::NoFrame);
  m_mealStuLabel->setObjectName("mealStuListWid");
  m_mealStuLabel->setFont(
      qFont{.family = "'Consolas', 'MiSans'", .pointSize = 18}());
  m_mealStuLabel->setFocusPolicy(Qt::NoFocus);
  auto mealStuToday = m_data.mealStu[dayToday()];
  for (const uint &id : mealStuToday) {
    m_mealStuLabel->setText(m_mealStuLabel->text() + "\n" +
                            m_data.idAndName(id));
  }
  m_mealStuTitle->setAlignment(Qt::AlignTop);
  m_mealStuLabel->setAlignment(Qt::AlignTop);
  m_mealStuLabel->setText(m_mealStuLabel->text().mid(1));

  m_mealStuTitle->setObjectName("mealStuTitle");
  m_mealStuTitle->setFont(qFont{.pointSize = 21, .weight = QFont::Bold}());

  // show students on duty
  m_stuOnDutyLabel->setSizePolicy(QSizePolicy::MinimumExpanding,
                                  QSizePolicy::MinimumExpanding);
  m_stuOnDutyLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
  m_stuOnDutyLabel->setFrameShape(QFrame::NoFrame);
  m_stuOnDutyLabel->setObjectName("stuOnDutyListWid");
  m_stuOnDutyLabel->setFont(
      qFont{.family = "'Consolas', 'MiSans'", .pointSize = 18}());
  m_stuOnDutyLabel->setFocusPolicy(Qt::NoFocus);
  m_stuOnDutyLabel->setAlignment(Qt::AlignTop);
  auto stuOnDutyToday = m_data.stuOnDuty[dayToday()];
  for (const QList<uint> &l : stuOnDutyToday) {
    if (l.empty()) continue;
    QString displayStr;
    for (const uint &id : l) displayStr += " " + m_data.idAndName(id);
    m_stuOnDutyLabel->setText(m_stuOnDutyLabel->text() + "\n" +
                              displayStr.mid(1));
  }
  m_stuOnDutyLabel->setText(m_stuOnDutyLabel->text().mid(1));

  m_stuOnDutyTitle->setObjectName("stuOnDutyTitle");
  m_stuOnDutyTitle->setFont(qFont{.pointSize = 21, .weight = QFont::Bold}());

  // init lines
  m_stuLine->setFrameShape(QFrame::VLine);
  m_stuLine->setObjectName("stuLine");
  // init layouts

  m_mainLayout->setMargin(10);
  m_mainLayout->setSpacing(10);
  m_mainLayout->addLayout(m_dateTimeLayout, 0, 0, 1, 3, Qt::AlignLeft);
  m_mainLayout->addLayout(m_mealStuLayout, 1, 0);
  m_mainLayout->addWidget(m_stuLine, 1, 1);
  m_mainLayout->addLayout(m_stuOnDutyLayout, 1, 2);

  m_dateTimeLayout->addWidget(m_labelTime, 0, 0, 2, 1, Qt::AlignBottom);
  m_dateTimeLayout->addWidget(m_labelDate, 0, 1, Qt::AlignBottom);
  m_dateTimeLayout->addWidget(m_labelDDDD, 1, 1, Qt::AlignBottom);
  m_mealStuLayout->setAlignment(Qt::AlignTop);
  m_stuOnDutyLayout->setAlignment(Qt::AlignTop);

  m_mealStuLayout->addWidget(m_mealStuTitle);
  m_mealStuLayout->addWidget(m_mealStuLabel);

  m_stuOnDutyLayout->addWidget(m_stuOnDutyTitle);
  m_stuOnDutyLayout->addWidget(m_stuOnDutyLabel);

  //  init timer
  m_timer.setInterval(500);  // 0.5 secs
  connect(&m_timer, &QTimer::timeout, this, &MainPanel::onHalfSecs);
  m_timer.start();
}

void MainPanel::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  QLinearGradient ling(rect().topLeft(), rect().bottomRight());

  ling.setColorAt(0, {0, 12, 64, 225});
  ling.setColorAt(1, {96, 125, 139, 225});
  painter.setBrush(ling);
  painter.setPen(Qt::transparent);
  painter.drawRoundedRect(rect(), 20, 20);
}
void MainPanel::onHalfSecs() {
  m_labelTime->setText(QTime::currentTime().toString(
      kTimeFormat[m_formatWithColons = !m_formatWithColons]));
}

void MainPanel::mousePressEvent(QMouseEvent *ev) {
  m_mouseStartPoint = pos() - ev->globalPos();
}

void MainPanel::mouseMoveEvent(QMouseEvent *ev) {
  move(ev->globalPos() + m_mouseStartPoint);
}