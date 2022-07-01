#ifndef LEDPANEL_H
#define LEDPANEL_H

#include <ws2811.h>
#include <Color.h>
#include <stdint.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>

#define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN                21
#define DMA                     10

class LEDPanel
{
public:
    LEDPanel(uint8_t panelsX, uint8_t panelsY, uint16_t panelWidth, uint16_t panelHeight);
    ~LEDPanel();
    
    // Show LED 
    void display();

    // Set Pixel Color
    void setPixel(uint16_t x, uint16_t y, Color rgb);

    // Draw Line
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Color rgb);

    // Draw Circle
    void drawCircle(uint16_t xCenter, uint16_t yCenter, Color rgb, float radius, bool fill);
    
    // Get Pixel Color
    Color getPixel(uint16_t x, uint16_t y);

    // Clear Pixel
    void clearPixel(uint16_t x, uint16_t y);

    // Fill Panel
    void fillPanel(Color rgb);

    // Clear Panel
    void clearPanel();

    // Get width, height
    uint16_t getWidth();
    uint16_t getHeight();


private:
    // Translate x, y to index
    uint16_t translateXYtoIndex(uint16_t x, uint16_t y);

    // Draw Octant, circle partial draw
    void drawOctant(uint16_t xCenter, uint16_t yCenter, int16_t x, int16_t y, Color rgb);

    uint16_t width;
    uint16_t height;

    uint8_t panelsX;
    uint8_t panelsY;
    uint16_t panelWidth;
    uint16_t panelHeight;
};

#endif