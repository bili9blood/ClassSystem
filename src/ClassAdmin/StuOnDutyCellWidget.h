#pragma once

#include <qevent.h>
#include <qtablewidget.h>

#include "ui_StuOnDutyCellWidget.h"

class StuOnDutyCellWidget : public QWidget {
  Q_OBJECT
 public:
  explicit StuOnDutyCellWidget(int row, int column, QList<uint> stuOnDuty,
                               QWidget *parent = nullptr);

 signals:
  void edited(QList<uint>, int row, int col);

 private slots:
  void onAdd();
  void onRemove();

  // 移除项目时不会触发
  void onDataChanged(const QModelIndex &idx, const QModelIndex &,
                     const QVector<int> &);

 private:
  QList<uint> m_stuOnDuty;
  int m_row;
  int m_col;

  Ui::StuOnDutyCellWidget ui;

 protected:
  void enterEvent(QEvent *ev) override;
  void leaveEvent(QEvent *ev) override;
};