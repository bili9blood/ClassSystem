#include "RollCallWindow.h"

#include <qmessagebox.h>

RollCallWindow::RollCallWindow(QWidget *parent) : QWidget(parent) {
  ui.setupUi(this);
  loadData();
  reset();

  // init font-size combobox
  for (const int &fontSize : kPointSizeList) {
    ui.comboBoxFontSize->addItem(QString::number(fontSize));
    if (fontSize == 36)
      ui.comboBoxFontSize->setCurrentIndex(ui.comboBoxFontSize->count() - 1);
  }

  // init timer
  connect(&m_timer, &QTimer::timeout, this, &RollCallWindow::onTimeout);

  // init rng
  m_rng = std::mt19937(std::random_device()());
}

void RollCallWindow::loadData() {
  QFile file("data.stm");
  if (!file.exists() || !ClassData::readFrom(&file, m_data)) {
    QMessageBox::critical(this, "ClassSystem",
                          "无法读取数据！<br/>程序将关闭。");
    exit(0);
  }
  file.close();
}

void RollCallWindow::reset() {
  ui.label->setText("未开始");
  ui.btnStartStop->setChecked(false);

  ui.nameList->clear();
  ui.calledList->clear();

  ui.nameList->addItems(m_data.students.values());
}

void RollCallWindow::toggleStartStop(bool checked) {
  ui.btnStartStop->setText(kStartStopText[checked]);

  if (checked) {
    if (ui.nameList->count() == 0) {
      ui.btnStartStop->setChecked(false);
      return;
    }
    m_timer.start(ui.spinBoxInterval->value());
  } else {
    m_timer.stop();
    moveToAnother(ui.nameList, m_calledIndex, ui.calledList);
  }
}

void RollCallWindow::changeFontSize(int index) {
  ui.label->setFont(qFont{.pointSize = kPointSizeList[index]}());
}

void RollCallWindow::moveToAnother(QListWidget *src, int index,
                                   QListWidget *dst) {
  dst->addItem(src->item(index)->text());
  src->takeItem(index);
}

void RollCallWindow::moveToAnother() {
  QListWidget *src =
                  sender() == ui.btnAddToCalled ? ui.nameList : ui.calledList,
              *dst = src == ui.nameList ? ui.calledList : ui.nameList;

  int curRow = src->currentRow();
  if (curRow == -1) return;

  moveToAnother(src, curRow, dst);
}

void RollCallWindow::onTimeout() {
  std::uniform_int_distribution<> distrib(0, ui.nameList->count() - 1);
  ui.label->setText(ui.nameList->item(m_calledIndex = distrib(m_rng))->text());
}