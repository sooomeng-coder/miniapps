#pragma once
#include <Arduino.h>

struct Restaurant {
  String name;
  String category;
  int walkMin;
  String menu;
  int price;
};

// 와이파이 연결이나 데이터 불러오기에 실패했을 때 쓰는 기본 목록.
// 평소에는 data/restaurants.json (GitHub Pages)에서 불러온다.
const Restaurant FALLBACK_RESTAURANTS[] = {
  {"김치찌개집", "한식", 3, "김치찌개", 9000},
  {"돈까스 가게", "일식", 5, "로스카츠", 11000},
  {"짬뽕 맛집", "중식", 7, "짬뽕", 10000},
  {"편의점", "간편식", 1, "도시락", 5000},
};
