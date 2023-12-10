#include <Windows.h>
#include <napi.h>

Napi::Value embedDesktop(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::Buffer<void *> wndHandle = info[0].As<Napi::Buffer<void *>>();
  HWND hwnd = static_cast<HWND>(*reinterpret_cast<void **>(wndHandle.Data()));
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
    ::SetParent(hwnd, resultHwnd);
  }
  return env.Undefined();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("embedDesktop", Napi::Function::New(env, embedDesktop));
  return exports;
}
NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
