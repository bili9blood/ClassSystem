import { Info } from "@renderer/types/info";
import { createSignal } from "solid-js";

const nullInfo: Info = {
  students: [],
  arrangements: [],
  lessons: [],
  lessonsTm: [],
  notices: [],
  events: []
};

const [info, setInfo] = createSignal<Info>(nullInfo);

const [isBackup, setIsBackup] = createSignal(true);

export { info, setInfo, isBackup, setIsBackup };
