import { Menu, Tray } from "electron";
import { join } from "path";

export function createTray() {
  const tray = new Tray(join(__dirname, "../../resources/logo.ico"));
  const contextMenu = Menu.buildFromTemplate([
    {
      label: "表格",
      click: () => {}
    },
    {
      label: "随机点名",
      click: () => {}
    },
    {
      label: "刷新",
      click: () => {}
    }
  ]);
  tray.setToolTip("ClassSystem 班级系统");
  tray.setContextMenu(contextMenu);
  tray.on("click", () => tray.popUpContextMenu());
  return tray;
}
