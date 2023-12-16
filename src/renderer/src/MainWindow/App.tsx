import { onMount } from "solid-js";
import Header from "./components/Header";
import Events from "./components/Events";
import Lessons from "./components/Lessons";
import Arrangements from "./components/Arrangements";
import Notices from "./components/Notices";
import Weather from "./components/Weather";

function Middle() {
  return (
    <article class="w-[48%]">
      <Events />
      <div class="divider"></div>
      <Arrangements />
    </article>
  );
}

export default function () {
  onMount(() => {
    window.class_system.send("mounted");
  });

  return (
    <div class="h-full w-full">
      <Header />
      <div class="divider"></div>
      <div class="flex w-full">
        <Notices />
        <div class="divider divider-horizontal"></div>
        <Middle />
        <div class="divider divider-horizontal"></div>
        <Lessons />
      </div>
      <div class="divider"></div>
      <Weather />
    </div>
  );
}
