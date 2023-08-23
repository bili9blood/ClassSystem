#include "MainWindow.h"

#include <qbuffer.h>
#include <qdesktopservices.h>
#include <qmessagebox.h>
#include <qpainter.h>
#include <qstandarditemmodel.h>

#include "Commands.h"
#include "EditMealStuDialog.h"
#include "ImportDialog.h"
#include "ResetPwdDialog.h"
#include "StuOnDutyCellWidget.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  ui.setupUi(this);

  ui.studentsTable->setColumnWidth(0, 80);
  connect(ui.studentsTable->model(), &QAbstractItemModel::dataChanged, this,
          &MainWindow::onStudentsChanged);

  m_mealStuBtns << ui.btnMealStuMon << ui.btnMealStuTue << ui.btnMealStuWed
                << ui.btnMealStuThur << ui.btnMealStuFri;
  m_mealStuLabels << ui.labelMealStuMon << ui.labelMealStuTue
                  << ui.labelMealStuWed << ui.labelMealStuThur
                  << ui.labelMealStuFri;

  connect(ui.stuOnDutyTable->model(), &QAbstractItemModel::dataChanged, this,
          &MainWindow::onDutyJobsEdited);

  QPalette pa = ui.scrollAreaMealStu->palette();
  pa.setBrush(QPalette::Window, Qt::white);
  ui.scrollAreaMealStu->setPalette(pa);
  ui.scrollAreaStuOnDuty->setPalette(pa);

  initServer();

  // init undo stack
  m_actUndo = m_undoStk->createUndoAction(this, "撤销：");
  m_actUndo->setIcon(QIcon(":/img/undo.png"));
  m_actUndo->setShortcut(QKeySequence::Undo /* Ctrl+Z */);
  ui.toolBar->insertAction(ui.actSave, m_actUndo);

  m_actRedo = m_undoStk->createRedoAction(this, "重做：");
  m_actRedo->setIcon(QIcon(":/img/redo.png"));
  m_actRedo->setShortcut(QKeySequence::Redo /* Ctrl+Y */);
  ui.toolBar->insertAction(ui.actSave, m_actRedo);

  ui.toolBar->insertSeparator(ui.actSave);

  m_undoView.setWindowTitle("操作");
  m_undoView.setWindowFlag(Qt::WindowMaximizeButtonHint, false);
  m_undoView.setWindowFlag(Qt::WindowCloseButtonHint, false);

  // menu bar
  connect(ui.actToggleUndoView, &QAction::triggered, &m_undoView,
          &QWidget::setVisible);
  connect(ui.actShowUserDocument, &QAction::triggered, [] {
    QDesktopServices::openUrl({"https://class-system-docs.gitee.io/"});
  });
}

/* ---------------------------------------------------------------- */
/*                         Students Methods                         */
/* ---------------------------------------------------------------- */

void MainWindow::addStudent() {
  ui.studentsTable->insertRow(ui.studentsTable->rowCount());
}

void MainWindow::removeStudent() {
  bool changed = false;
  ClassData::Data before = m_data;
  for (const auto &r : ui.studentsTable->selectedRanges()) {
    for (int i = r.bottomRow(); i >= r.topRow(); --i) {
      if (auto item = ui.studentsTable->item(i, 0))
        m_data.students.remove(item->text().toUInt());
      ui.studentsTable->removeRow(i);
      changed = true;
    }
  }
  if (changed) {
    change(before, "移除学生");
  }
}

void MainWindow::clearStudents() {
  ClassData::Data before = m_data;
  if (!ui.studentsTable->rowCount()) return;
  if (m_dataLoaded &&
      QMessageBox::warning(this, "清除学生", "所有的学生将被清除，确认继续？",
                           "确认", "取消"))
    return;
  while (ui.studentsTable->rowCount()) ui.studentsTable->removeRow(0);
  if (m_dataLoaded) {
    m_data.students.clear();
    change(before, "清除学生");
  }
}

