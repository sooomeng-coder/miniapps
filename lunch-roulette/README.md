# lunch-roulette 🍚

회사 근처 점심 메뉴를 랜덤으로 추천해주는 룰렛.
식당 목록 하나(`data/restaurants.json`)를 웹, 구글 챗 봇, ESP32 기기가 같이 씁니다.

```
              data/restaurants.json  (식당 목록 — 여기만 고치면 됨)
               │            │              │
   ┌───────────┘            │              └────────────┐
   ▼                        ▼                           ▼
 🌐 웹 룰렛              🤖 구글 챗 봇               🔘 ESP32 기기
 GitHub Pages            GitHub Actions             T-Display + 버튼
 링크로 접속해서 돌리기   평일 11:20 오늘의 추천 전송   버튼 눌러 돌리기
```

## 폴더 구조

```
lunch-roulette/
├── data/restaurants.json   식당 목록
├── web/index.html          웹 룰렛
├── bot/post-lunch.mjs      구글 챗 추천 봇
└── firmware/               ESP32 T-Display 코드 (PlatformIO)

.github/workflows/          (리포 루트)
├── lunch-roulette-pages.yml   웹 자동 배포
└── lunch-roulette-bot.yml     봇 자동 실행
```

## 식당 추가/수정

`data/restaurants.json`에 한 줄씩 추가하고 main에 올리면 웹/봇/기기에 자동 반영됩니다.
GitHub 웹사이트에서 파일을 열고 ✏️ 버튼으로 바로 고쳐도 됩니다.

```json
{ "name": "식당 이름", "category": "한식", "walkMin": 3, "menu": "대표메뉴", "price": 9000 }
```

> ⚠️ 이 리포는 공개(public)라서 식당 목록도 누구나 볼 수 있어요.

---

## 🌐 웹 룰렛

주소: **https://sooomeng-coder.github.io/miniapps/lunch-roulette/**

### 처음 한 번만 설정
1. GitHub 리포 → **Settings → Pages**
2. **Source**를 **GitHub Actions**로 선택

이후 `web/`나 `data/`가 바뀌어서 main에 올라가면 자동으로 다시 배포됩니다.

### 내 컴퓨터에서 미리보기
```bash
cd lunch-roulette
python3 -m http.server 8000
# 브라우저에서 http://localhost:8000/web/ 열기
```

---

## 🤖 구글 챗 봇

평일 오전 11:20쯤 구글 챗 방에 오늘의 추천 식당과 웹 룰렛 링크를 보냅니다.

### 처음 한 번만 설정
1. 구글 챗에서 봇을 넣을 **스페이스** 열기 → 스페이스 이름 클릭 → **앱 및 통합** → **웹훅 추가**
2. 이름(예: 점심요정) 입력 후 저장 → 생성된 **웹훅 URL 복사**
3. GitHub 리포 → **Settings → Secrets and variables → Actions → New repository secret**
   - Name: `GOOGLE_CHAT_WEBHOOK_URL`
   - Secret: 복사한 URL
4. 테스트: 리포 **Actions** 탭 → **lunch-roulette bot** → **Run workflow**

> 회사 구글 워크스페이스 설정에 따라 웹훅 메뉴가 안 보일 수 있어요. 그럴 땐 관리자에게 웹훅 허용을 요청해야 합니다.

### 보내는 시간 바꾸기
`.github/workflows/lunch-roulette-bot.yml`의 `cron` 값을 수정 (UTC 기준, 한국시간 −9시간).

### 내 컴퓨터에서 테스트
```bash
node lunch-roulette/bot/post-lunch.mjs   # 웹훅 URL 없으면 메시지를 화면에 출력만 함
```

---

## 🔘 ESP32 기기

### 준비물

| 부품 | 비고 |
| --- | --- |
| LilyGO TTGO **T-Display** (ESP32, 1.14" 화면) | 화면 + 버튼 내장 |
| (선택) 키캡 + 기계식 스위치 | 없으면 보드 내장 버튼으로 동작 |
| USB-C 케이블 | 데이터 전송 되는 것 |

### 키캡 버튼 연결 (선택)

스위치 다리 2개를 이렇게 연결 (저항 필요 없음):

```
스위치 다리 1 ── GPIO 21
스위치 다리 2 ── GND
```

키캡 없이도 보드의 **GPIO0 버튼**(USB 포트 옆 왼쪽 버튼)으로 똑같이 동작합니다.

### 와이파이 설정

`firmware/src/secrets.example.h`를 같은 폴더에 `secrets.h`로 복사하고 와이파이 이름/비밀번호 입력.
(`secrets.h`는 깃에 올라가지 않습니다.)

켜질 때 와이파이로 웹 룰렛과 같은 식당 목록을 불러옵니다.
와이파이가 없거나 실패하면 `firmware/src/menus.h`의 기본 목록을 사용합니다.

### 업로드

1. [VS Code](https://code.visualstudio.com/) 설치 → 확장 프로그램에서 **PlatformIO IDE** 설치
2. VS Code에서 `lunch-roulette/firmware` 폴더 열기
3. 보드를 USB로 연결 → 하단 파란 바의 **→ (Upload)** 클릭

라이브러리와 화면 설정은 `platformio.ini`에 들어 있어 자동 적용됩니다.

### 문제 해결

- **업로드가 안 됨**: 보드 오른쪽 버튼(BOOT)을 누른 채로 업로드 시작
- **화면이 안 켜짐 / 이상한 색**: 보드가 T-Display(ST7789, 135x240)가 맞는지 확인
- **"오프라인 목록 사용"이 뜸**: 와이파이 이름/비밀번호 확인. 회사 와이파이가 로그인 페이지 방식이면 핫스팟 등 일반 와이파이 사용

---

## 다음에 해볼 것

- 리뷰 & 별점 (다녀온 사람들이 한 줄 코멘트 남기기)
- 카카오 지도 API로 근처 식당 자동 수집
- 별점 높은 곳이 더 자주 나오게
