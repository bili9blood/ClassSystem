const path = require("path");

module.exports = {
  title: "ClassSystem 使用文档",
  head: [
    ["link", { rel: "icon", href: "logo.ico" }],
    ["link", { rel: "icon", href: "logo.png" }],
  ],
  locales: {
    "/": {
      lang: "zh-CN",
    },
  },

  themeConfig: {
    logo: "/logo.png",
    smoothScroll: true,
    sidebar: [
      "/",
      {
        title: "ClassSystem",
        path: "/class-system",
        collapsable: false,
        sidebarDepth: 3,
        children: ["/class-system/main-panel", "/class-system/popup-menu"],
      },
      {
        title: "ClassSystem 管理后台",
        path: "/class-admin",
        collapsable: false,
        sidebarDepth: 3,
        children: [
          "/class-admin/students",
          "/class-admin/meal-stu",
          "/class-admin/stu-on-duty",
        ],
      },
      "/class-data",
    ],
  },

  plugins: [
    [
      "vuepress-plugin-zooming",
      {
        delay: 1000,
        options: {
          bgColor: "#383a3c80",
          zIndex: 10000,
        },
      },
    ],
  ],
  configureWebpack: () => {
    const NODE_ENV = process.env.NODE_ENV;
    //判断是否是生产环境
    if (NODE_ENV === "production") {
      return {
        output: {
          publicPath:
            "https://cdn.jsdelivr.net/gh/class-system-docs/class-system-docs.github.io/",
        },
        resolve: {
          //配置路径别名
          alias: {
            public: path.resolve(__dirname, "./public"),
          },
        },
      };
    } else {
      return {
        resolve: {
          //配置路径别名
          alias: {
            public: path.resolve(__dirname, "./public"),
          },
        },
      };
    }
  },
};
