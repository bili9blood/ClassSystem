#pragma once

#include <qdatetime.h>
#include <qlist.h>
#include <qlocalsocket.h>
#include <qmap.h>

#include <algorithm>
#include <format>
#include <queue>
#include <utility>

struct ClassNotice {
  QDate date;
  QString str;
  int fontPtSize = 20;
};

Q_DECLARE_METATYPE(ClassNotice)

inline QDataStream &operator<<(QDataStream &out, const ClassNotice &notice) {
  return out << notice.date << notice.str << notice.fontPtSize;
}

inline QDataStream &operator>>(QDataStream &in, ClassNotice &notice) {
  return in >> notice.date >> notice.str >> notice.fontPtSize;
}

struct ClassEvent {
  QString name;
  QDate date;
  bool important;

  bool operator<(const ClassEvent &other) const {
    if (important != other.important) return other.important;
    return date > other.date;
  }
};

Q_DECLARE_METATYPE(ClassEvent)

inline QDataStream &operator<<(QDataStream &out, const ClassEvent &event) {
  return out << event.name << event.date << event.important;
}

inline QDataStream &operator>>(QDataStream &in, ClassEvent &event) {
  return in >> event.name >> event.date >> event.important;
}

namespace ClassData {
struct Data {
  QMap<uint, QString> students;
  QList<QStringList> lessons;
  QList<QTime> LessonsTm;
  QList<QList<uint>> mealStu;
  QList<QList<QList<uint>>> stuOnDuty;
  QList<QString> dutyJobs;
  QList<ClassNotice> notices;
  std::priority_queue<ClassEvent> events;
  inline QString idAndName(const uint &id) {
    if (!id) return {};
    return QString::fromStdString(
        std::format("{:02d}{}", id, students[id].toStdString()));
  }
};

inline bool writeTo(const ClassData::Data &d, QIODevice *device,
                    bool shouldClose = true) {
  if (!device->isOpen() &&
      !device->open(QIODevice::ReadWrite | QIODevice::Truncate)) {
    // qDebug() << device->errorString();
    return false;
  }
  QDataStream ds(device);
  ds.setVersion(QDataStream::Qt_5_15);
  ds << d.students << d.lessons << d.LessonsTm << d.mealStu << d.stuOnDuty
     << d.dutyJobs;
  ds << (int)d.notices.size();
  auto q = d.events;
  ds << (int)q.size();
  for (const auto &n : d.notices) ds << QVariant::fromValue(n);

  while (q.size()) {
    ds << QVariant::fromValue(q.top());
    q.pop();
  }
  if (shouldClose) device->close();
  return true;
}

inline bool readFrom(QIODevice *device, ClassData::Data &data,
                     bool shouldClose = true) {
  if (!device->isOpen() && !device->open(QIODevice::ReadWrite)) {
    // qDebug() << "ClassData::ReadFrom " << device->errorString();
    return false;
  }
  QDataStream ds(device);
  ClassData::Data d;
  ds.setVersion(QDataStream::Qt_5_15);
  int noticesSize, eventsSize;
  ds >> d.students >> d.lessons >> d.LessonsTm >> d.mealStu >> d.stuOnDuty >>
      d.dutyJobs >> noticesSize >> eventsSize;

  for (int i = 0; i < noticesSize; ++i) {
    QVariant v;
    ds >> v;
    const auto &[date, str, fontPtSize] = v.value<ClassNotice>();
    if (date == kForever || date > QDate::currentDate())
      d.notices << ClassNotice{date, str, fontPtSize};
  }
  for (int i = 0; i < eventsSize; ++i) {
    QVariant v;
    ds >> v;
    if (v.value<ClassEvent>().date >= QDate::currentDate())
      d.events.push(v.value<ClassEvent>());
  }
  if (!ClassData::writeTo(d, device, shouldClose)) return false;

  data = d;
  return true;
}
}  // namespace ClassData
