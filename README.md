# miniapps

작은 앱들을 모아두는 리포지토리입니다. 앱 하나당 폴더 하나로 관리합니다.

## 앱 목록

| 앱 | 설명 | 상태 | 링크 |
| --- | --- | --- | --- |
| _(아직 없음)_ | | | |

<!-- 예시: | [timer](./timer) | 뽀모도로 타이머 | 개발 중 | [데모](https://...) | -->

## 폴더 구조

```
miniapps/
├── README.md       ← 이 파일 (전체 앱 목록)
├── .gitignore
├── _template/      ← 새 앱 만들 때 복사해서 쓰는 템플릿
└── <app-name>/     ← 앱 하나당 폴더 하나
    ├── README.md
    └── ...
```

## 새 앱 추가하기

```bash
cp -r _template my-new-app
```

1. `my-new-app/README.md` 내용 채우기
2. 위 **앱 목록** 표에 한 줄 추가

## 규칙

나중에 앱 하나만 배포하거나 별도 리포로 분리하기 쉽도록:

- 각 앱은 **자기 폴더 안에서 완전히 독립**적으로 동작한다. (`package.json` 등 설정 파일도 앱 폴더 안에)
- 다른 앱 폴더의 파일을 `../other-app/...` 처럼 참조하지 않는다.
- 폴더 이름은 소문자 + 하이픈 (`my-app`)

## 배포 / 분리

- **폴더만 배포**: Vercel·Netlify에서 Root Directory를 앱 폴더로 지정
- **별도 리포로 분리** (커밋 기록 유지):
  ```bash
  git subtree split --prefix=<app-name> -b <app-name>-only
  git push <새 리포 URL> <app-name>-only:main
  ```
