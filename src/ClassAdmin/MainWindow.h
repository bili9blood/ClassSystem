#pragma once

#include <qgridlayout.h>
#include <qmainwindow.h>
#include <ui_MainWindow.h>

class MainWindow : public QMainWindow, public Ui::MainWindow {
  Q_OBJECT
 public:
  explicit MainWindow(QWidget *parent = nullptr);

 private slots:
  void addStudent();
  void removeStudent();
  void clearStudents();
  void importStudents();
  void resetPwd();

 private:
  bool m_changed = false;

  static constexpr const char *kWindowTitle[] = {
      "ClassSystem 管理后台", "ClassSystem 管理后台 *(未保存)"};

 protected:
  void paintEvent(QPaintEvent *ev) override;
};