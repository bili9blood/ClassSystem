import { Notice } from "@renderer/types/info";
import moment from "moment";
import { createEffect, createSignal } from "solid-js";
import { info } from "../stores/info";

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
    <footer>
      <div class="flex mb-2">
        <p class="text-3xl text-white font-bold">公告</p>
        <div class="flex-[3]"></div>
        <div class="flex flex-col items-end absolute right-4">
          <p class="text-2xl text-white">{notice().title}</p>
          <p class="text-lg text-gray-200">
            {notice().date === "FOREVER"
              ? "永久"
              : `至${moment(notice().date, "YYMMDD").format("MM-DD")}`}
          </p>
        </div>
      </div>
      <pre class="text-[1.35rem] leading-6 text-white font-[华文中宋]">{notice().text}</pre>
    </footer>
  );
}
