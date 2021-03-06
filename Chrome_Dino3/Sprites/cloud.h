// cactus1 24x7

#pragma once
#include <stdint.h>

constexpr int8_t cloudWidth = 24;
constexpr int8_t cloudHeight = 7;

const unsigned char cloudImg[] PROGMEM = {
  // Dimensions
  cloudWidth, cloudHeight,

  // Frame 0 - cloud1
  0x60, 0x60, 0x70, 0x78, 0x78, 0x78, 0x78, 0x78, 0x7e, 0x7e, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 
  0x7f, 0x7c, 0x7c, 0x7c, 0x7c, 0x78, 0x78, 0x70
};