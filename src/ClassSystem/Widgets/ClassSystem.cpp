#include "ClassSystem.h"

#include "ui_ClassSystem.h"

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

ClassSystem::ClassSystem(QWidget *parent)
    : QMainWindow{parent, Qt::FramelessWindowHint}, ui(new Ui::ClassSystem) {
  ui->setupUi(this);
  setParentToDesktop((HWND)winId());
  showFullScreen();
  setAttribute(Qt::WA_TranslucentBackground);
}

ClassSystem::~ClassSystem() { delete ui; }
void ClassSystem::paintEvent(QPaintEvent *ev) {
  QPainter painter(this);

  // painter.setRenderHint(QPainter::Antialiasing);
  // painter.setPen(Qt::transparent);
  // painter.setBrush(Qt::white);
  // painter.drawRoundedRect(rect(), 15, 15);
}

void ClassSystem::mouseMoveEvent(QMouseEvent *ev) { qDebug() << ev->pos(); }