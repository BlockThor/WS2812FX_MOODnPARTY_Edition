/*
  WS2812FX_MnP_edition.h - Library upgraded and adapted for use with MOOD'n'PARTY LIGHTS : Single page WebInterface for ESP8266 and Arduino
  link:
  
  Vasyl Yudin
  Jan 2024
  www.ledvine.com
  
  modes_utils.cpp - WS2812FX effects helper functions

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

#define MAIN_COLOR	_seg->colors[0]
#define BG_COLOR	_seg->colors[1]
#define SECOND_COLOR	_seg->colors[2]

#define IS_RGBMODE (_seg->colors[0] == BG_COLOR)
#define IS_MONOMODE (_seg->colors[2] == BG_COLOR)

#define RGBW32(r,g,b,w) (uint32_t((byte(w) << 24) | (byte(r) << 16) | (byte(g) << 8) | (byte(b))))
#define W(c) (byte((c) >> 24))
#define R(c) (byte((c) >> 16))
#define G(c) (byte((c) >> 8))
#define B(c) (byte(c))
// if (IS_RGBMODE) {
// } else if (IS_MONOMODE) {
// } else { // IS_DUOMODE
// }
uint8_t WS2812FX::getHueFromColor(uint32_t color){
   
  uint8_t r = (color >> 16) & 0xFF;
  uint8_t g = (color >>  8) & 0xFF;
  uint8_t b = (color        & 0xFF);
  
  uint8_t rgbMin, rgbMax, h, s, v;

  rgbMin = r < g ? (r < b ? r : b) : (g < b ? g : b);
  rgbMax = r > g ? (r > b ? r : b) : (g > b ? g : b);
  
  v = rgbMax;
  if (v == 0)
      return 0;

  s = 255 * long(rgbMax - rgbMin) / v;
  if (s == 0)
      return 0;

  if (rgbMax == r)
      h = 0 + 43 * (g - b) / (rgbMax - rgbMin);
  else if (rgbMax == g)
      h = 85 + 43 * (b - r) / (rgbMax - rgbMin);
  else
      h = 171 + 43 * (r - g) / (rgbMax - rgbMin);

  return h;
}

//overload Adafruit_NeoPixel fill() function to respect segment boundaries
void WS2812FX::fill(uint32_t c, uint16_t first, uint16_t count) {
  uint16_t i, end;

  // If first LED is past end of strip or outside segment boundaries, nothing to do
  if (first >= numLEDs || first < _seg->start || first > _seg->stop) {
    return;
  }

  // Calculate the index ONE AFTER the last pixel to fill
  if (count == 0) {
    end = _seg->stop + 1; // Fill to end of segment
  } else {
    end = first + count;
    if(end > (_seg->stop + 1)) end = _seg->stop + 1;
  }

  if (end > numLEDs) end = numLEDs;

  for (i = first; i < end; i++) {
    setPixelColor(i, c);
  }
}

//Clear to Raw Background Color (ignore brightness)
void WS2812FX::clearRawBG() {
  for(uint16_t i=_seg->start; i <= _seg->stop; i++) {
    setRawPixelColor(i, BG_COLOR);
  }
}

/*
 * Blink/strobe function
 * Alternate between color1 and color2
 * if(strobe == true) then create a strobe effect
 */
uint16_t WS2812FX::blink(uint32_t color1, uint32_t color2, bool strobe) {
  if(_seg_rt->counter_mode_call & 1) {
    uint32_t color = (IS_REVERSE) ? color1 : color2; // off
    fill(color, _seg->start, _seg_len);
    SET_CYCLE;
    return strobe ? _seg->speed - 20 : (_seg->speed / 2);
  } else {
    uint32_t color = (IS_REVERSE) ? color2 : color1; // on
    fill(color, _seg->start, _seg_len);
    return strobe ? 20 : (_seg->speed / 2);
  }
}

/*
 * Color wipe function
 * LEDs are turned on (color1) in sequence, then turned off (color2) in sequence.
 * if (bool rev == true) then LEDs are turned off in reverse order
 */
