import { resolve } from "path";
import { defineConfig, externalizeDepsPlugin } from "electron-vite";
import solid from "vite-plugin-solid";

export default defineConfig({
  main: {
    plugins: [externalizeDepsPlugin()]
  },
  preload: {
    plugins: [externalizeDepsPlugin()]
  },
  renderer: {
    resolve: {
      alias: {
        "@renderer": resolve("src/renderer/src")
      }
    },
    plugins: [solid()],
    build: {
      assetsInlineLimit: 0,
      rollupOptions: {
        input: {
          MainWindow: resolve(__dirname, "src/renderer/mainWindow.html"),
          MenuWindow: resolve(__dirname, "src/renderer/menuWindow.html")
        }
      }
    }
  }
});
