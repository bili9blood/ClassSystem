#include <qboxlayout.h>
#include <qdialog.h>
#include <qdialogbuttonbox.h>
#include <qlabel.h>
#include <qtextedit.h>

class ImportDialog : public QDialog {
  Q_OBJECT
 public:
  explicit ImportDialog(QString templateStr, QWidget *parent = nullptr);
  QString getData();

 private:
  QVBoxLayout *m_mainLayout = new QVBoxLayout(this);

  QLabel *m_label = new QLabel(this);
  QTextEdit *m_edit = new QTextEdit(this);
  QDialogButtonBox *m_buttonBox = new QDialogButtonBox(this);
};