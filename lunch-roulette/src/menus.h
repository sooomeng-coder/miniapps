#pragma once
#include <stdint.h>

// 회사 근처 식당 목록 — 여기만 고치면 됩니다.
// { "식당 이름", "종류", 도보 몇 분 }
struct Restaurant {
  const char* name;
  const char* category;
  uint8_t walkMin;
};

const Restaurant RESTAURANTS[] = {
  {"김치찌개집", "한식", 3},
  {"돈까스 가게", "일식", 5},
  {"짬뽕 맛집", "중식", 7},
  {"샐러드 바", "샐러드", 4},
  {"국밥집", "한식", 6},
  {"쌀국수", "아시안", 8},
  {"햄버거", "양식", 5},
  {"편의점", "간편식", 1},
};

const int RESTAURANT_COUNT = sizeof(RESTAURANTS) / sizeof(RESTAURANTS[0]);
