<script setup lang="ts">
import { ref } from "vue";

function getTime(time: number): [string, string, string] {
  var date = new Date(time);
  var hours =
    date.getHours() < 10 ? "0" + date.getHours() : "" + date.getHours();
  var minutes =
    date.getMinutes() < 10 ? "0" + date.getMinutes() : "" + date.getMinutes();
  var seconds =
    date.getSeconds() < 10 ? "0" + date.getSeconds() : "" + date.getSeconds();
  return [hours, minutes, seconds];
}

const hoursRef = ref("");
const minsRef = ref("");
const secsRef = ref("");

const colonsRef = ref("");

setInterval(() => {
  const prevSec = secsRef.value;

  [hoursRef.value, minsRef.value, secsRef.value] = getTime(Date.now());

  colonsRef.value =
    prevSec !== secsRef.value ? "\u2008:\u2008" : "\u2008\u2008\u2008";
}, 500);
</script>
<template>
  <div class="time">
    <span class="text">{{ hoursRef }}</span>
    <pre class="colons">{{ colonsRef }}</pre>
    <span class="text">{{ minsRef }}</span>
    <pre class="colons">{{ colonsRef }}</pre>
    <span class="text">{{ secsRef }}</span>
  </div>
</template>
<style scoped lang="less">
.time {
  font-size: 2rem;
}
.colons {
  display: inline;
  font-family: 华文中宋;
  width: 2em;
  position: relative;
  bottom: 0.3rem;
}
</style>
