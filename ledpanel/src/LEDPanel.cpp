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
    ws2811.channel[0].brightness = 55;
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

void LEDPanel::setPixel(uint16_t x, uint16_t y, Color rgb)
{
    unsigned int index = translateXYtoIndex(x, y);
    if (index >= width * height) {
        return;
    }
    else if (index < 0) {
        return;
    }
    ws2811.channel[0].leds[index] = ((uint32_t)(rgb.r * 255.0f) << 16) | ((uint32_t)(rgb.g * 255.0f) << 8) | (uint32_t)(rgb.b * 255.0f);
}

void LEDPanel::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Color rgb) {
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        int16_t tmp = x0;
        x0 = y0;
        y0 = tmp;
        tmp = x1;
        x1 = y1;
        y1 = tmp;
    }

    if (x0 > x1) {
        int16_t tmp = x0;
        x0 = x1;
        x1 = tmp;
        tmp = y0;
        y0 = y1;
        y1 = tmp;
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    }
    else {
        ystep = -1;
    }

    for (; x0 <= x1; x0++) {
        if (steep) {
            setPixel(y0, x0, rgb);
        }
        else {
            setPixel(x0, y0, rgb);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void LEDPanel::drawCircle(uint16_t xCenter, uint16_t yCenter, Color rgb, float radius, bool fill)
{
    if (fill) {
        for (int16_t i = xCenter - radius; i < xCenter + radius; i++) {
            for (int16_t j = yCenter - radius; j < yCenter + radius; j++) {
                if ( sqrt(pow(i - xCenter, 2) + pow(j - yCenter, 2) ) <= radius) {
                    setPixel(i, j, rgb);
                }
            }
        }
    }
    else {
        int16_t x = 0;
        int16_t y = radius;
        int16_t d = 3 - 2 * radius;
        drawOctant(xCenter, yCenter, x, y, rgb);
        while (y >= x)
        {
            x++;
            if (d > 0){
                y--;
                d = d + 4 * (x - y) + 10;
            }
            else {
                d = d + 4 * x + 6;
            }
            drawOctant(xCenter, yCenter, x, y, rgb);
        }
    }
}

void LEDPanel::drawOctant(uint16_t xCenter, uint16_t yCenter, int16_t x, int16_t y, Color rgb)
{
    setPixel(xCenter + x, yCenter + y, rgb);
    setPixel(xCenter - x, yCenter + y, rgb);
    setPixel(xCenter + x, yCenter - y, rgb);
    setPixel(xCenter - x, yCenter - y, rgb);
    setPixel(xCenter + y, yCenter + x, rgb);
    setPixel(xCenter - y, yCenter + x, rgb);
    setPixel(xCenter + y, yCenter - x, rgb);
    setPixel(xCenter - y, yCenter - x, rgb);
}

Color LEDPanel::getPixel(uint16_t x, uint16_t y)
{
    uint16_t index = translateXYtoIndex(x, y);
    uint8_t r = (pixels[index] >> 16) & 0xFF;
    uint8_t g = (pixels[index] >>  8) & 0xFF;
    uint8_t b = (pixels[index] >>  0) & 0xFF;
    return Color::ColorRGB(r, g, b);
}

void LEDPanel::clearPixel(uint16_t x, uint16_t y)
{
    Color empty(0, 0, 0);
    setPixel(x, y, empty);
}

void LEDPanel::fillPanel(Color rgb) 
{
    for (uint16_t y = 0; y < panelsY * panelHeight; y++) {
        for (uint16_t x = 0; x < panelsX * panelWidth; x++) {
            setPixel(x, y, rgb);
        }
    }
}

void LEDPanel::clearPanel() 
{
    Color empty(0, 0, 0);
    fillPanel(empty);
}

uint16_t LEDPanel::getWidth()
{
    return width;
}

uint16_t LEDPanel::getHeight()
{
    return height;
}