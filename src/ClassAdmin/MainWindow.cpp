#include "MainWindow.h"

#include <qmessagebox.h>
#include <qpainter.h>

#include "ImportDialog.h"
#include "ResetPwdDialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  ui.setupUi(this);

  QPalette pa = ui.scrollAreaMealStu->palette();
  pa.setBrush(QPalette::Window, Qt::transparent);
  ui.scrollAreaMealStu->setPalette(pa);
}

void MainWindow::addStudent() {
  ui.studentsTable->insertRow(ui.studentsTable->rowCount());
  m_changed = true;
}

void MainWindow::removeStudent() {
  for (const auto &r : ui.studentsTable->selectedRanges())
    for (int i = r.bottomRow(); i >= r.topRow(); --i)
      ui.studentsTable->removeRow(i);

  m_changed = true;
}

void MainWindow::clearStudents() {
  if (!ui.studentsTable->rowCount()) return;
  if (QMessageBox::warning(this, "清除学生", "所有的学生将被清除，确认继续？",
                           "确认", "取消"))
    return;
  ui.studentsTable->clearContents();
  while (ui.studentsTable->rowCount()) ui.studentsTable->removeRow(0);
  m_changed = true;
}

void MainWindow::importStudents() {
  ImportDialog dlg("1\t张三<br/>2\t李四<br/>3\t王五<br/>", this);
  if (QDialog::Rejected == dlg.exec()) return;
  QStringList students = dlg.getData().remove('\r').split('\n');
  if (students.isEmpty()) return;
  if (ui.studentsTable->rowCount() &&
      !QMessageBox::warning(this, "导入学生", "是否在导入前清除学生列表？",
                            "清除", "保留")) {
    clearStudents();
    m_changed = true;
  }
  for (const QString &stu : students) {
    if (stu.isEmpty()) continue;
    QString id = stu.left(stu.indexOf('\t')),
            name = stu.mid(stu.indexOf('\t') + 1);
    if (id.isEmpty() || name.isEmpty()) continue;

    bool ok;
    id.toUInt(&ok);
    if (!ok) {
      QMessageBox::critical(this, "导入学生",
                            "格式错误：<br/>"
                            "<u style=\"color: red;\">%1</u>\t%2"
                            "<br/>↑<br/>不是正整数！"_s.arg(id)
                                .arg(name));
      return;
    }
    int row = ui.studentsTable->rowCount();
    ui.studentsTable->insertRow(row);
    ui.studentsTable->setItem(row, 0, new QTableWidgetItem(id));
    ui.studentsTable->setItem(row, 1, new QTableWidgetItem(name));
  }
  m_changed = true;
}

void MainWindow::resetPwd() {
  ResetPwdDialog dlg(this);
  dlg.exec();
}

void MainWindow::paintEvent(QPaintEvent *) {
  ui.btnRemoveStudent->setEnabled(ui.studentsTable->selectedRanges().size());
  ui.btnClearStudents->setEnabled(ui.studentsTable->rowCount());
  ui.actDrop->setEnabled(m_changed);
  ui.actSave->setEnabled(m_changed);
  if (windowTitle() != kWindowTitle[m_changed])
    setWindowTitle(kWindowTitle[m_changed]);
}