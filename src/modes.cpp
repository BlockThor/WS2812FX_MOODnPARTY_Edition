/*
  WS2812FX_MnP_edition.h - Library upgraded and adapted for use with MOOD'n'PARTY LIGHTS : Single page WebInterface for ESP8266 and Arduino
  link:
  
  Vasyl Yudin
  Jan 2024
  www.ledvine.com
  
  modes.cpp - WS2812FX animation modes/effects

  LICENSE
  The MIT License (MIT)
  Copyright (c) 2024 Vasyl Yudin
  
  ------------------------------

  LICENSE

  The MIT License (MIT)

  Copyright (c) 2016  Harm Aldick

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include "WS2812FX_MnP_edition.h"

#define IS_RGBMODE (_seg->colors[0] == BG_COLOR)
#define IS_MONOMODE (_seg->colors[2] == BG_COLOR)

#define MAIN_COLOR	_seg->colors[0]
#define BG_COLOR	_seg->colors[1]
#define SECOND_COLOR	_seg->colors[2]

#define SETUP 	  if(CALL == 0)
#define EVERYSECONDCALL	if(CALL % 2)
#define CLEAR	fill(BG_COLOR, _seg->start, _seg_len)
// #define CLEAR	clearRawBG()

#define CALL _seg_rt->counter_mode_call
#define STEP _seg_rt->counter_mode_step
#define FIRST _seg->start
#define LAST _seg->stop
#define AUX1 _seg_rt->aux_param
#define AUX2 _seg_rt->aux_param2
#define AUX3 _seg_rt->aux_param3

#define HUE_START _seg_rt->aux_param3

// if (IS_RGBMODE) {
// } else if (IS_MONOMODE) {
// } else { // IS_DUOMODE
// }

/*
#####################################################
#
#  Mode Functions
#
##################################################### 
*/

// Plain static light.   Static Mood
uint16_t WS2812FX::mode_static(void) {
  SETUP HUE_START = random8(); 
  
  uint8_t size = 1 << SIZE_OPTION;
  float colorIndexIncr =  256 / (_seg_len) * size;
  
  if (IS_RGBMODE) { // RGB COLOR MODE
	for(uint16_t i=FIRST; i <= LAST; i++) {
	  setPixelColor(i, IS_REVERSE? color_wheel(HUE_START+colorIndexIncr*(_seg_len-1-i)) : color_wheel(HUE_START+colorIndexIncr*i));
	}
  } else if (IS_MONOMODE) { // MONO COLOR MODE
    fill(MAIN_COLOR, FIRST, _seg_len);
  } else { // DUO COLOR MODE  uint8_t colorIndexIncr =  256 / _seg_len;
	for(uint16_t i=FIRST; i <= LAST; i++) {
	  setPixelColor(i, color_blend(MAIN_COLOR, SECOND_COLOR, IS_REVERSE? (colorIndexIncr*(_seg_len-1-i)) : (colorIndexIncr*i) ));
	}
  }
  SET_CYCLE;
  return _seg->speed/4;
}

// Normal blinking. 50% on/off time.
uint16_t WS2812FX::mode_mood_click(void) { // ex mode_blink
  uint16_t speed;
  if (IS_RGBMODE) { // RGB COLOR MODE
	// speed = blink(color_wheel((CALL << 2) & 0xFF), color_wheel(0xFF - (CALL << 2) & 0xFF), false);
	speed = blink(color_wheel((CALL << 2) & 0xFF), color_wheel((0x80+(CALL << 2)) & 0xFF), false);
  } else if (IS_MONOMODE) { // MONO COLOR MODE
	speed = blink(MAIN_COLOR, BG_COLOR, false);
  } else { // DUO COLOR MODE
	speed = blink(MAIN_COLOR, SECOND_COLOR, false);
  }
  return speed*2;
}

// Does the "standby-breathing" of well known i-Devices. 
uint16_t WS2812FX::mode_breath(void) {
  float step = 0.01 * STEP;
  int lum = sin(step)*144+136;
  // int lum = sin(step)*160+144;
  uint32_t color1, color2;

  lum = min(255, max(0, lum));

  if (IS_RGBMODE) {
	color1 = color_wheel((CALL>>4) & 0xFF);
	color2 = BG_COLOR;
	// color2 = color_blend(color1, BG_COLOR, 160);
  } else if (IS_MONOMODE) {
	color1 = MAIN_COLOR;
	color2 = color_blend(color1, BG_COLOR, 240);
  } else { // IS_DUOMODE
	color1 = MAIN_COLOR;
	color2 = SECOND_COLOR;
  }
  uint32_t color =  color_blend(color1, color2, lum);
  fill(color, FIRST, _seg_len);

  STEP ++;
  if(STEP > 6283) {
    STEP = 0;
    SET_CYCLE;
  }
  return (_seg->speed / 80);
}

// Turns all LEDs after each other to a random color. Then starts over with another color.
uint16_t WS2812FX::mode_mood_train(void) {
  uint32_t color = color_wheel(AUX1);
  if(STEP < _seg_len) {
    
    if(IS_REVERSE) {
      setPixelColor(LAST - STEP, color);
    } else {
      setPixelColor(FIRST + STEP, color);
    }
    STEP = (STEP + 1);
    return (_seg->speed / (_seg_len * 2));
  } else {
    AUX1 = get_random_wheel_index(AUX1);
    STEP = 0;
    SET_CYCLE;
    return (_seg->speed * 2);
  }
}

// Lights every LED in a random color. Changes one random LED after the other to another random color.
uint16_t WS2812FX::mode_xmas_glance(void) { // ex mode_single_dynamic
  uint8_t size = 1 << SIZE_OPTION;
  SETUP {
    for(uint16_t i=FIRST; i <= LAST; i+=size) {
      fill(color_wheel(random8()), i, size);
    }
  }
  fill(color_wheel(random8()), FIRST + random16(_seg_len/size)*size, size);
  SET_CYCLE;
  return (_seg->speed / 16 * size) ;// slow down with bigger size
}

// Lights every LED in a random color. Changes all LED at the same time to new random colors.
uint16_t WS2812FX::mode_xmas_tick(void) { // ex mode_multi_dynamic
  uint8_t size = 1 << SIZE_OPTION;
  for(uint16_t i=FIRST; i <= LAST; i+=size) {
    fill(color_wheel(random8()), i, size);
  }
  SET_CYCLE;
  return (_seg->speed);
}

// Cycles all LEDs at once through a rainbow.
uint16_t WS2812FX::mode_mood_shift(void) { // ex mode_rainbow
  SETUP HUE_START = random16();
  
  uint32_t color = color_wheel16(STEP + HUE_START);
  fill(color, FIRST, _seg_len);

  STEP = (STEP + 0x08) & 0xFFFF;

  if(STEP == 0)  SET_CYCLE;

  return (_seg->speed / 32);
}

// Cycles a rainbow over the entire string of LEDs. -- Mood Flow
uint16_t WS2812FX::mode_mood_flow(void) { // ex mode_rainbow_cycle
  SETUP HUE_START = random16();
  uint8_t size = 1 << SIZE_OPTION;
  uint16_t hueInc =  0x7FFF / _seg_len / size;
	for(uint16_t i=FIRST; i <= LAST; i++) {
	  setPixelColor(i, IS_REVERSE? color_wheel16(STEP+HUE_START+hueInc*(_seg_len-1-i)) : color_wheel16(STEP+HUE_START+hueInc*i));
	}

  STEP += 16;
  if(STEP == 0) {
    SET_CYCLE;
  }
  return (_seg->speed / 32);
}

// Runs a block of pixels back and forth.
uint16_t WS2812FX::mode_scan(void) {
  return scan(MAIN_COLOR, SECOND_COLOR, false);
}

// Runs two blocks of pixels back and forth in opposite directions.
uint16_t WS2812FX::mode_dual_scan(void) {
  return scan(MAIN_COLOR, SECOND_COLOR, true);
}

