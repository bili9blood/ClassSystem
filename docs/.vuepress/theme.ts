import { hopeTheme } from "vuepress-theme-hope";

export default hopeTheme({
  sidebar: ["/", "class-system", "/class-admin", "/class-data"],
  breadcrumb: false,

  plugins: {
    mdEnhance: {
      // @ts-ignore
      card: true,
    },
  },

  //导航栏：GitHub仓库和编辑链接
  repo: "https://github.com/bili9blood/ClassSystem",
  repoLabel: "GitHub",
  repoDisplay: true,
  // 配置仓库中的文档路径，解决 `编辑此页` 按钮跳转页面错误的问题。
  docsDir: "/docs/",

  logo: "img/logo.png",

  fullscreen: true,

  pageInfo: ["Word", "ReadingTime", "Date"],
});
