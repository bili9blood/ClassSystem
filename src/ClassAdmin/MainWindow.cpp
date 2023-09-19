#include "MainWindow.h"

// ui
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qprogressdialog.h>
#include <qsharedmemory.h>
#include <qstandarditemmodel.h>

#include "EditMealStuDialog.h"
#include "ImportDialog.h"
#include "NoticesCellWidget.h"
#include "ResetPwdDialog.h"
#include "StuOnDutyCellWidget.h"

// other
#include <qbuffer.h>
#include <qdesktopservices.h>
#include <qprocess.h>

#include "Commands.h"
#include "ItemDelegates.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  ui.setupUi(this);

  // students
  ui.studentsTable->setColumnWidth(0, 80);
  ui.studentsTable->setItemDelegateForColumn(0, new IntDelegate);
  connect(ui.studentsTable->model(), &QAbstractItemModel::dataChanged, this,
          &MainWindow::onStudentsChanged);

  // students carry meals
  m_mealStuBtns << ui.btnMealStuMon << ui.btnMealStuTue << ui.btnMealStuWed
                << ui.btnMealStuThur << ui.btnMealStuFri;
  m_mealStuLabels << ui.labelMealStuMon << ui.labelMealStuTue
                  << ui.labelMealStuWed << ui.labelMealStuThur
                  << ui.labelMealStuFri;

  // studnts on duty
  ui.stuOnDutyTable->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);
  connect(ui.stuOnDutyTable->model(), &QAbstractItemModel::dataChanged, this,
          &MainWindow::onDutyJobsEdited);

  // lessons
  ui.lessonsTable->setItemDelegateForColumn(0, new TimeDelegate);
  connect(ui.lessonsTable->model(), &QAbstractItemModel::dataChanged, this,
          &MainWindow::onLessonsChanged);

  // notices
  ui.noticesTable->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);

  // events
  ui.eventsTable->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);
  ui.eventsTable->setItemDelegateForColumn(1, new DateDelegate);
  connect(ui.eventsTable->model(), &QAbstractItemModel::dataChanged,
          [this](const QModelIndex &idx) {
            if (m_loadingData) return;

            onEventsEdited(idx.row());
          });

  initServer();

  // init undo stack
  m_actUndo = m_undoStk->createUndoAction(this, "撤销：");
  m_actUndo->setIcon(QIcon(":/img/undo.png"));
  m_actUndo->setShortcut(QKeySequence::Undo /* Ctrl+Z */);
  ui.toolBar->insertAction(ui.actSync, m_actUndo);

  m_actRedo = m_undoStk->createRedoAction(this, "重做：");
  m_actRedo->setIcon(QIcon(":/img/redo.png"));
  m_actRedo->setShortcut(QKeySequence::Redo /* Ctrl+Y */);
  ui.toolBar->insertAction(ui.actSync, m_actRedo);

  ui.toolBar->insertSeparator(ui.actSync);

  m_undoView.setWindowTitle("操作");
  m_undoView.setWindowFlag(Qt::WindowMaximizeButtonHint, false);
  m_undoView.setWindowFlag(Qt::WindowCloseButtonHint, false);

  // menu bar
  connect(ui.actToggleUndoView, &QAction::triggered, &m_undoView,
          &QWidget::setVisible);
  connect(ui.actShowUserDocument, &QAction::triggered, [] {
    QDesktopServices::openUrl({"https://class-system-docs.gitee.io/"});
  });

  checkAvailableUpdates();

  // 注册按下 Delete 键删除的功能
#define PER_TABLE(x, y)                  \
  ui.x##Table->installEventFilter(this); \
  m_tablesRemoveFuncMap[ui.x##Table] = std::bind(&MainWindow::remove##y, this);

  PER_TABLE(students, Student)
  PER_TABLE(stuOnDuty, DutyJob)
  PER_TABLE(lessons, Lesson)
  PER_TABLE(notices, Notice)
  PER_TABLE(events, Event)

#undef PER_TABLE
}

/* ---------------------------------------------------------------- */
/*                         Students Methods                         */
/* ---------------------------------------------------------------- */

void MainWindow::addStudent() {
  ui.studentsTable->setRowCount(ui.studentsTable->rowCount() + 1);
}

void MainWindow::removeStudent() {
  bool changed = false;
  ClassData::Data before = classData;
  for (const auto &r : ui.studentsTable->selectedRanges()) {
    for (int i = r.bottomRow(); i >= r.topRow(); --i) {
      if (auto item = ui.studentsTable->item(i, 0))
        classData.students.remove(item->text().toUInt());
      ui.studentsTable->removeRow(i);
      changed = true;
    }
  }
  if (changed) {
    change(before, "移除学生");
  }
}

