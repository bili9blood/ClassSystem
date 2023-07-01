//
// Created by 35012 on 2023/7/1.
//

#ifndef CLASSSYSTEM_CLASSDATA_H
#define CLASSSYSTEM_CLASSDATA_H
#include <QDataStream>
#include <QIODevice>
#include <QObject>
#include <QVariant>
class ClassData : public QObject {
  Q_OBJECT
 public:
  explicit ClassData(QObject *parent = nullptr);
  enum DataName : uint8_t {
    Students,
    Lessons,
    TimeLessonsStart,
    StudentsCarryMeal,
    StudentsOnDuty
  };
 public slots:
  static QVariantList readFrom(QIODevice *device);
  static void writeTo(const QVariantList &data, QIODevice *device);
};

#endif  // CLASSSYSTEM_CLASSDATA_H
