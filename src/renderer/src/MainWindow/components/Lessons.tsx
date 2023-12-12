import { createSignal, For } from "solid-js";
import moment from "moment";
import { info } from "../stores/info";

export default function () {
  const [current, setCurrent] = createSignal(-1);

  setInterval(() => {
    const lessons = info().lessons;
    const lessonsTm = info().lessonsTm;
    for (let i = current(); i < lessons.length; ++i) {
      const tm = moment(lessonsTm[i], "hhmm");
      if (moment().isBetween(tm.subtract(10, "m"), tm.add(40, "m"))) {
        setCurrent(i);
        break;
      }
    }
  }, 5000);

  return (
    <div class="w-[30%] text-primary-text">
      <p class="text-3xl mb-6">课程</p>
      <For each={info().lessons}>
        {(lesson, idx) => (
          <div class="my-3 font-[华文中宋]">
            <span class="text-3xl " style={{ color: idx() === current() ? "#ee8545" : "" }}>
              {lesson}
            </span>
            &nbsp;
            <span class="text-xl text-gray-200">
              {moment(info().lessonsTm[idx()], "hhmm").format("HH:mm")}-
              {moment(info().lessonsTm[idx()], "hhmm").add(40, "m").format("HH:mm")}
            </span>
          </div>
        )}
      </For>
    </div>
  );
}