void MainWindow::clearStudents() {
  ClassData::Data before = classData;
  if (!ui.studentsTable->rowCount()) return;
  if (!m_loadingData &&
      QMessageBox::warning(this, "清除学生", "所有的学生将被清除，确认继续？",
                           "确认", "取消"))
    return;
  while (ui.studentsTable->rowCount()) ui.studentsTable->removeRow(0);
  if (!m_loadingData) {
    classData.students.clear();
    change(before, "清除学生");
  }
}

void MainWindow::importStudents() {
  bool changed = false;
  ClassData::Data before = classData;
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
    QString id = stu.left(stu.indexOf('\t')),
            name = stu.mid(stu.indexOf('\t') + 1).trimmed();
    if (id.isEmpty() || name.isEmpty()) continue;

    uint idNum = id.toUInt();
    int row = ui.studentsTable->rowCount();
    ui.studentsTable->insertRow(row);
    ui.studentsTable->setItem(row, 0, new QTableWidgetItem(id));
    ui.studentsTable->setItem(row, 1, new QTableWidgetItem(name));
    classData.students[idNum] = name;
    changed = true;
  }
  if (changed) {
    change(before, "导入学生");
  }
}

void MainWindow::onStudentsChanged(const QModelIndex &idx, const QModelIndex &,
                                   const QVector<int> &) {
  ClassData::Data before = classData;
  if (m_loadingData) return;
  int row = idx.row();

  uint id = 0;
  QString name;

  if (ui.studentsTable->item(row, 0))
    id = ui.studentsTable->item(row, 0)->data(Qt::DisplayRole).toUInt();

  if (ui.studentsTable->item(row, 1))
    name = ui.studentsTable->item(row, 1)->text();

  classData.students[id] = name;

  change(before, "编辑学生");
}

/* ---------------------------------------------------------------- */
/*                   Students Carry Meals Methods                   */
/* ---------------------------------------------------------------- */

void MainWindow::editMealStu() {
  ClassData::Data before = classData;
  auto btn = qobject_cast<QPushButton *>(sender());
  int idx = m_mealStuBtns.indexOf(btn);
  EditMealStuDialog dlg(idx, classData, this);
  if (QDialog::Rejected == dlg.exec()) return;
  QString str;
  std::tie(classData, str) = dlg.getResult();
  m_mealStuLabels[idx]->setText("%1：%2"_s.arg(oneDayOfWeek(idx), str));
  change(before, "编辑抬饭生");
}

