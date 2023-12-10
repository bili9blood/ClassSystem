import { onMount, type Component } from "solid-js";

const App: Component = () => {
  onMount(() => {
    window.ipcRenderer.send("mounted");
    (document.querySelector(".eeee") as HTMLElement).onmouseenter = () => {
      console.log("enter");
    };
  });
  return (
    <div class="h-full w-full text-primary-text eeee">
      Lorem ipsum dolor sit amet consectetur, adipisicing elit. Voluptatem possimus fugit sit
      tempora doloribus alias magni itaque esse, ducimus odio pariatur nostrum dolorum officia!
      Delectus doloribus voluptatem repudiandae minima. Excepturi? Nesciunt, dolores repellendus?
      Deleniti cupiditate dolore quod corrupti ab unde similique iusto at nihil quas asperiores
    </div>
  );
};

export default App;
