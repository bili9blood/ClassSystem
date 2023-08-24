#pragma once

#include <qdatetimeedit.h>
#include <qitemdelegate.h>
#include <qlineedit.h>

class IntDelegate : public QItemDelegate {
 public:
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                        const QModelIndex &) const override {
    auto lineEdit = new QLineEdit(parent);
    lineEdit->setValidator(new QIntValidator(0, INT_MAX, lineEdit));
    return lineEdit;
  }
};

class TimeDelegate : public QItemDelegate {
 public:
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                        const QModelIndex &) const override {
    return new QTimeEdit(parent);
  }

  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                            const QModelIndex &) const override {
    editor->setGeometry(option.rect);
  }

  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override {
    auto timeEdit = qobject_cast<QTimeEdit *>(editor);
    model->setData(index, timeEdit->time());
  }
};