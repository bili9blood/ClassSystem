import { For } from "solid-js";
import moment from "moment";
import { DailyArrangement } from "@renderer/types/info";
import { info } from "../stores/info";

function stuIdsToStr(students: number[]) {
  let stuMap: Record<number, string> = {};
  for (const { id, name } of info().students) stuMap[id] = name;

  return students.map((id) => id.toString() + stuMap[id]).join(" ");
}

export default function () {
  setTimeout(() => console.log(info().arrangements), 4000);
  return (
    <div class="text-primary-text grid grid-cols-2">
      <For each={info().arrangements}>
        {({ name, list }) => (
          <div>
            <span class="text-3xl font-bold">{name}：</span>
            <span class="text-2xl whitespace-pre">{stuIdsToStr(list)}</span>
          </div>
        )}
      </For>
    </div>
  );
}
