#include "ImportDialog.h"

ImportDialog::ImportDialog(QString templateStr, QWidget *parent)
    : QDialog(parent) {
  setWindowTitle("ClassAdmin 导入");
  // init text edit
  m_edit->setAcceptRichText(false);

  m_label->setText(
      "从 Excel 中选中特定部分按"
      " <i>Ctrl+C</i> 复制，再在此文本框内按 <i>Ctrl+V</i> 粘贴。"
      "<br/><br/>正确格式应该如下："
      "<br/><pre>%1</pre><br/>"
      "<i><strong>关于样例的解释见文档。</strong></i>"_s.arg(templateStr));

  // init button box`````````
  m_buttonBox->addButton("确定", QDialogButtonBox::AcceptRole);
  m_buttonBox->addButton("取消", QDialogButtonBox::RejectRole);

  connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

  // init layout
  m_mainLayout->addWidget(m_label);
  m_mainLayout->addWidget(m_edit);
  m_mainLayout->addWidget(m_buttonBox);
}

QString ImportDialog::getData() { return m_edit->toPlainText(); }