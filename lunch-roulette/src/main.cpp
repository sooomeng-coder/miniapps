#include <Arduino.h>
#include <TFT_eSPI.h>
#include <U8g2_for_TFT_eSPI.h>

#include "menus.h"

// 버튼: 보드 내장 버튼(GPIO0) 또는 외부 키캡 스위치(GPIO21 ↔ GND)
constexpr int PIN_BTN_BUILTIN = 0;
constexpr int PIN_BTN_KEYCAP = 21;
constexpr unsigned long DEBOUNCE_MS = 40;

// 룰렛 속도
constexpr int SPIN_INTERVAL_MS = 60;    // 돌아가는 동안 메뉴 바뀌는 간격
constexpr float SLOWDOWN_FACTOR = 1.18; // 멈출 때 점점 느려지는 비율
constexpr int STOP_INTERVAL_MS = 450;   // 이 간격보다 느려지면 멈춤

constexpr uint16_t COLOR_BG = TFT_BLACK;
constexpr uint16_t COLOR_TEXT = TFT_WHITE;
constexpr uint16_t COLOR_DIM = 0x8410;  // 회색
constexpr uint16_t COLOR_ACCENT = TFT_ORANGE;

TFT_eSPI tft;
TFT_eSprite canvas(&tft);  // 화면 깜빡임 방지용 버퍼
U8g2_for_TFT_eSPI u8f;     // 한글 출력

enum class State { Idle, Spinning, Stopping, Result };

State state = State::Idle;
int current = 0;
int lastPicked = -1;
float stepMs = SPIN_INTERVAL_MS;
unsigned long nextStepAt = 0;

bool buttonPressed() {
  static bool wasDown = false;
  static unsigned long changedAt = 0;

  bool down = digitalRead(PIN_BTN_BUILTIN) == LOW || digitalRead(PIN_BTN_KEYCAP) == LOW;
  unsigned long now = millis();
  if (down == wasDown || now - changedAt < DEBOUNCE_MS) return false;

  wasDown = down;
  changedAt = now;
  return down;
}

int randomOtherThan(int exclude) {
  if (RESTAURANT_COUNT < 2) return 0;
  int i;
  do {
    i = esp_random() % RESTAURANT_COUNT;
  } while (i == exclude);
  return i;
}

void drawCentered(const char* text, int y, uint16_t color) {
  u8f.setForegroundColor(color);
  int x = (canvas.width() - u8f.getUTF8Width(text)) / 2;
  u8f.setCursor(x, y);
  u8f.print(text);
}

void render() {
  canvas.fillSprite(COLOR_BG);

  if (state == State::Idle) {
    drawCentered("오늘 점심 뭐 먹지?", 60, COLOR_TEXT);
    drawCentered("버튼을 눌러 돌리기", 120, COLOR_DIM);
  } else {
    const Restaurant& r = RESTAURANTS[current];
    bool done = state == State::Result;

    if (done) {
      canvas.drawRoundRect(10, 30, canvas.width() - 20, 64, 8, COLOR_ACCENT);
      drawCentered("오늘은 여기!", 20, COLOR_ACCENT);
    }
    drawCentered(r.name, 60, done ? COLOR_ACCENT : COLOR_TEXT);

    char info[48];
    snprintf(info, sizeof(info), "%s / 도보 %d분", r.category, r.walkMin);
    drawCentered(info, 84, COLOR_DIM);

    const char* hint = state == State::Spinning ? "눌러서 멈추기"
                     : state == State::Stopping ? "..."
                     : "다시 돌리려면 누르기";
    drawCentered(hint, 124, COLOR_DIM);
  }

  canvas.pushSprite(0, 0);
}

void setup() {
  pinMode(PIN_BTN_BUILTIN, INPUT_PULLUP);
  pinMode(PIN_BTN_KEYCAP, INPUT_PULLUP);

  tft.init();
  tft.setRotation(1);  // 가로 240x135
  canvas.createSprite(tft.width(), tft.height());

  u8f.begin(canvas);
  u8f.setFont(u8g2_font_unifont_t_korean2);
  u8f.setFontMode(1);  // 투명 배경

  render();
}

void loop() {
  unsigned long now = millis();

  if (buttonPressed()) {
    if (state == State::Idle || state == State::Result) {
      state = State::Spinning;
      stepMs = SPIN_INTERVAL_MS;
      nextStepAt = now;
    } else if (state == State::Spinning) {
      state = State::Stopping;
    }
  }

  if ((state == State::Spinning || state == State::Stopping) && now >= nextStepAt) {
    current = randomOtherThan(current);

    if (state == State::Stopping) {
      stepMs *= SLOWDOWN_FACTOR;
      if (stepMs > STOP_INTERVAL_MS) {
        // 직전 결과와 같은 곳이 연속으로 나오지 않게
        if (current == lastPicked) current = randomOtherThan(lastPicked);
        lastPicked = current;
        state = State::Result;
      }
    }

    nextStepAt = now + (unsigned long)stepMs;
    render();
  }
}
