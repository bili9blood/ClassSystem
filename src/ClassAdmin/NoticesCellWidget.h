#pragma once

#include <qfontdatabase.h>

#include "ClassData.h"
#include "ui_NoticesCellWidget.h"

class NoticesCellWidget : public QFrame {
  Q_OBJECT
 public:
  explicit NoticesCellWidget(const ClassNotice &notice, int row,
                             QWidget *parent = nullptr);

 signals:
  void edited(const ClassNotice &notice, const int &row);

 private slots:
  void onTextChanged();
  void changeFontPtSize(int idx);
  void onDateChanged();

 private:
  Ui::NoticesCellWidget ui;

  int m_row;

  QFontDatabase m_fontDb;
  const QList<int> kPointSizeList = m_fontDb.pointSizes("微软雅黑");

  ClassNotice m_notice;
};