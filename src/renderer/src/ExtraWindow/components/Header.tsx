import LogoPng from "../../assets/logo.png";
import CloseSvg from "../../assets/close.svg";

export default function () {
  return (
    <header class="flex items-center h-14 px-3">
      <img src={LogoPng} alt="ClassSystem" class="h-9" />
      <p class="ml-2 text-xl font-[youshe-title]">ClassSystem 班级系统</p>
      <div class="flex-[3]"></div>
      <button
        class="btn btn-square btn-sm"
        onClick={() => window.class_system.emit("toggle-extra-window", false)}
      >
        <img src={CloseSvg} alt="关闭" />
      </button>
    </header>
  );
}
