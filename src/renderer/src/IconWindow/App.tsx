import { createSignal, onCleanup } from "solid-js";

import LogoPng from "../assets/logo.png";

let prevPosition: [number, number] = [0, 0];

let extraWindow: Window | null = null;

const [showIcon, setShowIcon] = createSignal(true);

onCleanup(() => {
  if (extraWindow) extraWindow.close();
});

function beginMoving() {
  window.class_system.send("begin-move-menu");

  prevPosition = [window.screenX, window.screenY];
}

function hideUntilExtraWinClosed() {
  setShowIcon(false);
  setInterval((timer) => {
    if (extraWindow?.document.getElementById("root")!.style.opacity === "0") {
      setShowIcon(true);
      clearInterval(timer);
    }
  }, 2000);
}

function endMoving() {
  window.class_system.send("end-move-menu");

  const [x, y] = [window.screenX, window.screenY];
  const [dx, dy] = [Math.abs(x - prevPosition[0]), Math.abs(y - prevPosition[1])];
  if (dx <= 10 && dy <= 10) {
    if (!extraWindow) extraWindow = window.open("./extraWindow.html")!;
    const extraShowed = extraWindow.document.getElementById("root")!.style.opacity === "1";
    extraWindow.class_system.emit("toggle-extra-window", !extraShowed);
    if (!extraShowed) {
      // 打开了额外窗口，暂时隐藏悬浮图标
      hideUntilExtraWinClosed();
    }
  }
}

export default function () {
  return (
    <img
      src={LogoPng}
      class={`h-[50px] w-[50px] transition-opacity ${showIcon() ? "opacity-100" : "opacity-0"}`}
      onMouseDown={beginMoving}
      onMouseUp={endMoving}
    />
  );
}
