#pragma once

#include <ClassData.h>
#include <qboxlayout.h>
#include <qevent.h>
#include <qfontdatabase.h>
#include <qtablewidget.h>
#include <qtabwidget.h>
#include <qwidget.h>

class TableWindow : public QWidget {
  Q_OBJECT
 public:
  explicit TableWindow(QWidget *parent = nullptr);

  void loadData();

 private:
  QVBoxLayout *m_mainLayout = new QVBoxLayout(this);

  QTabWidget *m_tabWidget = new QTabWidget(this);

  QTableWidget *m_lessonsTable = new QTableWidget(this);
  QTableWidget *m_mealStuTable = new QTableWidget(this);
  QTableWidget *m_stuOnDutyTable = new QTableWidget(this);

  QFontDatabase m_fontDb;
  const QList<int> kPointSizeList = m_fontDb.pointSizes("华文中宋");
  int m_pointSizeIndex = kPointSizeList.indexOf(16);

 protected:
  void wheelEvent(QWheelEvent *ev) override;
};
