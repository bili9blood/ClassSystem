import { For } from "solid-js";
import { info } from "../stores/info";

function stuIdsToStr(students: number[]) {
  let stuMap: Record<number, string> = {};
  for (const { id, name } of info().students) stuMap[id] = name;

  return students.map((id) => id.toString() + stuMap[id]).join(" ");
}

export default function () {
  return (
    <div class="grid grid-cols-2 text-white">
      <For each={info().arrangements}>
        {({ name, list }) => (
          <div class="my-3">
            <span class="text-3xl font-bold">{name}：</span>
            <span class="text-2xl whitespace-pre font-[华文中宋]">{stuIdsToStr(list)}</span>
          </div>
        )}
      </For>
    </div>
  );
}
