import { render } from "solid-js/web";
import "./index.css";
import "../assets/fonts.css";
import App from "./App";

const rootEl = document.getElementById("root")!;

render(() => <App />, rootEl);
window.class_system.addListener("toggle-extra-window", (show: boolean) => {
  rootEl.style.opacity = show ? "1" : "0";
});
