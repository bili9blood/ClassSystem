import { Notice } from "@renderer/types/info";
import { DailyWeather } from "@renderer/types/weather";
import moment from "moment";
import { createSignal, For } from "solid-js";

function Temperature({
  icon,
  max,
  min,
  idx
}: {
  icon: string;
  max: string;
  min: string;
  idx: number;
}) {
  console.log(icon);

  return (
    <div class="m-auto flex flex-col items-center">
      <p class="text-xl font-bold">
        <i class={`qi-${icon} mr-1`}></i>
        <span>{`${min}℃ ~ ${max}℃`}</span>
      </p>
      <p class="text-gray-100">{moment().add(idx, "d").format("MM-DD")}</p>
    </div>
  );
}

export default function () {
  const [weather, setWeather] = createSignal<DailyWeather[]>([]);
  window.class_system.on("weather", (_, w: DailyWeather[]) => setWeather(w));

  return (
    <footer class="w-full h-12 flex items-center justify-items-center text-white">
      <For each={weather()}>
        {(w, idx) => <Temperature icon={w.iconDay} min={w.tempMin} max={w.tempMax} idx={idx()} />}
      </For>
    </footer>
  );
}
