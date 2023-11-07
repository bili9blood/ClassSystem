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
  <div class="wrapper">
    <div class="time">
      <span>{{ hoursRef }}</span>
      <pre>{{ colonsRef }}</pre>
      <span>{{ minsRef }}</span>
      <pre>{{ colonsRef }}</pre>
      <span>{{ secsRef }}</span>
    </div>

    <div class="day"></div>
    <div class="date"></div>
  </div>
</template>
<style scoped lang="less">
.wrapper {
  display: grid;

  grid-gap: 20px;
  grid-template-columns: repeat(2, auto);
  grid-template-rows: repeat(2, auto);

  align-items: center;
  justify-items: center;
}

.time {
  grid-column: 1;
  grid-row: 1 / span 2;
  font-size: 2.5rem;
  text-shadow: #00000066 1px 1px 4px;

  pre {
    display: inline;
    font-family: 华文中宋;
    width: 2em;
    position: relative;
    bottom: 0.3rem;
  }
}

.day {
  grid-row: 1;
}

.date {
  grid-row: 2;
  grid-column: 2;
}
</style>
