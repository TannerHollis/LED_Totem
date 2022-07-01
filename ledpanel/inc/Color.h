#ifndef COLOR_H
#define COLOR_H

#include <cmath>

class Color 
{
public:
    Color(float r = 0, float g = 0, float b = 0) : r(r), g(g), b(b)
    {
        // Do nothing.
    }
    
    ~Color()
    {
        // Do nothing.
    }

    Color Scale(float scale)
    {
        return ColorRGB(r * scale, g * scale, b * scale);
    }

    Color Lerp(Color in, float val)
    {
        float red = interpolate(r, in.r, val);
        float green = interpolate(g, in.g, val);
        float blue = interpolate(b, in.b, val);
        return Color::ColorRGB(red, green, blue);
    }

    static Color ColorRGB(float r, float g, float b)
    {
        Color rgb(r, g, b);
        rgb.r = r;
        rgb.g = g;
        rgb.b = b;

        Color hsv = rgb2hsv(rgb);
        rgb.h = hsv.h;
        rgb.s = hsv.s;
        rgb.v = hsv.v;
        return rgb;
    }

    static Color ColorHSV(float h, float s, float v)
    {
        Color hsv;
        hsv.h = h;
        hsv.s = s;
        hsv.v = v;

        Color rgb = hsv2rgb(hsv);
        hsv.r = rgb.r;
        hsv.g = rgb.g;
        hsv.b = rgb.b;
        return hsv;
    }

    static Color ColorLerpRainbow(float min, float max, float val)
    {
        float minH = 240;
        float maxH = 0;
        float x = (val - min) / (max - min);
        if (x > 1.0f) {
            x = 1.0f;
        }
        else if (x < 0.0f) {
            x = 0.0f;
        }
        float h = x * (maxH - minH) + minH;
        return Color::ColorHSV(h, 1.0f, 1.0f);
    }

    float h, s, v;
    float r, g, b;

private:
    static Color rgb2hsv(Color in)
    {
        Color out;
        double      min, max, delta;

        min = in.r < in.g ? in.r : in.g;
        min = min < in.b ? min : in.b;

        max = in.r > in.g ? in.r : in.g;
        max = max > in.b ? max : in.b;

        out.v = max;                                // v
        delta = max - min;
        if (delta < 0.00001)
        {
            out.s = 0;
            out.h = 0; // undefined, maybe nan?
            return out;
        }
        if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
            out.s = (delta / max);                  // s
        }
        else {
            // if max is 0, then r = g = b = 0              
            // s = 0, h is undefined
            out.s = 0.0;
            out.h = NAN;                            // its now undefined
            return out;
        }
        if (in.r >= max)                           // > is bogus, just keeps compilor happy
            out.h = (in.g - in.b) / delta;        // between yellow & magenta
        else
            if (in.g >= max)
                out.h = 2.0 + (in.b - in.r) / delta;  // between cyan & yellow
            else
                out.h = 4.0 + (in.r - in.g) / delta;  // between magenta & cyan

        out.h *= 60.0;                              // degrees

        if (out.h < 0.0)
            out.h += 360.0;

        return out;
    }

    static Color hsv2rgb(Color in)
    {
        double      hh, p, q, t, ff;
        long        i;
        Color out(0, 0, 0);

        if (in.s <= 0.0) {       // < is bogus, just shuts up warnings
            out.r = in.v;
            out.g = in.v;
            out.b = in.v;
            return out;
        }
        hh = in.h;
        if (hh >= 360.0) hh = 0.0;
        hh /= 60.0;
        i = (long)hh;
        ff = hh - i;
        p = in.v * (1.0 - in.s);
        q = in.v * (1.0 - (in.s * ff));
        t = in.v * (1.0 - (in.s * (1.0 - ff)));

        switch (i) {
        case 0:
            out.r = in.v;
            out.g = t;
            out.b = p;
            break;
        case 1:
            out.r = q;
            out.g = in.v;
            out.b = p;
            break;
        case 2:
            out.r = p;
            out.g = in.v;
            out.b = t;
            break;

        case 3:
            out.r = p;
            out.g = q;
            out.b = in.v;
            break;
        case 4:
            out.r = t;
            out.g = p;
            out.b = in.v;
            break;
        case 5:
        default:
            out.r = in.v;
            out.g = p;
            out.b = q;
            break;
        }
        return out;
    }

    float interpolate(float min, float max, float val)
    {
        float x = (val - min) / (max - min);
        if (x > 1.0f) {
            x = 1.0f;
        }
        else if (x < 0.0f) {
            x = 0.0f;
        }
        return x;
    }
};

#endif