// Fades the LEDs between two colors
uint16_t WS2812FX::mode_mood_swing(void) { // ex mode_fade
  int lum = Adafruit_NeoPixel::sine8(STEP);
  // if(lum > 255) lum = 511 - lum; // lum = 0 -> 255 -> 0

  uint32_t color;
  if (IS_RGBMODE) {
   color = color_blend( BG_COLOR, color_wheel(AUX1), lum);
   if (lum == 0) AUX1 = random8();
  } else if (IS_MONOMODE) {
   color = color_blend(BG_COLOR, MAIN_COLOR, lum);
  } else { // IS_DUOMODE
   color = color_blend(SECOND_COLOR, MAIN_COLOR, lum);
  }
  fill(color, FIRST, _seg_len);

  STEP ++;
  if(STEP >= 255) {
    STEP = 0;
    SET_CYCLE;
  }
  return (_seg->speed / 128);
}

// Theatre-style crawling lights. Inspired by the Adafruit examples.
uint16_t WS2812FX::mode_theater_chase(void) {
  if (IS_RGBMODE) {
  AUX1 = (AUX1 + 1) & 0xFF;
  return tricolor_chase(color_wheel(AUX1), BG_COLOR, BG_COLOR);
  } else if (IS_MONOMODE) {
    return tricolor_chase(MAIN_COLOR, BG_COLOR, BG_COLOR);
  } else { // IS_DUOMODE
    return tricolor_chase(MAIN_COLOR, SECOND_COLOR, BG_COLOR);
  }
}

// Running lights effect with smooth sine transition.
uint16_t WS2812FX::mode_waves(void) {
	uint32_t color;
  uint8_t size = 1 << SIZE_OPTION;
  uint8_t sineIncr = max(1, (256 / _seg_len) / size * 4);
  for(uint16_t i=0; i < _seg_len; i++) {
    int lum = (int)sine8(((i + STEP) * sineIncr));
    int bri = (int)sine8(((i + STEP+64) * sineIncr));
    if (IS_RGBMODE) {
      color = color_blend(color_wheel(STEP), color_wheel(STEP+42), lum);
    } else if (IS_MONOMODE) {
      color = color_blend(MAIN_COLOR, BG_COLOR, lum);
    } else { // IS_DUOMODE
      color = color_blend(MAIN_COLOR, SECOND_COLOR, lum);
    }
    if(IS_REVERSE) {
      setPixelColor(FIRST + i, color);
    } else {
      setPixelColor(LAST - i,  color);
    }
  }
  STEP = (STEP + 1) % 256;
  if(STEP == 0) SET_CYCLE;
  return (_seg->speed / _seg_len * 4);
}

// Blink several LEDs on, reset, repeat. Inspired by 
//www.tweaking4all.com/hardware/arduino/arduino-led-strip-effects/
uint16_t WS2812FX::mode_twinkle(void) {
  return twinkle(MAIN_COLOR, BG_COLOR);
}

// Blink several LEDs in random colors on, reset, repeat.Inspired by //www.tweaking4all.com/hardware/arduino/arduino-led-strip-effects/
uint16_t WS2812FX::mode_twinkle_random(void) {
  return twinkle(color_wheel(random8()), BG_COLOR);
}

// Blink several LEDs on, fading out.
uint16_t WS2812FX::mode_twinkle_fade(void) {
  uint32_t color;
  SETUP CLEAR;
  
  fade_out(BG_COLOR, 18 * FADE_RATE + 128);
  
  if (IS_RGBMODE) {
    color = color_wheel(random8());
  } else if (IS_MONOMODE) {
    color = MAIN_COLOR;
  } else { // IS_DUOMODE
    EVERYSECONDCALL {
      color = MAIN_COLOR;
    } else {
      color = SECOND_COLOR;
    }
  }

  if(random8(240/_seg_len) == 0) {
    uint8_t size = 1 << SIZE_OPTION;
    uint16_t index = FIRST + random16(_seg_len/size + 1)*size;
    fill(color, index, size);
    SET_CYCLE;
  }
  return (_seg->speed / 16);
}

// Blinks one LED at a time.Inspired by 
//www.tweaking4all.com/hardware/arduino/arduino-led-strip-effects/
uint16_t WS2812FX::mode_sparkle(void) {
  if (IS_RGBMODE) {
    return sparkle(color_wheel(random8()), color_wheel(random8()));
  } else if (IS_MONOMODE) {
    return sparkle(MAIN_COLOR, MAIN_COLOR);
  } else { // IS_DUOMODE
    return sparkle(SECOND_COLOR, MAIN_COLOR);
  }
}

// Classic Strobe effect.
uint16_t WS2812FX::mode_strobe(void) {
  // return blink(MAIN_COLOR, BG_COLOR, true);
    uint32_t color;
  if (IS_RGBMODE) {
    color = color_wheel((CALL << 2) & 0xFF);
  } else if (IS_MONOMODE) {
    color = MAIN_COLOR;
  } else { // IS_DUOMODE
    color = (CALL % 0x04 == 0) ? MAIN_COLOR : SECOND_COLOR; // off
  }
 
  if(CALL & 0x01) {
    fill(BG_COLOR, FIRST, _seg_len);
    SET_CYCLE;
    return _seg->speed - 50;
  } else {
      fill(color, FIRST, _seg_len);
    return 50;
  }
}

// Classic Strobe effect. Cycling through the rainbow.
uint16_t WS2812FX::mode_strobe_rainbow(void) {
  return blink(color_wheel((CALL << 2) & 0xFF), BG_COLOR, true);
}

// Strobe effect with different strobe count and pause, controlled by speed.
uint16_t WS2812FX::mode_multi_strobe(void) {
  // fill(BG_COLOR, FIRST, _seg_len);
  CLEAR;

  uint16_t delay = 200 + ((9 - (_seg->speed % 10)) * 100);
  uint16_t count = 2 * ((_seg->speed / 100) + 1);
  if(STEP < count) {
    if((STEP & 1) == 0) {


      // fill(MAIN_COLOR, FIRST, _seg_len);
      fill(color_wheel((CALL << 2) & 0xFF), FIRST, _seg_len);
      delay = 20;
    } else {
      delay = 50;
    }
  }

  STEP = (STEP + 1) % (count + 1);
  if(STEP == 0) SET_CYCLE;
  return delay;
}

// Classic Blink effect. Cycling through the rainbow.
uint16_t WS2812FX::mode_blink_rainbow(void) {
  return blink(color_wheel((CALL << 2) & 0xFF), BG_COLOR, false)*2;
}

// Main color running on white.
uint16_t WS2812FX::mode_chase_white(void) {
  return chase(WHITE, MAIN_COLOR, MAIN_COLOR);
}

// White running on _color.
uint16_t WS2812FX::mode_chase_color(void) {
  return chase(MAIN_COLOR, WHITE, WHITE);
}

// White running followed by random color.
uint16_t WS2812FX::mode_chase_random(void) {
  if(STEP == 0) {
    AUX1 = get_random_wheel_index(AUX1);
  }
  return chase(color_wheel(AUX1), WHITE, WHITE);
}

// White running on rainbow.
uint16_t WS2812FX::mode_chase_rainbow(void) {
  uint8_t color_sep = 256 / _seg_len;
  uint8_t color_index = CALL & 0xFF;
  uint32_t color = color_wheel(((STEP * color_sep) + color_index) & 0xFF);

  return chase(color, WHITE, WHITE);
}

// White flashes running on _color.
uint16_t WS2812FX::mode_chase_flash(void) {
  return chase_flash(MAIN_COLOR, WHITE);
}

// White flashes running, followed by random color.
uint16_t WS2812FX::mode_chase_flash_random(void) {
  return chase_flash(color_wheel(AUX1), WHITE);
}

