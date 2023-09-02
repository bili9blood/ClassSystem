import { defineUserConfig } from "vuepress";
import theme from "./theme";

export default defineUserConfig({
  title: "ClassSystem 使用文档",
  lang: "zh-CN",

  head: [
    ["link", { rel: "icon", href: "/img/logo.ico" }],
    ["link", { rel: "icon", href: "/img/logo.png" }],
  ],

  theme,

  plugins: [],
});