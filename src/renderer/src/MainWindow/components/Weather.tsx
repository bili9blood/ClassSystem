import { DailyWeather } from "@renderer/types/weather";
import moment from "moment";
import { createSignal, For } from "solid-js";

function DailyWeatherComponent({
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
  const date = moment().add(idx, "d");
  const weekdayMap = ["星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"];

  return (
    <div class="m-auto flex flex-col items-center">
      <p class="text-xl font-bold">
        <i class={`qi-${icon} mr-1`}></i>
        <span>{`${min}℃ ~ ${max}℃`}</span>
      </p>
      <p class={idx === 0 ? "text-highlight font-bold" : "text-gray-100"}>
        <span>{date.format("MM-DD")}</span>
        &nbsp;
        <span>{weekdayMap[date.weekday()]}</span>
      </p>
    </div>
  );
}

export default function () {
  const [weather, setWeather] = createSignal<DailyWeather[]>([]);
  window.class_system.on("weather", (_, w: DailyWeather[]) => setWeather(w));

  return (
    <footer class="w-full h-12 flex items-center justify-items-center text-white">
      <For each={weather()}>
        {(w, idx) => (
          <DailyWeatherComponent icon={w.iconDay} min={w.tempMin} max={w.tempMax} idx={idx()} />
        )}
      </For>
    </footer>
  );
}
