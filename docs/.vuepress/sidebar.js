export default [
  "/",
  {
    text: "展示端",
    link: "/class-system",
    collapsible: false,
    children: ["/class-system/main-panel", "/class-system/popup-menu"],
  },
  {
    text: "管理后台",
    link: "/class-admin",
    collapsible: false,
    children: [
      "/class-admin/import",
      "/class-admin/students",
      "/class-admin/lessons",
      "/class-admin/notices-and-events",
      "/class-admin/password",
    ],
  },
  "/class-data",
];
