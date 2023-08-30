export default [
  "/",
  {
    text: "ClassSystem",
    link: "/class-system",
    collapsble: false,
    children: ["/class-system/main-panel", "/class-system/popup-menu"],
  },
  {
    text: "ClassSystem 管理后台",
    link: "/class-admin",
    collapsble: false,
    children: [
      "/class-admin/students",
      "/class-admin/meal-stu",
      "/class-admin/stu-on-duty",
    ],
  },
  "/class-data",
];