// Rainbow running on white.
uint16_t WS2812FX::mode_chase_rainbow_white(void) {
  uint16_t n = STEP;
  uint16_t m = (STEP + 1) % _seg_len;
  uint32_t color2 = color_wheel(((n * 256 / _seg_len) + (CALL & 0xFF)) & 0xFF);
  uint32_t color3 = color_wheel(((m * 256 / _seg_len) + (CALL & 0xFF)) & 0xFF);

  return chase(WHITE, color2, color3);
}


// Black running on _color.
uint16_t WS2812FX::mode_chase_blackout(void) {
  return chase(MAIN_COLOR, BLACK, BLACK);
}

// Black running on rainbow.
uint16_t WS2812FX::mode_chase_blackout_rainbow(void) {
  uint8_t color_sep = 256 / _seg_len;
  uint8_t color_index = CALL & 0xFF;
  uint32_t color = color_wheel(((STEP * color_sep) + color_index) & 0xFF);

  return chase(color, BLACK, BLACK);
}

// Random color introduced alternating from start and end of strip.
uint16_t WS2812FX::mode_color_sweep_random(void) {
  if(STEP % _seg_len == 0) { // aux_param will store our random color wheel index
    AUX1 = get_random_wheel_index(AUX1);
  }
  uint32_t color = color_wheel(AUX1);
  return color_wipe(color, color, true) * 2;
}

// Alternating color/white pixels running.
uint16_t WS2812FX::mode_running_color(void) {
  if (IS_RGBMODE) {
  uint8_t size = 2 << SIZE_OPTION;
  if((STEP) % size == 0) {
    AUX1 = get_random_wheel_index(AUX1);
  }

  uint32_t color = color_wheel(AUX1);

  return running(color, color);
  } else if (IS_MONOMODE) {
    return running(MAIN_COLOR, BG_COLOR);

  } else { // IS_DUOMODE
    return running(MAIN_COLOR, SECOND_COLOR);
  }
}

// Alternating red/blue pixels running.
uint16_t WS2812FX::mode_running_red_blue(void) {
  return running(RED, BLUE);
}

// Random colored pixels running.
uint16_t WS2812FX::mode_running_random(void) {
  uint8_t size = 2 << SIZE_OPTION;
  if((STEP) % size == 0) {
    AUX1 = get_random_wheel_index(AUX1);
  }

  uint32_t color = color_wheel(AUX1);

  return running(color, color);
}

// K.I.T.T.
uint16_t WS2812FX::mode_larson_scanner(void) {
  fade_out(BG_COLOR, 36 * FADE_RATE);
  
  uint8_t colorIndexIncr =  256 / _seg_len;

  if (STEP < _seg_len) {
    if (IS_RGBMODE) { // RGB COLOR MODE
      setPixelColor(LAST - STEP, color_wheel(colorIndexIncr * (LAST - STEP)));
    } else { // MONO and DUO COLOR MODE
      setPixelColor(LAST - STEP, MAIN_COLOR);
    }
  } else {
    uint16_t index = (_seg_len * 2) - STEP - 2;
    if (IS_RGBMODE) { // RGB COLOR MODE
      setPixelColor(LAST - index, color_wheel(colorIndexIncr * (LAST - STEP)));
    } else if (IS_MONOMODE) { // MONO COLOR MODE
      setPixelColor(LAST - index, MAIN_COLOR);
    } else { // DUO COLOR MODE
      setPixelColor(LAST - index, SECOND_COLOR);
    }
  }

  STEP++;
  if (STEP >= (uint16_t)((_seg_len * 2) - 2)) {
    STEP = 0;
    SET_CYCLE;
  }

  return (_seg->speed / _seg_len);
}

// Firing comets from one end.
uint16_t WS2812FX::mode_comet(void) {

  fade_out(BG_COLOR, 36 * FADE_RATE);

  if(IS_REVERSE) {
    setPixelColor(LAST - STEP, MAIN_COLOR);
  } else {
    setPixelColor(FIRST + STEP, MAIN_COLOR);
  }

  STEP = (STEP + 1) % _seg_len;
  if(STEP == 0) SET_CYCLE;

  return (_seg->speed / _seg_len);
}

// Firework sparks.
uint16_t WS2812FX::mode_fireworks(void) {
  uint32_t color = BLACK;

  if (IS_RGBMODE) {
    color = color_wheel(random8());
  } else if (IS_MONOMODE) {
    color = MAIN_COLOR;
  } else { // IS_DUOMODE
    do { // randomly choose a non-BLACK color from the colors array
      color = _seg->colors[random8(MAX_NUM_COLORS)];
    } while (color == BLACK);
  }
  return fireworks(color);
}

// Alternating red/green pixels running.
uint16_t WS2812FX::mode_merry_christmas(void) {
  return running(RED, GREEN);
}

// Alternating white/red/black pixels running.
uint16_t WS2812FX::mode_circus_combustus(void) {
  return tricolor_chase(RED, WHITE, BLACK);
}

// Alternating orange/purple pixels running.
uint16_t WS2812FX::mode_halloween(void) {
  return running(PURPLE, ORANGE);
}

// Bicolor chase mode
uint16_t WS2812FX::mode_bicolor_chase(void) {
  return chase(MAIN_COLOR, BG_COLOR, SECOND_COLOR);
}

// Tricolor chase mode
uint16_t WS2812FX::mode_tricolor_chase(void) {
  return tricolor_chase(MAIN_COLOR, BG_COLOR, SECOND_COLOR);
}

/* An adaptation of Mark Kriegsman's FastLED twinkeFOX effect
*  https://gist.github.com/kriegsman/756ea6dcae8e30845b5a
*  Upgrade by Vasyl Yudin: increase time for LED off & LED max
*/
uint16_t WS2812FX::mode_xmas_twinkle(void) { // ex mode_twinkleFOX
  uint16_t mySeed = 0; // reset the random number generator seed

  // Get and translate the segment's size option
  uint8_t size = 1 << SIZE_OPTION; // 1,2,4,8

  uint32_t blendedColor;

  for (uint16_t i = FIRST; i <= LAST; i+=size) {
    // Use Mark Kriegsman's clever idea of using pseudo-random numbers to determine
    // each LED's initial and increment blend values
    mySeed = (mySeed * 2053) + 13850; // a random, but deterministic, number
    uint16_t initValue = (mySeed + (mySeed >> 8)) & 0xff; // the LED's initial blend index (0-255)
    mySeed = (mySeed * 2053) + 13850; // another random, but deterministic, number
    uint16_t incrValue = (((mySeed + (mySeed >> 8)) & 0x03) *2) + 1; // blend index increment (1,3,5,7)

    // We're going to use a sine function to blend colors, instead of Mark's triangle
    // function, simply because a sine lookup table is already built into the
    // Adafruit_NeoPixel lib. Yes, I'm lazy.
    // Use the counter_mode_call var as a clock "tick" counter and calc the blend index
    uint8_t blendIndex = (initValue + (CALL * incrValue)) & 0xff; // 0-255
    // Index into the built-in Adafruit_NeoPixel sine table to lookup the blend amount
    uint16_t bigBlendAmt = Adafruit_NeoPixel::sine8(blendIndex)*2;
	  // Add some Magic: change blend amount to add more time for FullOff and for FullOn
    uint8_t blendAmt = (bigBlendAmt > 383 ? 255 : (bigBlendAmt<128 ? 0 : bigBlendAmt-128));
    // Modify Color according to mode
    if(IS_RGBMODE) {
      blendedColor = color_blend(color_wheel(initValue), BG_COLOR, blendAmt);
    } else if(!(IS_MONOMODE) && (initValue < 128)==0) {
      blendedColor = color_blend(SECOND_COLOR, BG_COLOR, blendAmt);
    } else {
      blendedColor = color_blend(MAIN_COLOR, BG_COLOR, blendAmt);
    }

    // Assign the new color to the number of LEDs specified by the SIZE option
    fill(blendedColor, i, size);
  }
  SET_CYCLE;
  return _seg->speed / 16;
}

