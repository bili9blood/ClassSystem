#pragma once

#include <config.h>
#include <qapplication.h>
#include <qdatetime.h>
#include <qdebug.h>
#include <qdir.h>
#include <qwidget.h>
#include <windows.h>
#include <windowsx.h>

const QDate kForever = QDate(1970, 1, 1);

inline QColor invertColor(const QColor &color) {
  return {255 - color.red(), 255 - color.green(), 255 - color.blue()};
}

inline void setParentToDesktop(QWidget *pWidget) {
  HWND resultHwnd = nullptr;
  ::EnumWindows(
      static_cast<WNDENUMPROC>(
          // lambda: 通过枚举找到桌面图标窗口
          [](HWND hwnd, LPARAM lParam) {
            long wflags = ::GetWindowLong(hwnd, GWL_STYLE);
            if (!(wflags & WS_VISIBLE)) {
              return TRUE;
            };

            HWND sndWnd;
            if (!(sndWnd = ::FindWindowExW(hwnd, nullptr, L"SHELLDLL_DefView",
                                           nullptr))) {
              return TRUE;
            }
            HWND targetWnd;
            if (!(targetWnd = ::FindWindowExW(sndWnd, nullptr, L"SysListView32",
                                              L"FolderView"))) {
              return TRUE;
            }

            HWND *resultHwnd = (HWND *)lParam;
            *resultHwnd = targetWnd;
            return FALSE;
          }),
      (LPARAM)&resultHwnd);

  // 设置桌面为父窗口
  if (resultHwnd) {
    ::SetParent((HWND)pWidget->winId(), resultHwnd);
  }
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

constexpr const char kClassAdminSpec[] = "A";
constexpr const char kClassSystemSpec[] = "S";
enum class MsgType : uint { Request, Save };

struct ChangeStatus {
  ChangeStatus() {}
  ChangeStatus(bool v) : m_changed(v) {}

  void operator()() { m_changed = true; }
  [[nodiscard]] operator bool &() { return m_changed; }

 private:
  bool m_changed = false;
};

inline QString operator""_s(const char *str, size_t size) {
  return QByteArray(str, size);
}

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

inline int oneDayOfWeek(const QString &s) {
  if (s == "周一") return 0;
  if (s == "周二") return 1;
  if (s == "周三") return 2;
  if (s == "周四") return 3;
  if (s == "周五") return 4;

  // 数据里只有工作日的信息，按正常来会越界访问
  if (s == "周六") return 0;
  if (s == "周日") return 1;
  return -1;
}

//! override
inline QString oneDayOfWeek(const int &i) {
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

inline int dayToday() {
  return oneDayOfWeek(QDate::currentDate().toString("ddd"));
}