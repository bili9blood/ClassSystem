import { Info } from "@renderer/types/info";
import { createSignal } from "solid-js";

const [info, setInfo] = createSignal<Info | null>(null);

const [isBackup, setIsBackup] = createSignal(true);

export { info, setInfo, isBackup, setIsBackup };
