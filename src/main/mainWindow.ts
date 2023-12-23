import { BrowserWindow, screen, ipcMain } from "electron";
import { join } from "path";
import { is } from "@electron-toolkit/utils";
import embed_desktop from "../native/build/Release/embed-desktop.node";
import { saveSettings, useSettings } from "./settings";
export function createMainWindow() {
  const win = new BrowserWindow({
    show: false,
    frame: false,
    fullscreenable: false,
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
  win.hookWindowMessage(0x0116, () => {
    win.setEnabled(false);
    win.setEnabled(true);
  });

  // 防止启动白屏，页面挂载完成再显示窗口
  ipcMain.on("mainwindow-mounted", () => {
    win.show();
  });

  // 窗口不能移出屏幕
  win.on("moved", () => {
    const { width: sw, height: sh } = screen.getPrimaryDisplay().workAreaSize;
    const [ww, wh] = win.getSize();
    const [x, y] = win.getPosition();
    const bound = (min: number, val: number, max: number) => Math.max(min, Math.min(val, max));
    win.setPosition(bound(0, x, sw - ww), bound(0, y, sh - wh));

    (async () =>
      saveSettings(Object.assign(await useSettings(), { position: win.getPosition() })))();
  });

  if (is.dev && process.env["ELECTRON_RENDERER_URL"]) {
    win.loadURL(process.env["ELECTRON_RENDERER_URL"] + "/mainWindow.html");
  } else {
    win.loadFile(join(__dirname, "../renderer/mainWindow.html"));
  }

  win.on("show", async () => {
    embed_desktop.embedDesktop(win.getNativeWindowHandle());
    const settings = await useSettings();
    win.setSize(settings.size[0], settings.size[1]);
    win.setPosition(settings.position[0], settings.position[1]);
  });

  win.on("closed", () => ipcMain.emit("quit"));
  return win;
}
