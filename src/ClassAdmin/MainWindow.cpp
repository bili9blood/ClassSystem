#include "MainWindow.h"

#include <qbuffer.h>
#include <qmessagebox.h>
#include <qpainter.h>
#include <qstandarditemmodel.h>

#include "Commands.h"
#include "EditMealStuDialog.h"
#include "ImportDialog.h"
#include "ResetPwdDialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  QTableWidget w;
  ui.setupUi(this);

  ui.studentsTable->setColumnWidth(0, 80);
  connect(ui.studentsTable->model(), &QAbstractItemModel::dataChanged, this,
          &MainWindow::onStudentsChanged);

  m_mealStuBtns << ui.btnMealStuMon << ui.btnMealStuTue << ui.btnMealStuWed
                << ui.btnMealStuThur << ui.btnMealStuFri;
  m_mealStuLabels << ui.labelMealStuMon << ui.labelMealStuTue
                  << ui.labelMealStuWed << ui.labelMealStuThur
                  << ui.labelMealStuFri;

  QPalette pa = ui.scrollAreaMealStu->palette();
  pa.setBrush(QPalette::Window, Qt::transparent);
  ui.scrollAreaMealStu->setPalette(pa);

  initServer();

  // init undo stack
  m_actUndo = m_undoStk->createUndoAction(this, "撤销");
  m_actUndo->setIcon(QIcon(":/img/undo.png"));
  m_actUndo->setShortcut(QKeySequence::Undo);
  ui.toolBar->insertAction(ui.actSave, m_actUndo);

  m_actRedo = m_undoStk->createRedoAction(this, "重做");
  m_actRedo->setIcon(QIcon(":/img/redo.png"));
  m_actRedo->setShortcut(QKeySequence::Redo);
  ui.toolBar->insertAction(ui.actSave, m_actRedo);

  ui.toolBar->insertSeparator(ui.actSave);
}

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
    change(before);
  }
}

void MainWindow::clearStudents() {
  ClassData::Data before = m_data;
  if (!ui.studentsTable->rowCount()) return;
  if (m_dataLoaded &&
      QMessageBox::warning(this, "清除学生", "所有的学生将被清除，确认继续？",
                           "确认", "取消"))
    return;
  ui.studentsTable->clearContents();
  while (ui.studentsTable->rowCount()) ui.studentsTable->removeRow(0);
  if (m_dataLoaded) {
    m_data.students.clear();
    change(before);
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
    change(before);
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

  change(before);
}

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
  change(before);
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

  change(before);
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
          change(before);
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
          change(before);
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

void MainWindow::resetPwd() {
  ResetPwdDialog dlg(this);
  dlg.exec();
}

void MainWindow::change(const ClassData::Data &before) {
  m_undoStk->push(new ChangeDataCommand(before, &m_data, this));
  m_changed = true;
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
  m_dataLoaded = true;
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