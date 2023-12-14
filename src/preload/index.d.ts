import { type IpcRenderer } from "electron";
declare global {
  interface Window {
    class_system: IpcRenderer;
  }
}
