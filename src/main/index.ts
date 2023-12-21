import { app, ipcMain } from "electron";
import { electronApp, is, optimizer } from "@electron-toolkit/utils";
import { createMainWindow } from "./mainWindow";
import { fetchInfo, getBackupInfo } from "./info";
import { fetchSentences } from "./sentences";
import { clearInterval } from "timers";
import { fetchWeather } from "./weather";
import { checkUpdate } from "./checkUpdate";
import { createMenuWindow } from "./menuWindow";

process.env["ELECTRON_DISABLE_SECURITY_WARNINGS"] = "true";

// App 单例化
if (!app.requestSingleInstanceLock()) app.quit();

// 禁用硬件加速，不禁用无法在桌面实现透明
app.disableHardwareAcceleration();

ipcMain.addListener("quit", () => app.quit());

app.whenReady().then(() => {
  electronApp.setAppUserModelId("com.class-system");
  app.on("browser-window-created", (_, window) => {
    optimizer.watchWindowShortcuts(window);
  });

  const mainWindow = createMainWindow();
  const fetchInfoAndSend = async () => {
    mainWindow.webContents.send("fetched-info", await fetchInfo());
  };
  const fetchWeatherAndSend = async () => {
    mainWindow.webContents.send("weather", await fetchWeather());
  };

  let timerFetchInfo: NodeJS.Timer;
  let timerFetchWeather: NodeJS.Timer;
  mainWindow.on("show", async () => {
    clearInterval(timerFetchInfo);
    clearInterval(timerFetchWeather);
    mainWindow.webContents.send("backup-info", await getBackupInfo());
    mainWindow.webContents.send("sentences", await fetchSentences());
    mainWindow.webContents.send("version", app.getVersion());

    await fetchInfoAndSend();
    timerFetchInfo = setInterval(fetchInfoAndSend, 10_000);

    await fetchWeatherAndSend();
    // 每 30 分钟请求一次天气
    timerFetchWeather = setInterval(fetchWeatherAndSend, 1000 * 60 * 30);

    if (!is.dev) await checkUpdate(app.getVersion());
  });

  const menuWindow = createMenuWindow();
});
