#pragma once

#include <config.h>
#include <qapplication.h>
#include <qdebug.h>
#include <qwidget.h>
#include <windows.h>
#include <windowsx.h>

inline QColor invertColor(const QColor &color) {
  return {255 - color.red(), 255 - color.green(), 255 - color.blue()};
}

inline void setParentToDesktop(HWND hwnd) {
  HWND hProgman = FindWindow("Progman", nullptr);
  SendMessage(hProgman, 0x52c, 0, 0);
  SetParent(hwnd, hProgman);
  EnumWindows(
      (int (*)(HWND, LPARAM))  // cast
          [](HWND hwnd, LPARAM) {
            HWND hDefView =
                FindWindowEx(hwnd, nullptr, "SHELLDLL_DefView", nullptr);
            if (hDefView != nullptr) {
              HWND hWorkerW = FindWindowEx(nullptr, hwnd, "WorkerW", nullptr);
              ShowWindow(hWorkerW, SW_HIDE);
              return FALSE;
            }
            return TRUE;
          },
      0);
}

constexpr void setWidgetTransparent(QWidget *widget) {
  widget->setAttribute(Qt::WA_TranslucentBackground);
}

struct qFont {
  const QString &family = "MiSans";
  int pointSize = -1;
  int weight = -1;
  bool italic = false;
  inline QFont operator()() { return {family, pointSize, weight, italic}; }
};

inline QStringList matchedList(const QString &str, const QString &cap) {
  QStringList list;
  QRegExp rx(cap);

  int pos = 0;
  while ((pos = rx.indexIn(str, pos)) != -1) {
    int n = rx.matchedLength();
    list.append(str.mid(pos, n));
    pos += n;
  }
  return list;
}

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
