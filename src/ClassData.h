#pragma once
#pragma warning(disable : 4267)

#include <qdatetime.h>
#include <qlist.h>
#include <qlocalsocket.h>
#include <qmap.h>

#include <algorithm>
#include <format>
#include <utility>

struct ClassNotice {
  QDate date;
  QString str;
  int fontPtSize = 20;
};

struct ClassEvent {
  QString name;
  QDate date;
  bool important;

  bool operator>(const ClassEvent &other) const {
    if (important != other.important) return important;
    return date < other.date;
  }
};

namespace ClassData {
struct Data {
  Data() {
    using StringVectr = QVector<QString>;
    lessons << StringVectr() << StringVectr() << StringVectr() << StringVectr()
            << StringVectr();

    using MealStuEachDay = QVector<uint>;
    mealStu << MealStuEachDay() << MealStuEachDay() << MealStuEachDay()
            << MealStuEachDay() << MealStuEachDay();

    using StuOnDutyEachDay = QVector<QVector<uint>>;
    stuOnDuty << StuOnDutyEachDay() << StuOnDutyEachDay() << StuOnDutyEachDay()
              << StuOnDutyEachDay() << StuOnDutyEachDay();
  }

  QMap<uint, QString> students;
  QVector<QVector<QString>> lessons;
  QVector<QTime> lessonsTm;
  QVector<QVector<uint>> mealStu;
  QVector<QVector<QVector<uint>>> stuOnDuty;
  QVector<QString> dutyJobs;
  QVector<ClassNotice> notices;
  QVector<ClassEvent> events;
  inline QString idAndName(const uint &id) {
    if (!id) return {};
    return QString::fromStdString(
        std::format("{:02d}{}", id, students[id].toStdString()));
  }
};

inline ClassData::Data parse(const nlohmann::json &data) {
  ClassData::Data ret;

  // students
  for (auto it = data["students"].cbegin(); it != data["students"].cend(); ++it)
    ret.students[std::stoi(it.key())] = QString::fromStdString(it.value());

  // lessons
  for (int i = 0; i < 5; ++i) {
    auto lessonsCountInOneDay = data["lessons"][i].size();
    ret.lessons[i].resize(lessonsCountInOneDay);
    for (int j = 0; j < lessonsCountInOneDay; ++j)
      ret.lessons[i][j] = QString::fromStdString(data["lessons"][i][j]);
  }

  // lessonsTm
  auto lessonsTmCount = data["lessonsTm"].size();
  ret.lessonsTm.resize(lessonsTmCount);
  for (int i = 0; i < lessonsTmCount; ++i)
    ret.lessonsTm[i] = QTime::fromMSecsSinceStartOfDay(data["lessonsTm"][i]);

  // mealStu
  for (int i = 0; i < 5; ++i) {
    auto vec = data["mealStu"][i].get<std::vector<int>>();
    ret.mealStu[i] = {vec.cbegin(), vec.cend()};
  }

  // stuOnDuty
  for (int i = 0; i < 5; ++i) {
    auto jobsCount = data["stuOnDuty"][i].size();
    ret.stuOnDuty[i].resize(jobsCount);

    for (int j = 0; j < jobsCount; ++j) {
      auto vec = data["stuOnDuty"][i][j].get<std::vector<int>>();
      ret.stuOnDuty[i][j] = {vec.cbegin(), vec.cend()};
    }
  }

  // dutyJobs
  ret.dutyJobs.resize(data["dutyJobs"].size());
  for (auto [retIt, jsonIt] =
           std::pair(ret.dutyJobs.begin(), data["dutyJobs"].cbegin());
       retIt != ret.dutyJobs.end() && jsonIt != data["dutyJobs"].cend();
       ++retIt, ++jsonIt)
    *retIt = QString::fromStdString(jsonIt.value());

  // notices
  auto noticesCount = data["notices"].size();
  ret.notices.resize(noticesCount);
  for (int i = 0; i < noticesCount; ++i) {
    auto notice = data["notices"][i];
    ret.notices[i] = {QDate::fromJulianDay(notice["date"]),
                      QString::fromStdString(notice["str"]),
                      notice["fontPtSize"]};
  }

  // events
  auto eventsCount = data["events"].size();
  ret.events.resize(eventsCount);
  for (int i = 0; i < eventsCount; ++i) {
    auto ev = data["events"][i];
    ret.events[i] = {QString::fromStdString(ev["name"]),
                     QDate::fromJulianDay(ev["date"]), ev["important"]};
  }

  return ret;
}
}  // namespace ClassData

inline ClassData::Data classData;