#ifndef LEDPANEL_H
#define LEDPANEL_H

#include <ws2811.h>
#include <stdint.h>
#include <cstdlib>
#include <cstdio>

#define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN                21
#define DMA                     10

typedef struct {
    unsigned char r, g, b;
} Color_t;

class LEDPanel
{
public:
    LEDPanel(uint8_t panelsX, uint8_t panelsY, uint16_t panelWidth, uint16_t panelHeight);
    ~LEDPanel();
    
    // Show LED 
    void display();

    // Set Pixel Color
    void setPixel(uint16_t x, uint16_t y, Color_t rgb);
    
    // Get Pixel Color
    Color_t getPixel(uint16_t x, uint16_t y);

    // Clear Pixel
    void clearPixel(uint16_t x, uint16_t y);

    // Fill Panel
    void fillPanel(Color_t rgb);

    // Clear Panel
    void clearPanel();

private:
    // Translate x, y to index
    uint16_t translateXYtoIndex(uint16_t x, uint16_t y);

    uint16_t width;
    uint16_t height;

    uint8_t panelsX;
    uint8_t panelsY;
    uint16_t panelWidth;
    uint16_t panelHeight;
};

#endif