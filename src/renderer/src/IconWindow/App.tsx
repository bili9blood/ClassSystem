import LogoPng from "../assets/logo.png";

let prevPosition: [number, number] = [0, 0];

let extraWindow: Window | null = null;

function beginMoving() {
  window.class_system.send("begin-move-menu");

  prevPosition = [window.screenX, window.screenY];
}

function endMoving() {
  window.class_system.send("end-move-menu");

  const [x, y] = [window.screenX, window.screenY];
  const [dx, dy] = [Math.abs(x - prevPosition[0]), Math.abs(y - prevPosition[1])];
  if (dx <= 10 && dy <= 10) {
    if ((extraWindow && extraWindow.closed) || !extraWindow)
      extraWindow = window.open("./extraWindow.html");
  }
}

export default function () {
  return (
    <img src={LogoPng} class="h-[50px] w-[50px]" onMouseDown={beginMoving} onMouseUp={endMoving} />
  );
}
