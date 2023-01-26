#define DESKTOP

#ifdef DESKTOP
#include <iostream>
#include <string>
#else
#include "Adafruit_BMP3XX.h"
#include "SD.H"
#endif
#include "mock_altimeter.h"


int main(void)
{
    std::cout << "heelo world" << std::endl;
    MOCK_ALTIMETER altimeter = MOCK_ALTIMETER::getInstance();
    return 0;
}