void MainWindow::importStudents() {
  bool changed = false;
  ClassData::Data before = m_data;
  ImportDialog dlg("1\t张三<br/>2\t李四<br/>3\t王五<br/>", this);
  if (QDialog::Rejected == dlg.exec()) return;
  QStringList students = dlg.getData().remove('\r').split('\n');
  if (students.isEmpty()) return;
  if (!QRegExp(R"((\d+\t\S+\s*)+)").exactMatch(dlg.getData())) {
    QMessageBox::critical(this, "导入学生", "格式错误，无法导入！");
    return;
  }
  if (ui.studentsTable->rowCount() &&
      !QMessageBox::warning(this, "导入学生", "是否在导入前清除学生列表？",
                            "清除", "保留")) {
    clearStudents();
  }
  for (const QString &stu : students) {
    if (stu.isEmpty()) continue;
    QString id = stu.left(stu.indexOf('\t')).simplified(),
            name = stu.mid(stu.indexOf('\t') + 1).simplified();
    if (id.isEmpty() || name.isEmpty()) continue;

    uint idNum = id.toUInt();
    int row = ui.studentsTable->rowCount();
    ui.studentsTable->insertRow(row);
    ui.studentsTable->setItem(row, 0, new QTableWidgetItem(id));
    ui.studentsTable->setItem(row, 1, new QTableWidgetItem(name));
    m_data.students[idNum] = name;
    changed = true;
  }
  if (changed) {
    change(before, "导入学生");
  }
}

void MainWindow::onStudentsChanged(const QModelIndex &idx, const QModelIndex &,
                                   const QVector<int> &) {
  ClassData::Data before = m_data;
  if (!m_dataLoaded) return;
  int row = idx.row();
  if (!ui.studentsTable->item(row, 0) || !ui.studentsTable->item(row, 1))
    return;
  m_data.students[ui.studentsTable->item(row, 0)->text().toUInt()] =
      ui.studentsTable->item(row, 1)->text();

  change(before, "编辑学生");
}

/* ---------------------------------------------------------------- */
/*                   Students Carry Meals Methods                   */
/* ---------------------------------------------------------------- */

void MainWindow::editMealStu() {
  ClassData::Data before = m_data;
  auto btn = qobject_cast<QPushButton *>(sender());
  int idx = m_mealStuBtns.indexOf(btn);
  QString strBefore = m_mealStuLabels[idx]->text();
  EditMealStuDialog dlg(idx, m_data, this);
  if (QDialog::Rejected == dlg.exec()) return;
  QString str;
  std::tie(m_data, str) = dlg.getResult();
  m_mealStuLabels[idx]->setText("%1：%2"_s.arg(oneDayOfWeek(idx)).arg(str));
  change(before, "编辑抬饭生");
}

void MainWindow::clearMealStu() {
  ClassData::Data before = m_data;
  int code = QMessageBox::warning(
      this, "清除抬饭生", "所有的抬饭生将被清除，确认继续？", "确认", "取消");
  if (1 == code) return;

  for (int i = 0; i < 5; ++i) {
    m_data.mealStu[i].clear();
    m_mealStuLabels[i]->setText(oneDayOfWeek(i) + "：");
  }

  change(before, "清除抬饭生");
}

