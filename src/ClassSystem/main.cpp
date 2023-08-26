#include <qbuffer.h>
#include <qfontdatabase.h>
#include <qsharedmemory.h>

#include "MainPanel.h"

#define QT_NO_OPENGL

void checkHasRunningApp() {
  auto sm = new QSharedMemory("CLASS-SYSTEM-PATH");
  if (sm->attach()) exit(0);

  QByteArray ba = QApplication::applicationDirPath().toUtf8();
  QBuffer b;
  b.open(QBuffer::WriteOnly);
  b.write(ba);
  sm->create(b.size());
  memcpy(sm->data(), b.data(), b.size());
}

void checkDirs() {
  const QStringList kDirs = {"screenshots"};
  QDir c = QApplication::applicationDirPath();
  for (const QString &dirName : kDirs)
    if (!c.exists(dirName)) c.mkdir(dirName);
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);

  checkHasRunningApp();

  qRegisterMetaTypeStreamOperators<ClassNotice>("ClassNotice");
  qRegisterMetaTypeStreamOperators<ClassEvent>("ClassEvent");
  // add fonts

  if (int fontId =
          QFontDatabase::addApplicationFont(":/font/MiSans-Regular.ttf");
      fontId != -1)
    QApplication::setFont(
        QFontDatabase::applicationFontFamilies(fontId).first());

  checkDirs();

  // show panel
  MainPanel panel;
  panel.show();

  return QApplication::exec();
}