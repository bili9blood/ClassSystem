import { promises as fs } from "fs";

export type Settings = {
  position: [number, number];
  size: [number, number];
  classId: string;
  serverBaseUrl: string;
};

let settings: Settings | null = null;

export async function loadSettings() {
  const data = await fs.readFile("settings.json", "utf-8");
  settings = JSON.parse(data);
}
export async function useSettings() {
  if (!settings) await loadSettings();
  return settings!;
}

export async function saveSettings(_settings: Settings) {
  settings = _settings;
  const data = JSON.stringify(settings);
  await fs.writeFile("settings.json", data);
}
