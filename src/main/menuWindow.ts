import { BrowserWindow, screen } from "electron";
import remote from "@electron/remote/main";
import { join } from "path";
import { is } from "@electron-toolkit/utils";

export function createMenuWindow() {
  const menuWindow = new BrowserWindow({
    width: 310,
    height: 300,
    useContentSize: true,
    type: "toolbar",
    frame: false,
    resizable: false,
    transparent: true,
    backgroundColor: "#00000000",
    alwaysOnTop: true,
    webPreferences: {
      sandbox: false,
      nodeIntegration: true,
      contextIsolation: false,
      preload: join(__dirname, "../preload/preload.js")
    }
  });
  remote.enable(menuWindow.webContents);

  // 禁用窗口菜单
  menuWindow.hookWindowMessage(0x0116, () => {
    menuWindow.setEnabled(false);
    menuWindow.setEnabled(true);
  });

  menuWindow.on("ready-to-show", () => {
    menuWindow.show();
    setInterval(() => menuWindow.setAlwaysOnTop(true, "pop-up-menu"), 200);
  });

  // 窗口不能移出屏幕
  menuWindow.on("moved", () => {
    const { width: sw, height: sh } = screen.getPrimaryDisplay().workAreaSize;
    const [ww, wh] = menuWindow.getSize();
    const [x, y] = menuWindow.getPosition();
    const bound = (min: number, val: number, max: number) => Math.max(min, Math.min(val, max));
    menuWindow.setPosition(bound(0, x, sw - ww), bound(0, y, sh - wh));
  });

  if (is.dev && process.env["ELECTRON_RENDERER_URL"]) {
    menuWindow.loadURL(process.env["ELECTRON_RENDERER_URL"] + "/menuWindow.html");
  } else {
    menuWindow.loadFile(join(__dirname, "../renderer/menuWindow.html"));
  }

  return menuWindow;
}
