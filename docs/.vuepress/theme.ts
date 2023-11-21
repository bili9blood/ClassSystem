import { hopeTheme } from "vuepress-theme-hope";

export default hopeTheme({
  sidebar,
  breadcrumb: false,

  plugins: {
    mdEnhance: {
      card: true,
    },
  },

  //导航栏：GitHub仓库和编辑链接
  repo: "https://github.com/bili9blood/ClassSystem",
  repoLabel: "GitHub",
  repoDisplay: true,

  logo: "img/logo.png",
  fullscreen: true,

  contributors: true,
  lastUpdated: true,
  sidebarIcon: true,

  prevLink: true,
  nextLink: true,

  pageInfo: ["Word", "ReadingTime", "Date"]
});
