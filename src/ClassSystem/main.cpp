#include <qbuffer.h>
#include <qfontdatabase.h>
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

int main(int argc, char **argv) {
  QApplication a(argc, argv);
  QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);

  // 共享内存
  QSharedMemory sm("CLASS-SYSTEM-PATH");
  // 程序单例化
  if (sm.attach()) exit(0);

  // 将程序路径存储到共享内存中
  QByteArray ba = QApplication::applicationDirPath().toUtf8();
  QBuffer b;
  b.open(QBuffer::WriteOnly);
  b.write(ba);
  sm.create(b.size());
  memcpy(sm.data(), b.data(), b.size());

  // 注册元类型
  qRegisterMetaTypeStreamOperators<ClassNotice>("ClassNotice");
  qRegisterMetaTypeStreamOperators<ClassEvent>("ClassEvent");

  // 添加 MiSans 并设为默认字体
  if (int fontId =
          QFontDatabase::addApplicationFont(":/font/MiSans-Regular.ttf");
      fontId != -1)
    QApplication::setFont(
        QFontDatabase::applicationFontFamilies(fontId).first());

  createDirs();

  // 显示主面板
  MainPanel panel;
  panel.show();

  return QApplication::exec();
}