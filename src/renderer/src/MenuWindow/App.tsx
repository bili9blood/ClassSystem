import { createSignal, onCleanup, onMount } from "solid-js";
import LogoPng from "../assets/logo.png";

let iconEl: HTMLImageElement;

let prevPosition = { x: 0, y: 0 };

const [expanded, setExpanded] = createSignal(false);

function beginMoving() {
  window.class_system.send("begin-move-menu");

  [prevPosition.x, prevPosition.y] = [window.screenX, window.screenY];
}

function endMoving() {
  window.class_system.send("end-move-menu");

  const [x, y] = [window.screenX, window.screenY];
  const [dx, dy] = [x - prevPosition.x, y - prevPosition.y];
  if (dx <= 20 && dy <= 20) {
    setExpanded(!expanded());
  }
}

export default function () {
  return (
    <div class="h-full w-full flex items-start">
      <div
        class={`h-[300px] w-[250px] mr-[10px] bg-red-500 ${expanded() || "opacity-0"}`}
        onMouseDown={(e) => console.log(e)}
      ></div>
      <img
        src={LogoPng}
        ref={iconEl}
        class="h-[50px] w-[50px]"
        onMouseDown={beginMoving}
        onMouseUp={endMoving}
      />
    </div>
  );
}
