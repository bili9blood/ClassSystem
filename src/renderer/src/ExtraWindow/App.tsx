import { For } from "solid-js";
import Header from "./components/Header";
import RollCall from "./tabs/RollCall";
import Tables from "./tabs/Tables";

const tabs = [
  { title: "随机点名", component: <RollCall /> },
  { title: "表格", component: <Tables /> }
];

export default function () {
  return (
    <div>
      <Header />
      <div class="divider my-[-.75rem]"></div>

      <div role="tablist" class="tabs tabs-bordered mt-4 px-3 w-full">
        <For each={tabs}>
          {({ title, component }, i) => (
            <>
              <input
                type="radio"
                name="tab"
                role="tab"
                class="tab"
                aria-label={title}
                checked={i() === 0}
              />
              {component}
            </>
          )}
        </For>
      </div>
    </div>
  );
}