/* An adaptation of Mark Kriegsman's FastLED twinkeFOX effect
*  https://gist.github.com/kriegsman/756ea6dcae8e30845b5a
*  Upgrade by Vasyl Yudin: change bri to hue
*/ 
uint16_t WS2812FX::mode_xmas_glitter(void) {
  SETUP HUE_START = random16();
  uint16_t mySeed = 0; // reset the random number generator seed

  // Get and translate the segment's size option
  uint8_t size = 1 << SIZE_OPTION; // 1,2,4,8

  for (uint16_t i = FIRST; i <= LAST; i+=size) {
    mySeed = (mySeed * 2053) + HUE_START; // a random, but deterministic, number
    uint8_t initValue = (mySeed + (mySeed >> 8)) & 0xff; // the LED's initial blend index (0-255)
    mySeed = (mySeed * 2053) + 13850; // another random, but deterministic, number
    uint8_t incrValue = ((mySeed + (mySeed >> 8)) & 0x03) + 1; // blend index increment (1,3,5,7)

    uint8_t blendIndex = (initValue + (CALL * incrValue)) & 0xff; // 0-255
    // Index into the built-in Adafruit_NeoPixel sine table to lookup the blend amount
    uint8_t blendAmt = Adafruit_NeoPixel::sine8(blendIndex)/2;

    fill(color_wheel(initValue+blendAmt), i, size);
  }
  SET_CYCLE;
  return _seg->speed / 8;
}

// A combination of the Fireworks effect and the running effect to create an effect that looks like rain.
uint16_t WS2812FX::mode_rain(void) {
  // randomly choose colors[0] or colors[2]
  uint32_t rainColor;
  if (IS_RGBMODE) {
    rainColor = color_wheel(random8());
  } else if (IS_MONOMODE) {
    rainColor = MAIN_COLOR;
  } else { // IS_DUOMODE
    rainColor = (random8() & 1) == 0 ? MAIN_COLOR : SECOND_COLOR;
  }
  // run the fireworks effect to create a "raindrop"
  fireworks(rainColor);
  // shift everything two pixels
  if(IS_REVERSE) {
    copyPixels(FIRST + 2, FIRST, _seg_len - 2);
  } else {
    copyPixels(FIRST, FIRST + 2, _seg_len - 2);
  }
  return (_seg->speed / 16);
}

// Block dissolve effect
uint16_t WS2812FX::mode_dissolve(void) { //ex mode_block_dissolve
  uint32_t color;
  if (IS_RGBMODE) {// get the target color
    color = color_wheel(AUX1);
  } else {
    color = _seg->colors[AUX1];
  }
  
  // get the decimated color after setPixelColor() has mangled it
  // in accordance to the brightness setting
  setPixelColor(FIRST, color);
  uint32_t desColor = getPixelColor(FIRST);
  
  // find a random pixel that isn't the target color and update it
  for (uint16_t i = 0; i < _seg_len; i++) {
    int index = FIRST + random16(_seg_len);
    if (getPixelColor(index) != desColor) {
      setPixelColor(index, color);
      return _seg->speed / 8;
    }
  }

  // if didn't find a random pixel that wasn't the target color,
  // then set the entire segment to the target color
  fill(color, FIRST, _seg_len);

  // choose a new target color
  if (IS_RGBMODE) {
    AUX1 = get_random_wheel_index(AUX1);
  } else if (IS_MONOMODE) {
    AUX1 = (AUX1 + 1) % 2;
  } else { // IS_DUOMODE
    AUX1 = (AUX1 + 2) % MAX_NUM_COLORS;
  }

  if (AUX1 == 0) SET_CYCLE;
  return _seg->speed / 16;
}

// ICU effect
uint16_t WS2812FX::mode_icu(void) {
  uint8_t size = 1 << SIZE_OPTION;
  uint16_t pos = STEP; // current eye position
  uint16_t dest = AUX3;       // eye destination
  uint16_t index = FIRST + pos;        // index of the first eye
  uint16_t index2 = index + _seg_len/2;      // index of the second eye

  CLEAR;

  // if the eyes have not reached their destination
  if(pos != dest) {
    // move the eyes right or left depending on position relative to destination
    int dir = dest > pos ? 1 : -1;
	fill(MAIN_COLOR, index + dir, size);
	fill(MAIN_COLOR, index2 + dir, size);
    STEP += dir; // update the eye position
    return (_seg->speed / _seg_len);
  } else { // the eyes have reached their destination
    if(random8(6) == 0) {  // blink the eyes once in a while
      AUX3 = random16(_seg_len/2-size); // and set a new destination
      SET_CYCLE;
      return _seg->speed/4;
    } else {
		fill(MAIN_COLOR, index, size);
		fill(MAIN_COLOR, index2, size);
		AUX3 = max(0, min(_seg_len-size, AUX3 + random8(3)-1)); // move to short
		return _seg->speed/4 + random16(_seg->speed/4); // short pause
    }
  }
}

// Dual Larson effect
uint16_t WS2812FX::mode_dual_larson(void) {
  uint32_t color1, color2;
  uint8_t colorIndexIncr =  256 / _seg_len;
	if (IS_RGBMODE) {
		color1 = color_wheel(colorIndexIncr * (IS_REVERSE? (FIRST + STEP):(LAST  - STEP)));
		color2 = color_wheel(colorIndexIncr * (IS_REVERSE? (LAST  - STEP):(FIRST + STEP)));
	} else if (IS_MONOMODE) {
		color1 = MAIN_COLOR;
		color2 = MAIN_COLOR;
	} else { // IS_DUOMODE
		color1 = MAIN_COLOR;
		color2 = SECOND_COLOR;
	}
  
  fade_out(BG_COLOR, 36 * FADE_RATE);

  STEP += AUX1 ? -1 : 1; // update the LED index

  // setPixelColor(FIRST + AUX3, MAIN_COLOR);
  // setPixelColor(LAST  - AUX3, SECOND_COLOR ? SECOND_COLOR : MAIN_COLOR);
  setPixelColor(FIRST + STEP, color1);
  setPixelColor(LAST  - STEP, color2);

  if(STEP == 0 || STEP >= _seg_len - 1) {
    AUX1 = !AUX1; // change direction
    SET_CYCLE;
  }

  return (_seg->speed / _seg_len);
}

// Running random2 effect (simplified version of the custom RandomChase effect)
uint16_t WS2812FX::mode_running_tails(void) {
  uint8_t size = 8 << SIZE_OPTION; // 8, 16, 32, 64	
  uint32_t color = IS_REVERSE ? getPixelColor(LAST): getPixelColor(FIRST);
  // dim the color
  color = color_blend(color, BLACK, 0x300/size);
  // periodically change the color
  if((STEP) % size == 0) {
    // color = ((uint32_t)random8() << 16) | random16();
    color = colorHSV(random16(), random8(0x50)+0xB0);
    // color = Adafruit_NeoPixel::Color(random8(),random8(),random8());
  }

  return running(color, color);
}

// simplied version of the custom filler up mode
uint16_t WS2812FX::mode_filler_up(void) {
  uint8_t size = 1 << SIZE_OPTION;

  if(AUX3 >= _seg_len) { // if glass is full, reset
    AUX3 = 0; // empty the glass
    AUX1 = !AUX1; // swap fg and bg colors
    SET_CYCLE;
  }

  uint32_t fgColor = AUX1 ? MAIN_COLOR : BG_COLOR;
  uint32_t bgColor = AUX1 ? BG_COLOR : MAIN_COLOR;

  if(IS_REVERSE) {
    fill(bgColor, FIRST, _seg_len); // fill with bg color
    fill(fgColor, LAST - STEP, size); // drop
    if(AUX3) fill(fgColor, FIRST, AUX3);
  } else {
    fill(bgColor, FIRST, _seg_len); // fill with bg color
    fill(fgColor, FIRST + STEP, size); // drop
    if(AUX3) fill(fgColor, FIRST + _seg_len - AUX3, AUX3);
  }

  STEP++; // move the drop

  // when drop reaches the fill line, incr the fill line
  if(STEP >= _seg_len - AUX3) {
    AUX3++;
    STEP = 0;
  }

  return (_seg->speed / _seg_len);
}