void MainWindow::clearMealStu() {
  ClassData::Data before = classData;
  int code = QMessageBox::warning(
      this, "清除抬饭生", "所有的抬饭生将被清除，确认继续？", "确认", "取消");
  if (1 == code) return;

  for (int i = 0; i < 5; ++i) {
    classData.mealStu[i].clear();
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
        ClassData::Data before = classData;
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
              classData.mealStu[i] << tmp;

              QString text = m_mealStuLabels[i]->text();
              if (text.size()) text += ' ';
              text += classData.idAndName(tmp);
              m_mealStuLabels[i]->setText(text);

              tmp = 0;
              changed = true;

              continue;
            }
            tmp = tmp * 10 + (ch.toLatin1() ^ 48);
          }
          if (tmp) {
            classData.mealStu[4] << tmp;

            QString text = m_mealStuLabels[4]->text();
            if (text.size()) text += ' ';
            text += classData.idAndName(tmp);
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
        ClassData::Data before = classData;
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
              tmp = tmp.trimmed();
              if (tmp.isEmpty()) continue;
              uint id = classData.students.key(tmp);
              classData.mealStu[i] << id;

              QString text = m_mealStuLabels[i]->text();
              if (text.size()) text += ' ';
              text += classData.idAndName(id);
              m_mealStuLabels[i]->setText(text);

              tmp.clear();
              changed = true;

              continue;
            }
            tmp += ch;
          }
          tmp = tmp.trimmed();
          if (tmp.size()) {
            uint id = classData.students.key(tmp);
            classData.mealStu[4] << id;

            QString text = m_mealStuLabels[4]->text();
            if (text.size()) text += ' ';
            text += classData.idAndName(id);
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
  for (const auto &ls : classData.mealStu) {
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
  ui.stuOnDutyTable->setRowCount(row + 1);

  ClassData::Data before = classData;

  auto &jobs = classData.dutyJobs;
  while (jobs.size() <= row) jobs << "";
  for (int i = 0; i < 5; ++i) {
    auto &ls = classData.stuOnDuty[i];
    while (ls.size() <= row) ls << QList<uint>();
  }

  auto jobItem = new QTableWidgetItem;
  jobItem->setBackground(kDutyJobsColor);
  jobItem->setForeground(Qt::black);
  jobItem->setFlags(jobItem->flags() | Qt::ItemIsEditable);
  ui.stuOnDutyTable->setItem(row, 0, jobItem);

  for (int i = 0; i < 5; ++i) {
    auto widget = new StuOnDutyCellWidget(row, i + 1, ui.stuOnDutyTable);
    connect(widget, &StuOnDutyCellWidget::edited, this,
            &MainWindow::onStuOnDutyEdited);
    ui.stuOnDutyTable->setCellWidget(row, i + 1, widget);
  }
  change(before, "新增值日职位");
}

void MainWindow::removeDutyJob() {
  int row = ui.stuOnDutyTable->currentRow();
  if (-1 == row) return;

  ClassData::Data before = classData;

  ui.stuOnDutyTable->removeRow(row);

  classData.dutyJobs.removeAt(row);
  for (int i = 0; i < 5; ++i) {
    classData.stuOnDuty[i].removeAt(row);
  }

  loadData();
  change(before, "移除值日职位");
}

void MainWindow::clearStuOnDuty() {
  if (classData.dutyJobs.empty()) return;

  if (!m_loadingData) {
    int code = QMessageBox::warning(
        this, "清除值日生", "所有值日生将被清除，是否继续？", "确认", "取消");
    if (code) return;
  }

  ClassData::Data before = classData;

  // clear table
  while (ui.stuOnDutyTable->rowCount()) ui.stuOnDutyTable->removeRow(0);

  // clear data
  classData.dutyJobs.clear();
  for (int i = 0; i < 5; ++i) classData.stuOnDuty[i].clear();

  change(before, "清除值日生");
}

void MainWindow::onDutyJobsEdited(const QModelIndex &idx, const QModelIndex &,
                                  const QVector<int> &) {
  if (idx.column() || m_loadingData) return;
  ClassData::Data before = classData;
  auto &jobs = classData.dutyJobs;
  int row = idx.row();

  while (jobs.size() <= row) jobs << "";
  for (int i = 0; i < 5; ++i) {
    auto &ls = classData.stuOnDuty[i];
    while (ls.size() <= row) ls << QList<uint>();
  }

  if (auto item = ui.stuOnDutyTable->item(row, 0))
    classData.dutyJobs[row] = item->text();
  change(before, "编辑值日职位");
}

void MainWindow::onStuOnDutyEdited(const QList<uint> &ls, const int &row,
                                   const int &column) {
  ClassData::Data before = classData;
  classData.stuOnDuty[column - 1 /* 星期几 */][row] = ls;
  change(before, "编辑值日生");
}

/* ---------------------------------------------------------------- */
/*                          Lessons Methods                         */
/* ---------------------------------------------------------------- */

void MainWindow::addLesson() {
  int row = ui.lessonsTable->rowCount();
  if (int cRow = ui.lessonsTable->currentRow(); cRow != -1) row = cRow + 1;

  m_loadingData = true;

  ui.lessonsTable->setRowCount(row + 1);

  ui.lessonsTable->setItem(row, 0, new QTableWidgetItem);

  ClassData::Data before = classData;

  while (classData.lessonsTm.size() <= row) classData.lessonsTm << QTime();
  for (int i = 0; i < 5; ++i) {
    ui.lessonsTable->setItem(row, i, new QTableWidgetItem);

    auto &ls = classData.lessons[i];
    while (ls.size() <= row) ls << QString();
  }

  m_loadingData = false;
  change(before, "添加课节");
}

void MainWindow::removeLesson() {
  int row = ui.lessonsTable->currentRow();
  if (row == -1) return;

  ui.lessonsTable->removeRow(row);

  ClassData::Data before = classData;

  classData.lessonsTm.removeAt(row);
  auto &ls = classData.lessons;
  for (int i = 0; i < 5; ++i) {
    ls[i].removeAt(row);
  }

  change(before, "移除第%1节课"_s.arg(row + 1));
}

void MainWindow::clearLessons() {
  if (!m_loadingData) {
    int code = QMessageBox::warning(
        this, "清空课程", "所有课程将被清除，是否继续？", "确认", "取消");
    if (1 == code) return;
  }

  for (int i = classData.lessonsTm.size() - 1; i >= 0; --i)
    ui.lessonsTable->removeRow(i);

  if (m_loadingData) return;

  ClassData::Data before = classData;

  classData.lessonsTm.clear();
  for (int i = 0; i < 5; ++i) {
    classData.lessons[i].clear();
  }

  if (!m_loadingData) change(before, "清空课程");
}

void MainWindow::importLessons() {
  QString templateStr = R"(8:50	班会	数学	语文	生物	地理
9:40	语文	物理	体育	英语	政治
10:30	英语	历史	历史	语文	数学

<i>时间中间可以是英文冒号也可以是中文冒号。</i>
)";

  ImportDialog dlg(templateStr, this);
  bool changed = false;
  int code = dlg.exec();
  if (code == QDialog::Rejected) return;

  QString str = dlg.getData();
  if (str.trimmed().isEmpty()) return;
  if (!QRegExp(R"((\d{1,2}[:：]\d{1,2}\t(\S+\t){4}\S+\s*)+)").exactMatch(str)) {
    QMessageBox::critical(this, "导入课程", "格式错误，无法导入!");
    return;
  }

  clearLessons();

  m_loadingData = true;

  ClassData::Data before = classData;

  QStringList strLs = str.simplified().split(' ');

  constexpr int kColumnCount = 6;  // 1 (时间) + 5 (周一 ~ 周五) == 6
  int rowCount = strLs.size() / kColumnCount;

  ui.lessonsTable->setRowCount(rowCount);

  while (classData.lessonsTm.size() < rowCount) classData.lessonsTm << QTime();

  for (int i = 0; i < 5; ++i) {
    auto &ls = classData.lessons[i];
    while (ls.size() < rowCount) ls << QString();
  }

  for (int i = 0; i < rowCount; ++i) {
    QString tmStr = strLs[i * kColumnCount];

    QTime time;

    QListWidget w;

    if (QRegExp(R"(\d{1,2}:\d{1,2})").exactMatch(tmStr)) {
      classData.lessonsTm[i] = time = QTime::fromString(tmStr, "h:m");
    } else {  // 中文冒号
      classData.lessonsTm[i] = time = QTime::fromString(tmStr, "h：m");
    }

    auto timeItem = new QTableWidgetItem;
    timeItem->setData(Qt::DisplayRole, time);
    ui.lessonsTable->setItem(i, 0, timeItem);

    for (int j = 1; j < kColumnCount; ++j) {
      QString lesson = strLs[i * kColumnCount + j];
      classData.lessons[j - 1][i] = lesson;
      ui.lessonsTable->setItem(i, j, new QTableWidgetItem(lesson));
      changed = true;
    }
  }
  m_loadingData = false;

  if (changed) change(before, "导入课程");
}

