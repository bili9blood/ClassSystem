import { render } from "solid-js/web";
import "./index.css";
import "qweather-icons/font/qweather-icons.css";
import App from "./App";
import { info, setInfo, setIsBackup } from "./stores/info";
import { Info } from "@renderer/types/info";
import { Sentence } from "@renderer/types/sentences";
import { setSentences } from "./stores/sentences";

window.class_system.on("backup-info", (_, info: Info | null) => {
  if (info) setInfo(info);
  setIsBackup(true);
});

window.class_system.on("fetched-info", (_, _info: Info | null) => {
  if (_info) {
    if (JSON.stringify(_info) !== JSON.stringify(info())) setInfo(_info);
  }
  setIsBackup(false);
});

window.class_system.on("sentences", (_, sentences: Sentence[]) => {
  setSentences(sentences);
});

render(() => <App />, document.getElementById("root") as HTMLElement);
