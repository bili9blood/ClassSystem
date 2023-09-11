#pragma once

#include <config.h>
#include <qapplication.h>
#include <qdatetime.h>
#include <qdebug.h>
#include <qdir.h>
#include <qheaderview.h>
#include <qsettings.h>
#include <qtableview.h>
#include <qwidget.h>

#include <algorithm>
#include <format>

#ifdef _WIN32
#include <windows.h>
#include <windowsx.h>
#endif

inline void tableViewStretch(QTableView *view) {
  view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

inline void messageHandler(QtMsgType type, const QMessageLogContext &context,
                           const QString &msg) {
  QString typeStr;
  switch (type) {
    case QtDebugMsg:
      printf("%s\n", msg.toUtf8().constData());
      return;

#define PER_LOG_TYPE(x) \
  case Qt##x##Msg:      \
    typeStr = #x;       \
    break;

      PER_LOG_TYPE(Info)
      PER_LOG_TYPE(Warning)
      PER_LOG_TYPE(Critical)
      PER_LOG_TYPE(Fatal)

#undef PER_LOG_TYPE
  }
  QString text =
      QString::asprintf("[%s] [%s] %s\n",
                        QDateTime::currentDateTime()
                            .toString("yyyy-MM-dd hh:mm:ss.zzz")
                            .toUtf8()
                            .constData(),
                        typeStr.toUtf8().constData(), msg.toUtf8().constData());

  QFile logFile("logs.txt");
  if (!logFile.open(QFile::WriteOnly | QFile::Append)) return;
  logFile.write(text.toUtf8());
  logFile.close();
}

inline void initLogger() {
  QFileInfo logFileInfo(QApplication::applicationDirPath() + "/logs.txt");
  if (logFileInfo.exists() && logFileInfo.size() > 5242880 /*5Mb*/)
    QFile::remove(logFileInfo.filePath());

  QFile logFile(logFileInfo.filePath());
  logFile.open(QFile::WriteOnly | QFile::Append);
  logFile.write(R"(

------------------
 Program Started.
------------------
)");
  logFile.close();

  qInstallMessageHandler(messageHandler);
}

namespace settings {
inline QSettings ini("settings.ini", QSettings::IniFormat);

inline int smallFontSize;
inline int mediumFontSize;
inline int largeFontSize;
inline int superFontSize;

inline QSize popupMenuSize;
inline int menuButtonWidth;

inline void loadIni() {
  bool ok;
  /* ---------------------------- General --------------------------- */
  if (QSize tmp = ini.value("popupMenuSize").toSize(); !tmp.isEmpty())
    popupMenuSize = tmp;
  else
    popupMenuSize = {40, 100};

  if (int tmp = ini.value("menuButtonWidth").toInt(&ok); ok && tmp > 0)
    menuButtonWidth = tmp;
  else
    menuButtonWidth = 140;

  /* ------------------------- FontPointSize ------------------------ */
  std::tie(smallFontSize, mediumFontSize, largeFontSize, superFontSize) =
      std::tuple(16, 20, 28, 36);

  ini.beginGroup("FontPointSize");

#define PER_FONT_SIZE(x) \
  if (int tmp = ini.value(#x).toInt(&ok); ok && tmp > 0) x##FontSize = tmp;

  PER_FONT_SIZE(small)
  PER_FONT_SIZE(medium)
  PER_FONT_SIZE(large)
  PER_FONT_SIZE(super)

#undef PER_FONT_SIZE

  ini.endGroup();
}

}  // namespace settings

const QDate kForever = QDate(1970, 1, 1);

inline QColor invertColor(const QColor &color) {
  return {255 - color.red(), 255 - color.green(), 255 - color.blue()};
}

#ifdef _WIN32
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

#else
inline void setParentToDesktop(QWidget *) {}
#endif

inline void setWidgetTransparent(QWidget *widget) {
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

inline QString operator""_s(const char *str, size_t size) {
  return QByteArray(str, size);
}

inline bool copyDir(QString src, QString dst) {
  QDir srcDir(src);
  QDir dstDir(dst);

  if (!dstDir.exists()) {
    dstDir.mkdir(dstDir.absolutePath());
  }

  auto list =
      srcDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);

  for (const QFileInfo &info : list) {
    if (info.isDir()) {
      if (!copyDir(info.filePath(), dst + "/" + info.fileName())) return false;
    } else {  // info.isFile()
      QFile file(info.filePath());
      if (!file.copy(dst + "/" + info.fileName())) return false;
    }
  }

  return true;
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