#include "MainWindow.h"

#include <qmessagebox.h>
#include <qpainter.h>

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

void MainWindow::importStudents() {}

void MainWindow::paintEvent(QPaintEvent *) {
  m_btnRemoveStudent->setEnabled(m_studentsTable->selectedRanges().size());
  m_btnClearStudents->setEnabled(m_studentsTable->rowCount());
  m_actDrop->setEnabled(m_changed);
  m_actSave->setEnabled(m_changed);
  if (windowTitle() != kWindowTitle[m_changed])
    setWindowTitle(kWindowTitle[m_changed]);
}