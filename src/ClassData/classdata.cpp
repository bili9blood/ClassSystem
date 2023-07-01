//
// Created by 35012 on 2023/7/1.
//

#include "ClassData.h"
ClassData::ClassData(QObject *parent) : QObject(parent) {}
QVariantList ClassData::readFrom(QIODevice *device) {
  auto fileErr = [] {
    QMessageBox::critical(nullptr, "错误", "无法加载数据文件，请检查文件！");
    QApplication::quit();
  };

  if (!device->open(QIODevice::ReadOnly)) fileErr();
  QDataStream dataStream(device);
  QVariantList data;
  dataStream.setVersion(QDataStream::Qt_5_15);
  dataStream >> data[Students]    // QMap<QString>   (k:v)id:name
      >> data[Lessons]            // QList<QStringList>
      >> data[TimeLessonsStart]   // QList<QTime>
      >> data[StudentsCarryMeal]  // QList<QStringList>
      >> data[StudentsOnDuty]     // QList<QStringList>
      ;
}
void ClassData::writeTo(const QVariantList &data, QIODevice *device) {
  auto fileErr = [] {
    QMessageBox::critical(nullptr, "错误", "无法保存数据文件，请检查文件！");
    QApplication::quit();
  };

  if (!device->open(QIODevice::WriteOnly)) fileErr();
  QDataStream dataStream(device);
  dataStream.setVersion(QDataStream::Qt_5_15);
  dataStream << data[Students]           // QMap<QString>   (k:v)id:name
             << data[Lessons]            // QList<QStringList>
             << data[TimeLessonsStart]   // QList<QTime>
             << data[StudentsCarryMeal]  // QList<QStringList>
             << data[StudentsOnDuty]     // QList<QStringList>
      ;
}
