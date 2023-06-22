

#include <QApplication>

#include "ClassSystem.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  ClassSystem w;
  w.show();
  return a.exec();
}