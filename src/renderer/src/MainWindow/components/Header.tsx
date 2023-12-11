import { onMount, createSignal, createEffect } from "solid-js";
import moment from "moment";
import { sentences } from "../stores/sentences";
import { Sentence } from "@renderer/types/sentences";

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
    <div class="flex-[2] min-w-[5em] countdown countdown-invert text-5xl font-bold">
      <span class="hours"></span>
      {":"}
      <span class="minutes"></span>
      {":"}
      <span class="seconds"></span>
    </div>
  );
}

function SentenceComponent() {
  const [sentence, setSentence] = createSignal<Sentence>({ id: 0, text: "", author: "" });

  let idx = 0,
    timer = 0;

  createEffect(() => {
    if (timer) clearInterval(timer);
    if (sentences().length > 0) {
      setSentence(sentences()[0]);
      setInterval(() => {
        const length = sentences().length;
        idx = (idx + 1) % length;
        setSentence(sentences()[idx]);
      }, 60_000);
    }
  });

  return (
    <div class="flex flex-col max-w-[60%]">
      <p class="indent-[2em] text-xl break-words">{sentence().text}</p>
      <p class="text-end">——{sentence().author}</p>
    </div>
  );
}

export default function () {
  return (
    <div class="flex items-center text-primary-text">
      <Clock />
      <SentenceComponent />
      <div class="flex-[3]"></div>
    </div>
  );
}
