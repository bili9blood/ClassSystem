import { onMount, createSignal, createEffect } from "solid-js";
import moment from "moment";
import { sentences } from "../stores/sentences";
import { Sentence } from "@renderer/types/sentences";
import LogoPng from "../assets/logo.png";

function DateComponent() {
  const weekdayMap = ["星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"];

  return (
    <div class="flex w-[80%] mx-auto text-2xl font-semibold">
      <p class="text-start">{moment().format("MM-DD")}</p>
      <div class="min-w-[10%]"></div>
      <p class="text-end">{weekdayMap[moment().weekday()]}</p>
    </div>
  );
}

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
    <div class="flex-[2] mx-auto countdown countdown-invert text-5xl font-bold">
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
    if (sentences().length > 0) {
      if (timer) clearInterval(timer);
      setSentence(sentences()[0]);
      setInterval(() => {
        const length = sentences().length;
        idx = (idx + 1) % length;
        setSentence(sentences()[idx]);
      }, 60_000);
    }
  });

  return (
    <div class="flex flex-col max-w-[60%] mr-2 overflow-hidden">
      <p class="indent-[2em] text-xl break-words">{sentence().text}</p>
      <p class="text-end">——{sentence().author}</p>
    </div>
  );
}

function Title() {
  return (
    <div class="flex flex-col">
      <div class="flex items-center">
        <img src={LogoPng} alt="ClassSystem" class="h-8 w-8 mx-2" />
        <p class="font-[youshe-title] text-2xl whitespace-nowrap">ClassSystem 班级系统</p>
      </div>
    </div>
  );
}

export default function () {
  return (
    <div class="flex items-center text-primary-text">
      <div class="flex flex-col min-w-[4.5rem]">
        <DateComponent />
        <Clock />
      </div>
      <SentenceComponent />
      <div class="flex-[3]"></div>
      <Title />
    </div>
  );
}
