import { createSignal, For } from "solid-js";
import moment from "moment";
import { info } from "../stores/info";

function Lesson({
  isCurrent,
  ltCurrent,
  lesson,
  tm
}: {
  isCurrent: boolean;
  ltCurrent: boolean;
  lesson: string;
  tm: string;
}) {
  if (ltCurrent) {
    return (
      <div class="my-3 font-[华文中宋]">
        <span class="text-2xl text-gray-400" style={{ color: isCurrent ? "#ee8545" : "" }}>
          {lesson}
        </span>
        &nbsp;
        <span class="text-xl text-gray-400">
          {moment(tm, "HHmm").format("HH:mm")}-{moment(tm, "HHmm").add(40, "m").format("HH:mm")}
        </span>
      </div>
    );
  }

  return (
    <div class="my-3 font-[华文中宋]">
      <span class={isCurrent ? "text-4xl text-[#ee8545] font-bold" : "text-3xl"}>{lesson}</span>
      &nbsp;
      <span class={isCurrent ? "text-3xl text-[#ee8545]" : "text-2xl text-gray-200"}>
        {moment(tm, "HHmm").format("HH:mm")}-{moment(tm, "HHmm").add(40, "m").format("HH:mm")}
      </span>
    </div>
  );
}

export default function () {
  const [current, setCurrent] = createSignal(-1);

  // @ts-ignore
  window.knn = "0800";

  setInterval(() => {
    const lessons = info().lessons;
    const lessonsTm = info().lessonsTm;
    // @ts-ignore
    console.log(window.knn);

    for (let i = current(); i < lessons.length; ++i) {
      const tm = moment(lessonsTm[i], "HHmm");
      // @ts-ignore
      const duringClass = moment(window.knn, "HHmm").isBetween(
        tm.clone().subtract(10, "m"),
        tm.clone().add(40, "m"),
        undefined,
        "[]"
      );

      if (duringClass) {
        setCurrent(i);
        break;
      }
    }
  }, 1000);

  return (
    <div class="w-[30%] text-primary-text">
      <p class="text-3xl mb-6">课程</p>
      <For each={info().lessons}>
        {(lesson, i) => (
          <Lesson
            isCurrent={i() === current()}
            ltCurrent={i() < current()}
            lesson={lesson}
            tm={info().lessonsTm[i()]}
          />
        )}
      </For>
    </div>
  );
}
