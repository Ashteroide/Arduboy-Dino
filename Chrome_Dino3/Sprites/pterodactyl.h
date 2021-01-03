// Pterodactyl 23x17px

#pragma once
#include <stdint.h>

constexpr uint8_t pteroWidth = 23;
constexpr uint8_t pteroHeight = 17;

const unsigned char pteroImg[] PROGMEM = {
    // Dimensions
    pteroWidth, pteroHeight,

    // Frame 0 - PteroUp
    0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xf8, 0xe0, 0x87, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 
    0x07, 0x0f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1e, 0x1e, 0x0e, 0x0a, 0x0a, 0x02, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00,

    // Frame 1 - PteroMiddle
    0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xf8, 0xe0, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 
    0x07, 0x0f, 0x1f, 0x1f, 0x1f, 0x1e, 0x1e, 0x1e, 0x1e, 0x0e, 0x0a, 0x0a, 0x02, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00,

    // Frame 2 - PteroDown
    0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xf8, 0xe0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 
    0xff, 0xff, 0x1f, 0x0f, 0x0f, 0x0f, 0x0e, 0x0e, 0x06, 0x06, 0x06, 0x02, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00
};