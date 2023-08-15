#include "MainWindow.h"

#include <qbuffer.h>
#include <qmessagebox.h>
#include <qpainter.h>

#include "EditMealStuDialog.h"
#include "ImportDialog.h"
#include "ResetPwdDialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  ui.setupUi(this);

  ui.studentsTable->setColumnWidth(0, 80);

  m_mealStuBtns << ui.btnMealStuMon << ui.btnMealStuTue << ui.btnMealStuWed
                << ui.btnMealStuThur << ui.btnMealStuFri;
  m_mealStuLabels << ui.labelMealStuMon << ui.labelMealStuTue
                  << ui.labelMealStuWed << ui.labelMealStuThur
                  << ui.labelMealStuFri;

  QPalette pa = ui.scrollAreaMealStu->palette();
  pa.setBrush(QPalette::Window, Qt::transparent);
  ui.scrollAreaMealStu->setPalette(pa);

  initServer();
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
  if (m_init &&
      QMessageBox::warning(this, "清除学生", "所有的学生将被清除，确认继续？",
                           "确认", "取消"))
    return;
  ui.studentsTable->clearContents();
  while (ui.studentsTable->rowCount()) ui.studentsTable->removeRow(0);
  if (m_init) m_changed = true;
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

void MainWindow::editMealStu() {
  auto btn = qobject_cast<QPushButton *>(sender());
  int idx = m_mealStuBtns.indexOf(btn);
  QString strBefore = m_mealStuLabels[idx]->text();
  EditMealStuDialog dlg(idx, m_data, this);
  if (QDialog::Rejected == dlg.exec()) return;
  QString str;
  std::tie(m_data, str) = dlg.getResult();
  m_mealStuLabels[idx]->setText("%1：%2"_s.arg(oneDayOfWeek(idx)).arg(str));
  m_changed = true;
}

void MainWindow::resetPwd() {
  ResetPwdDialog dlg(this);
  dlg.exec();
}

void MainWindow::saveData() {
  QBuffer b;
  b.open(QBuffer::WriteOnly);
  b.write(kClassAdminSpec, 2);
  QDataStream ds(&b);
  ds << MsgType::Save;

  ClassData::writeTo(m_data, &b);

  m_socket->open(QLocalSocket::ReadWrite);
  m_socket->write(b.data());
  b.close();
  m_changed = false;
}

void MainWindow::dropData() {
  int code = QMessageBox::warning(this, "放弃修改",
                                  "所有将还原被客户端的数据覆盖，是否继续？",
                                  "确认", "取消");
  if (1 == code) return;

  QDataStream ds(m_socket);
  ds << MsgType::Request;
}

void MainWindow::loadData() {
  m_init = false;
  m_changed = false;
  clearStudents();

  // students
  for (const auto &[id, name] : m_data.students.toStdMap()) {
    int row = ui.studentsTable->rowCount();
    ui.studentsTable->insertRow(row);
    ui.studentsTable->setItem(row, 0,
                              new QTableWidgetItem(QString::number(id)));
    ui.studentsTable->setItem(row, 1, new QTableWidgetItem(name));
  }

  // students carry meals
  for (int i = 0; i < m_data.mealStu.size(); ++i) {
    QString str{oneDayOfWeek(i) + "："};
    auto ls = m_data.mealStu[i];
    for (auto it = ls.cbegin(); it != ls.cend(); ++it) {
      str += m_data.idAndName(*it);
      if (it + 1 != ls.cend()) str += ' ';
    }
    m_mealStuLabels[i]->setText(str);
  }
  m_init = true;
}

void MainWindow::onReadyRead() {
  if (kClassAdminSpec == m_socket->read(2)) return;
  ClassData::readFrom(m_socket, m_data, false);

  loadData();
}

void MainWindow::onNewConnection() {
  m_socket = m_server->nextPendingConnection();
  QDataStream ds(m_socket);
  ds << MsgType::Request;
  connect(m_socket, &QLocalSocket::readyRead, this, &MainWindow::onReadyRead);
  connect(m_socket, &QLocalSocket::disconnected, [this] {
    m_socket->deleteLater();
    m_socket = nullptr;
  });
}

void MainWindow::initServer() {
  if (!m_server->listen(kServerName)) {
    QMessageBox::critical(this, "ClassAdmin", "无法监听 Socket ！");
    QApplication::quit();
  }
  m_server->setMaxPendingConnections(1);
  connect(m_server, &QLocalServer::newConnection, this,
          &MainWindow::onNewConnection);
}

void MainWindow::paintEvent(QPaintEvent *) {
  ui.btnRemoveStudent->setEnabled(ui.studentsTable->selectedRanges().size());
  ui.btnClearStudents->setEnabled(ui.studentsTable->rowCount());
  ui.actDrop->setEnabled(m_changed);
  ui.actSave->setEnabled(m_changed);
  if (windowTitle() != kWindowTitle[m_changed])
    setWindowTitle(kWindowTitle[m_changed]);
}