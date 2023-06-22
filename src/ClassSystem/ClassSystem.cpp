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
    : QMainWindow{parent}, ui(new Ui::ClassSystem) {
  ui->setupUi(this);
  setParentToDesktop((HWND)winId());
  show();
}

ClassSystem::~ClassSystem() { delete ui; }