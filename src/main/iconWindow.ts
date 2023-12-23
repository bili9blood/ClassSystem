import { BrowserWindow, screen } from "electron";
import { ipcMain } from "electron";
import { join } from "path";
import { is } from "@electron-toolkit/utils";

export function createIconWindow() {
  const iconSize = 50;

  const win = new BrowserWindow({
    width: iconSize,
    height: iconSize,
    useContentSize: true,
    type: "toolbar",
    frame: false,
    resizable: false,
    transparent: true,
    fullscreenable: false,
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
  win.hookWindowMessage(0x0116, () => {
    win.setEnabled(false);
    win.setEnabled(true);
  });

  win.on("ready-to-show", () => {
    win.show();
    setInterval(() => win.setAlwaysOnTop(true, "pop-up-menu"), 200);
  });

  if (is.dev && process.env["ELECTRON_RENDERER_URL"]) {
    win.loadURL(process.env["ELECTRON_RENDERER_URL"] + "/iconWindow.html");
  } else {
    win.loadFile(join(__dirname, "../renderer/iconWindow.html"));
  }

  win.webContents.setWindowOpenHandler(() => {
    return {
      action: "allow",
      overrideBrowserWindowOptions: {
        width: 800,
        height: 600,
        frame: false,
        center: true,
        resizable: false,
        transparent: true,
        fullscreenable: false,
        backgroundColor: "#00000000",
        type: "toolbar",
        alwaysOnTop: true,
        webPreferences: {
          sandbox: false,
          nodeIntegration: true,
          contextIsolation: false,
          preload: join(__dirname, "../preload/preload.js")
        }
      }
    };
  });

  let winStartPosition = { x: 0, y: 0 };
  let mouseStartPosition = { x: 0, y: 0 };
  let movingInterval: NodeJS.Timer;

  ipcMain.on("begin-move-menu", () => {
    const winPosition = win.getPosition();
    winStartPosition = { x: winPosition[0], y: winPosition[1] };
    mouseStartPosition = screen.getCursorScreenPoint();
    clearInterval(movingInterval);
    // 新开
    movingInterval = setInterval(() => {
      // 实时更新位置
      const cursorPosition = screen.getCursorScreenPoint();
      const x = winStartPosition.x + cursorPosition.x - mouseStartPosition.x;
      const y = winStartPosition.y + cursorPosition.y - mouseStartPosition.y;

      win.setContentBounds({ x, y, width: iconSize, height: iconSize });
    });
  });

  ipcMain.on("end-move-menu", () => {
    clearInterval(movingInterval);

    // 窗口不能移出屏幕
    const { width: sw, height: sh } = screen.getPrimaryDisplay().size;
    const [x, y] = win.getPosition();
    const bound = (min: number, val: number, max: number) => Math.max(min, Math.min(val, max));

    win.setPosition(bound(0, x, sw - iconSize), bound(0, y, sh - iconSize));
  });

  win.on("closed", () => ipcMain.emit("quit"));
  return win;
}
