#include "classdata.h"

ClassData::ClassData(QIODevice* device, QObject* parent)
    : QObject(parent), mDevice(device) {
  // testData();
  load();
}

ClassData::~ClassData() { save(); }

void ClassData::load() {
  if (!mDevice->open(QFile::ReadOnly)) {
    qDebug("读取时文件打开失败!");
    throw QString("读取时文件打开失败");
  }
  dataStream.setDevice(mDevice);

  dataStream.setVersion(QDataStream::Qt_5_15);
  dataStream >> onDuty >> fan >> notices >> students >> lessons >>
      timeLessonsStart >> numOfStudents >> lastGroup >> lastWorked >>
      eventName >> eventDate;
  mDevice->close();
  qDebug("data file loaded");
  if (lastWorked < QDate::currentDate()) {
    lastWorked = QDate::currentDate();
    lastGroup += 3;
    lastGroup %= 14;
  }
  // delete notices which expired
  foreach (const QString& str, notices)
    if (!str.startsWith("NULL") &&
        QDate::fromString(str.left(str.indexOf("__"))) >= QDate::currentDate())
      notices.removeOne(str);
}

void ClassData::save() {
  if (!mDevice->open(QFile::WriteOnly | QFile::Truncate)) {
    qDebug("保存时文件打开失败！");
    throw QString("保存时文件打开失败！");
  }
  dataStream.setDevice(mDevice);
  dataStream.setVersion(QDataStream::Qt_5_15);
  dataStream << onDuty << fan << notices << students << lessons
             << timeLessonsStart << numOfStudents << lastGroup << lastWorked
             << eventName << eventDate;
  mDevice->close();
  qDebug("data file saved");
}
