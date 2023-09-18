#pragma once

#include <qcheckbox.h>
#include <qcombobox.h>
#include <qdatetimeedit.h>
#include <qitemdelegate.h>
#include <qlineedit.h>

#define ITEM_DELEGATE(name, onCreate, setData)                           \
  class name : public QItemDelegate {                                    \
   public:                                                               \
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &, \
                          const QModelIndex &) const override {          \
      onCreate                                                           \
    }                                                                    \
                                                                         \
    void updateEditorGeometry(QWidget *editor,                           \
                              const QStyleOptionViewItem &option,        \
                              const QModelIndex &) const override {      \
      editor->setGeometry(option.rect);                                  \
    }                                                                    \
                                                                         \
    void setModelData(QWidget *editor, QAbstractItemModel *model,        \
                      const QModelIndex &index) const override {         \
      setData                                                            \
    }                                                                    \
  };

// clang-format off

ITEM_DELEGATE(
              IntDelegate,
              auto lineEdit = new QLineEdit(parent);
              lineEdit->setValidator(new QIntValidator(0, INT_MAX, lineEdit));
              return lineEdit;
              ,
             )

ITEM_DELEGATE(
              TimeDelegate,
              return new QTimeEdit(parent);
              ,
              auto timeEdit = qobject_cast<QTimeEdit *>(editor);
              model->setData(index, timeEdit->time());
             )

ITEM_DELEGATE(
              DateDelegate,
              return new QDateEdit(parent);
              ,
              auto dateEdit = qobject_cast<QDateEdit *>(editor);
              model->setData(index, dateEdit->date());
             )

ITEM_DELEGATE(
              DayInWeekDelegate,
              auto comboBox = new QComboBox(parent);
              comboBox->addItems({"周一", "周二", "周三", "周四", "周五"});
              return comboBox;
              ,
              auto comboBox = qobject_cast<QComboBox *>(editor);
              model->setData(index, comboBox->currentIndex());
             )

#undef ITEM_DELEGATE