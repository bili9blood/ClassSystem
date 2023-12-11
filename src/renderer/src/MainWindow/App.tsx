import { onMount } from "solid-js";
import Header from "./components/Header";

export default function () {
  onMount(() => {
    window.ipcRenderer.send("mounted");
  });

  return (
    <div class="h-full w-full">
      <Header />
    </div>
  );
}