// Rainbow Larson effect
uint16_t WS2812FX::mode_rainbow_larson(void) {
  
  fade_out(BG_COLOR, 36 * FADE_RATE);

  AUX3 += AUX1 ? -1 : 1; // update the LED index

  if(IS_REVERSE) {
    setPixelColor(LAST - AUX3, color_wheel(CALL << 4));
    //setPixelColor(LAST - AUX3, color_wheel((AUX3 << 8) / _seg_len));
  } else {
    setPixelColor(FIRST + AUX3, color_wheel(CALL << 4));
    //setPixelColor(FIRST + AUX3, color_wheel((AUX3 << 8) / _seg_len));
  }

  if(AUX3 == 0 || AUX3 >= _seg_len - 1) {
    AUX1 = !AUX1; // change direction
    SET_CYCLE;
  }

  return (_seg->speed / (_seg_len));
}

// Firework with rainbow colors
uint16_t WS2812FX::mode_rainbow_fireworks(void) {
  for(uint16_t i=FIRST; i <= LAST; i++) {
    uint32_t color = getRawPixelColor(i); // get the raw pixel color (ignore global brightness)
    color = (color >> 1) & 0x7F7F7F7F;    // fade all pixels

    setRawPixelColor(i, color);

    // search for the fading red pixels, and create the appropriate neighboring pixels
    if(color == 0x7F0000) {
      setPixelColor(i-1, 0xFF7F00); // orange
      setPixelColor(i+1, 0xFF7F00);
    } else if(color == 0x3F0000) {
      setPixelColor(i-2, 0xFFFF00); // yellow
      setPixelColor(i+2, 0xFFFF00);
    } else if(color == 0x1F0000) {
      setPixelColor(i-3, 0x00FF00); // green
      setPixelColor(i+3, 0x00FF00);
    } else if(color == 0x0F0000) {
      setPixelColor(i-4, 0x0000FF); // blue
      setPixelColor(i+4, 0x0000FF);
    } else if(color == 0x070000) {
      setPixelColor(i-5, 0x4B0082); // indigo
      setPixelColor(i+5, 0x4B0082);
    } else if(color == 0x030000) {
      setPixelColor(i-6, 0x9400D3); // violet
      setPixelColor(i+6, 0x9400D3);
    }
  }

  // occasionally create a random red pixel
  if(random8(600/_seg_len) == 0) {
    uint16_t index = FIRST + 6 + random16(max(1, _seg_len - 12));
    setRawPixelColor(index, RED); // set the raw pixel color (ignore global brightness)
    SET_CYCLE;
  }
  return(_seg->speed / 32);
}

// fade between 3 colors
uint16_t WS2812FX::mode_trifade(void) {
  uint32_t colorsMain[] = { MAIN_COLOR, BG_COLOR, SECOND_COLOR };
  uint32_t colorsAlt[]  = { MAIN_COLOR, BLACK, BG_COLOR, BLACK, SECOND_COLOR, BLACK };

  uint32_t* colors = colorsMain;
  uint8_t numColors = sizeof(colorsMain) / sizeof(uint32_t);
  if(IS_REVERSE) { // if reverse bit is set, fade to black between colors
      colors = colorsAlt;
      numColors = sizeof(colorsAlt) / sizeof(uint32_t);
  }

  uint32_t color1 = colors[AUX1];
  uint32_t color2 = colors[(AUX1 + 1) % numColors];

  uint32_t color = color_blend(color1, color2, AUX3);
  fill(color, FIRST, _seg_len);

  AUX3 = (AUX3 + 4) % 256;
  if(AUX3 == 0) {
    AUX1 = (AUX1 + 1) % numColors;
    SET_CYCLE;
  }

  return (_seg->speed / 128);
}

//  VU Meter
uint16_t WS2812FX::mode_vu_meter(void) {
  uint32_t color = color_wheel(STEP);
  static uint8_t randomData[1]; // default: one channel of random data
  uint8_t size = 0x01 << SIZE_OPTION; // 1,2,4,8

  // if external data source not set, config for one channel of random data
  uint8_t* src = _seg_rt->extDataSrc != NULL ? _seg_rt->extDataSrc : randomData;
  uint16_t cnt = _seg_rt->extDataCnt != 0    ? _seg_rt->extDataCnt : 1;

  if (src == randomData) { // if using random data, generate some
    for (uint8_t i = 0; i < cnt; i++) {
      int randomData = src[i] + random8(32) - random8(32);
      src[i] = (randomData < 0 || randomData > 255) ? 128 : randomData;
    }
  }

  uint16_t channelSize = _seg_len / cnt; // num LEDs in each channel

  for (uint8_t i = 0; i < cnt; i++) { // for each channel
    uint8_t scaledLevel = (src[i] * channelSize) / 256;
    for (uint16_t j = 0; j < channelSize; j++) {
      uint16_t index = FIRST + (i * channelSize) + j;
      if (j <= scaledLevel) {
        if (IS_RGBMODE) { // Rainbow color
          // color = color_wheel(channelSize - scaledLevel);
          setPixelColor(index, color);
        } else if (j < channelSize - 3 * size ) setPixelColor(index, MAIN_COLOR); // green
        else if (j < channelSize - 2 * size)    setPixelColor(index, MAIN_COLOR); // yellow
        else                                    setPixelColor(index, RED); // red
      } else {
        setPixelColor(index, BG_COLOR);
      }
    }
  }
  SET_CYCLE;

  if (CALL % 0xFFFF) STEP = (STEP + 1) & 0xFF;
  return (_seg->speed / 64);
}

// create pulses that start in the middle of the segment and move toward it's edges
// time two pulses to mimic a heartbeat
uint16_t WS2812FX::mode_heartbeat(void) {
  static unsigned long then = 0;
  unsigned long now = millis();

  // Get and translate the segment's size option
  uint8_t size = 2 << SIZE_OPTION; // 2,4,8,16

  // copy pixels from the middle of the segment to the edges
  uint16_t bytesPerPixelBlock = size * getNumBytesPerPixel();
  uint16_t centerOffset = (_seg_len / 2) * getNumBytesPerPixel();
  uint16_t byteCount = centerOffset - bytesPerPixelBlock;
  memmove(getPixels(), getPixels() + bytesPerPixelBlock, byteCount);
  memmove(getPixels() + centerOffset + bytesPerPixelBlock, getPixels() + centerOffset, byteCount);

  fade_out(BG_COLOR, 36 * FADE_RATE);

  int beatTimer = now - then;
  if((beatTimer > 400) && !AUX1) { // time for the second beat? (400ms after the first beat)
    uint16_t startLed = FIRST + (_seg_len / 2) - size;
    fill(IS_MONOMODE? MAIN_COLOR : SECOND_COLOR, startLed, size * 2); // create the second beat

    AUX1 = true; // is second beat
  }
  if(beatTimer > 1200) { // time for the first beat? (1200ms)
    uint16_t startLed = FIRST + (_seg_len / 2) - size;
    fill(MAIN_COLOR, startLed, size * 2); // create the first beat

    AUX1 = false; // is first beat
    then = now; // reset the beat timer
    SET_CYCLE;
  }

  return(_seg->speed / 32);
}

