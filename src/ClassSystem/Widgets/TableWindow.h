#pragma once

#include <ClassData.h>
#include <qboxlayout.h>
#include <qevent.h>
#include <qtablewidget.h>
#include <qtabwidget.h>
#include <qwidget.h>

class TableWindow : public QWidget {
  Q_OBJECT
 public:
  explicit TableWindow(QWidget *parent = nullptr);

  void reloadUi();

 private:
  QVBoxLayout *m_mainLayout = new QVBoxLayout(this);

  QTabWidget *m_tabWidget = new QTabWidget(this);

  QTableWidget *m_lessonsTable = new QTableWidget(this);
  QTableWidget *m_mealStuTable = new QTableWidget(this);
  QTableWidget *m_stuOnDutyTable = new QTableWidget(this);

  ClassData::Data m_data;
};
