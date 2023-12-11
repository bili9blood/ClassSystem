import { Sentence } from "@renderer/types/sentences";
import { createSignal } from "solid-js";

const [sentences, setSentences] = createSignal<Sentence[]>([]);

export { sentences, setSentences };