// Random commets
uint16_t WS2812FX::mode_comets(void) { //ex mode_multi_comet
  static uint16_t comets[6];
  uint32_t color;
  // if external data source not set, config for two comets.
  // note: the multi_comet data array is data type uint16_t, but extDataSrc
  // is uint8_t, so be careful when you cast and count an external data source.
  // i.e. uint16_t cometData[4]; // four comets
  //      setExtDataSrc(0, (uint8_t*)cometData, sizeof(cometData) / sizeof(cometData[0]));
  uint16_t* src = _seg_rt->extDataSrc != NULL ? (uint16_t*)_seg_rt->extDataSrc : comets;
  uint16_t  cnt = _seg_rt->extDataCnt != 0    ? _seg_rt->extDataCnt            : 6;

  EVERYSECONDCALL fade_out(BG_COLOR, 36 * FADE_RATE);

  for (uint8_t i = 0; i < cnt; i++) {
    if (src[i] < _seg_len) { // if comet is active, move it one pixel

      if (IS_RGBMODE) { // RGB COLOR MODE
        color = color_wheel(i * 40 + 10);
      } else if (IS_MONOMODE) {
        color = MAIN_COLOR;
      } else { // IS_DUOMODE
        color = i % 2 ? SECOND_COLOR : MAIN_COLOR;
      }


      if (IS_REVERSE) {
        setPixelColor(FIRST + src[i], color);
      } else {
        setPixelColor(LAST - src[i],  color);
      }
      src[i]++;
    } else {
      if (random(_seg_len) == 0) {
        src[i] = 0; // randomly start a comet
        SET_CYCLE;
      }
    }
  }

  return (_seg->speed / _seg_len);
}

// Pop corn effect
uint16_t WS2812FX::mode_popcorn(void) {

  static Popcorn popcorn[5]; // allocate space for 5 kernels of popcorn
  static float coeff = 0.0f;
  // if external data source not set, config for five popcorn kernels
  Popcorn* src = _seg_rt->extDataSrc != NULL ? (Popcorn*)_seg_rt->extDataSrc : popcorn;
  uint16_t cnt = _seg_rt->extDataCnt != 0    ? _seg_rt->extDataCnt           : 5;

  SETUP {
    coeff = pow((float)_seg_len, 0.5223324f) * 0.3944296f; // calculate the velocity coeff once (the secret sauce)
  }

  fade_out(BG_COLOR, 36 * FADE_RATE);

  for (int8_t i = 0; i < cnt; i++) { // for each kernel
    if (src[i].position >= 0.0f) { // if kernel is active, update its position and slow it down
		src[i].position += src[i].velocity;
		src[i].velocity -= 0.0058f; // gravity = -0.025
		uint16_t ledIndex = IS_REVERSE ? LAST - src[i].position : FIRST + src[i].position;
		if (ledIndex >= FIRST && ledIndex <= LAST) setPixelColor(ledIndex, src[i].color);
    } else { // if kernel is inactive, randomly pop it (make it active)
      if (random8() < 2) { // POP!!!
        src[i].position = 0.0f;
        src[i].velocity = coeff * ((66 + random8(34)) / 400.0f); // initial fast velocity
        if (IS_RGBMODE) {
          src[i].color = color_wheel(random8());
          // src[i].color = colorHSV(random16(), random8(0xB0)+0x55);;
          // src[i].color = color_wheel(byte(CALL>>1));
        } else if (IS_MONOMODE) {
          src[i].color = MAIN_COLOR;
        } else { // IS_DUOMODE
          EVERYSECONDCALL src[i].color = MAIN_COLOR; else src[i].color = SECOND_COLOR;
        }
        SET_CYCLE;
      }
    }
  }
  return (_seg->speed / 128);
}

/*
 * Custom modes
 */
uint16_t WS2812FX::mode_custom_0() {
  return customModes[0]();
}
uint16_t WS2812FX::mode_custom_1() {
  return customModes[1]();
}
uint16_t WS2812FX::mode_custom_2() {
  return customModes[2]();
}
uint16_t WS2812FX::mode_custom_3() {
  return customModes[3]();
}
uint16_t WS2812FX::mode_custom_4() {
  return customModes[4]();
}
uint16_t WS2812FX::mode_custom_5() {
  return customModes[5]();
}
uint16_t WS2812FX::mode_custom_6() {
  return customModes[6]();
}
uint16_t WS2812FX::mode_custom_7() {
  return customModes[7]();
}


/*
 *  - = = = = N E W   M O D E S = = = = -
 */

// - = = F I R E 2012 = = -

// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 200-1000
#define COOLING  600
// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 100
uint16_t WS2812FX::mode_fire2012(void){
  // Array of temperature readings at each simulation cell
  static uint8_t flame_len;
  static uint8_t heat[0xFF];
  uint32_t color;

  SETUP { // initialize segment
    HUE_START = random16();
    if (_seg_len > 0xFF) flame_len = 0xFF; else flame_len = _seg_len;
    memset(heat, 0, flame_len);
  }

  // Step 1.  Cool down every heated cell a little
  for ( int i = 0; i < flame_len; i++) {
    if(heat[i]>0){
      int t = heat[i] - random16((COOLING / flame_len) + 2);
      heat[i] = t < 0 ? 0 : t;
    }
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = flame_len - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < SPARKING ) {
    int y = random8(flame_len / 10) + 1;
    unsigned int t = heat[y] + random8(160) + 95;
    heat[y] = t > 255 ? 255 : t;
    heat[0] = 160; // bottom always  hot
  }

  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < flame_len; j++) {
    if (IS_RGBMODE) {
      // color = colorHSV(CALL + HUE_START + heat[j] * 16, heat[j] > 127 ? 256 - (heat[j] - 127) : 255, heat[j] < 128 ? heat[j] * 2 : 255);
      color = colorHSV(CALL + HUE_START + 0xFFFF - heat[j] * 0x101, 255, heat[j] < 128 ? heat[j] * 2 : 255);
    } else if (IS_MONOMODE) {
      uint8_t hue = getHueFromColor(MAIN_COLOR);
      color = colorHSV(hue*0xFF + heat[j] * 16, heat[j] > 127 ? 256 - (heat[j] - 127) : 255, heat[j] < 128 ? heat[j] * 2 : 255);
    } else {  // ORIGINAL MODE
      color = colorHSV(heat[j] * 16, heat[j] > 127 ? 256 - (heat[j] - 127) : 255, heat[j] < 128 ? heat[j] * 2 : 255);
      // switch COLDWHITE to WARMWHITE: Candle=0xFF9329
      if ((color >> 8 & 0xFF) > 0x93) color = (color & 0xFF00FF) | 0x9300; // limit green color
      if ((color & 0xFF) > 0x29) color = (color & 0xFFFF00) | 0x29;// limit blue color
    }

    if ( IS_REVERSE ) {
      setPixelColor(FIRST + (_seg_len - 1) - j, color);
    } else {
      setPixelColor(FIRST + j, color);
    }
  }
  return (_seg->speed / 32);
}

// - = = Firefly = = -
uint16_t WS2812FX::mode_firefly(void){
  SETUP HUE_START = random16();
  float phase = (CALL+HUE_START) * (0.02 + _num_segment*0.005);

  float pX = (sin(phase * 1.750) + sin(phase * 1.130)) * _seg_len / 5.0 +_seg_len / 2;
  // float pY = (sin(phase * 1.130) + 1.0) * _seg_len / 4; // fly n die
  // float pY = sin(phase * 1.130) * _seg_len / 4.2 +_seg_len / 4;
  float pY = (sin(phase * 1.150) + sin(sin(phase * 1.150)*2.5))*(0.026*_seg_len+0.6);
	
  for (uint16_t i = 0; i <= _seg_len; i++) {
    float distX = float(i) - pX;
    float distY = pY;
    float dist = (distX * distX + distY * distY)*max(1.0,(25-0.2*_seg_len));
    int b = 255 - max(0, min(255, int(dist)));
    // if(b>120) b += (sin(phase*4)+sin(phase*4.5))*20;
    // b = max(0, min(255, b));
    if (IS_RGBMODE) {
      setPixelColor(FIRST+i, colorHSV(CALL+HUE_START+b*0xFF, 255, b));
    } else if (IS_MONOMODE) {
      uint8_t hue = getHueFromColor(MAIN_COLOR);
      setPixelColor(FIRST+i, colorHSV(hue*0xFF, 240, b));
    } else { // IS_DUOMODE - ORIGINAL MODE
      setPixelColor(FIRST+i, colorHSV(13*0xFF, 240, b));
    }
  }
  return _seg->speed / 32;
}

