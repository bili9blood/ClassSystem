import { defineUserConfig } from "vuepress";
import theme from "./theme";

export default defineUserConfig({
  title: "ClassSystem 使用文档",
  head: [
    ["link", { rel: "icon", href: "/img/logo.ico" }],
    ["link", { rel: "icon", href: "/img/logo.png" }],
  ],
  locales: {
    "/": {
      lang: "zh-CN",
    },
  },

  theme,

  plugins: [],
});
