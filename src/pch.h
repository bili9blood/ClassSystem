#pragma once

#include <config.h>
#include <windows.h>
#include <windowsx.h>

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
#include <utility>

using namespace std::string_literals;

static WINBOOL enumWinProc(HWND hwnd, LPARAM) {
  HWND hDefView = FindWindowEx(hwnd, nullptr, "SHELLDLL_DefView", nullptr);
  if (hDefView != nullptr) {
    HWND hWorkerW = FindWindowEx(nullptr, hwnd, "WorkerW", nullptr);
    ShowWindow(hWorkerW, SW_HIDE);
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
struct SemVer {
  uint8_t major, minor, patch;
  inline QString operator()() const {
    return QString("%1.%2.%3")
        .arg(uint(major))
        .arg(uint(minor))
        .arg(uint(patch));
  }
};

constexpr SemVer currentVersion{MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION};

inline int weekday(const QString &s) {
  if (s == "周一") return 0;
  if (s == "周二") return 1;
  if (s == "周三") return 2;
  if (s == "周四") return 3;
  if (s == "周五") return 4;
  if (s == "周六") return 5;
  if (s == "周日") return 6;
  return -1;
}

//! override
inline QString weekday(const int &i) {
  switch (i) {
    case 0:
      return {"周一"};
    case 1:
      return {"周二"};
    case 2:
      return {"周三"};
    case 3:
      return {"周四"};
    case 4:
      return {"周五"};
    case 5:
      return {"周六"};
    case 6:
      return {"周日"};
    default:
      return {};
  }
}
