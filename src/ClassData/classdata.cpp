//
// Created by 35012 on 2023/7/1.
//

#include "ClassData.h"
ClassData::Data ClassData::readFrom(QIODevice *device) {
  if (!device->open(QIODevice::ReadOnly))
    throw std::runtime_error(
        qPrintable(QString("数据读取错误:%1").arg(device->errorString())));
  QDataStream ds(device);
  ClassData::Data d;
  ds.setVersion(QDataStream::Qt_5_15);
  ds >> d.mStudents >> d.mLessons >> d.mTimeLessonsStart >>
      d.mStudentsCarryMeals >> d.mStudentsOnDuty;
  return d;
}
void ClassData::writeTo(const ClassData::Data &d, QIODevice *device) {
  if (!device->open(QIODevice::WriteOnly))
    throw std::runtime_error(
        qPrintable(QString("无法写入文件:%1").arg(device->errorString())));
  QDataStream ds(device);
  ds.setVersion(QDataStream::Qt_5_15);
  ds << d.mStudents << d.mLessons << d.mTimeLessonsStart
     << d.mStudentsCarryMeals << d.mStudentsOnDuty;
}
