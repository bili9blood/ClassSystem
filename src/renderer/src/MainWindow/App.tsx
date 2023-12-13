import { onMount } from "solid-js";
import Header from "./components/Header";
import Events from "./components/Events";
import Lessons from "./components/Lessons";
import Arrangements from "./components/Arrangements";

function LeftSide() {
  return (
    <div class="w-[70%]">
      <Events />
      <div class="divider"></div>
      <Arrangements />
    </div>
  );
}

export default function () {
  onMount(() => {
    window.ipcRenderer.send("mounted");
  });

  return (
    <div class="h-full w-full">
      <Header />
      <div class="divider"></div>
      <div class="flex w-full">
        <LeftSide />
        <div class="divider divider-horizontal"></div>
        <Lessons />
      </div>
    </div>
  );
}