void MainWindow::onLessonsChanged(const QModelIndex &idx, const QModelIndex &,
                                  const QVector<int> &) {
  if (m_loadingData) return;
  int row = idx.row(), col = idx.column();
  ClassData::Data before;
  if (row == 0) {  // lessonsTm changed
    classData.lessonsTm[row] =
        ui.lessonsTable->item(row, 0)->data(Qt::DisplayRole).toTime();

    change(before, "编辑第%1节课时间"_s.arg(row + 1));
  } else {  // lessons changed
    classData.lessons[col - 1][row] = ui.lessonsTable->item(row, col)->text();

    change(before, "编辑%1第%2节课"_s.arg(oneDayOfWeek(col - 1)).arg(row + 1));
  }
}

/* ---------------------------------------------------------------- */
/*                          Notices Methods                         */
/* ---------------------------------------------------------------- */

void MainWindow::addNotice() {
  ClassData::Data before = classData;

  ClassNotice notice;
  classData.notices << notice;

  int row = ui.noticesTable->rowCount();
  ui.noticesTable->setRowCount(row + 1);

  auto w = new NoticesCellWidget(notice, row, ui.noticesTable);
  connect(w, &NoticesCellWidget::edited, this, &MainWindow::onNoticesEdited);
  ui.noticesTable->setCellWidget(row, 0, w);

  change(before, "添加公告");
}

void MainWindow::removeNotice() {
  int row = ui.noticesTable->currentRow();
  if (row == -1) return;

  ui.noticesTable->removeRow(row);

  ClassData::Data before = classData;

  classData.notices.removeAt(row);

  loadData();
  change(before, "移除公告");
}

void MainWindow::clearNotices() {
  if (!m_loadingData) {
    int code = QMessageBox::warning(
        this, "清空公告", "所有公告将被清空，是否继续？", "确认", "取消");
    if (1 == code) return;
  }

  for (int i = ui.noticesTable->rowCount() - 1; i >= 0; --i) {
    ui.noticesTable->removeRow(i);
  }

  if (!m_loadingData) {
    ClassData::Data before = classData;
    classData.notices.clear();
    change(before, "清空公告");
  }
}

void MainWindow::onNoticesEdited(const ClassNotice &notice, const int &row) {
  if (m_loadingData) return;

  ClassData::Data before;
  classData.notices[row] = notice;
  change(before, "编辑公告");
}

/* ---------------------------------------------------------------- */
/*                          Events Methods                          */
/* ---------------------------------------------------------------- */

