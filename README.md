# Seamless Perlin Noise Generator

이 프로젝트는 FBM(Fractal Brownian Motion) 기법을 활용하여 seamless한(타일 가능한) Perlin 노이즈 텍스처를 생성하는 프로그램입니다.  
C++(SFML 기반)과 Python(NumPy, Pillow, Tkinter 기반) 두 가지 구현 버전을 제공하며, 각 코드에서 중요한 파라미터만 조정하면 쉽게 사용할 수 있습니다.

## Features
- **Seamless Noise:** 모듈러 연산과 좌표 스케일링을 통해 경계가 자연스럽게 이어지는 타일 가능한 노이즈 생성.
- **FBM 구현:** 여러 옥타브(octave)를 합산하여 자연스러운 디테일과 계층적 변화를 표현.
- 간단한 파라미터 조정을 통해 다양한 해상도와 노이즈 패턴 생성 가능.

---

## C++ 코드 사용법

### Requirements
- Visual Studio (또는 C++17 이상을 지원하는 컴파일러)

### 설정 및 실행
1. **프로젝트 열기:** Visual Studio에서 프로젝트를 엽니다.
2. **파라미터 설정:** 코드 상단에서 다음의 값을 설정합니다.
   - `PERIOD` : 노이즈 패턴의 주기
   - `windowWidth` : 생성할 이미지의 너비
   - `windowHeight` : 생성할 이미지의 높이
   - `OCTAVES` : FBM에 사용할 옥타브 수
3. **빌드 및 실행:** Debug 모드에서 실행하면, 노이즈 텍스처가 창에 표시됩니다.
4. **이미지 저장:** 프로그램 종료 시 자동으로 이미지가 저장됩니다.  
   저장 경로는 코드 하단에서 아래와 같이 지정할 수 있습니다.
    ```cpp
    std::string filepath = "../seamless_noise.png";
    ```

## Python 코드 사용법

### Requirements
- Python 3.x
- NumPy
- Pillow (PIL)
- Tkinter (대부분의 Python 배포판에 기본 포함)

### 설정 및 실행
1. **스크립트 열기:** Python 스크립트를 텍스트 에디터나 IDE에서 엽니다.
2. **파라미터 설정:** 코드 상단에서 다음의 값을 설정합니다.
   - `PERIOD` : 노이즈 패턴의 주기
   - `WIDTH` : 생성할 이미지의 너비
   - `HEIGHT` : 생성할 이미지의 높이
   - `OCTAVES` : FBM에 사용할 옥타브 수
3. **스크립트 실행:** 스크립트를 실행하면, Tkinter 창에 노이즈 이미지가 표시됩니다.
4. **이미지 저장:** 창의 "Save Image" 버튼을 클릭하면 파일 다이얼로그가 열리며, 원하는 위치에 저장할 수 있습니다.
