import { defineUserConfig } from "vuepress";
import theme from "./theme";

export default defineUserConfig({
  title: "ClassSystem 使用文档",
  lang: "zh-CN",

  head: [
    ["link", { rel: "icon", href: "/docs/img/logo.ico" }],
    ["link", { rel: "icon", href: "/docs/img/logo.png" }],
  ],

  base: "/docs/",

  theme,

  // home: "https://class-system-docs.gitee.io/",

  plugins: [],
});
