import { createWriteStream } from "fs";
import { get } from "http";
import { useSettings } from "./settings";
import { spawn } from "child_process";

function downloadLatestAsar(url: string) {
  const stream = createWriteStream("./resources/latest");
  get(url, (res) => {
    res.pipe(stream);
    res.on("end", () => {
      stream.close();
      console.log(process.pid);
      spawn("..\\update.bat", [process.pid.toString()], {
        detached: true,
        cwd: "./resources/"
      });
    });
  });
}

export async function checkUpdate(version: string) {
  const { serverBaseUrl } = await useSettings();
  const res = await fetch(`${serverBaseUrl}/latest/version`);
  const latestVersion = await res.text();
  if (latestVersion === version) return;

  downloadLatestAsar(`${serverBaseUrl}/latest/asar`);
}
