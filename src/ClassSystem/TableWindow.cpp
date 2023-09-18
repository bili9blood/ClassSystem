#include "TableWindow.h"

#include <qheaderview.h>
#include <qmessagebox.h>
#include <qpainter.h>

TableWindow::TableWindow(QWidget *parent) : QWidget(parent) {
  setWindowTitle("表格");
  setStyleSheet(R"(
QTableWidget, QTabWidget, QTabWidget::pane, QTabBar::tab {
  background-color: transparent;
}

QTabBar::tab:selected {
  background-color: #27385a;
  color: white;
}
  )");
  // init tab widget
  m_tabWidget->addTab(m_lessonsTable, QIcon(), "课程表");
  m_tabWidget->addTab(m_mealStuTable, QIcon(), "抬饭表");
  m_tabWidget->addTab(m_stuOnDutyTable, QIcon(), "值日表");
  m_tabWidget->setUsesScrollButtons(false);
  m_tabWidget->setAttribute(Qt::WA_StyledBackground);

  // init tables
  m_lessonsTable->setFrameShape(QFrame::NoFrame);
  m_lessonsTable->setAttribute(Qt::WA_TransparentForMouseEvents);
  m_lessonsTable->setFocusPolicy(Qt::NoFocus);
  tableViewStretch(m_lessonsTable);
  m_lessonsTable->setColumnCount(5);
  m_lessonsTable->setRowCount(8);
  m_lessonsTable->setWordWrap(false);

  m_mealStuTable->setFrameShape(QFrame::NoFrame);
  m_mealStuTable->setAttribute(Qt::WA_TransparentForMouseEvents);
  m_mealStuTable->setFocusPolicy(Qt::NoFocus);
  tableViewStretch(m_mealStuTable);
  m_mealStuTable->setColumnCount(5);
  m_mealStuTable->verticalHeader()->setVisible(false);
  m_mealStuTable->setWordWrap(false);

  m_stuOnDutyTable->setFrameShape(QFrame::NoFrame);
  m_stuOnDutyTable->setAttribute(Qt::WA_TransparentForMouseEvents);
  m_stuOnDutyTable->setFocusPolicy(Qt::NoFocus);
  tableViewStretch(m_stuOnDutyTable);
  m_stuOnDutyTable->setColumnCount(5);
  m_stuOnDutyTable->setWordWrap(false);

  //  init layouts
  m_mainLayout->setMargin(5);
  QFontDatabase s;
  m_mainLayout->addWidget(m_tabWidget);

  loadData();
}

void TableWindow::loadData() {
  // lessons
  m_lessonsTable->clear();
  for (int i = 0; i < 5; ++i) {
    m_lessonsTable->setHorizontalHeaderItem(
        i, new QTableWidgetItem(oneDayOfWeek(i)));
    for (int j = 0; j < classData.lessons[i].size(); ++j) {
      m_lessonsTable->setVerticalHeaderItem(
          j, new QTableWidgetItem("第%1节"_s.arg(j + 1)));
      auto item = new QTableWidgetItem("%1\n(%2-%3)"_s.arg(
          classData.lessons[i][j], classData.lessonsTm[j].toString("hh:mm"),
          classData.lessonsTm[j].addSecs(2400).toString("hh:mm")));
      item->setTextAlignment(Qt::AlignCenter);
      item->setFont(qFont{.family = "华文中宋",
                          .pointSize = kPointSizeList[m_pointSizeIndex]}());
      if (i == dayToday()) item->setBackground(QColor(230, 230, 230));
      m_lessonsTable->setItem(j, i, item);
    }
  }

  // students carry meals
  int mx =
      std::max<int>({classData.mealStu[0].size(), classData.mealStu[1].size(),
                     classData.mealStu[2].size(), classData.mealStu[3].size(),
                     classData.mealStu[4].size()});

  m_mealStuTable->clear();
  m_mealStuTable->setRowCount(mx);
  for (int i = 0; i < 5; ++i) {
    m_mealStuTable->setHorizontalHeaderItem(
        i, new QTableWidgetItem(oneDayOfWeek(i)));
    for (int j = 0; j < classData.mealStu[i].size(); ++j) {
      auto item =
          new QTableWidgetItem(classData.idAndName(classData.mealStu[i][j]));
      item->setTextAlignment(Qt::AlignCenter);
      item->setFont(qFont{.family = "华文中宋",
                          .pointSize = kPointSizeList[m_pointSizeIndex]}());
      if (i == dayToday()) item->setBackground(QColor(230, 230, 230));
      m_mealStuTable->setItem(j, i, item);
    }
  }

  // students on duty
  m_stuOnDutyTable->clear();
  m_stuOnDutyTable->setRowCount(classData.dutyJobs.size());
  for (int i = 0; i < 5; ++i) {
    m_stuOnDutyTable->setHorizontalHeaderItem(
        i, new QTableWidgetItem(oneDayOfWeek(i)));
    for (int j = 0; j < classData.dutyJobs.size(); ++j) {
      m_stuOnDutyTable->setVerticalHeaderItem(
          j, new QTableWidgetItem(classData.dutyJobs[j]));
      QString str;
      for (const uint &id : classData.stuOnDuty[i][j]) {
        if (str.size()) str += '\n';
        str += classData.idAndName(id);
      }
      auto item = new QTableWidgetItem(str);
      item->setTextAlignment(Qt::AlignCenter);
      item->setFont(qFont{.family = "华文中宋",
                          .pointSize = kPointSizeList[m_pointSizeIndex]}());
      if (i == dayToday()) item->setBackground(QColor(230, 230, 230));
      m_stuOnDutyTable->setItem(j, i, item);
    }
  }
}

void TableWindow::wheelEvent(QWheelEvent *ev) {
  m_pointSizeIndex +=
      ev->angleDelta().y() > 0 && m_pointSizeIndex + 1 != kPointSizeList.size();
  m_pointSizeIndex -= ev->angleDelta().y() < 0 && m_pointSizeIndex != 0;
  loadData();
}