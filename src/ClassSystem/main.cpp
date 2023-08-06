#include <qfontdatabase.h>
#include <qsharedmemory.h>

#include "Widgets/MainPanel.h"
#include "Widgets/PopupMenu.h"

int main(int argc, char *argv[]) {
  // single application
  QSharedMemory sm("CLASS-SYSTEM-SINGLE-KEY");
  if (sm.attach()) return 0;
  sm.create(1);

  QApplication a(argc, argv);
  // add fonts
  int fontId = QFontDatabase::addApplicationFont(":/font/MiSans-Regular.ttf");
  if (fontId != -1)
    QApplication::setFont(
        QFontDatabase::applicationFontFamilies(fontId).first());

  PopupMenu menu;
  menu.show();
  // show panel
  MainPanel panel;
  panel.show();

  // generate template .stm file
  // ClassData::writeTo(ClassData::testData(), new QFile("data.stm"));

  return QApplication::exec();
}