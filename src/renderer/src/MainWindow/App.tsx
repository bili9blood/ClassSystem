import { onMount } from "solid-js";
import { Info } from "../types/info";
import { setInfo, setIsBackup } from "./stores/info";
import Header from "./components/Header";

export default function () {
  onMount(() => {
    window.ipcRenderer.send("mounted");
  });
  window.ipcRenderer.on("backup-info", (_, info: Info | null) => {
    setInfo(info);
    setIsBackup(true);
  });
  window.ipcRenderer.on("fetched-info", (_, info: Info | null) => {
    setInfo(info);
    setIsBackup(false);
  });
  return (
    <div class="h-full w-full">
      <Header />
    </div>
  );
}
