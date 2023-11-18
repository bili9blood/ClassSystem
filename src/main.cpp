#include <qbuffer.h>
#include <qfontdatabase.h>
#include <qlockfile.h>
#include <qsharedmemory.h>

#include "MainPanel.h"

#define QT_NO_OPENGL

// 创建程序运行时可能需要的文件夹
void createDirs() {
  const QStringList kDirs = {"screenshots"};
  QDir curDir = QApplication::applicationDirPath();
  for (const QString &dirName : kDirs)
    if (!curDir.exists(dirName)) curDir.mkdir(dirName);
}

// 程序单例化
void singleApp() {
  QString path = QDir::temp().absoluteFilePath("ClassSystem.lock");
  auto lockFile = new QLockFile(path);

  // tryLock尝试创建锁定文件。此函数如果获得锁，则返回true; 否则返回false。
  // 如果另一个进程（或另一个线程）已经创建了锁文件，则此函数将最多等待timeout毫秒
  if (!lockFile->tryLock(100)) exit(0);
}

int main(int argc, char **argv) {
  if (argc == 2 && strcmp(argv[1], "-v") == 0) {
    printf("v%s\n", cs::config::projectVersion);
    return 0;
  }

  QApplication a(argc, argv);
  QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
  QApplication::setWindowIcon(QIcon(":/img/logo.png"));

  singleApp();

  cs::initLogger();

  // 将程序路径存储到共享内存中
  QSharedMemory sm("CLASS-SYSTEM-PATH");
  QByteArray ba = QApplication::applicationDirPath().toUtf8();

  QBuffer b;
  b.open(QBuffer::WriteOnly);
  b.write(ba);

  sm.create(b.size());
  if (sm.error() == QSharedMemory::AlreadyExists) {
    sm.attach();
    sm.detach();
    sm.create(b.size());
  }

  memcpy(sm.data(), b.data(), b.size());

  // 添加 MiSans 并设为默认字体
  if (int fontId =
          QFontDatabase::addApplicationFont(":/font/MiSans-Regular.ttf");
      fontId != -1)
    QApplication::setFont(
        QFontDatabase::applicationFontFamilies(fontId).first());

  // 添加标题字体
  QFontDatabase::addApplicationFont(":/font/title.ttf");

  createDirs();

  cs::settings::loadIni();
  QApplication::setQuitOnLastWindowClosed(false);

  // 显示主面板
  MainPanel panel;
  panel.show();

  return QApplication::exec();
}