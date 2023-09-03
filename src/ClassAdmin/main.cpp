#include <qcryptographichash.h>
#include <qdialogbuttonbox.h>
#include <qinputdialog.h>
#include <qlockfile.h>
#include <qmessagebox.h>

#include "MainWindow.h"

// 程序单例化
void singleApp() {
  // 本测试程序id取名为SingleApp
  QString path = QDir::temp().absoluteFilePath("ClassAdmin.lock");
  auto lockFile = new QLockFile(path);

  // tryLock尝试创建锁定文件。此函数如果获得锁，则返回true; 否则返回false。
  // 如果另一个进程（或另一个线程）已经创建了锁文件，则此函数将最多等待timeout毫秒
  if (!lockFile->tryLock(100)) {
    qDebug() << "Already Running!";
    exit(0);
  }
}

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

  // 禁用输入法
  if (auto lineEdit = pwdDlg.findChild<QLineEdit *>()) {
    lineEdit->setAttribute(Qt::WA_InputMethodEnabled, false);
  }

  // 重复询问密码
  for (int retryCnt = 0, code; (code = pwdDlg.exec()),
           pwd != QCryptographicHash::hash(pwdDlg.textValue().toUtf8(),
                                           QCryptographicHash::Md5)
                                       .toHex();
       ++retryCnt) {
    if (code == QDialog::Rejected) exit(0);
    qDebug("pwderr");
    if (retryCnt == 5) {
      QMessageBox::critical(nullptr, "密码", "尝试次数过多！");
      exit(0);
    }
    QMessageBox::warning(nullptr, "密码",
                         "密码错误，还有%1次尝试机会！"_s.arg(5 - retryCnt));
  }
}

int main(int argc, char **argv) {
  QApplication a(argc, argv);
  QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
  QApplication::setWindowIcon(QIcon(":/img/logo.png"));

  qRegisterMetaType<ClassData::Data>("ClassData::Data");
  qRegisterMetaTypeStreamOperators<ClassNotice>("ClassNotice");
  qRegisterMetaTypeStreamOperators<ClassEvent>("ClassEvent");

  singleApp();

  inputPwd();

  MainWindow w;
  w.show();

  return QApplication::exec();
}