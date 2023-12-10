import { defineUserConfig } from "vuepress";
import theme from "./theme";
import { cut } from "nodejs-jieba";
import { searchProPlugin } from "vuepress-plugin-search-pro";

export default defineUserConfig({
  title: "ClassSystem 使用文档",
  lang: "zh-CN",

  head: [
    ["link", { rel: "icon", href: "/docs/img/logo.ico" }],
    ["link", { rel: "icon", href: "/docs/img/logo.png" }]
  ],

  base: "/docs/",

  theme,

  // home: "https://class-system-docs.gitee.io/",

  plugins: [
    searchProPlugin({
      indexContent: true,
      indexOptions: {
        tokenize: (text, fieldName) => (fieldName === "id" ? [text] : cut(text, true))
      }
    })
  ]
});
