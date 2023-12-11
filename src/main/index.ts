import { app, ipcMain } from "electron";
import { electronApp, optimizer } from "@electron-toolkit/utils";
import { createMainWindow } from "./mainWindow";
import { fetchInfo, getBackupInfo } from "./info";
import { fetchSentences } from "./sentences";

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
  mainWindow.on("show", async () => {
    mainWindow.webContents.send("backup-info", await getBackupInfo());
    mainWindow.webContents.send("fetched-info", await fetchInfo());
    mainWindow.webContents.send("sentences", await fetchSentences());
  });
});
