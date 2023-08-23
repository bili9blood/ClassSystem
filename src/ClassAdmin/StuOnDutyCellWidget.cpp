#include "StuOnDutyCellWidget.h"

#include "IntDelegate.h"

StuOnDutyCellWidget::StuOnDutyCellWidget(int row, int column,
                                         QList<uint> stuOnDuty, QWidget *parent)
    : m_row(row), m_col(column), m_stuOnDuty(stuOnDuty), QWidget(parent) {
  ui.setupUi(this);

  ui.listWidget->setItemDelegate(new IntDelegate);

  for (const uint &id : stuOnDuty) {
    if (!id) continue;
    auto item = new QListWidgetItem(QString::number(id));
    item->setFlags(item->flags() | Qt::ItemIsEditable);

    ui.listWidget->addItem(item);
  }

  connect(ui.listWidget->model(), &QAbstractItemModel::dataChanged, this,
          &StuOnDutyCellWidget::onDataChanged);
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