import { BrowserWindow, screen } from "electron";
import { ipcMain } from "electron";
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

  // 禁用窗口菜单
  menuWindow.hookWindowMessage(0x0116, () => {
    menuWindow.setEnabled(false);
    menuWindow.setEnabled(true);
  });

  menuWindow.on("ready-to-show", () => {
    menuWindow.show();
    setInterval(() => menuWindow.setAlwaysOnTop(true, "pop-up-menu"), 200);
  });

  if (is.dev && process.env["ELECTRON_RENDERER_URL"]) {
    menuWindow.loadURL(process.env["ELECTRON_RENDERER_URL"] + "/menuWindow.html");
  } else {
    menuWindow.loadFile(join(__dirname, "../renderer/menuWindow.html"));
  }

  let winStartPosition = { x: 0, y: 0 };
  let mouseStartPosition = { x: 0, y: 0 };
  let movingInterval: NodeJS.Timeout;

  ipcMain.on("begin-move-menu", () => {
    const winPosition = menuWindow.getPosition();
    winStartPosition = { x: winPosition[0], y: winPosition[1] };
    mouseStartPosition = screen.getCursorScreenPoint();
    clearInterval(movingInterval);
    // 新开
    movingInterval = setInterval(() => {
      // 实时更新位置
      const cursorPosition = screen.getCursorScreenPoint();
      const x = winStartPosition.x + cursorPosition.x - mouseStartPosition.x;
      const y = winStartPosition.y + cursorPosition.y - mouseStartPosition.y;
      const { width: sw, height: sh } = screen.getPrimaryDisplay().workAreaSize;
      const [ww, wh] = menuWindow.getSize();
      const bound = (min: number, val: number, max: number) => Math.max(min, Math.min(val, max));
      menuWindow.setPosition(bound(0, x, sw - ww), bound(0, y, sh - wh));
    }, 20);
  });

  ipcMain.on("end-move-menu", () => {
    clearInterval(movingInterval);
  });

  return menuWindow;
}
