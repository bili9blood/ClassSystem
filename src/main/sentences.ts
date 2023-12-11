import { useSettings } from "./settings";

export type Sentence = {
  id: number;
  text: string;
  author: string;
};

export async function fetchSentences() {
  const { serverBaseUrl } = await useSettings();

  const res = await fetch(`${serverBaseUrl}/api/sentences`);
  const resBody = await res.json();

  if (res.status !== 200 || resBody.status !== 200) return [] as Sentence[];

  return resBody.data.sentences as Sentence[];
}
