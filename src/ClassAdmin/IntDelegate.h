#pragma once

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