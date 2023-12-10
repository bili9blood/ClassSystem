import { promises as fs } from "fs";
import moment from "moment";
import { useSettings } from "./settings";

export type Student = { id: number; name: string };

export type DailyArrangement = { title: string; list: number[] };

export type DailyLessons = string[];

export type Notice = { title: string; text: string; date: "FOREVER" | string };

export type Event = { title: string; date: string; important: boolean };

export type Info = {
  students: Student[];
  arrangements: DailyArrangement[];
  lessons: DailyLessons;
  notices: Notice[];
  events: Event[];
};

export async function getBackupInfo() {
  // 查看 backup 文件夹
  try {
    await fs.stat("backup/");
  } catch {
    await fs.mkdir("backup/");
  }
  const files = await fs.readdir("backup/");
  let info: Info | null = null;
  for (const file of files) {
    let shouldDelete = false;

    if (file.endsWith(".json")) {
      const basename = file.replace(".json", "");
      const date = moment(basename, "YYMMDD", true);
      if (!date.isValid() || date.diff(moment(), "d") < 0) shouldDelete = true;
      if (date.diff(moment(), "d") === 0) {
        const data = await fs.readFile("backup/" + file, "utf8");
        info = JSON.parse(data);
        break;
      }
    } else shouldDelete = true;

    if (shouldDelete) fs.unlink("backup/" + file);
  }

  return info;
}

async function fetchBackupInfo() {
  const settings = await useSettings();
  const date = moment().add(1, "d").format("YYMMDD");

  const res = await fetch(
    `${settings.serverBaseUrl}/api/info/daily?classId=${settings.classId}&date=${date}`
  );
  const resBody = await res.json();

  if (res.status === 200 && resBody.status === 200) {
    await fs.writeFile(`backup/${date}.json`, JSON.stringify(resBody.data), "utf-8");
  }
}

export async function fetchInfo() {
  let info: Info | null = null;
  const settings = await useSettings();
  const res = await fetch(`${settings.serverBaseUrl}/api/info/daily?classId=${settings.classId}`);
  const resBody = await res.json();

  if (res.status === 200 && resBody.status === 200) {
    info = resBody.data as Info;
    await fs.writeFile(`backup/${moment().format("YYMMDD")}.json`, JSON.stringify(info), "utf-8");
  }
  await fetchBackupInfo();
  return info;
}
