import math
import numpy as np
from PIL import Image, ImageTk
import tkinter as tk
from tkinter import filedialog

# 전역 상수: 원하는 주기(period)
PERIOD = 10

# 윈도우 크기 
WIDTH=800
HEIGHT=800

# FBM 옥타브 수
OCTAVES=6

def randomGradient(ix, iy):
    """
    주어진 그리드 좌표 (ix, iy)를 주기 내에서 wrap-around 처리하여,
    해싱 방식으로 임의의 각도를 만들고, 해당 각도의 단위 벡터 (sin, cos)를 반환합니다.
    """
    # 주기 적용: 좌표를 PERIOD로 나눈 나머지로 만듭니다.
    ix = ix % PERIOD
    iy = iy % PERIOD

    w = 32
    s = w // 2

    a = ix & 0xFFFFFFFF
    b = iy & 0xFFFFFFFF

    a = (a * 3284157443) & 0xFFFFFFFF
    b = b ^ (((a << s) | (a >> (w - s))) & 0xFFFFFFFF)
    b = (b * 1911520717) & 0xFFFFFFFF
    a = a ^ (((b << s) | (b >> (w - s))) & 0xFFFFFFFF)
    a = (a * 2048419325) & 0xFFFFFFFF

    # 최대 부호 없는 정수 2147483647.0 사용
    random_val = a * (3.14159265 / 2147483647.0)
    return math.sin(random_val), math.cos(random_val)

def dotGridGradient(ix, iy, x, y):
    """
    (ix, iy) 좌표의 랜덤 그라디언트와, 점 (x, y)와 그리드 좌표 사이의 벡터 내적을 계산합니다.
    """
    grad = randomGradient(ix, iy)
    dx = x - float(ix)
    dy = y - float(iy)
    return dx * grad[0] + dy * grad[1]

def smoothstepInterpolate(a, b, t):
    """
    smoothstep 보간 함수를 이용해 a와 b 사이를 보간합니다.
    """
    t = max(0.0, min(1.0, t))
    smoothT = t * t * (3 - 2 * t)
    return a + (b - a) * smoothT

def perlin(x, y):
    """
    주어진 (x, y) 좌표에 대해 Perlin 노이즈 값을 계산합니다.
    """
    x0 = math.floor(x)
    y0 = math.floor(y)
    x1 = x0 + 1
    y1 = y0 + 1

    sx = x - x0
    sy = y - y0

    n00 = dotGridGradient(x0, y0, x, y)
    n10 = dotGridGradient(x1, y0, x, y)
    n01 = dotGridGradient(x0, y1, x, y)
    n11 = dotGridGradient(x1, y1, x, y)

    ix0 = smoothstepInterpolate(n00, n10, sx)
    ix1 = smoothstepInterpolate(n01, n11, sx)
    return smoothstepInterpolate(ix0, ix1, sy)

def generate_noise_image(windowWidth=800, windowHeight=800, octaves=6):
    # 픽셀 데이터를 저장할 NumPy 배열 (높이, 너비, RGBA)
    pixels = np.zeros((windowHeight, windowWidth, 4), dtype=np.uint8)

    # x, y 방향 각각 주기를 정규화
    periodNormalizedX = float(PERIOD) / windowWidth
    periodNormalizedY = float(PERIOD) / windowHeight

    baseFreq = 1.0
    baseAmp = 1.0

    for y in range(windowHeight):
        for x in range(windowWidth):
            val = 0.0
            freq = baseFreq
            amp = baseAmp

            # 여러 옥타브로 FBM 구현
            for i in range(octaves):
                noise_val = perlin(x * periodNormalizedX * freq, y * periodNormalizedY * freq)
                val += noise_val * amp
                freq *= 2
                amp /= 2

            # 대비 조정 및 클리핑 (-1 ~ 1)
            val *= 1.2
            if val > 1.0:
                val = 1.0
            elif val < -1.0:
                val = -1.0

            # -1 ~ 1을 0 ~ 255 그레이스케일 값으로 변환
            color = int(((val + 1.0) * 0.5) * 255)
            pixels[y, x] = [color, color, color, 255]

    return Image.fromarray(pixels, mode="RGBA")

def main():
    # 노이즈 이미지 생성
    img = generate_noise_image(WIDTH, HEIGHT, OCTAVES)

    # Tkinter 윈도우 생성 및 이미지 표시
    root = tk.Tk()
    root.title("Seamless Perlin Noise")

    # PIL 이미지를 Tkinter에서 사용 가능한 PhotoImage로 변환
    photo = ImageTk.PhotoImage(img)
    label = tk.Label(root, image=photo)
    label.pack()

    # Save 버튼 클릭 시 파일 다이얼로그를 통해 저장
    def save_image():
        file_path = filedialog.asksaveasfilename(defaultextension=".png",
                                                 filetypes=[("PNG files", "*.png"), ("All files", "*.*")], initialdir=".")
        if file_path:
            img.save(file_path)
            print("Image saved to:", file_path)

    save_button = tk.Button(root, text="Save Image", command=save_image)
    save_button.pack(pady=10)

    root.mainloop()

if __name__ == "__main__":
    main()
