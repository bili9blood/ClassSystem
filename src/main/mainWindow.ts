import { BrowserWindow, screen, ipcMain } from "electron";
import { join } from "path";
import { is } from "@electron-toolkit/utils";
import embed_desktop from "../native/build/Release/embed-desktop.node";
import { saveSettings, useSettings } from "./settings";
export function createMainWindow() {
  const mainWindow = new BrowserWindow({
    width: 960,
    height: 720,
    icon: join(__dirname, "../../resources/logo.png"),
    show: false,
    frame: false,
    transparent: true,
    resizable: false,
    backgroundColor: "#00000000",
    webPreferences: {
      sandbox: false,
      nodeIntegration: true,
      contextIsolation: false,
      preload: join(__dirname, "../preload/preload.js")
    }
  });

  // 禁用窗口菜单
  mainWindow.hookWindowMessage(0x0116, () => {
    mainWindow.setEnabled(false);
    mainWindow.setEnabled(true);
  });

  // 防止启动白屏，页面挂载完成再显示窗口
  ipcMain.on("mounted", () => {
    mainWindow.show();
  });

  // 窗口不能移出屏幕
  mainWindow.on("moved", () => {
    const { width: sw, height: sh } = screen.getPrimaryDisplay().workAreaSize;
    const [ww, wh] = mainWindow.getSize();
    const [x, y] = mainWindow.getPosition();
    const bound = (min: number, val: number, max: number) => Math.max(min, Math.min(val, max));
    mainWindow.setPosition(bound(0, x, sw - ww), bound(0, y, sh - wh));

    (async () =>
      saveSettings(Object.assign(await useSettings(), { position: mainWindow.getPosition() })))();
  });

  if (is.dev && process.env["ELECTRON_RENDERER_URL"]) {
    mainWindow.loadURL(process.env["ELECTRON_RENDERER_URL"] + "/mainWindow.html");
  } else {
    mainWindow.loadFile(join(__dirname, "../renderer/mainWindow.html"));
  }

  mainWindow.on("show", async () => {
    embed_desktop.embedDesktop(mainWindow.getNativeWindowHandle());
    const [x, y] = (await useSettings()).position;
    mainWindow.setPosition(x, y);
  });

  mainWindow.on("closed", () => ipcMain.emit("quit"));
  mainWindow.webContents.emit("page", "MainWindow");
  return mainWindow;
}
