import { onMount } from "solid-js";
import { Info } from "../types/info";

export default function () {
  onMount(() => {
    window.ipcRenderer.send("mounted");
  });
  window.ipcRenderer.on("backup-info", (_, info: Info | null) => {
    console.log("backup", info);
  });
  window.ipcRenderer.on("fetched-info", (_, info: Info | null) => {
    console.log("fetched", info);
  });
  return (
    <div class="h-full w-full text-primary-text eeee">
      Lorem ipsum dolor sit amet consectetur, adipisicing elit. Voluptatem possimus fugit sit
      tempora doloribus alias magni itaque esse, ducimus odio pariatur nostrum dolorum officia!
      Delectus doloribus voluptatem repudiandae minima. Excepturi? Nesciunt, dolores repellendus?
      Deleniti cupiditate dolore quod corrupti ab unde similique iusto at nihil quas asperiores
    </div>
  );
}
