#include "calc.h"
#include "view.h"

void AlphaGetImage(const AlphaConfig *conf, sf::Image *img)
{
    uint8_t *res = conf->res;

    uint32_t width  = conf->width;
    uint32_t height = conf->height;

    uint32_t pos = 0;

    for (uint32_t yi = 0; yi < height; ++yi)
    {
        for (uint32_t xi = 0; xi < width; ++xi, pos += BYTES_PER_PIXEL)
        {
            uint32_t color = *(uint32_t*) (res + pos);

            img->setPixel(xi, yi, sf::Color((uint8_t) ( color          & 0xFF),
                                            (uint8_t) ((color >> 0x8 ) & 0xFF),
                                            (uint8_t) ((color >> 0x10) & 0xFF),
                                            (uint8_t) ((color >> 0x18) & 0xFF)));
        }
    }
}

void AlphaRun(AlphaConfig *conf)
{
    sf::RenderWindow window(sf::VideoMode(conf->width, conf->height), "Alpha-blending");

    sf::Image img = {};
    img.create(conf->width, conf->height);

    sf::Texture texture = {};
    sf::Sprite  sprite  = {};

    sf::Clock clock_10sec = {};
    sf::Clock clock       = {};

    int32_t frame_cnt = 0;

    double   sum   = 0;
    uint64_t ticks = 0;

    float fps_avg = 0;

    while (window.isOpen())
    {
        sf::Event event = {};

        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        for (int32_t i = 0; i < ACCURANCY; ++i)
        #ifdef _OPTIMIZE
            AlphaCalcAVX512(conf);
        #else
            AlphaCalcNoOpts(conf);
        #endif

        AlphaGetImage(conf, &img);

        texture.loadFromImage(img);
        sprite.setTexture(texture);

        float fps = GetFPS(&clock, &frame_cnt) * ACCURANCY;

        sum += fps;
        ++ticks;

        if (clock_10sec.getElapsedTime().asSeconds() > 10.f)
        {
            fps_avg = sum / ticks;

            sum   = 0;
            ticks = 0;

            clock_10sec.restart();
        }

        char title[16] = "";
        snprintf(title, sizeof(title), "%.2f", fps_avg);
        
        window.setTitle(title);
        window.clear();
        window.draw(sprite);
        window.display();
    }
}

float GetFPS(sf::Clock *clock, int32_t *frame_cnt)
{
    ++*frame_cnt;

    float curr_time = clock->getElapsedTime().asSeconds();
    float fps       = (float) *frame_cnt / curr_time;

    if (curr_time > 1.f || *frame_cnt == MAX_FRAME_CNT)
    {
        clock->restart();
        *frame_cnt = 0;
    }

    return fps;
}

