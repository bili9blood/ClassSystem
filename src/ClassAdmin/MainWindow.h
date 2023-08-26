#pragma once

// ui
#include <qcheckbox.h>
#include <qgridlayout.h>
#include <qmainwindow.h>
#include <ui_MainWindow.h>

// local server
#include <qlocalserver.h>
#include <qlocalsocket.h>

// network
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>

// undo framework
#include <qundostack.h>
#include <qundoview.h>

#include "ClassData.h"

class MainWindow : public QMainWindow {
  friend class ChangeDataCommand;

  Q_OBJECT public : explicit MainWindow(QWidget *parent = nullptr);

 private slots:
  // students
  void addStudent();
  void removeStudent();
  void clearStudents();
  void importStudents();
  // 移除学生时不会触发
  void onStudentsChanged(const QModelIndex &idx, const QModelIndex &,
                         const QVector<int> &);

  // students carry meals
  void editMealStu();
  void clearMealStu();
  void importMealStu();

  // students on duty
  void addDutyJob();
  void removeDutyJob();
  void clearStuOnDuty();
  void onDutyJobsEdited(const QModelIndex &idx, const QModelIndex &,
                        const QVector<int> &);
  void onStuOnDutyEdited(const QList<uint> &ls, const int &row,
                         const int &column);

  // lessons
  void addLesson();
  void removeLesson();
  void clearLessons();
  void importLessons();
  void onLessonsChanged(const QModelIndex &idx, const QModelIndex &,
                        const QVector<int> &);

  // notices
  void addNotice();
  void removeNotice();
  void clearNotices();
  void onNoticesEdited(const ClassNotice &notice, const int &row);

  // events
  void addEvent();
  void removeEvent();
  void clearEvents();
  void onEventsEdited(const int &row);
  void onEventsCheckBoxesClicked();

  // toolbar
  void resetPwd();

  void change(const ClassData::Data &before, const QString &text);
  void sync();
  void drop();

  // menubar
  void openFile();
  void saveToFile();

  // local server
  void onReadyRead();
  void onNewConnection();
  void initServer();
  void loadData();

  // check online updates
  void checkAvailableUpdates();
  void onRequestFinished(QNetworkReply *reply);
  bool isGtLocalVer(int major, int minor, int patch);

  // update ClassSystem
  void copyUpdateFiles();

 private:
  Ui::MainWindow ui;

  QList<QPushButton *> m_mealStuBtns;
  QList<QLabel *> m_mealStuLabels;

  const QColor kDutyJobsColor = {240, 240, 240};

  ClassData::Data m_data;
  bool m_changed = false;

  QLocalServer *m_server = new QLocalServer(this);
  QLocalSocket *m_socket = nullptr;
  static constexpr const char kServerName[] = "CLASS-ADMIN-SERVER";
  bool m_connected = false;

  bool m_fileOpened = false;
  QFile m_file;

  bool m_updated = false;

  static constexpr const char *kWindowTitle[] = {
      "ClassSystem 管理后台", "ClassSystem 管理后台 *(未保存)"};

  bool m_loadingData = false;
  bool m_isFirstLoad = true;

  // 正在运行 `change()`
  bool m_doingChange = false;

  // undo & redo
  QUndoStack *m_undoStk = new QUndoStack(this);
  QAction *m_actUndo;
  QAction *m_actRedo;
  QUndoView m_undoView = QUndoView(m_undoStk, nullptr);

  // network
  QNetworkAccessManager *m_nam = new QNetworkAccessManager(this);

 protected:
  void paintEvent(QPaintEvent *ev) override;
  void closeEvent(QCloseEvent *ev) override;
};