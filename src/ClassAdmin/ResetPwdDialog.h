#include <qcryptographichash.h>
#include <qdialog.h>
#include <qdialogbuttonbox.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qlineedit.h>

class ResetPwdDialog : public QDialog {
  Q_OBJECT
 public:
  explicit ResetPwdDialog(QWidget *parent = nullptr);

 private:
  QFormLayout *m_mainLayout = new QFormLayout(this);

  QLabel *m_oldPwdLabel = new QLabel("原密码：", this);
  QLabel *m_newPwdLabel = new QLabel("新密码：", this);
  QLabel *m_repeatNewPwdLabel = new QLabel("重复新密码：", this);
  QLabel *m_errorLabel = new QLabel(this);

  QLineEdit *m_oldPwdLineEdit = new QLineEdit(this);
  QLineEdit *m_newPwdLineEdit = new QLineEdit(this);
  QLineEdit *m_repeatNewPwdLineEdit = new QLineEdit(this);

  QDialogButtonBox *m_buttonBox = new QDialogButtonBox(this);
};