uint16_t WS2812FX::color_wipe(uint32_t color1, uint32_t color2, bool rev) {
  if(_seg_rt->counter_mode_step < _seg_len) {
    uint32_t led_offset = _seg_rt->counter_mode_step;
    if(IS_REVERSE) {
      setPixelColor(_seg->stop - led_offset, color1);
    } else {
      setPixelColor(_seg->start + led_offset, color1);
    }
  } else {
    uint32_t led_offset = _seg_rt->counter_mode_step - _seg_len;
    if((IS_REVERSE && !rev) || (!IS_REVERSE && rev)) {
      setPixelColor(_seg->stop - led_offset, color2);
    } else {
      setPixelColor(_seg->start + led_offset, color2);
    }
  }

  _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) % (_seg_len * 2);

  if(_seg_rt->counter_mode_step == 0) SET_CYCLE;

  return (_seg->speed / (_seg_len * 2));
}


/*
 * scan function - runs a block of pixels back and forth.
 */
uint16_t WS2812FX::scan(uint32_t color1, uint32_t color2, bool dual) {
  int8_t dir = _seg_rt->aux_param ? -1 : 1;
  uint8_t size = 1 << SIZE_OPTION;

  // clearRawBG();
  fill(BG_COLOR, _seg->start, _seg_len);

	if (IS_RGBMODE) {
	  uint8_t colorIndexIncr =  256 / _seg_len;
	  color1 = color_wheel(colorIndexIncr*(_seg->start + _seg_rt->counter_mode_step)/2);
	  color2 = color_wheel(colorIndexIncr*(_seg->stop - _seg_rt->counter_mode_step)/2+128);
      fill(color1, _seg->start + _seg_rt->counter_mode_step, size);
      if(dual) fill(color2, _seg->stop - _seg_rt->counter_mode_step - size + 1, size);
	} else if (IS_MONOMODE) {
      fill(color1, _seg->start + _seg_rt->counter_mode_step, size);
	  if(dual) fill(color1, _seg->stop - _seg_rt->counter_mode_step - size + 1, size);
	} else { // IS_DUOMODE
      fill(_seg_rt->aux_param ? color1 : color2, _seg->start + _seg_rt->counter_mode_step, size);
      if(dual) fill(_seg_rt->aux_param ? color2 : color1, _seg->stop - _seg_rt->counter_mode_step - size + 1, size);
	}

  _seg_rt->counter_mode_step += dir;
  if(_seg_rt->counter_mode_step == 0) {
    _seg_rt->aux_param = 0;
    SET_CYCLE;
  }
  if(_seg_rt->counter_mode_step >= (uint16_t)(_seg_len - size)) _seg_rt->aux_param = 1;

  return (_seg->speed / (_seg_len * 2));
}

/*
 * Tricolor chase function
 */
uint16_t WS2812FX::tricolor_chase(uint32_t color1, uint32_t color2, uint32_t color3) {
  uint8_t sizeCnt = 1 << SIZE_OPTION;
  uint8_t sizeCnt2 = sizeCnt + sizeCnt;
  uint8_t sizeCnt3 = sizeCnt2 + sizeCnt;
  uint16_t index = _seg_rt->counter_mode_step % sizeCnt3;
  for(uint16_t i=0; i < _seg_len; i++, index++) {
    index = index % sizeCnt3;

    uint32_t color = color3;
    if(index < sizeCnt) color = color1;
    else if(index < sizeCnt2) color = color2;

    if(IS_REVERSE) {
      setPixelColor(_seg->start + i, color);
    } else {
      setPixelColor(_seg->stop - i, color);
    }
  }

  _seg_rt->counter_mode_step++;
  if(_seg_rt->counter_mode_step % _seg_len == 0) SET_CYCLE;

  return (_seg->speed / 8);
}

/*
 * twinkle function
 */
uint16_t WS2812FX::twinkle(uint32_t color1, uint32_t color2) {
  if(_seg_rt->counter_mode_step == 0) {
    fill(color2, _seg->start, _seg_len);
    uint16_t min_leds = (_seg_len / 4) + 1; // make sure, at least one LED is on
    _seg_rt->counter_mode_step = random(min_leds, min_leds * 2);
    SET_CYCLE;
  }

  setPixelColor(_seg->start + random16(_seg_len), color1);

  _seg_rt->counter_mode_step--;
  return (_seg->speed / _seg_len);
}

