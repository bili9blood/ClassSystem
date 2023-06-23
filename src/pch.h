#pragma once

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QEvent>
#include <QFile>
#include <QHash>
#include <QObject>
#include <QPainter>
#include <QPointer>
#include <QSettings>
#include <QString>
#include <QtGlobal>
#include <config.h>
#include <windows.h>
#include <windowsx.h>

static WINBOOL enumWinProc(HWND hwnd, LPARAM lParam) {
  HWND hDefView = FindWindowEx(hwnd, 0, "SHELLDLL_DefView", 0);
  if (hDefView != 0) {
    HWND hWorkerw = FindWindowEx(0, hwnd, "WorkerW", 0);
    ShowWindow(hWorkerw, SW_HIDE);
    return FALSE;
  }
  return TRUE;
}
static void setParentToDesktop(HWND hwnd) {
  HWND hProgman = FindWindow("Progman", nullptr);
  SendMessage(hProgman, 0x52c, 0, 0);
  SetParent(hwnd, hProgman);
  EnumWindows(enumWinProc, 0);
}

#define SET_WIDGET_TRANSPARENT setAttribute(Qt::WA_TranslucentBackground)
inline int daysInWeek(const QString &s) {
  if (0 == s.compare(QString("周一")))
    return 0;
  if (0 == s.compare(QString("周二")))
    return 1;
  if (0 == s.compare(QString("周三")))
    return 2;
  if (0 == s.compare(QString("周四")))
    return 3;
  if (0 == s.compare(QString("周五")))
    return 4;
  if (0 == s.compare(QString("周六")))
    return 5;
  if (0 == s.compare(QString("周日")))
    return 6;
}

inline QString daysInWeek(const int &i) {
  switch (i) {
  case 0:
    return QString("周一");
  case 1:
    return QString("周二");
  case 2:
    return QString("周三");
  case 3:
    return QString("周四");
  case 4:
    return QString("周五");
  case 5:
    return QString("周六");
  case 6:
    return QString("周日");
  }
}