void MainWindow::addEvent() {
  int cnt = ui.eventsTable->rowCount();
  ui.eventsTable->setRowCount(cnt + 1);

  m_loadingData = true;

  ui.eventsTable->setItem(cnt, 0, new QTableWidgetItem);

  auto dateItem = new QTableWidgetItem;
  dateItem->setData(Qt::DisplayRole, QDate());
  ui.eventsTable->setItem(cnt, 1, dateItem);

  ui.eventsTable->setCellWidget(cnt, 2, new QCheckBox);

  m_loadingData = false;

  ClassData::Data before = classData;

  classData.events.push_back({});

  loadData();
  change(before, "添加事件");
}

void MainWindow::removeEvent() {
  int row = ui.eventsTable->currentRow();
  if (row == -1) return;
  ui.eventsTable->removeRow(row);

  ClassData::Data before = classData;

  classData.events.removeAt(row);

  change(before, "移除事件");
}

void MainWindow::clearEvents() {
  if (!m_loadingData) {
    int code = QMessageBox::warning(
        this, "清空事件", "所有的事件将被清空，是否继续？", "确认", "取消");
    if (1 == code) return;
  }

  for (int i = ui.eventsTable->rowCount(); i >= 0; --i) {
    ui.eventsTable->removeRow(i);
  }

  if (m_loadingData) return;

  ClassData::Data before = classData;
  classData.events.clear();
  change(before, "清空事件");
}

void MainWindow::onEventsEdited(const int &row) {
  if (m_loadingData) return;

  ClassData::Data before = classData;
  classData.events[row] = {
      ui.eventsTable->item(row, 0)->text(),
      ui.eventsTable->item(row, 1)->data(Qt::DisplayRole).toDate(),
      qobject_cast<QCheckBox *>(ui.eventsTable->cellWidget(row, 2))
          ->isChecked()};

  change(before, "编辑事件");
}

void MainWindow::onEventsCheckBoxesClicked() {
  if (m_loadingData) return;

  for (int i = 0; i < ui.eventsTable->rowCount(); ++i) {
    if (ui.eventsTable->cellWidget(i, 2) == sender()) {
      onEventsEdited(i);
      return;
    }
  }
}

/* ---------------------------------------------------------------- */
/*                           Temp Methods                           */
/* ---------------------------------------------------------------- */

void MainWindow::addTemp() {}

void MainWindow::removeTemp(QTableWidget *table) {}

