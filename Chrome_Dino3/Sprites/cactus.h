// cactus 10x20

#pragma once
#include <stdint.h>

constexpr int8_t cactusWidth = 10;
constexpr int8_t cactusHeight = 20;

const unsigned char cactusImg[] PROGMEM = {
  // Dimensions
  cactusWidth, cactusHeight,

  // Frame 0 - cactus
  0xc0, 0xe0, 0x00, 0xfe, 0xff, 0xff, 0xfe, 0x00, 0xf0, 0xe0, 0x0f, 0x1f, 0x18, 0xff, 0xff, 0xff, 
  0xff, 0x18, 0x0f, 0x07, 0x00, 0x00, 0x04, 0x07, 0x07, 0x07, 0x07, 0x04, 0x00, 0x00
};