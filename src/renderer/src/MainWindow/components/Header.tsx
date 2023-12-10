import { createEffect, createSignal, onMount } from "solid-js";
import moment from "moment";
import { info } from "../stores/info";

function Clock() {
  onMount(() => {
    const hoursEl = document.querySelector(".hours") as HTMLElement;
    const minutesEl = document.querySelector(".minutes") as HTMLElement;
    const secondsEl = document.querySelector(".seconds") as HTMLElement;

    setInterval(() => {
      const now = moment().add(1, "s"); // 抵消动画延迟及时间
      hoursEl.style.setProperty("--value", `${99 - now.get("h")}`);
      minutesEl.style.setProperty("--value", `${99 - now.get("m")}`);
      secondsEl.style.setProperty("--value", `${99 - now.get("s")}`);
    }, 1000);
  });

  return (
    <div class="flex-1 countdown countdown-invert text-5xl">
      <span class="hours"></span>
      {":"}
      <span class="minutes"></span>
      {":"}
      <span class="seconds"></span>
    </div>
  );
}
export default function () {
  const [sentence, setSentence] = createSignal("");
  createEffect(() => {
    const i = info();
    if (i) {
      console.log(i);

      setSentence(i.notices[0].text);
      console.log(sentence());
    }
  });

  return (
    <div class="flex text-primary-text">
      <Clock />
      {sentence()}
    </div>
  );
}
