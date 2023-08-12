#include "ResetPwdDialog.h"

ResetPwdDialog::ResetPwdDialog(QWidget *parent) : QDialog(parent) {
  setWindowTitle("ClassAdmin 更改密码");
  // init line edits
  m_oldPwdLineEdit->setEchoMode(QLineEdit::Password);
  m_oldPwdLineEdit->setAttribute(Qt::WA_InputMethodEnabled, false);

  m_newPwdLineEdit->setEchoMode(QLineEdit::Password);
  m_newPwdLineEdit->setAttribute(Qt::WA_InputMethodEnabled, false);

  m_repeatNewPwdLineEdit->setEchoMode(QLineEdit::Password);
  m_repeatNewPwdLineEdit->setAttribute(Qt::WA_InputMethodEnabled, false);

  connect(m_repeatNewPwdLineEdit, &QLineEdit::editingFinished, [this] {
    if (m_newPwdLineEdit->text() != m_repeatNewPwdLineEdit->text()) {
      m_errorLabel->setText("两次输入的新密码不一致！");
    } else {
      m_errorLabel->setText({});
    }
  });

  // init error label
  m_errorLabel->setStyleSheet("color: red;");

  // init button box
  m_buttonBox->addButton("确定", QDialogButtonBox::AcceptRole);
  m_buttonBox->addButton("取消", QDialogButtonBox::RejectRole);

  connect(m_buttonBox, &QDialogButtonBox::accepted, [this] {
    QFile file("PWD");
    file.open(QFile::ReadOnly);
    if (QCryptographicHash::hash(m_oldPwdLineEdit->text().toUtf8(),
                                 QCryptographicHash::Md5)
            .toHex() == file.readAll()) {
      file.close();
      file.open(QFile::WriteOnly | QFile::Truncate);
      file.write(QCryptographicHash::hash(m_newPwdLineEdit->text().toUtf8(),
                                          QCryptographicHash::Md5)
                     .toHex());
      file.close();
      accept();
    }
    file.close();
    m_errorLabel->setText("旧密码错误！");
  });
  connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

  // init layout
  m_mainLayout->addRow(m_oldPwdLabel, m_oldPwdLineEdit);
  m_mainLayout->addRow(m_newPwdLabel, m_newPwdLineEdit);
  m_mainLayout->addRow(m_repeatNewPwdLabel, m_repeatNewPwdLineEdit);
  m_mainLayout->addWidget(m_errorLabel);
  m_mainLayout->addWidget(m_buttonBox);
}