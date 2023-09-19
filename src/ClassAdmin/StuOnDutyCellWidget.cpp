#include "StuOnDutyCellWidget.h"

#include <qelapsedtimer.h>

#include "ClassData.h"
#include "ItemDelegates.h"

StuOnDutyCellWidget::StuOnDutyCellWidget(int row, int column, QWidget *parent)
    : QWidget(parent),
      m_stuOnDuty(classData.stuOnDuty[column - 1][row]),
      m_row(row),
      m_col(column) {
  QElapsedTimer t;
  t.start();
  ui.setupUi(this);
  qDebug() << t.elapsed();

  ui.listWidget->setItemDelegate(new IntDelegate);

  for (const uint &id : m_stuOnDuty) {
    if (!id) continue;
    auto item = new QListWidgetItem(QString::number(id));
    item->setFlags(item->flags() | Qt::ItemIsEditable);

    ui.listWidget->addItem(item);
  }

  connect(ui.listWidget->model(), &QAbstractItemModel::dataChanged, this,
          &StuOnDutyCellWidget::onDataChanged);

  ui.btnAdd->hide();
  ui.btnRemove->hide();

  QSizePolicy policy = ui.btnAdd->sizePolicy();
  policy.setRetainSizeWhenHidden(true);
  ui.btnAdd->setSizePolicy(policy);
  ui.btnRemove->setSizePolicy(policy);
  // qDebug() << t.elapsed();
}

void StuOnDutyCellWidget::onAdd() {
  auto item = new QListWidgetItem("0");
  item->setFlags(item->flags() | Qt::ItemIsEditable);

  if (ui.listWidget->currentItem()) {
    int row = ui.listWidget->currentRow() + 1;

    ui.listWidget->insertItem(row, item);
    m_stuOnDuty.insert(row, 0);
  } else {
    ui.listWidget->addItem(item);
    m_stuOnDuty << 0;
  }
}

void StuOnDutyCellWidget::onRemove() {
  if (!ui.listWidget->currentItem()) return;

  int row = ui.listWidget->currentRow();
  ui.listWidget->takeItem(row);
  m_stuOnDuty.removeAt(row);

  emit edited(m_stuOnDuty, m_row, m_col);
}

void StuOnDutyCellWidget::onDataChanged(const QModelIndex &idx,
                                        const QModelIndex &,
                                        const QVector<int> &) {
  int row = idx.row();
  m_stuOnDuty[row] = ui.listWidget->item(row)->data(Qt::DisplayRole).toUInt();
  emit edited(m_stuOnDuty, m_row, m_col);
}

void StuOnDutyCellWidget::enterEvent(QEvent *) {
  ui.btnAdd->show();
  ui.btnRemove->show();
}

void StuOnDutyCellWidget::leaveEvent(QEvent *) {
  ui.btnAdd->hide();
  ui.btnRemove->hide();
}