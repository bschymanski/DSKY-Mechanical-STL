#include <Adafruit_NeoPixel.h>
#include <neopixel.h>

// initialize the neoPixels
#define PIN            6
#define NUMPIXELS      14
Adafruit_NeoPixel neoPixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void NeoPixelSetup(void)
{
		neoPixels.setBrightness(150);
		neoPixels.begin();
}
// this is the function that sets the Lamps witch colors
void setLamp(int color, int lampNumber)
{
    /*  green                   = 1,
        white                   = 2,
        yellow                  = 3,
        orange                  = 4,
        blue                    = 5,
        red                     = 6,
        off                     = 7
    */
    switch (color)
    {
        case green:
            // Statement(s)
            neoPixels.setPixelColor(lampNumber, neoPixels.Color(0,100,0));
            neoPixels.show();   // show the updated pixel color on the hardware
            break;
        case white:
            neoPixels.setPixelColor(lampNumber, neoPixels.Color(100,100,80));
            neoPixels.show();   // show the updated pixel color on the hardware
            // Statement(s)
            break;
        case yellow:
            neoPixels.setPixelColor(lampNumber, neoPixels.Color(100,100,0));
            neoPixels.show();   // show the updated pixel color on the hardware
            // Statement(s)
            break;
        case orange:
            neoPixels.setPixelColor(lampNumber, neoPixels.Color(255,165,0));
            neoPixels.show();   // show the updated pixel color on the hardware
            // Statement(s)
            break;
        case blue:
            neoPixels.setPixelColor(lampNumber, neoPixels.Color(0,0,200));
            neoPixels.show();   // show the updated pixel color on the hardware
            // Statement(s)
            break;
        case red:
            neoPixels.setPixelColor(lampNumber, neoPixels.Color(100,0,0));
            neoPixels.show();   // show the updated pixel color on the hardware
            // Statement(s)
            break;
        case off:
            neoPixels.setPixelColor(lampNumber, neoPixels.Color(0,0,0));
            neoPixels.show();   // show the updated pixel color on the hardware
            // Statement(s)
            break;
        default:
            // Statement(s)
            break; // Wird nicht benötigt, wenn Statement(s) vorhanden sind
    }
}