#include <Windows.h>
#include <io.h>

#include <functional>
#include <string>
#include <thread>

constexpr const char latestFileName[] = "LATEST";
constexpr const char mainProgramName[] = "ClassSystem.exe";

int main(int argc, char** argv) {
  if (argc < 2 || _access(latestFileName, 0)) return 1;

  // 主程序进程PID
  const std::string pid = argv[1];

  // 结束主程序
  system(("taskkill /f /pid " + pid).c_str());

  // 等待主程序完全关闭
  ::Sleep(500);

  ::CopyFile(latestFileName, mainProgramName, false);
  ::DeleteFile(latestFileName);

  // 重新启动主程序
  std::thread th(std::bind(system, mainProgramName));
  th.detach();
}