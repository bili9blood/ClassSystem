#include <QApplication>
#include <QFontDatabase>
#include <QMessageBox>
#include <QSharedMemory>

#include "Widgets/ClassSystem.h"

int main(int argc, char *argv[]) {
  // single application
  QSharedMemory sm("CLASS-SYSTEM-SINGLE-KEY");
  if (sm.attach()) return 0;
  sm.create(1);

  QApplication a(argc, argv);
  // hello
  //  add fonts
  int id = QFontDatabase::addApplicationFont(":/font/Inconsolata-Regular.ttf");
  if (id != -1)
    QApplication::setFont(QFontDatabase::applicationFontFamilies(id).first());
  // show window
  ClassSystem w;
  w.show();
  return QApplication::exec();
}