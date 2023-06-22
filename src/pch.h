#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QHash>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QtGlobal>


constexpr int daysInWeek(const QString &s) {
  if (s == "周一")
    return 0;
  if (s == "周二")
    return 1;
  if (s == "周三")
    return 2;
  if (s == "周四")
    return 3;
  if (s == "周五")
    return 4;
  if (s == "周六")
    return 5;
  if (s == "周日")
    return 6;
}

constexpr const QString &daysInWeek(const int &i) {
  switch (i) {
  case 0:
    return std::move(QString("周一"));
  case 1:
    return std::move(QString("周二"));
  case 2:
    return std::move(QString("周三"));
  case 3:
    return std::move(QString("周四"));
  case 4:
    return std::move(QString("周五"));
  case 5:
    return std::move(QString("周六"));
  case 6:
    return std::move(QString("周日"));
  }
}