// - = = UA Flag = = -
uint16_t WS2812FX::mode_uaflag(void){

  float phase = CALL * (0.02 + _num_segment*0.001);
  float split = (sin(phase*1.0) + sin(phase*2) + sin(phase*4.5))/3.0 * _seg_len/4 + _seg_len/2;

  for (uint16_t i = 0; i <= _seg_len; i++) { // for each led
    int b = ((i<split - _seg_len/4) || (i>split + _seg_len/4)) ? 16 : 255;
    int h = (split < i)? HUE_LIGHTBLUE : HUE_WHARMYELLOW;
    if (IS_RGBMODE) {
        setPixelColor(FIRST+i, colorHSV(CALL*4+h, 255, b));
    } else if (IS_MONOMODE) {
        setPixelColor(FIRST+i, (split < i)? BLUE : YELLOW);
    } else { // IS_DUOMODE - ORIGINAL
        setPixelColor(FIRST+i, colorHSV(h, 255, b));
    }
  }
  return _seg->speed / 32;
}

// - = = O C E A N = = -
uint16_t WS2812FX::mode_ocean(void){
  const double LendthOfWaves = 0.35;
  const double SpeedOfWaves = 0.05;
  const double BreathOfWaves = 0.009;
  float phase = CALL * (BreathOfWaves + _num_segment*0.0001);
  float flow = (sin(phase)+0.9)/2;

  for (uint16_t i = 0; i <= _seg_len; i++) {
	int b = ((sin((i+CALL*SpeedOfWaves)*LendthOfWaves)+1)*128)*flow;
	  b = max(16, min(255, b));
	if (IS_RGBMODE) {
      setPixelColor(FIRST+i, colorHSV(HUE_LIGHTBLUE - b*0xFF, 255, b));
	} else if (IS_MONOMODE) {
      uint8_t hue = getHueFromColor(MAIN_COLOR);
      setPixelColor(FIRST+i, colorHSV(hue*0xFF, 255 - b/4, b));
	} else { // IS_DUOMODE - ORIGINAL
      setPixelColor(FIRST+i, colorHSV(HUE_OCEAN - b*0x2F, 255, b));
	}
  }
  return _seg->speed / 64;
}

// - = = O R B I T A = = -
uint16_t WS2812FX::mode_orbita(void){
	const double coefR = 0.033;
	const double coefG = 0.032;
	const double coefB = 0.031;
	const double coefS = 0.001;

	int r = sin(CALL*coefR)*(double)(_seg_len/2-1)+_seg_len/2;
	int g = sin(CALL*coefG)*(double)(_seg_len/2-1)+_seg_len/2;
	int b = sin(CALL*coefB)*(double)(_seg_len/2-1)+_seg_len/2;
	int s = (sin(CALL*coefS)+1)*32;

  fade_out(BG_COLOR, 36 * FADE_RATE);

	if (IS_RGBMODE) {
		setPixelColor(r, getPixelColor(r) | colorHSV(CALL + HUE_RED));
		setPixelColor(g, getPixelColor(g) | colorHSV(CALL + HUE_GREEN));
		setPixelColor(b, getPixelColor(b) | colorHSV(CALL + HUE_BLUE));
	} else if (IS_MONOMODE) {
		setPixelColor(r, getPixelColor(r) | MAIN_COLOR);
		setPixelColor(g, getPixelColor(g) | DIM(MAIN_COLOR));
		setPixelColor(b, getPixelColor(b) | DARK(MAIN_COLOR));
	} else { // IS_DUOMODE - ORIGINAL
		setPixelColor(r, getPixelColor(r) | MAIN_COLOR);
		setPixelColor(g, getPixelColor(g) | MAIN_COLOR | SECOND_COLOR);
		setPixelColor(b, getPixelColor(b) | SECOND_COLOR);
		// setPixelColor(r, getPixelColor(r) | RED);
		// setPixelColor(g, getPixelColor(g) | GREEN);
		// setPixelColor(b, getPixelColor(b) | BLUE);
	}
  return _seg->speed / (64+s);
}

// - = = M A C H I N A = = -
uint16_t WS2812FX::mode_machina(void) {
  uint8_t size = (_seg_len/4) << SIZE_OPTION;
  static Machina mach[3];
  SETUP for (uint8_t i = 0; i < 3; i++) {
    mach[i].dest = random8(_seg_len) + 0xFFF;
    mach[i].pos = mach[i].dest;
    // mach[i].color = color_wheel(random8());
    mach[i].color = color_wheel(i*85); // RGB
	if(!IS_RGBMODE) mach[i].color  |= GRAY; // Make colour slight pastel
  }

  CLEAR;

  for (uint8_t i = 0; i < 3; i++) {
    if (mach[i].pos != mach[i].dest) { // if the Item have not reached their destination
      mach[i].pos += mach[i].dest > mach[i].pos ? 1 : -1; // move the spot right or left depending on position relative to destination
    } else { // the Item have reached their destination
      if (random8(12) == 0) {
        mach[i].dest += random16(_seg_len) - _seg_len/2; // set a new destination
		if (mach[i].dest < size) mach[i].dest += _seg_len;
      }
    }

    for (uint8_t j = 0; j < size; j++) {
      uint8_t pos = (mach[i].pos+j) % _seg_len;
      setPixelColor(pos, getPixelColor(pos) | mach[i].color);
    }
  }
  return _seg->speed / 32;
}

// - = = PartySky = = -
uint16_t WS2812FX::mode_party_sky(void) {
  SETUP CLEAR;
  
  fade_out(BG_COLOR, 18 * FADE_RATE + 128);
  uint32_t color;
  if (IS_RGBMODE) {
    color = color_wheel(random8());
  } else if (IS_MONOMODE) {
    color = MAIN_COLOR;
  } else { // IS_DUOMODE
    color = CALL%2 ? MAIN_COLOR : SECOND_COLOR;
  }
  
	if(random16(16) == 0) {  // white Stars
		// setPixelColor(random16(_seg_len), WHITE);
		setPixelColor(random16(_seg_len), 0x808080 | color);
	}
	if (random8(8) == 0) { // make star field
		for (uint16_t i = FIRST; i <= LAST; i++) {
			if (random8(8) == 0) {
				setPixelColor(i, getPixelColor(i) | DIM(color));
			}
		}
	}

  STEP++;
  return _seg->speed / 32;
}

// - = = = D I S C O = = = -
uint16_t WS2812FX::mode_disco(void) {
  SETUP CLEAR;
  
  EVERYSECONDCALL fade_out(BG_COLOR, 9 * FADE_RATE + 190);
  uint32_t color;
  if (IS_RGBMODE) {
    color = color_wheel(STEP++*21);
  } else if (IS_MONOMODE) {
    color = MAIN_COLOR;
  } else { // IS_DUOMODE
    color = CALL%2 ? MAIN_COLOR : SECOND_COLOR;
  }
  
	if (random8(8) == 0) { // make it shine
    uint8_t size = (_seg_len/4) << SIZE_OPTION;
    size -= random8(size/2);
    uint8_t pos = random8(_seg_len);
    for (uint8_t j = 0; j < size; j++) {
      // pos = (pos+j);// % _seg_len;
      // setPixelColor(pos, getPixelColor(pos) | color);
      setPixelColor((pos+j)%_seg_len, color);
    }
	}

  return _seg->speed / 32;
}