void MainWindow::importMealStu() {
  /* ------------------------ constants begin ----------------------- */

  const QString kMsgBoxText = R"(请选择导入模式：
1. 学号模式 如果表格里是抬饭生的学号，请选择此模式。
2. 姓名模式 如果表格里是抬饭生的姓名，请选择此模式。
)";
  const QString kImportTemplates[] = {
      "1\t2\t3\t4\t5\n20\t31\t43\t56\t21",
      "张三\t李四\t王五\t赵六\t孙七\n周八\t吴九\t郑十\t小郝\t小惠"};
  const QString kTemplateSuffix =
      "\n\n<u>共5纵列，代表周一至周五；每一列代表当日的抬饭生。</u>";

  const std::function<void(const QString &)> kFuncs[] = {

      // 学号模式
      [this](QString str) {
        bool changed = false;
        ClassData::Data before = m_data;
        auto lines = str.remove('\r').split('\n');
        if (lines.empty()) return;
        if (!QRegExp(R"(((\d*\t){4}\d*\t*\n*)+)").exactMatch(str)) {
          QMessageBox::critical(this, "导入抬饭生", "格式错误，无法导入！");
          return;
        }

        for (auto it = lines.cbegin(); it != lines.cend(); ++it) {
          uint tmp = 0;
          int i = -1;
          for (const QChar &ch : *it) {
            if (ch == '\t') {
              ++i;
              if (!tmp) continue;
              m_data.mealStu[i] << tmp;

              QString text = m_mealStuLabels[i]->text();
              if (text.size()) text += ' ';
              text += m_data.idAndName(tmp);
              m_mealStuLabels[i]->setText(text);

              tmp = 0;
              changed = true;

              continue;
            }
            tmp = tmp * 10 + (ch.toLatin1() ^ 48);
          }
          if (tmp) {
            m_data.mealStu[4] << tmp;

            QString text = m_mealStuLabels[4]->text();
            if (text.size()) text += ' ';
            text += m_data.idAndName(tmp);
            m_mealStuLabels[4]->setText(text);

            tmp = 0;
            changed = true;
          }
        }
        if (changed) {
          change(before, "导入抬饭生");
        }
      },

      // 姓名模式
      [this](QString str) {
        bool changed = false;
        ClassData::Data before = m_data;
        auto lines = str.remove('\r').split('\n');
        if (lines.empty()) return;
        if (!QRegExp(R"(((\S*\t){4}\S*\t*\n*)+)").exactMatch(str)) {
          QMessageBox::critical(this, "导入抬饭生", "格式错误，无法导入！");
          return;
        }

        for (auto it = lines.cbegin(); it != lines.cend(); ++it) {
          QString tmp;
          int i = -1;
          for (const QChar &ch : *it) {
            if (ch == '\t') {
              ++i;
              tmp = tmp.simplified();
              if (tmp.isEmpty()) continue;
              uint id = m_data.students.key(tmp);
              m_data.mealStu[i] << id;

              QString text = m_mealStuLabels[i]->text();
              if (text.size()) text += ' ';
              text += m_data.idAndName(id);
              m_mealStuLabels[i]->setText(text);

              tmp.clear();
              changed = true;

              continue;
            }
            tmp += ch;
          }
          tmp = tmp.simplified();
          if (tmp.size()) {
            uint id = m_data.students.key(tmp);
            m_data.mealStu[4] << id;

            QString text = m_mealStuLabels[4]->text();
            if (text.size()) text += ' ';
            text += m_data.idAndName(id);
            m_mealStuLabels[4]->setText(text);

            changed = true;
          }
        }
        if (changed) {
          change(before, "导入抬饭生");
        }
      }};

  /* ------------------------- constants end ------------------------ */

  bool empty = true;
  for (const auto &ls : m_data.mealStu) {
    if (ls.size()) {
      empty = false;
      break;
    }
  }

  if (!empty &&
      !QMessageBox::warning(this, "导入抬饭生", "是否在导入前清除所有抬饭生？",
                            "清除", "保留")) {
    clearMealStu();
  }

  int mode = QMessageBox::information(this, "导入抬饭生", kMsgBoxText,
                                      "学号模式", "姓名模式");
  ImportDialog dlg(kImportTemplates[mode] + kTemplateSuffix, this);
  if (QDialog::Rejected == dlg.exec()) return;

  kFuncs[mode](dlg.getData());
}

/* ---------------------------------------------------------------- */
/*                     Students On Duty Methods                     */
/* ---------------------------------------------------------------- */

void MainWindow::addDutyJob() {
  int row = ui.stuOnDutyTable->rowCount();
  ui.stuOnDutyTable->insertRow(row);

  ClassData::Data before = m_data;

  auto &jobs = m_data.dutyJobs;
  while (jobs.size() <= row) jobs << "";
  for (int i = 0; i < 5; ++i) {
    auto &ls = m_data.stuOnDuty[i];
    while (ls.size() <= row) ls << QList<uint>();
  }

  auto jobItem = new QTableWidgetItem;
  jobItem->setBackground(kDutyJobsColor);
  jobItem->setFlags(jobItem->flags() | Qt::ItemIsEditable);
  ui.stuOnDutyTable->setItem(row, 0, jobItem);

  for (int i = 0; i < 5; ++i) {
    auto widget = new StuOnDutyCellWidget(row, i + 1, m_data.stuOnDuty[i][row],
                                          ui.stuOnDutyTable);
    connect(widget, &StuOnDutyCellWidget::edited, this,
            &MainWindow::onStuOnDutyEdited);
    ui.stuOnDutyTable->setCellWidget(row, i + 1, widget);
  }
  change(before, "新增值日职位");
}

void MainWindow::removeDutyJob() {
  int row = ui.stuOnDutyTable->currentRow();
  if (-1 == row) return;

  ClassData::Data before = m_data;

  ui.stuOnDutyTable->removeRow(row);

  m_data.dutyJobs.removeAt(row);
  for (int i = 0; i < 5; ++i) {
    m_data.stuOnDuty[i].removeAt(row);
  }

  change(before, "移除值日职位");
}

void MainWindow::clearStuOnDuty() {
  if (m_data.dutyJobs.empty()) return;

  if (m_dataLoaded) {
    int code = QMessageBox::warning(
        this, "清除值日生", "所有值日生将被清除，是否继续？", "确认", "取消");
    if (code) return;
  }

  ClassData::Data before = m_data;

  // clear table
  while (ui.stuOnDutyTable->rowCount()) ui.stuOnDutyTable->removeRow(0);

  // clear data
  m_data.dutyJobs.clear();
  for (int i = 0; i < 5; ++i) m_data.stuOnDuty[i].clear();

  change(before, "清除值日生");
}

