#ifndef LEDPANEL_H
#define LEDPANEL_H

#include <ws2811.h>

#define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN                21
#define DMA                     10

typedef struct {
    unsigned char r, g, b;
} Color_t;



class LEDPanel
{
public:
    LEDPanel(unsigned char panelsX, unsigned char panelsY, unsigned int panelWidth, unsigned int panelHeight);
    ~LEDPanel();
    
    // Show LED 
    void display();

    // Set Pixel Color
    void setPixel(unsigned int x, unsigned int y, Color_t rgb);
    
    // Get Pixel Color
    Color_t getPixel(unsigned int x, unsigned int y);

    // Clear Pixel
    void clearPixel(unsigned int x, unsigned int y);

    // Fill Panel
    void fillPanel(Color_t rgb);

    // Clear Panel
    void clearPanel();

private:
    // Translate x, y to index
    unsigned int translateXYtoIndex(unsigned int x, unsigned int y);

    unsigned char panelsX;
    unsigned char panelsY;
    unsigned int panelWidth;
    unsigned int panelHeight;
};

#endif