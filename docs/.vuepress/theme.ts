import { hopeTheme } from "vuepress-theme-hope";

export default hopeTheme({
  sidebar: ["/", "class-system", "/class-admin", "/class-data"],
  breadcrumb: false,

  plugins: {
    mdEnhance: {
      card: true,
    },
    git: true,
    // 页面底下的 “编辑此页” 无法与仓库 md 文件对号
  },

  //导航栏：GitHub仓库和编辑链接
  repo: "https://github.com/bili9blood/ClassSystem",
  repoLabel: "GitHub",
  repoDisplay: true,

  logo: "img/logo.png",

  fullscreen: true,

  pageInfo: ["Word", "ReadingTime"]
});
