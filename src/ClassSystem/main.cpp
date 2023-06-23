#include <QApplication>
#include <QFontDatabase>
#include <QMessageBox>
#include <QSharedMemory>

#include "Widgets/ClassSystem.h"
int main(int argc, char *argv[]) {
  // single application
  QSharedMemory sm("CLASS-SYSTEM-SINGLE-KEY");
  if (sm.attach())
    return 0;
  sm.create(1);

  QApplication a(argc, argv);

  // add fonts
  QFontDatabase::addApplicationFont(":/font/Inconsolata-Regular");

  // show window
  ClassSystem w;
  w.show();
  return a.exec();
}