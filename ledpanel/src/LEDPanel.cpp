#include <LEDPanel.h>

ws2811_t ws2811;

ws2811_led_t* pixels;

LEDPanel::LEDPanel(uint8_t panelsX, uint8_t panelsY, uint16_t panelWidth, uint16_t panelHeight) :
    panelsX(panelsX),
    panelsY(panelsY),
    panelWidth(panelWidth),
    panelHeight(panelHeight),
    width(panelsX * panelWidth),
    height(panelsY * panelHeight)
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

uint16_t LEDPanel::translateXYtoIndex(uint16_t x, uint16_t y)
{
    uint8_t panelX = x / panelWidth;
    uint8_t panelY = y / panelHeight;
    x = x % panelWidth;
    y = y % panelHeight;
    uint16_t index = (panelX + panelX * panelY) * panelWidth * panelHeight + y * panelWidth + ((y % 2 == 1) ? panelWidth - x - 1 : x);
    return index;
}

void LEDPanel::display()
{
    ws2811_render(&ws2811);
}

void LEDPanel::setPixel(uint16_t x, uint16_t y, Color_t rgb)
{
    unsigned int index = translateXYtoIndex(x, y);
    pixels[index] = ((uint16_t)rgb.r << 16) | ((uint16_t)rgb.g << 8) | (uint16_t)rgb.b;
}

Color_t LEDPanel::getPixel(uint16_t x, uint16_t y)
{
    uint16_t index = translateXYtoIndex(x, y);
    Color_t color;
    color.r = (pixels[index] >> 16) & 0xFF;
    color.g = (pixels[index] >>  8) & 0xFF;
    color.b = (pixels[index] >>  0) & 0xFF;
    return color;
}

void LEDPanel::clearPixel(uint16_t x, uint16_t y)
{
    Color_t color = { 0, 0, 0 };
    setPixel(x, y, color);
}

void LEDPanel::fillPanel(Color_t rgb) 
{
    for (uint16_t y = 0; y < panelsY * panelHeight; y++) {
        for (uint16_t x = 0; x < panelsX * panelWidth; x++) {
            setPixel(x, y, rgb);
        }
    }
}

void LEDPanel::clearPanel() 
{
    Color_t color = { 0, 0, 0 };
    fillPanel(color);
}