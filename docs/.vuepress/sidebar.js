export default [
  "/",
  {
    text: "前台展示端",
    link: "/class-system",
    collapsible: false,
    children: ["/class-system/main-panel", "/class-system/popup-menu"],
  },
  {
    text: "后台管理端",
    link: "/class-admin",
    collapsible: false,
    children: [
      "/class-admin/sync-drop",
      "/class-admin/import",
      "/class-admin/students",
      "/class-admin/lessons",
      "/class-admin/notices-and-events",
      "/class-admin/password",
    ],
  },
  "/class-data",
];
