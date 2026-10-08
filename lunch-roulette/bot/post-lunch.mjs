// 평일 점심 전에 구글 챗 방에 오늘의 메뉴를 추천하는 봇.
// GitHub Actions(.github/workflows/lunch-roulette-bot.yml)에서 실행된다.
//
// 로컬 테스트: GOOGLE_CHAT_WEBHOOK_URL=... node bot/post-lunch.mjs
//             (URL 없이 실행하면 메시지를 출력만 함)

import { readFile } from "node:fs/promises";

const dataUrl = new URL("../data/restaurants.json", import.meta.url);
const { restaurants } = JSON.parse(await readFile(dataUrl, "utf8"));

const pick = restaurants[Math.floor(Math.random() * restaurants.length)];
const price = pick.price ? ` ${pick.price.toLocaleString("ko-KR")}원` : "";

const lines = [
  "🍚 *오늘의 점심 추천*",
  "",
  `👉 *${pick.name}* (${pick.category})`,
  `🚶 도보 ${pick.walkMin}분 · ${pick.menu}${price}`,
];
if (process.env.ROULETTE_URL) {
  lines.push("", `🎲 다른 데 갈래요? 룰렛 다시 돌리기 → ${process.env.ROULETTE_URL}`);
}
const text = lines.join("\n");

const webhook = process.env.GOOGLE_CHAT_WEBHOOK_URL;
if (!webhook) {
  console.log(text);
  process.exit(0);
}

const res = await fetch(webhook, {
  method: "POST",
  headers: { "Content-Type": "application/json; charset=UTF-8" },
  body: JSON.stringify({ text }),
});
if (!res.ok) {
  console.error(`구글 챗 전송 실패: ${res.status} ${await res.text()}`);
  process.exit(1);
}
console.log(`전송 완료: ${pick.name}`);