void MainWindow::removeTemp() {
  auto ptr = sender();

#define PER_TEMP(x)                 \
  if (ptr == ui.btnRemoveTemp##x) { \
    removeTemp(ui.temp##x##Table);  \
    return;                         \
  }

#define PER_TEMP_2ND(x, y)          \
  if (ptr == ui.btnRemoveTemp##x) { \
    removeTemp(ui.temp##y##Table);  \
    return;                         \
  }

  PER_TEMP(MealStu)
  PER_TEMP(StuOnDuty)
  PER_TEMP_2ND(Lesson, Lessons)

#undef PER_TEMP
#undef PER_TEMP_2ND
}

void MainWindow::clearTemps() {}

/* ---------------------------------------------------------------- */
/*                          ToolBar Methods                         */
/* ---------------------------------------------------------------- */

void MainWindow::resetPwd() {
  ResetPwdDialog dlg(this);
  dlg.exec();
}

void MainWindow::change(const ClassData::Data &before, const QString &text) {
  m_doingChange = true;

  QString textWithTime =
      "%1 %2"_s.arg(QTime::currentTime().toString("hh:mm"), text);

  m_undoStk->push(new ChangeDataCommand(before, this, textWithTime));

  m_doingChange = false;
  m_changed = true;
  update();
}

void MainWindow::sync() {
  if (!m_connected) {
    QMessageBox::warning(this, "同步",
                         "未连接到 ClassSystem，无法进行同步！"
                         "<br/><br />"
                         "建议：将修改保存到文件，待连接到 ClassSystem "
                         "后再将文件内容同步。");
    return;
  }

  QBuffer b;
  b.open(QBuffer::WriteOnly);
  QDataStream ds(&b);
  ds << MsgType::Save;

  ClassData::writeTo(classData, &b);

  m_socket->open(QLocalSocket::ReadWrite);
  m_socket->write(b.data());
  b.close();
  m_changed = false;
}

void MainWindow::drop() {
  const auto getFromClassSystem = [this] {
    int code = QMessageBox::warning(
        this, "放弃", "所有修改将被 ClassSystem 的数据覆盖，是否继续？", "确认",
        "取消");
    if (1 == code) return;

    m_undoStk->clear();
    QDataStream ds(m_socket);
    ds << MsgType::Request;
  };

  const auto getFromFile = [this] {
    int code = QMessageBox::warning(
        this, "放弃",
        "所有修改将被 %1 的数据覆盖，是否继续？"_s.arg(m_file.fileName()),
        "确认", "取消");
    if (1 == code) return;

    m_undoStk->clear();
    m_file.open(QFile::ReadWrite);
    ClassData::readFrom(&m_file, classData);
    m_file.close();
    loadData();
  };

  const auto clear = [this] {
    int code = QMessageBox::warning(this, "放弃", "所有修改将清空，是否继续？",
                                    "确认", "取消");
    if (1 == code) return;

    m_undoStk->clear();
    classData = ClassData::Data();
    loadData();
  };

  if (m_connected && !m_fileOpened)
    getFromClassSystem();
  else if (!m_connected && m_fileOpened)
    getFromFile();
  else if (!m_connected && !m_fileOpened)
    clear();
  else  // m_connected && m_fileOpened
    (0 == QMessageBox::information(this, "放弃", "请选择重新加载的数据来源：",
                                   "ClassSystem",
                                   "打开的文件：" + m_file.fileName()))
        ? getFromClassSystem()
        : getFromFile();
}

/* ---------------------------------------------------------------- */
/*                          MenuBar Methods                         */
/* ---------------------------------------------------------------- */

void MainWindow::openFile() {
  QString fileName = QFileDialog::getOpenFileName(this, "打开文件", {},
                                                  "ClassData 数据文件 (*.stm)");
  if (fileName.isEmpty()) return;
  m_file.setFileName(fileName);

  m_file.open(QFile::ReadWrite);
  ClassData::readFrom(&m_file, classData);
  m_file.close();
  m_fileOpened = true;

  loadData();
}

void MainWindow::saveToFile() {
  if (m_fileOpened) {
    m_file.open(QFile::WriteOnly | QFile::Truncate);
    ClassData::writeTo(classData, &m_file);
  } else {  // !m_fileOpened
    QString fileName = QFileDialog::getSaveFileName(
        this, "保存为文件", "data.stm", "ClassData 数据文件 (*.stm)");
    if (fileName.isEmpty()) return;

    QFile saveFile(fileName);

    saveFile.open(QFile::WriteOnly | QFile::Truncate);
    ClassData::writeTo(classData, &saveFile);
  }

  m_changed = false;
  update();
}

/* ---------------------------------------------------------------- */
/*                      Check Available Updates                     */
/* ---------------------------------------------------------------- */

void MainWindow::checkAvailableUpdates() {
  constexpr char remoteVerUrl[] =
      R"(https://gitee.com/class-system-docs/ClassSystem/raw/master/version.txt)";

  connect(m_nam, &QNetworkAccessManager::finished, this,
          &MainWindow::onRequestFinished);
  m_nam->get(QNetworkRequest({remoteVerUrl}));
}

void MainWindow::onRequestFinished(QNetworkReply *reply) {
  auto err = reply->error();

  if (err != QNetworkReply::NoError) {
    QMessageBox::critical(
        this, "检查更新",
        "无法检查更新！<br/><br/>原因：" + reply->errorString());
    return;
  }

  // 无错误，读取远程版本并对比
  QTextStream ts(reply);
  auto remoteVer = ts.readLine().toUtf8();
  QUrl downloadUrl(ts.readLine());

  int major, minor, patch;

  sscanf_s(remoteVer, "v%d.%d.%d", &major, &minor, &patch);

  if (!isGtLocalVer(major, minor, patch)) {
    qInfo("Update: Everything up-to-date.");
    return;
  }

  qInfo("Update: New version found. Local: v%d.%d.%d Remote: %s",
        PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH,
        remoteVer.constData());
  QFile file(":/text/update-info-msg.md");
  file.open(QFile::ReadOnly);

  QMessageBox msg(this);
  msg.setWindowTitle("检查更新");
  msg.setText(QString(file.readAll())
                  .arg("v%1.%2.%3"_s.arg(PROJECT_VERSION_MAJOR)
                           .arg(PROJECT_VERSION_MINOR)
                           .arg(PROJECT_VERSION_PATCH))
                  .arg(QString::fromUtf8(remoteVer)));
  msg.setTextFormat(Qt::MarkdownText);
  msg.addButton("打开链接", QMessageBox::AcceptRole);
  msg.addButton("关闭页面", QMessageBox::RejectRole);

  file.close();
  while (0 == msg.exec()) QDesktopServices::openUrl(downloadUrl);
}

bool MainWindow::isGtLocalVer(int major, int minor, int patch) {
  // 比较的顺序不能变

  if (major < PROJECT_VERSION_MAJOR) return false;
  if (major > PROJECT_VERSION_MAJOR) return true;

  if (minor < PROJECT_VERSION_MINOR) return false;
  if (minor > PROJECT_VERSION_MINOR) return true;

  if (patch < PROJECT_VERSION_PATCH) return false;
  if (patch > PROJECT_VERSION_PATCH) return true;

  return false;
}

/* ---------------------------------------------------------------- */
/*                        Update ClassSystem                        */
/* ---------------------------------------------------------------- */

void MainWindow::copyUpdateFiles() {
  if (m_updated) return;

  // 从共享内存读取 ClassSystem 程序路径
  QString path;
  QSharedMemory sm("CLASS-SYSTEM-PATH");
  sm.attach();
  path = QString::fromUtf8(QByteArray((char *)sm.data(), sm.size()));
  sm.detach();

  qInfo("Copy Update Files: Get ClassSystem path: %s",
        path.toUtf8().constData());

  QDir updatesDir(QApplication::applicationDirPath() + "/class-system-update/");

  QProcess process;
  process.setProgram("taskkill");
  process.setArguments({"/f", "/t", "/im", "ClassSystem.exe"});
  process.start();
  process.waitForFinished();

#ifdef _WIN32
  Sleep(500);
#endif

  qInfo("Copy Update Files: Killed `ClassSystem.exe`.");

  if (!copyDir(updatesDir.path(), path)) {
    QMessageBox::critical(this, "更新 ClassSystem", "无法更新 ClassSystem。");
  }

  updatesDir.removeRecursively();

  QProcess::startDetached(path + "/ClassSystem.exe", {});

  qInfo("Copy Update Files: Reopened ClasssSystem.");

  m_updated = true;
}

/* ---------------------------------------------------------------- */
/*                 Init QLocalServer & QLocalSocket                 */
/* ---------------------------------------------------------------- */

void MainWindow::onReadyRead() {
  ClassData::readFrom(m_socket, classData, false);

  loadData();
}

void MainWindow::onNewConnection() {
  qInfo("Local Socket: Connected.");

  m_connected = true;
  m_socket = m_server->nextPendingConnection();
  QDataStream ds(m_socket);
  if (!m_isFirstLoad && m_changed) {
    int code = QMessageBox::warning(
        this, "ClassSystem 管理后台",
        "连接到 ClassSystem，是否向其询问数据并加载？（会覆盖现有数据）", "是",
        "否");
    if (0 == code) {
      ds << MsgType::Request;
    }
  } else  // m_isFirstLoaded
    ds << MsgType::Request;
  connect(m_socket, &QLocalSocket::readyRead, this, &MainWindow::onReadyRead);
  connect(m_socket, &QLocalSocket::disconnected, [this] {
    m_connected = false;
    update();
    m_socket->deleteLater();
    m_socket = nullptr;
  });

  // update ClassSystem
  QDir appDir(QApplication::applicationDirPath());
  if (appDir.exists("class-system-update")) copyUpdateFiles();
}

void MainWindow::initServer() {
#ifdef _WIN32
  if (!m_server->listen(kServerName)) {
    QMessageBox::critical(
        this, "ClassAdmin",
        "<h2>无法监听 Socket ！</h2><br/>" + m_server->errorString());
    qFatal("Local Socket: Unable to listen: %s",
           m_server->errorString().toUtf8().constData());
    QApplication::quit();
  }
  m_server->setMaxPendingConnections(1);
  connect(m_server, &QLocalServer::newConnection, this,
          &MainWindow::onNewConnection);
#else
  if (!m_server->listen(kServerName)) {
    if (m_server->serverError() == QAbstractSocket::AddressInUseError) {
      QLocalServer::removeServer(kServerName);
      m_server->listen(kServerName);
    } else {
      QMessageBox::critical(
          this, "ClassAdmin",
          "<h2>无法监听 Socket ！</h2><br/>" + m_server->errorString());
      QApplication::quit();
    }
  }

  m_server->setMaxPendingConnections(1);
  connect(m_server, &QLocalServer::newConnection, this,
          &MainWindow::onNewConnection);
#endif
}

void MainWindow::loadData() {
  m_loadingData = true;

  clearStudents();

  // students
  for (const auto &[id, name] : classData.students.toStdMap()) {
    int row = ui.studentsTable->rowCount();
    ui.studentsTable->insertRow(row);
    ui.studentsTable->setItem(row, 0,
                              new QTableWidgetItem(QString::number(id)));
    ui.studentsTable->setItem(row, 1, new QTableWidgetItem(name));
  }

  // students carry meals
  for (int i = 0; i < classData.mealStu.size(); ++i) {
    QString str{oneDayOfWeek(i) + "："};
    auto ls = classData.mealStu[i];
    for (auto it = ls.cbegin(); it != ls.cend(); ++it) {
      str += classData.idAndName(*it);
      if (it + 1 != ls.cend()) str += ' ';
    }
    m_mealStuLabels[i]->setText(str);
  }

  // students on duty
  ui.stuOnDutyTable->setRowCount(classData.dutyJobs.size());

  for (int i = 0; i < classData.dutyJobs.size(); ++i) {
    auto jobItem = new QTableWidgetItem(classData.dutyJobs[i]);
    jobItem->setBackground(kDutyJobsColor);
    jobItem->setForeground(Qt::black);
    jobItem->setFlags(jobItem->flags() | Qt::ItemIsEditable);
    ui.stuOnDutyTable->setItem(i, 0, jobItem);

    for (int j = 0; j < 5; ++j) {
      auto widget = new StuOnDutyCellWidget(i, j + 1, ui.stuOnDutyTable);
      connect(widget, &StuOnDutyCellWidget::edited, this,
              &MainWindow::onStuOnDutyEdited);
      ui.stuOnDutyTable->setCellWidget(i, j + 1, widget);
    }
  }

  // lessons
  clearLessons();
  ui.lessonsTable->setRowCount(classData.lessonsTm.size());

  for (int i = 0; i < classData.lessonsTm.size(); ++i) {
    auto timeItem = new QTableWidgetItem;
    timeItem->setData(Qt::DisplayRole, classData.lessonsTm[i]);
    ui.lessonsTable->setItem(i, 0, timeItem);

    for (int j = 0; j < 5; ++j) {
      ui.lessonsTable->setItem(i, j + 1,
                               new QTableWidgetItem(classData.lessons[j][i]));
    }
  }

  // notices
  clearNotices();
  ui.noticesTable->setRowCount(classData.notices.size());

  for (int i = 0; i < classData.notices.size(); ++i) {
    auto w = new NoticesCellWidget(classData.notices[i], i, ui.noticesTable);
    connect(w, &NoticesCellWidget::edited, this, &MainWindow::onNoticesEdited);
    ui.noticesTable->setCellWidget(i, 0, w);
  }

  // events
  clearEvents();
  ui.eventsTable->setRowCount(classData.events.size());

  for (int i = 0; i < classData.events.size(); ++i) {
    auto ev = classData.events[i];

    ui.eventsTable->setItem(i, 0, new QTableWidgetItem(ev.name));

    auto dateItem = new QTableWidgetItem;
    dateItem->setData(Qt::DisplayRole, ev.date);
    ui.eventsTable->setItem(i, 1, dateItem);

    auto w = new QCheckBox(ui.eventsTable);
    w->setChecked(ev.important);
    connect(w, &QCheckBox::clicked, this,
            &MainWindow::onEventsCheckBoxesClicked);
    ui.eventsTable->setCellWidget(i, 2, w);
  }

  m_changed = false;
  m_loadingData = false;
  m_isFirstLoad = false;
  update();
}

/* ---------------------------------------------------------------- */
/*                              Events                              */
/* ---------------------------------------------------------------- */

void MainWindow::paintEvent(QPaintEvent *) {
  // students
  ui.btnRemoveStudent->setEnabled(ui.studentsTable->selectedRanges().size());
  ui.btnClearStudents->setEnabled(ui.studentsTable->rowCount());

  // students on duty
  if (classData.dutyJobs.empty()) {
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

  // lessons
  for (int i = 0; i < ui.lessonsTable->rowCount(); ++i) {
    QString text = "第%1节"_s.arg(i + 1);
    if (auto item = ui.lessonsTable->verticalHeaderItem(i))
      item->setText(text);
    else
      ui.lessonsTable->setVerticalHeaderItem(i, new QTableWidgetItem(text));
  }

  // title
  if (windowTitle() != kWindowTitle[m_changed])
    setWindowTitle(kWindowTitle[m_changed]);
}

void MainWindow::closeEvent(QCloseEvent *ev) {
  if (!m_changed) {
    ev->accept();
    return;
  }

  int code = QMessageBox::warning(this, "关闭 ClassAdmin",
                                  "修改尚未保存/同步，是否放弃修改并关闭程序？",
                                  "关闭", "取消");
  if (1 == code) {
    ev->ignore();
  } else {
    ev->accept();
  }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *ev) {
  if (ev->type() == QEvent::KeyPress) {
    auto keyEv = static_cast<QKeyEvent *>(ev);
    if (keyEv->key() == Qt::Key_Delete && m_tablesRemoveFuncMap.contains(obj)) {
      // 在表格中按下 Delete 键删除
      m_tablesRemoveFuncMap[obj]();
      return true;
    }
  }
  return false;
}