// - = = = M E T E O R = = = -
uint16_t WS2812FX::mode_meteor(void) {
  SETUP CLEAR;
  
  uint8_t bri = CALL % 8;
  uint32_t color;
  if (IS_RGBMODE) {
    color = color_wheel(CALL%255);
  } else if (IS_MONOMODE) {
    color = MAIN_COLOR;
  } else { // IS_DUOMODE
    color = AUX1%2 ? MAIN_COLOR : SECOND_COLOR;
  }
  uint8_t h = getHueFromColor(color);
  
  if(bri == 0){
    STEP = (STEP + 1);
    for(uint16_t i=FIRST; i <= LAST; i++) {
      uint8_t fade = min(255, random8(96)*FADE_RATE+96);
      if(getPixelColor(i)!=BG_COLOR) fade_out_pixel(i, BG_COLOR, fade);
    }
  } else {  
    if(IS_REVERSE) {
      color = colorHSV(h*255, 255, bri*32);
      setPixelColor(FIRST + STEP, color);
    } else {
      color = colorHSV(h*255, 255, bri*32);
      setPixelColor(LAST - STEP, color);
    }
  }

  if(STEP > _seg_len+24) {
    if(random8()==0) STEP = 0;
    AUX1++;
    SET_CYCLE;
  }
  return (_seg->speed / 128);
}

// - = = = Party Strob = = = -
uint16_t WS2812FX::mode_party_strobe(void) {
    uint32_t color = color_wheel(random8());
  if(CALL & 0x01) {
    fill(BG_COLOR, FIRST, _seg_len);
    SET_CYCLE;
    return random16(_seg->speed/2) + 50;
  } else {
      fill(color, FIRST, _seg_len);
    return 50;
  }
}

// - = = C O L O R F A L L = = -
uint16_t WS2812FX::mode_colorfall(void){
  float phase = CALL * (0.02 + _num_segment*0.001);
  float hue = ((sin(phase*1.0) + sin(phase*2.0) + sin(phase*4.5))+3)*0x2A;
  uint32_t color = color_wheel(hue);

  if(IS_REVERSE) {
    copyPixels(FIRST + 1, FIRST, _seg_len - 1);
    setPixelColor(FIRST, color);
  } else {
    copyPixels(FIRST, FIRST + 1, _seg_len - 1);
    setPixelColor(LAST, color);
  }
  return _seg->speed / 24;
}

// - = = S P L I T = = -
uint16_t WS2812FX::mode_split(void){
  float phase = CALL * (0.02 + _num_segment*0.001);
  float hue = ((sin(phase*1.0) + sin(phase*2.0) + sin(phase*4.5))+3)*0x3F;
  uint32_t color = color_wheel(hue);
  const uint8_t split = _seg_len/2;

  if(IS_REVERSE) {
    copyPixels(FIRST + 1, FIRST, split);
    copyPixels(FIRST+split, FIRST+split + 1, split);
    setPixelColor(FIRST, color);
    setPixelColor(LAST, color);
  } else {
    copyPixels(FIRST, FIRST + 1, split);
    copyPixels(FIRST+split + 1, FIRST+split, split);
    setPixelColor(LAST/2, color);
    setPixelColor(LAST/2+1, color);
  }
  return _seg->speed / 16;
}

// - = = FLYING S P L I T = = -
uint16_t WS2812FX::mode_flying_split(void){
  float phase = CALL * (0.02 + _num_segment*0.001);
  float magic = (sin(phase*1.0) + sin(phase*2.0) + sin(phase*4.5));
  // uint32_t color = 
  int split = _seg_len/2 + magic*_seg_len/9;
  uint32_t color1, color2;

  if (IS_RGBMODE) {
	// color1 = color_wheel((magic+3)*0x3F);
	color1 = colorHSV((magic+3)*0x3FFF);
	color2 = color1;
  } else if (IS_MONOMODE) {
	color1 = MAIN_COLOR;
	color2 = MAIN_COLOR;
  } else { // IS_DUOMODE
	color1 = IS_REVERSE? SECOND_COLOR: MAIN_COLOR;
	color2 = IS_REVERSE? MAIN_COLOR: SECOND_COLOR;
  }
  
  fade_out(BG_COLOR, min(255,48 * FADE_RATE));
  
  copyPixels(FIRST, FIRST+1, split-1);
  copyPixels(FIRST+split+2, FIRST+split+1, _seg_len - split-2);
  setPixelColor(FIRST + split-1, color1);
  setPixelColor(FIRST + split, color1);
  setPixelColor(FIRST + split+1, color2);
  setPixelColor(FIRST + split+2, color2);

  return _seg->speed / 24;
}

//  VU Meter Peak
uint16_t WS2812FX::mode_vu_meter_peak(void) {
  SETUP AUX3 = 0;
  uint32_t color;
  uint8_t size = 0x01 << SIZE_OPTION; // 1,2,4,8

  int randomData = AUX1 + random8(32) - random8(32);
  AUX1 = (randomData < 0 || randomData > 255) ? 128 : randomData;

  uint8_t scaledLevel = (AUX1 * _seg_len) / 256;
  
  if (IS_RGBMODE) { // Rainbow color
    color = color_wheel(STEP);
  } else {
    color = MAIN_COLOR;
  }
  
  for (uint16_t j = 0; j < _seg_len; j++) { // fill field
    uint16_t index = IS_REVERSE? LAST - j: FIRST + j;
    if (j <= scaledLevel) {
      setPixelColor(index, color);
    } else {
      setPixelColor(index, BG_COLOR);
    }
  }
  if(AUX3 < scaledLevel) { // PEAK
    AUX3 = scaledLevel;
  } else {
    if(!(STEP%4)) AUX3--;
  }
  uint16_t indexPeak = IS_REVERSE? LAST-AUX3: AUX3;
  if (IS_RGBMODE) {
    fill(color,indexPeak, size);
  } else if (IS_MONOMODE) {
    fill(MAIN_COLOR, indexPeak, size);
  } else { // IS_DUOMODE
    fill(SECOND_COLOR, indexPeak, size);
  }
  
  STEP = (STEP + 1) & 0xFF;
  SET_CYCLE;
  return (_seg->speed / 64);
}

// - = = S P L I T = = -
uint16_t WS2812FX::mode_portal(void){
  uint32_t color1, color2;
  const uint8_t split = _seg_len/2;
  
  if (IS_RGBMODE) {
    float phase = CALL * (0.02 + _num_segment*0.001);
    float hue = ((sin(phase*1.0) + sin(phase*2.0) + sin(phase*4.5))+3)*0x3F;
    color1  = color_wheel(hue);
    color2 = color1;// color2 = color_wheel(0xFFFFFF-hue); 
  } else if (IS_MONOMODE) {
    color1 = MAIN_COLOR;  
    color2 = color1;
  } else { // IS_DUOMODE
    if(IS_REVERSE) {
      color1 = SECOND_COLOR;
      color2 = MAIN_COLOR;
    } else {
      color1 = MAIN_COLOR;
      color2 = SECOND_COLOR;
    }
  }
  
  for(uint16_t i=FIRST; i <= LAST; i++) { // random fade some pixel
    // uint8_t fade = min(255, random8(96)*FADE_RATE+96);
    uint8_t fade = min(255, random8(_seg_len*3/2)*FADE_RATE+96);
    if(getPixelColor(i)!=BG_COLOR) fade_out_pixel(i, BG_COLOR, fade);
  }
  
  copyPixels(FIRST + 1, FIRST, split);
  copyPixels(FIRST+split, FIRST+split + 1, split);
  setPixelColor(FIRST, color1);
  setPixelColor(LAST, color2);
  
  return _seg->speed / 24;
}