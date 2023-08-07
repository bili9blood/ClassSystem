#include <qfontdatabase.h>
#include <qsharedmemory.h>
#include <qtranslator.h>

#include "Widgets/MainPanel.h"
#include "Widgets/PopupMenu.h"

void checkDirs() {
  const QStringList dirs = {"screenshots"};
  auto c = QDir::current();
  foreach (const QString &dirName, dirs)
    if (!c.exists(dirName)) c.mkdir(dirName);
}

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

  checkDirs();

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
  // generate template .stm file
  // ClassData::writeTo(ClassData::testData(), new QFile("data.stm"));

  return QApplication::exec();
}