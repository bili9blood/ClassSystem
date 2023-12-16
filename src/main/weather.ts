import { useSettings } from "./settings";

export type DailyWeather = {
  tempMax: string;
  tempMin: string;
  iconDay: string;
};

export async function fetchWeather(): Promise<DailyWeather[]> {
  const { serverBaseUrl } = await useSettings();
  const res = await fetch(`${serverBaseUrl}/api/weather`);
  const resBody = await res.json();
  return resBody.data;
}
