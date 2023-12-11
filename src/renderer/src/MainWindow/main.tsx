import { render } from "solid-js/web";
import "./assets/index.css";
import App from "./App";
import { setInfo, setIsBackup } from "./stores/info";
import { Info } from "@renderer/types/info";
import { Sentence } from "@renderer/types/sentences";
import { setSentences } from "./stores/sentences";

render(() => <App />, document.getElementById("root") as HTMLElement);

window.ipcRenderer.on("backup-info", (_, info: Info | null) => {
  if (info) setInfo(info);
  setIsBackup(true);
});

window.ipcRenderer.on("fetched-info", (_, info: Info | null) => {
  if (info) setInfo(info);
  setIsBackup(false);
});

window.ipcRenderer.on("sentences", (_, sentences: Sentence[]) => {
  setSentences(sentences);
});
