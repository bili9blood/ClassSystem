import { app, shell, BrowserWindow } from "electron";
import { join } from "path";
import { electronApp, optimizer, is } from "@electron-toolkit/utils";
import fs, { accessSync, readFileSync } from "fs";
import embed_desktop from "../native/build/Release/embed-desktop.node";

class AppSettings {
  position: { x: number; y: number } = { x: 500, y: 100 };
}

let appSettings: AppSettings;

try {
  accessSync("settings.json", fs.constants.R_OK | fs.constants.W_OK);
  appSettings = JSON.parse(readFileSync("settings.json").toString());
} catch (e) {
  appSettings = new AppSettings();
}

// 程序单例化
if (!app.requestSingleInstanceLock()) app.quit();

function createWindow(): void {
  // Create the browser window.
  const mainWindow = new BrowserWindow({
    ...appSettings.position,

    show: false,
    frame: false,
    transparent: true,
    resizable: true,

    webPreferences: {
      preload: join(__dirname, "../preload/index.js"),
      sandbox: false
    }
  });

  mainWindow.on("ready-to-show", () => {
    mainWindow.show();
  });

  mainWindow.webContents.setWindowOpenHandler((details) => {
    shell.openExternal(details.url);
    return { action: "deny" };
  });

  // HMR for renderer base on electron-vite cli.
  // Load the remote URL for development or the local html file for production.
  if (is.dev && process.env["ELECTRON_RENDERER_URL"]) {
    mainWindow.loadURL(process.env["ELECTRON_RENDERER_URL"]);
  } else {
    mainWindow.loadFile(join(__dirname, "../renderer/index.html"));
  }

  mainWindow.hookWindowMessage(278 /* , function () {
    mainWindow.blur();
    mainWindow.focus();
    mainWindow.setEnabled(false);
    setTimeout(() => {
      mainWindow.setEnabled(true);
    }, 100);
    return true;
  });
}

app.disableHardwareAcceleration();

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.whenReady().then(() => {
  // Set app user model id for windows
  electronApp.setAppUserModelId("com.electron");

  // Default open or close DevTools by F12 in development
  // and ignore Command/Control + R in production.
  // see https://github.com/alex8088/electron-toolkit/tree/master/packages/utils
  app.on("browser-window-created", (_, window) => {
    optimizer.watchWindowShortcuts(window);
    try {
      embed_desktop.embed(window.getNativeWindowHandle());
    } catch (e) {
      console.log(e);
    }
  });

  createWindow();

  app.on("activate", function () {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (BrowserWindow.getAllWindows().length === 0) createWindow();
  });
});

// Quit when all windows are closed, except on macOS. There, it's common
// for applications and their menu bar to stay active until the user quits
// explicitly with Cmd + Q.
app.on("window-all-closed", () => {
  if (process.platform !== "darwin") {
    app.quit();
  }
});
