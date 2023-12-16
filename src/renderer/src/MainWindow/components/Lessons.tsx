import { createEffect, createSignal, For } from "solid-js";
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
  const tmText = `${moment(tm, "HHmm").format("HH:mm")} - ${moment(tm, "HHmm")
    .add(40, "m")
    .format("HH:mm")}`;

  if (ltCurrent) {
    return (
      <div class="my-3 font-[华文中宋]">
        <span class="text-2xl text-gray-400">{lesson}</span>
        &nbsp;
        <span class="text-2xl text-gray-400">{tmText}</span>
      </div>
    );
  }

  return (
    <div class="my-3 font-[华文中宋]">
      <span class={isCurrent ? "text-4xl text-[#ee8545] font-bold" : "text-3xl"}>{lesson}</span>
      &nbsp;
      <span class={isCurrent ? "text-2xl text-[#ee8545]" : "text-2xl text-gray-200"}>{tmText}</span>
    </div>
  );
}

export default function () {
  const [current, setCurrent] = createSignal(-1);
  const [lessons, setLessons] = createSignal(info().lessons);

  createEffect(() => setLessons(info().lessons), [info]);

  setInterval(() => {
    const lessonsTm = info().lessonsTm;

    for (let i = 0; i < lessons().length; ++i) {
      const tm = moment(lessonsTm[i], "HHmm");
      const duringClass = moment().isBetween(
        tm.clone().subtract(10, "m"),
        tm.clone().add(40, "m"),
        void 0,
        "[]"
      );

      if (duringClass) {
        if (current() !== i) {
          setCurrent(i);
          const backup = lessons();
          setLessons([]);
          setLessons(backup);
        }
        break;
      }
    }
  }, 2000);

  return (
    <aside class="w-[25%] text-white">
      <p class="text-3xl mb-6 font-bold">课程</p>
      <For each={lessons()}>
        {(lesson, i) => (
          <Lesson
            isCurrent={i() === current()}
            ltCurrent={i() < current()}
            lesson={lesson}
            tm={info().lessonsTm[i()]}
          />
        )}
      </For>
    </aside>
  );
}