/*
 * fade out functions
 */
void WS2812FX::fade_out() {
  fade_out(BG_COLOR);
}

void WS2812FX::fade_out(uint32_t targetColor) {
  for(uint16_t i=_seg->start; i <= _seg->stop; i++) {
    fade_out_pixel(i, targetColor);
  }
}

void WS2812FX::fade_out(uint32_t targetColor, uint8_t rate) {
  for(uint16_t i=_seg->start; i <= _seg->stop; i++) {
    fade_out_pixel(i, targetColor, rate);
  }
}

void WS2812FX::fade_out_pixel(uint16_t n) {
  fade_out_pixel(n, BG_COLOR, FADE_RATE);
}

void WS2812FX::fade_out_pixel(uint16_t n, uint32_t targetColor) {
  fade_out_pixel(n, targetColor, FADE_RATE);
}

void WS2812FX::fade_out_pixel(uint16_t n, uint32_t targetColor, uint8_t rate) {
  
  uint32_t color = getPixelColor(n); // current color

  if (color == targetColor || rate == 0) return;
  
    uint32_t w1 = (color >> 24) & 0xFF;
    uint32_t r1 = (color >> 16) & 0xFF;
    uint32_t g1 = (color >>  8) & 0xFF;
    uint32_t b1 =  color        & 0xFF;
    uint32_t w3, r3, g3, b3;
  
  if(targetColor == BLACK){
    w3 = (w1 * (int)(rate) ) >> 8;
    r3 = (r1 * (int)(rate) ) >> 8;
    g3 = (g1 * (int)(rate) ) >> 8;
    b3 = (b1 * (int)(rate) ) >> 8;
    
  } else {
    uint32_t w2 = W(targetColor);
    uint32_t r2 = R(targetColor);
    uint32_t g2 = G(targetColor);
    uint32_t b2 = B(targetColor);

    w3 = ((w1 * (int)rate) + (w2 * (int)(0xFF - rate))) >> 8;
    r3 = ((r1 * (int)rate) + (r2 * (int)(0xFF - rate))) >> 8;
    g3 = ((g1 * (int)rate) + (g2 * (int)(0xFF - rate))) >> 8;
    b3 = ((b1 * (int)rate) + (b2 * (int)(0xFF - rate))) >> 8;
  }
  setPixelColor(n, r3, g3, b3, w3);
}

/*
 * color blend function
 */
uint32_t WS2812FX::color_blend(uint32_t color1, uint32_t color2, uint8_t blendAmt) {
  uint32_t blendedColor;
  blend((uint8_t*)&blendedColor, (uint8_t*)&color1, (uint8_t*)&color2, sizeof(uint32_t), blendAmt);
  return blendedColor;
}

uint8_t* WS2812FX::blend(uint8_t *dest, uint8_t *src1, uint8_t *src2, uint16_t cnt, uint8_t blendAmt) {
  if(blendAmt == 0) {
    memmove(dest, src1, cnt);
  } else if(blendAmt == 255) {
    memmove(dest, src2, cnt);
  } else {
    for(uint16_t i=0; i<cnt; i++) {
//    dest[i] = map(blendAmt, 0, 255, src1[i], src2[i]);
      dest[i] =  blendAmt * ((int)src2[i] - (int)src1[i]) / 256 + src1[i]; // map() function
    }
  }
  return dest;
}

/*
 * Sparkle function
 * color1 = background color
 * color2 = sparkle color
 */
 uint16_t WS2812FX::sparkle(uint32_t color1, uint32_t color2) {
  clearRawBG();

  uint8_t size = 1 << SIZE_OPTION;

  if(random8(6) == 0) fill(color2, _seg->start + random16(_seg_len - size + 1), size);
  if(random8(6) == 0) fill(color1, _seg->start + random16(_seg_len - size + 1), size);

  SET_CYCLE;
  return (_seg->speed / 16);
}

/*
 * color chase function.
 * color1 = background color
 * color2 and color3 = colors of two adjacent leds
 */
