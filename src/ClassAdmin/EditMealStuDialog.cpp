#include "EditMealStuDialog.h"

#include "ItemDelegates.h"

EditMealStuDialog::EditMealStuDialog(int dayOfWeek, ClassData::Data data,
                                     QWidget *parent)
    : QDialog(parent, Qt::MSWindowsFixedSizeDialogHint),
      m_data(data),
      m_dayOfWeek(dayOfWeek) {
  ui.setupUi(this);

  auto mealStu = data.mealStu[dayOfWeek];
  for (const auto &id : mealStu) {
    auto item = new QListWidgetItem(QString::number(id));
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    ui.listWidget->addItem(item);
  }

  ui.buttonBox->setStandardButtons(QDialogButtonBox::NoButton);
  ui.buttonBox->addButton("确认", QDialogButtonBox::AcceptRole);
  ui.buttonBox->addButton("取消", QDialogButtonBox::RejectRole);

  ui.listWidget->setItemDelegate(new IntDelegate);
  connect(ui.listWidget->model(), &QAbstractItemModel::dataChanged, this,
          &EditMealStuDialog::onDataChanged);

  setWindowTitle("编辑%1抬饭生"_s.arg(oneDayOfWeek(dayOfWeek)));
}

std::pair<ClassData::Data, QString> EditMealStuDialog::getResult() {
  return std::make_pair(m_data, ui.labelPreview->text());
}

void EditMealStuDialog::addMealStu() {
  auto item = new QListWidgetItem("0");
  ui.listWidget->addItem(item);
  item->setFlags(item->flags() | Qt::ItemIsEditable);
}

void EditMealStuDialog::removeMealStu() {
  if (ui.listWidget->currentItem() == nullptr) {
    ui.btnRemove->setDisabled(true);
    return;
  }

  auto item = ui.listWidget->takeItem(ui.listWidget->currentRow());
  if (item) {
    delete item;
    item = nullptr;
  }

  m_data.mealStu[m_dayOfWeek].removeAt(ui.listWidget->currentRow());
  m_changed = true;
}

void EditMealStuDialog::onDataChanged(const QModelIndex &idx,
                                      const QModelIndex &,
                                      const QVector<int> &) {
  QList<uint> &ls = m_data.mealStu[m_dayOfWeek];
  while (idx.row() >= ls.size()) ls << 0;
  ls[idx.row()] = idx.data().toUInt();
  m_changed = true;
}

void EditMealStuDialog::paintEvent(QPaintEvent *) {
  ui.labelPreview->setFixedWidth(ui.scrollArea->width());
  ui.buttonBox->buttons()[0]->setEnabled(m_changed);  // `确认` 按钮
  ui.btnRemove->setEnabled(ui.listWidget->currentItem());

  // gen preview
  QString previewStr;
  for (int i = 0; i < ui.listWidget->count(); ++i) {
    previewStr += m_data.idAndName(ui.listWidget->item(i)->text().toUInt());
    if (i + 1 < ui.listWidget->count()) previewStr += ' ';
  }
  ui.labelPreview->setText(previewStr);
}