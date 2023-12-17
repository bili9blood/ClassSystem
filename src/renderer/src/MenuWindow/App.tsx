import { app, getCurrentWindow } from "@electron/remote";
import { onMount } from "solid-js";
import LogoPng from "../assets/logo.png";

let iconEl: HTMLImageElement;

export default function () {
  onMount(() => {
    console.log(getCurrentWindow());
  });

  return (
    <div class="h-full w-full flex items-start">
      <div class="h-[300px] w-[250px] mr-[10px] bg-red-500"></div>
      <img src={LogoPng} ref={iconEl} class="h-[50px] w-[50px]" />
    </div>
  );
}
