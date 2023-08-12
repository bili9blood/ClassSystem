#include <qcryptographichash.h>
#include <qinputdialog.h>
#include <qmessagebox.h>
#include <qsharedmemory.h>

#include "MainWindow.h"

void inputPwd() {
  QByteArray pwd;
  QFile pwdFile("PWD");
  if (!pwdFile.exists()) {
    QMessageBox::warning(nullptr, "密码", "密码文件丢失，密码重置为初始密码！");
    if (!pwdFile.isOpen()) pwdFile.open(QFile::WriteOnly);
    pwdFile.write(pwd = "e10adc3949ba59abbe56e057f20f883e");
    pwdFile.close();
  } else {
    if (!pwdFile.isOpen()) pwdFile.open(QFile::ReadOnly);
    pwd = pwdFile.readAll();
    pwdFile.close();
  }

  QInputDialog pwdDlg;
  pwdDlg.setTextEchoMode(QLineEdit::Password);
  pwdDlg.setWindowTitle("ClassAdmin 密码");
  pwdDlg.setLabelText("请输入密码：");
  int retryCnt = 0;
  while (pwdDlg.exec(),
         pwd != QCryptographicHash::hash(pwdDlg.textValue().toUtf8(),
                                         QCryptographicHash::Md5)
                    .toHex()) {
    qDebug("pwderr");
    if (retryCnt == 5) {
      QMessageBox::critical(nullptr, "密码", "尝试次数过多！");
      exit(0);
    }
    QMessageBox::warning(nullptr, "密码",
                         "密码错误，还有%1次尝试机会！"_s.arg(5 - retryCnt));
    ++retryCnt;
  }
}

int main(int argc, char **argv) {
  // single application
  QSharedMemory sm("CLASS-ADMIN-SINGLE-KEY");
  if (sm.attach()) return 0;
  sm.create(1);

  QApplication a(argc, argv);

  inputPwd();

  MainWindow w;
  w.show();

  return a.exec();
}