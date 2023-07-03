#include <QApplication>
#include <QEventLoop>
#include <QFontDatabase>
#include <QMessageBox>
#include <QSharedMemory>

#include "Core/Updater.h"
#include "Widgets/MainPanel.h"
#include "Widgets/SuspendedWidget.h"
#include "src/ClassData.h"

void switchToDesktop() {  // same as press WIN+D
  INPUT input[4];
  memset(input, 0, sizeof(input));

  input[0].type = input[1].type = input[2].type = input[3].type =
      INPUT_KEYBOARD;

  input[0].ki.wVk = input[3].ki.wVk = VK_LWIN;
  input[1].ki.wVk = input[2].ki.wVk = 'D';

  input[2].ki.dwFlags = input[3].ki.dwFlags = KEYEVENTF_KEYUP;

  SendInput(4, input, sizeof(INPUT));
}

int main(int argc, char *argv[]) {
  // single application
  QSharedMemory sm("CLASS-SYSTEM-SINGLE-KEY");
  if (sm.attach()) return 0;
  sm.create(1);

  QApplication a(argc, argv);
  // add fonts
  int id = QFontDatabase::addApplicationFont(":/font/MiSans-Regular.ttf");
  if (id != -1)
    QApplication::setFont(QFontDatabase::applicationFontFamilies(id).first());
  // show SuspendedWidget
  Updater u;
  QEventLoop loop;
  QMetaObject::invokeMethod(&u, &Updater::check, Qt::QueuedConnection);
  QObject::connect(&u, &Updater::checked, &loop, &QEventLoop::quit);
  loop.exec();

  SuspendedWidget susWid;
  susWid.show();
  // show panel
  MainPanel panel;
  panel.show();

  // generate template .stm file
  // ClassData::writeTo(ClassData::testData(), new QFile("data.stm"));

  switchToDesktop();
  return QApplication::exec();
}