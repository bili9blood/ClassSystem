#include "NoticesCellWidget.h"

NoticesCellWidget::NoticesCellWidget(const ClassNotice &notice, int row,
                                     QWidget *parent)
    : QFrame(parent), m_row(row), m_notice(notice) {
  ui.setupUi(this);

  // frame
  setFrameShape(QFrame::Box);
  setFrameShadow(QFrame::Plain);
  setLineWidth(1);

  ui.textEdit->setPlainText(notice.str);

  for (const int &i : kPointSizeList) {
    ui.comboBox->addItem(QString::number(i));
  }

  ui.textEdit->setFont(qFont{.pointSize = notice.fontPtSize}());
  ui.comboBox->setCurrentText(QString::number(notice.fontPtSize));

  ui.dateEdit->setMinimumDate(QDate::currentDate().addDays(1));
  if (notice.date == kForever) {
    ui.checkBox->setChecked(true);

    ui.labelDate->setDisabled(true);
    ui.dateEdit->setDisabled(true);
  } else {
    ui.dateEdit->setDate(notice.date);
  }
}

void NoticesCellWidget::onTextChanged() {
  m_notice.str = ui.textEdit->toPlainText();

  emit edited(m_notice, m_row);
}

void NoticesCellWidget::changeFontPtSize(int idx) {
  int fontPtSize = kPointSizeList[idx];

  m_notice.fontPtSize = fontPtSize;
  ui.textEdit->setFont(qFont{.pointSize = fontPtSize}());

  emit edited(m_notice, m_row);
}

void NoticesCellWidget::onDateChanged() {
  if (ui.checkBox->isChecked()) {
    m_notice.date = kForever;

    ui.labelDate->setDisabled(true);
    ui.dateEdit->setDisabled(true);
  } else {
    m_notice.date = ui.dateEdit->date();
  }

  emit edited(m_notice, m_row);
}