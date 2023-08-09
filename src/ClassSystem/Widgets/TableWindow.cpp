#include "TableWindow.h"

#include <qheaderview.h>
#include <qpainter.h>

TableWindow::TableWindow(QWidget *parent) : QWidget(parent) {
  setWindowTitle("表格");
  setStyleSheet(R"(
QTableWidget, QTableWidget::Item, QTabWidget, QTabWidget::pane, QTabBar::tab {
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
  m_lessonsTable->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);
  m_lessonsTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_lessonsTable->setColumnCount(5);
  m_lessonsTable->setRowCount(8);
  m_lessonsTable->setWordWrap(false);

  //  init layouts
  m_mainLayout->addWidget(m_tabWidget);

  reloadUi();
}

void TableWindow::reloadUi() {
  m_data = ClassData::readFrom(new QFile("data.stm"));

  // lessons
  for (int i = 0; i < 5; ++i) {
    m_lessonsTable->setHorizontalHeaderItem(
        i, new QTableWidgetItem(oneDayOfWeek(i)));
    for (int j = 0; j < 8; ++j) {
      m_lessonsTable->setVerticalHeaderItem(
          j, new QTableWidgetItem(QString("第%1节").arg(j + 1)));
      auto item = new QTableWidgetItem(
          QString("%1\n(%2-%3)")
              .arg(m_data.lessons[i][j], m_data.LessonsTm[j].toString("hh:mm"),
                   m_data.LessonsTm[j].addSecs(2400).toString("hh:mm")));
      item->setTextAlignment(Qt::AlignCenter);
      item->setFont(
          qFont{.family = "华文中宋",
                .pointSize = 16,
                .weight = i == dayToday() ? QFont::Bold : QFont::Normal}());
      m_lessonsTable->setItem(j, i, item);
    }
  }
}
