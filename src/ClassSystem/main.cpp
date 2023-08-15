#include <qfontdatabase.h>
#include <qsharedmemory.h>

#include "MainPanel.h"
#include "PopupMenu.h"

#define QT_NO_OPENGL

void checkHasRunningApp() {
  auto sm = new QSharedMemory("CLASS-SYSTEM-SINGLE-KEY");
  if (sm->attach()) exit(0);
  sm->create(1);
}

void checkDirs() {
  const QStringList kDirs = {"screenshots"};
  auto c = QDir::current();
  for (const QString &dirName : kDirs)
    if (!c.exists(dirName)) c.mkdir(dirName);
}

int main(int argc, char *argv[]) {
  checkHasRunningApp();
  QApplication a(argc, argv);
  QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);

  qRegisterMetaTypeStreamOperators<ClassNotice>("ClassNotice");
  qRegisterMetaTypeStreamOperators<ClassEvent>("ClassEvent");
  // add fonts
  int fontId = QFontDatabase::addApplicationFont(":/font/MiSans-Regular.ttf");
  if (fontId != -1)
    QApplication::setFont(
        QFontDatabase::applicationFontFamilies(fontId).first());

  checkDirs();

  // generate template .stm file
  // ClassData::writeTo(ClassData::testData(), new QFile("data.stm"));

  PopupMenu menu;
  menu.show();
  // show panel
  MainPanel panel;
  panel.show();

  // init System Tray Icon
  QSystemTrayIcon *m_trayIcon =
      new QSystemTrayIcon(QIcon("D:/Downloads/icon.png"));
  QMenu *m_trayMenu = new QMenu();
  QAction *m_quitAction = new QAction("退出");
  QObject::connect(m_quitAction, &QAction::triggered, [] { exit(0); });
  m_trayMenu->addAction(m_quitAction);
  m_trayIcon->setContextMenu(m_trayMenu);
  m_trayIcon->show();

  return QApplication::exec();
}