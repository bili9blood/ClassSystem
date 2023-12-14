import { createSignal, createEffect, For } from "solid-js";
import moment from "moment";
import { Event } from "@renderer/types/info";
import { info } from "../stores/info";

function CommonEventIcon() {
  return (
    <svg viewBox="0 0 100 100" xmlns="http://w3.org/2000/svg" class="w-3 h-3 my-1">
      <circle cx="50" cy="50" r="50" fill="#e5e6e6"></circle>
    </svg>
  );
}

function ImportantEventIcon() {
  return (
    <svg viewBox="0 0 100 100" xmlns="http://w3.org/2000/svg" class="w-5 h-5">
      <circle cx="50" cy="50" r="50" fill="#ee8545"></circle>
    </svg>
  );
}

export default function () {
  const [eventsToBeShowed, setEventsToBeShowed] = createSignal<Event[]>([]);

  createEffect(() => {
    if (info().events.length > 0) setEventsToBeShowed(info().events.slice(0, 4));
  });

  return (
    <div class="overflow-hidden">
      <p class="text-3xl text-white font-bold">事件</p>
      <ul class="timeline">
        <For each={eventsToBeShowed()}>
          {({ title, date, important }, idx) => (
            <li>
              {idx() > 0 && <hr />}
              <div class="timeline-start mx-2 text-white">
                <span class="text-2xl">{moment(date, "YYMMDD").format("MM-DD")}</span>
                &nbsp;
                <span class="text-xl font-normal">{moment(date, "YYMMDD").diff(void 0, "d")}</span>
                <span class="font-thin">天后</span>
              </div>
              <div class="timeline-middle">
                {important ? <ImportantEventIcon /> : <CommonEventIcon />}
              </div>
              <div class="timeline-end timeline-box">{title}</div>
              {idx() < eventsToBeShowed().length - 1 && <hr />}
            </li>
          )}
        </For>
      </ul>
    </div>
  );
}
