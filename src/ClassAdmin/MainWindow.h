#pragma once

#include <qgridlayout.h>
#include <qlocalserver.h>
#include <qlocalsocket.h>
#include <qmainwindow.h>
#include <ui_MainWindow.h>

#include "ClassData.h"

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit MainWindow(QWidget *parent = nullptr);

 private slots:
  // students
  void addStudent();
  void removeStudent();
  void clearStudents();
  void importStudents();

  // 移除项目时不会触发
  void onStudentsChanged(const QModelIndex &idx, const QModelIndex &,
                         const QVector<int> &);

  // students carry meals
  void editMealStu();
  void clearMealStu();
  void importMealStu();

  // toolbar
  void resetPwd();
  void saveData();
  void dropData();

  // local server
  void onReadyRead();
  void onNewConnection();

 private:
  Ui::MainWindow ui;

  QList<QPushButton *> m_mealStuBtns;
  QList<QLabel *> m_mealStuLabels;

  ClassData::Data m_data;
  bool m_changed = false;

  QLocalServer *m_server = new QLocalServer(this);
  QLocalSocket *m_socket = nullptr;
  static constexpr const char kServerName[] = "CLASS-ADMIN-SERVER";

  static constexpr const char *kWindowTitle[] = {
      "ClassSystem 管理后台", "ClassSystem 管理后台 *(未保存)"};

  void initServer();
  void loadData();

  bool m_dataLoaded = false;

 protected:
  void paintEvent(QPaintEvent *ev) override;
};