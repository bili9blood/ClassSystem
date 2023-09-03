import { hopeTheme } from "vuepress-theme-hope";
import sidebar from "./sidebar";

export default hopeTheme({
  sidebar,
  breadcrumb: false,

  plugins: {
    mdEnhance: {
      card: true,
    },
  },

  logo: "img/logo.png",

  contributors: false,

  prevLink: false,
  nextLink: false,

  pageInfo: ["Date", "Word", "ReadingTime"],
});
