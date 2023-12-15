import { Notice } from "@renderer/types/info";
import moment from "moment";
import { createEffect, createSignal, For } from "solid-js";
import { info } from "../stores/info";

function BaseParagraph(props: { class?: string; text: string }) {
  return <p class={`${props.class} text-xl text-white font-[华文中宋] mb-4`}>{props.text}</p>;
}

function Paragraph({ text }: { text: string }) {
  if (text.startsWith(">>")) {
    return <BaseParagraph class="text-right" text={text.slice(2)} />;
  }
  if (text.startsWith("^^")) {
    return <BaseParagraph class="text-center" text={text.slice(2)} />;
  }
  return <BaseParagraph text={text} />;
}

export default function () {
  const [notice, setNotice] = createSignal<Notice>({ title: "", text: "", date: "FOREVER" });
  let idx = 0,
    timer: NodeJS.Timer;
  const update = () => {
    setNotice(info().notices[idx]);
    idx = (idx + 1) % info().notices.length;
  };
  createEffect(() => {
    clearInterval(timer);
    if (info().notices.length > 0) {
      update();
      timer = setInterval(update, 15_000);
    }
  }, [info]);

  return (
    <aside class="w-[27%]">
      <div class="flex mb-2">
        <p class="text-3xl text-white font-bold">公告</p>
        <div class="flex-[3]"></div>
        <div class="flex flex-col items-end">
          <p class="text-2xl text-white">{notice().title}</p>
          <p class="text-lg text-gray-200">
            {notice().date === "FOREVER"
              ? "永久"
              : `至${moment(notice().date, "YYMMDD").format("MM-DD")}`}
          </p>
        </div>
      </div>
      <For each={notice().text.split("\n")}>{(par) => <Paragraph text={par} />}</For>
    </aside>
  );
}
