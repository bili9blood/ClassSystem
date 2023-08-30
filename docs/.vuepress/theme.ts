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
});
