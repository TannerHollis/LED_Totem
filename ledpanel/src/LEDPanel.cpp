#include "LEDPanel.h"
#include <ws2811.h>
#include <stdio.h>
#include <stdlib.h>

ws2811_t ws2811;

ws2811_led_t* pixels;

LEDPanel::LEDPanel(unsigned char panelsX, unsigned char panelsY, unsigned int panelWidth, unsigned int panelHeight) : 
    panelsX(panelsX),
    panelsY(panelsY),
    panelWidth(panelWidth),
    panelHeight(panelHeight)
{
    // Initialize the controller properties
    ws2811.freq = WS2811_TARGET_FREQ;
    ws2811.dmanum = DMA;

    // Initialize controller, channel 0 properties
    ws2811.channel[0].gpionum = GPIO_PIN;
    ws2811.channel[0].count = panelsX * panelsY * panelWidth * panelHeight;
    ws2811.channel[0].invert = 0;
    ws2811.channel[0].brightness = 255;
    ws2811.channel[0].strip_type = WS2812_STRIP;

    // Initialize second channel, NOT USED
    ws2811.channel[1].gpionum = 0;
    ws2811.channel[1].count = 0;
    ws2811.channel[1].brightness = 0;

    //Initialize pixel array
    pixels = (ws2811_led_t*)malloc(sizeof(ws2811_led_t) * ws2811.channel[0].count);
    ws2811.channel[0].leds = pixels;

    // Initialize WS2811 controller
    ws2811_return_t ret;
    ret = ws2811_init(&ws2811);

    if (ret != WS2811_SUCCESS) {
        printf("WS2811 Initialization failed.\n");
    }
    else {
        printf("WS2811 Initialization success!\n");
    }
}

LEDPanel::~LEDPanel() 
{
    ws2811_fini(&ws2811);
}

unsigned int LEDPanel::translateXYtoIndex(unsigned int x, unsigned int y)
{
    unsigned char panelX = x / panelWidth;
    unsigned char panelY = y / panelHeight;
    x = x % panelWidth;
    y = y % panelHeight;
    unsigned int index = (panelX + panelX * panelY) * panelWidth * panelHeight + y * panelWidth + ((y % 2 == 1) ? panelWidth - x - 1 : x);
    return index;
}

void LEDPanel::display()
{
    ws2811_render(&ws2811);
}

void LEDPanel::setPixel(unsigned int x, unsigned int y, Color_t rgb) 
{
    unsigned int index = translateXYtoIndex(x, y);
    pixels[index] = ((unsigned int)rgb.r << 16) | ((unsigned int)rgb.g << 8) | (unsigned int)rgb.b;
}

Color_t LEDPanel::getPixel(unsigned int x, unsigned y)
{
    unsigned int index = translateXYtoIndex(x, y);
    Color_t color;
    color.r = (pixels[index] >> 16) & 0xFF;
    color.g = (pixels[index] >>  8) & 0xFF;
    color.b = (pixels[index] >>  0) & 0xFF;
    return color;
}

void LEDPanel::clearPixel(unsigned x, unsigned y) 
{
    Color_t color = { 0, 0, 0 };
    setPixel(x, y, color);
}

void LEDPanel::fillPanel(Color_t rgb) 
{
    for (unsigned int y = 0; y < panelsY * panelHeight; y++) {
        for (unsigned int x = 0; x < panelsX * panelWidth; x++) {
            setPixel(x, y, rgb);
        }
    }
}

void LEDPanel::clearPanel() 
{
    Color_t color = { 0, 0, 0 };
    fillPanel(color);
}