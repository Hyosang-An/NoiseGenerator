#include <SFML/Graphics.hpp>
#include <algorithm> // std::clamp
#include <cmath>
#include <iostream>


struct vector2
{
    float x, y;
};

// 최초에 그리드 몇개로 나눌지 (x방향, y방향 동일하게 적용)
const int PERIOD = 5;

// 이미지 사이즈
const int windowWidth = 800;
const int windowHeight = 800;
// FBM을 위한 옥타브 파라미터
const int OCTAVES = 3;

// 주어진 그리드 좌표를 주기(period) 내에서 wrap-around하도록 처리하여 랜덤 그라디언트를 생성합니다.
vector2 randomGradient(int ix, int iy)
{
    // 주기 적용: 좌표를 period로 나눈 나머지를 사용합니다.
    ix = ix % PERIOD;
    iy = iy % PERIOD;

    // 기존 해싱 방식으로 임의의 각도를 생성합니다.
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2;
    unsigned a = ix, b = iy;
    a *= 3284157443u;

    b ^= (a << s) | (a >> (w - s));
    b *= 1911520717u;

    a ^= (b << s) | (b >> (w - s));
    a *= 2048419325u;

    // a 값을 0 ~ 2*PI 범위의 각도로 변환
    float random = a * (3.14159265f / static_cast<float>(~(~0u >> 1)));

    vector2 v;
    v.x = sin(random);
    v.y = cos(random);
    return v;
}

// 두 점 사이의 내적 계산. period 정보는 randomGradient에서 사용됩니다.
float dotGridGradient(int ix, int iy, float x, float y)
{
    vector2 gradient = randomGradient(ix, iy);

    float dx = x - (float)ix;
    float dy = y - (float)iy;

    return (dx * gradient.x + dy * gradient.y);
}

// smoothstep 보간을 이용한 선형 보간 함수
double smoothstepInterpolate(double a, double b, double t)
{
    t = std::clamp(t, 0.0, 1.0);
    double smoothT = t * t * (3 - 2 * t);
    return a + (b - a) * smoothT;
}

// seamless한 Perlin 노이즈 계산 함수
float perlin(float x, float y)
{
    // 그리드 셀의 정수 좌표 결정
    int x0 = (int)floor(x);
    int y0 = (int)floor(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    // 보간을 위한 소수부
    float sx = x - (float)x0;
    float sy = y - (float)y0;

    // 모서리 내적 값 계산 (모듈러 처리가 randomGradient 내부에서 이루어짐)
    float n00 = dotGridGradient(x0, y0, x, y);
    float n10 = dotGridGradient(x1, y0, x, y);
    float n01 = dotGridGradient(x0, y1, x, y);
    float n11 = dotGridGradient(x1, y1, x, y);

    // x 방향 보간
    float ix0 = smoothstepInterpolate(n00, n10, sx);
    float ix1 = smoothstepInterpolate(n01, n11, sx);

    // y 방향 보간하여 최종 결과 리턴
    return smoothstepInterpolate(ix0, ix1, sy);
}

int main()
{

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Seamless Perlin Noise");

    sf::Uint8* pixels = new sf::Uint8[windowWidth * windowHeight * 4];

    // GRID_SIZE 대신, period를 기준으로 노이즈 좌표를 매핑합니다.
    // 이미지 전체가 몇 개의 period로 구성될 것인지를 조정합니다.
    float periodNormalizedX = (float)PERIOD / windowWidth; // 예시: windowWidth 크기에 맞춰 period 적용
    float periodNormalizedY = (float)PERIOD / windowHeight;


    float baseFreq = 1.0f;
    float baseAmp = 1.0f;

    for (int y = 0; y < windowHeight; y++)
    {
        for (int x = 0; x < windowWidth; x++)
        {
            int index = (y * windowWidth + x) * 4;
            float val = 0.0f;
            float freq = baseFreq;
            float amp = baseAmp;

            // 각 옥타브마다 노이즈 값을 합산합니다.
            for (int i = 0; i < OCTAVES; i++)
            {
                // 좌표에 noiseScale과 주파수를 곱해주어 period를 기준으로 wrap-around하도록 합니다.
                val += perlin(x * periodNormalizedX * freq, y * periodNormalizedY * freq) * amp;
                freq *= 2;
                amp /= 2;
            }

            // 대비 조정 및 클리핑
            val *= 1.2f;
            if (val > 1.0f)
                val = 1.0f;
            if (val < -1.0f)
                val = -1.0f;

            // -1 ~ 1을 0 ~ 255 범위의 색상으로 변환
            int color = static_cast<int>(((val + 1.0f) * 0.5f) * 255);

            pixels[index] = color;
            pixels[index + 1] = color;
            pixels[index + 2] = color;
            pixels[index + 3] = 255;
        }
    }

    sf::Texture texture;
    texture.create(windowWidth, windowHeight);
    texture.update(pixels);
    sf::Sprite sprite(texture);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(sprite);
        window.display();
    }

    sf::Image image;
    image.create(windowWidth, windowHeight, pixels);

    std::string filepath = "../seamless_noise.png";
    if (image.saveToFile(filepath))
    {
        std::cout << "Image saved successfully." << std::endl;
    }
    else
    {
        std::cout << "Failed to save the image." << std::endl;
    }

    delete[] pixels;
    return 0;
}