void MainWindow::onDutyJobsEdited(const QModelIndex &idx, const QModelIndex &,
                                  const QVector<int> &) {
  if (idx.column() || !m_dataLoaded) return;
  ClassData::Data before = m_data;
  auto &jobs = m_data.dutyJobs;
  int row = idx.row();

  while (jobs.size() <= row) jobs << "";
  for (int i = 0; i < 5; ++i) {
    auto &ls = m_data.stuOnDuty[i];
    while (ls.size() <= row) ls << QList<uint>();
  }

  if (auto item = ui.stuOnDutyTable->item(row, 0))
    m_data.dutyJobs[row] = item->text();
  change(before, "编辑值日职位");
}

void MainWindow::onStuOnDutyEdited(const QList<uint> &ls, const int &row,
                                   const int &column) {
  ClassData::Data before = m_data;
  m_data.stuOnDuty[column - 1 /* 星期几 */][row] = ls;
  change(before, "编辑值日生");
}

/* ---------------------------------------------------------------- */
/*                          ToolBar Methods                         */
/* ---------------------------------------------------------------- */

void MainWindow::resetPwd() {
  ResetPwdDialog dlg(this);
  dlg.exec();
}

void MainWindow::change(const ClassData::Data &before, const QString &text) {
  QString textWithTime =
      "%1 %2"_s.arg(QTime::currentTime().toString("hh:mm"), text);

  m_undoStk->push(new ChangeDataCommand(before, this, textWithTime));
  m_changed = true;
  update();
}

void MainWindow::save() {
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

void MainWindow::drop() {
  m_undoStk->clear();
  int code = QMessageBox::warning(this, "放弃修改",
                                  "所有将还原被客户端的数据覆盖，是否继续？",
                                  "确认", "取消");
  if (1 == code) return;

  QDataStream ds(m_socket);
  ds << MsgType::Request;
}

void MainWindow::loadData() {
  m_dataLoaded = false;
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

  // students on duty
  for (int row = ui.stuOnDutyTable->rowCount(); row < m_data.dutyJobs.size();
       ++row)
    ui.stuOnDutyTable->insertRow(row);

  for (int i = 0; i < m_data.dutyJobs.size(); ++i) {
    auto jobItem = new QTableWidgetItem(m_data.dutyJobs[i]);
    jobItem->setBackground(kDutyJobsColor);
    jobItem->setFlags(jobItem->flags() | Qt::ItemIsEditable);
    ui.stuOnDutyTable->setItem(i, 0, jobItem);

    for (int j = 0; j < 5; ++j) {
      auto widget = new StuOnDutyCellWidget(i, j + 1, m_data.stuOnDuty[j][i],
                                            ui.stuOnDutyTable);
      connect(widget, &StuOnDutyCellWidget::edited, this,
              &MainWindow::onStuOnDutyEdited);
      ui.stuOnDutyTable->setCellWidget(i, j + 1, widget);
    }
  }

  m_dataLoaded = true;
}

/* ---------------------------------------------------------------- */
/*                 Init QLocalServer & QLocalSocket                 */
/* ---------------------------------------------------------------- */

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

/* ---------------------------------------------------------------- */
/*                              Events                              */
/* ---------------------------------------------------------------- */

void MainWindow::paintEvent(QPaintEvent *) {
  // students
  ui.btnRemoveStudent->setEnabled(ui.studentsTable->selectedRanges().size());
  ui.btnClearStudents->setEnabled(ui.studentsTable->rowCount());

  // students on duty
  if (m_data.dutyJobs.empty()) {
    // show label, hide table

    ui.labelNoDutyJobs->show();
    ui.labelNoDutyJobs->setSizePolicy(QSizePolicy::Ignored,
                                      QSizePolicy::Ignored);

    ui.stuOnDutyTable->hide();
    ui.stuOnDutyTable->setSizePolicy(QSizePolicy::Preferred,
                                     QSizePolicy::Preferred);
  } else {
    // hide label, show table

    ui.labelNoDutyJobs->hide();
    ui.labelNoDutyJobs->setSizePolicy(QSizePolicy::Preferred,
                                      QSizePolicy::Preferred);

    ui.stuOnDutyTable->show();
    ui.stuOnDutyTable->setSizePolicy(QSizePolicy::Ignored,
                                     QSizePolicy::Ignored);
  }

  // toolbar
  ui.actDrop->setEnabled(m_changed);
  ui.actSave->setEnabled(m_changed);

  // title
  if (windowTitle() != kWindowTitle[m_changed])
    setWindowTitle(kWindowTitle[m_changed]);
}