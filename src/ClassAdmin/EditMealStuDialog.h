#pragma once

#include <qdialog.h>
#include <qitemdelegate.h>
#include <qlineedit.h>
#include <ui_EditMealStuDialog.h>

#include "ClassData.h"

class EditMealStuDialog : public QDialog {
  Q_OBJECT
 public:
  explicit EditMealStuDialog(int dayOfWeek, ClassData::Data data,
                             QWidget *parent = nullptr);
  std::pair<ClassData::Data, QString> getResult();

 private:
  Ui::EditMealStuDialog ui;

  int m_dayOfWeek;
  ClassData::Data m_data;

  bool m_changed = false;

 private slots:
  void addMealStu();
  void removeMealStu();

  // 移除项目时不会触发
  void onDataChanged(const QModelIndex &idx, const QModelIndex &,
                     const QVector<int> &);

 protected:
  void paintEvent(QPaintEvent *ev) override;
};
