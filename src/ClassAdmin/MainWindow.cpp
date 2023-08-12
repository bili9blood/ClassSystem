#include "MainWindow.h"

#include <qmessagebox.h>
#include <qpainter.h>

#include "ImportDialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) { setupUi(this); }

void MainWindow::addStudent() {
  m_studentsTable->insertRow(m_studentsTable->rowCount());
  m_changed = true;
}

void MainWindow::removeStudent() {
  for (const auto &r : m_studentsTable->selectedRanges())
    for (int i = r.bottomRow(); i >= r.topRow(); --i)
      m_studentsTable->removeRow(i);

  m_changed = true;
}

void MainWindow::clearStudents() {
  if (!m_studentsTable->rowCount()) return;
  if (QMessageBox::warning(this, "清除学生", "所有的学生将被清除，确认继续？",
                           "确认", "取消"))
    return;
  m_studentsTable->clearContents();
  while (m_studentsTable->rowCount()) m_studentsTable->removeRow(0);
  m_changed = true;
}

void MainWindow::importStudents() {
  ImportDialog dlg("1\t张三<br/>2\t李四<br/>3\t王五<br/>", this);
  if (QDialog::Rejected == dlg.exec()) return;
  QStringList students = dlg.getData().remove('\r').split('\n');
  if (students.isEmpty()) return;
  if (m_studentsTable->rowCount() &&
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
    int row;
    m_studentsTable->insertRow(row = m_studentsTable->rowCount());
    m_studentsTable->setItem(row, 0, new QTableWidgetItem(id));
    m_studentsTable->setItem(row, 1, new QTableWidgetItem(name));
  }
  m_changed = true;
}

void MainWindow::paintEvent(QPaintEvent *) {
  m_btnRemoveStudent->setEnabled(m_studentsTable->selectedRanges().size());
  m_btnClearStudents->setEnabled(m_studentsTable->rowCount());
  m_actDrop->setEnabled(m_changed);
  m_actSave->setEnabled(m_changed);
  if (windowTitle() != kWindowTitle[m_changed])
    setWindowTitle(kWindowTitle[m_changed]);
}