uint16_t WS2812FX::chase(uint32_t color1, uint32_t color2, uint32_t color3) {
  uint8_t size = 1 << SIZE_OPTION;
  for(uint8_t i=0; i<size; i++) {
    uint16_t a = (_seg_rt->counter_mode_step + i) % _seg_len;
    uint16_t b = (a + size) % _seg_len;
    uint16_t c = (b + size) % _seg_len;
    if(IS_REVERSE) {
      setPixelColor(_seg->stop - a, color1);
      setPixelColor(_seg->stop - b, color2);
      setPixelColor(_seg->stop - c, color3);
    } else {
      setPixelColor(_seg->start + a, color1);
      setPixelColor(_seg->start + b, color2);
      setPixelColor(_seg->start + c, color3);
    }
  }

  if(_seg_rt->counter_mode_step + (size * 3) == _seg_len) SET_CYCLE;

  _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) % _seg_len;
  return (_seg->speed / _seg_len);
}

/*
 * running white flashes function.
 * color1 = background color
 * color2 = flash color
 */
uint16_t WS2812FX::chase_flash(uint32_t color1, uint32_t color2) {
  const static uint8_t flash_count = 4;
  uint8_t flash_step = _seg_rt->counter_mode_call % ((flash_count * 2) + 1);

  if(flash_step < (flash_count * 2)) {
    uint32_t color = (flash_step % 2 == 0) ? color2 : color1;
    uint16_t n = _seg_rt->counter_mode_step;
    uint16_t m = (_seg_rt->counter_mode_step + 1) % _seg_len;
    if(IS_REVERSE) {
      setPixelColor(_seg->stop - n, color);
      setPixelColor(_seg->stop - m, color);
    } else {
      setPixelColor(_seg->start + n, color);
      setPixelColor(_seg->start + m, color);
    }
    return 30;
  } else {
    _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) % _seg_len;
    if(_seg_rt->counter_mode_step == 0) {
      // update aux_param so mode_chase_flash_random() will select the next color
      _seg_rt->aux_param = get_random_wheel_index(_seg_rt->aux_param);
      SET_CYCLE;
    }
  }
  return (_seg->speed / _seg_len);
}

/*
 * Alternating pixels running function.
 */
uint16_t WS2812FX::running(uint32_t color1, uint32_t color2) {
  uint8_t size = 2 << SIZE_OPTION;
  uint32_t color = (_seg_rt->counter_mode_step & size) ? color1 : color2;

  if(IS_REVERSE) {
    copyPixels(_seg->start, _seg->start + 1, _seg_len - 1);
    setPixelColor(_seg->stop, color);
  } else {
    copyPixels(_seg->start + 1, _seg->start, _seg_len - 1);
    setPixelColor(_seg->start, color);
  }

  _seg_rt->counter_mode_step++;
  if((_seg_rt->counter_mode_step % _seg_len) == 0) SET_CYCLE;
  return (_seg->speed / 16);
}

/*
 * Fireworks function.
 */
uint16_t WS2812FX::fireworks(uint32_t color) {
  // fade_out();
  
  fade_out(BG_COLOR, 24 * FADE_RATE);

// for better performance, manipulate the Adafruit_NeoPixels pixels[] array directly
  uint8_t *pixels = getPixels();
  uint8_t bytesPerPixel = getNumBytesPerPixel(); // 3=RGB, 4=RGBW
  uint16_t startPixel = _seg->start * bytesPerPixel + bytesPerPixel;
  uint16_t stopPixel = _seg->stop * bytesPerPixel;
  for(uint16_t i=startPixel; i <stopPixel; i++) {
    uint16_t tmpPixel = (pixels[i - bytesPerPixel] >> 2) +
      pixels[i] +
      (pixels[i + bytesPerPixel] >> 2);
    pixels[i] =  tmpPixel > 255 ? 255 : tmpPixel;
  }

  uint8_t size = 2 << SIZE_OPTION;
  if(!_triggered) {
    for(uint16_t i=0; i<max(1, _seg_len/20); i++) {
      if(random8(10) == 0) {
        uint16_t index = _seg->start + random16(_seg_len - size + 1);
        fill(color, index, size);
        SET_CYCLE;
      }
    }
  } else {
    for(uint16_t i=0; i<max(1, _seg_len/10); i++) {
      uint16_t index = _seg->start + random16(_seg_len - size + 1);
      fill(color, index, size);
      SET_CYCLE;
    }
  }

  return (_seg->speed / 16);
}

