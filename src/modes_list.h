/*
  WS2812FX_MnP_edition.h - Library upgraded and adapted for use with MOOD'n'PARTY LIGHTS : Single page WebInterface for ESP8266 and Arduino
  link:
  
  Vasyl Yudin
  Jan 2024
  www.ledvine.com
  
  modes_list.h - WS2812FX header file for ESP8266 and ESP32 microprocessors

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
#ifndef mode_esp_h
#define mode_esp_h

#define MODE_COUNT (sizeof(_modes)/sizeof(_modes[0]))
#define MODE_PTR(x) this->*_modes[x].mode_ptr
#define MODE_NAME(x) _modes[x].name

#define FX_MODE_STATIC                   0
#define FX_MODE_MOOD_CLICK               1 // ex FX_MODE_BLINK
#define FX_MODE_BREATH                   2
#define FX_MODE_MOOD_TRAIN               3
#define FX_MODE_XMAS_GLANCE              4 // ex FX_MODE_SINGLE_DYNAMIC
#define FX_MODE_XMAS_TICK                5 // ex FX_MODE_MULTI_DYNAMIC
#define FX_MODE_XMAS_TWINKLE             6 // ex FX_MODE_TWINKLEFOX
#define FX_MODE_XMAS_GLITTER             7
#define FX_MODE_MOOD_SHIFT               8 // ex FX_MODE_RAINBOW
#define FX_MODE_MOOD_FLOW                9 // ex FX_MODE_RAINBOW_CYCLE
#define FX_MODE_SCAN                    10
#define FX_MODE_DUAL_SCAN               11
#define FX_MODE_MOOD_SWING              12
#define FX_MODE_THEATER_CHASE           13
#define FX_MODE_WAVES                   14
#define FX_MODE_TWINKLE                 15 // ex FX_MODE_RUNNING_LIGHTS
#define FX_MODE_TWINKLE_RANDOM          16 //F
#define FX_MODE_TWINKLE_FADE            17 //F
#define FX_MODE_SPARKLE                 18
#define FX_MODE_STROBE                  19
#define FX_MODE_STROBE_RAINBOW          20 // SPECIAL
#define FX_MODE_MULTI_STROBE            21 //F
#define FX_MODE_BLINK_RAINBOW           22
#define FX_MODE_CHASE_WHITE             23 //F
#define FX_MODE_CHASE_COLOR             24 //F 
#define FX_MODE_CHASE_RANDOM            25 //F 
#define FX_MODE_CHASE_RAINBOW           26 //F 
#define FX_MODE_CHASE_FLASH             27 //F 
#define FX_MODE_CHASE_FLASH_RANDOM      28 //F
#define FX_MODE_CHASE_RAINBOW_WHITE     29 //F
#define FX_MODE_CHASE_BLACKOUT          30 //F
#define FX_MODE_CHASE_BLACKOUT_RAINBOW  31 //F
#define FX_MODE_COLOR_SWEEP_RANDOM      32 //F
#define FX_MODE_RUNNING_COLOR           33 //F 
#define FX_MODE_RUNNING_RED_BLUE        34 //F 
#define FX_MODE_RUNNING_RANDOM          35 //F
#define FX_MODE_LARSON_SCANNER          36 //F
#define FX_MODE_COMET                   37
#define FX_MODE_FIREWORKS               38 //F
#define FX_MODE_MERRY_CHRISTMAS         39
#define FX_MODE_CIRCUS_COMBUSTUS        40 //F
#define FX_MODE_HALLOWEEN               41 //F
#define FX_MODE_BICOLOR_CHASE           42 //F
#define FX_MODE_TRICOLOR_CHASE          43 //F
#define FX_MODE_RAIN                    44 //F
#define FX_MODE_DISSOLVE                45 // rename DISSOLVE
#define FX_MODE_ICU                     46 // SPECIAL
#define FX_MODE_DUAL_LARSON             47 
#define FX_MODE_RUNNING_TAILS           48 // ex ...
#define FX_MODE_FILLER_UP               49 // SPECIAL
#define FX_MODE_RAINBOW_LARSON          50 //F
#define FX_MODE_RAINBOW_FIREWORKS       51 // SPECIAL
#define FX_MODE_TRIFADE                 52 //X - Future FADE for PALLETRE
#define FX_MODE_VU_METER                53 // SPECIAL
#define FX_MODE_HEARTBEAT               54 // SPECIAL
#define FX_MODE_COMETS                  55 // ex FX_MODE_MULTI_COMET
#define FX_MODE_POPCORN                 56 
#define FX_MODE_FIRE2012                57 // SPECIAL 
#define FX_MODE_FIREFLY                 58 // SPECIAL 
#define FX_MODE_UAFLAG                  59 // SPECIAL 
#define FX_MODE_OCEAN                   60 // SPECIAL 
#define FX_MODE_ORBITA                  61 // SPECIAL 
#define FX_MODE_MACHINA		       				62 // SPECIAL 
#define FX_MODE_PARTYSKY			     			63 // SPECIAL 
#define FX_MODE_DISCO			            	64 // SPECIAL 
#define FX_MODE_METEOR			     		  	65 // SPECIAL 
#define FX_MODE_PARTY_STROBE    		  	66 // SPECIAL 
#define FX_MODE_COLORFALL        		  	67 // SPECIAL 
#define FX_MODE_SPLIT           		  	68 // SPECIAL 
#define FX_MODE_VU_METER_PEAK   		  	69 // SPECIAL 
#define FX_MODE_FLYING_SPIRIT    		  	70 // SPECIAL 
#define FX_MODE_PORTAL          		  	71 // SPECIAL 
#define FX_MODE_CUSTOM_0                72 // custom modes need to go at the end
#define FX_MODE_CUSTOM_1                73 
#define FX_MODE_CUSTOM_2                74 
#define FX_MODE_CUSTOM_3                75
#define FX_MODE_CUSTOM_4                76
#define FX_MODE_CUSTOM_5                77
#define FX_MODE_CUSTOM_6                78
#define FX_MODE_CUSTOM_7                79


// OLD NAMES 
// #define FX_MODE_CUSTOM                  67 // 
// #define FX_MODE_FADE                    11
// #define FX_MODE_RAINBOW                 7 // Rename MOOD_SHIFT
// #define FX_MODE_RAINBOW_CYCLE           8 // Rename MOOD_FLOW
// #define FX_MODE_SINGLE_DYNAMIC          5 // Rename XMAS_GLANCE
// #define FX_MODE_MULTI_DYNAMIC           6 // Rename XMAS_TICK
// #define FX_MODE_MULTI_COMET             55 

// #define FX_MODE_COLOR_WIPE               0 //x
// #define FX_MODE_COLOR_WIPE_INV           0 //x 
// #define FX_MODE_COLOR_WIPE_REV           0 //x
// #define FX_MODE_COLOR_WIPE_REV_INV       0 //x
// #define FX_MODE_FLASH_SPARKLE            0 //x
// #define FX_MODE_HYPER_SPARKLE            0 //x
// #define FX_MODE_TWINKLE_FADE_RANDOM      0 //w
// #define FX_MODE_BITS                     0 //x
// #define FX_MODE_FIRE_FLICKER_INTENSE     0 //x
// #define FX_MODE_FIRE_FLICKER             0 //x
// #define FX_MODE_FIRE_FLICKER_SOFT        0 //x
// #define FX_MODE_FIREWORKS_RANDOM         0 //W
// #define FX_MODE_OSCILLATOR               0 // SPECIAL ??
// #define FX_MODE_FLIPBOOK                 0 //x

typedef struct Mode {
  const __FlashStringHelper* name;
  uint16_t (WS2812FX::*mode_ptr)(void);
} mode;

// create GLOBAL names to allow WS2812FX to compile with sketches and other libs
// that store strings in PROGMEM (get rid of the "section type conflict with __c"
// errors once and for all. Amen.)
const char name_0[]  PROGMEM = "Static Mood";
const char name_1[]  PROGMEM = "Mood Click";
const char name_2[]  PROGMEM = "Breath";
const char name_3[]  PROGMEM = "Mood Swipe"; // ex "Color Wipe Random"
const char name_4[]  PROGMEM = "Xmas Glance";
const char name_5[]  PROGMEM = "Xmas Tick";
const char name_6[]  PROGMEM = "Xmas Twinkle";
const char name_7[]  PROGMEM = "Xmas Glitter";
const char name_8[]  PROGMEM = "Mood Shift"; //ex "Rainbow"
const char name_9[]  PROGMEM = "Mood Flow"; // ex "Rainbow Cycle"
const char name_10[] PROGMEM = "Scan"; //
const char name_11[] PROGMEM = "Dual Scan"; //
const char name_12[] PROGMEM = "Mood Swing";
const char name_13[] PROGMEM = "Theater Chase"; // TO DO RGB AND DUO
const char name_14[] PROGMEM = "Waves"; // ex "Running Lights"
const char name_15[] PROGMEM = "Twinkle"; // REMOVE: WTF?
const char name_16[] PROGMEM = "Twinkle Random";  // REMOVE: WTF?
const char name_17[] PROGMEM = "Twinkle Fade";
const char name_18[] PROGMEM = "Sparkle"; 
const char name_19[] PROGMEM = "Strobe";  // TO DO RGB AND DUO
const char name_20[] PROGMEM = "Strobe Rainbow"; // REMOVE IF PREV IS RGB
const char name_21[] PROGMEM = "Multi Strobe"; // RGB ONLY ???
const char name_22[] PROGMEM = "Blink Rainbow"; // ???
const char name_23[] PROGMEM = "Chase White"; // HIDE: NOT LIKE
const char name_24[] PROGMEM = "Chase Color"; // HIDE: NOT LIKE
const char name_25[] PROGMEM = "Chase Random"; // HIDE: NOT LIKE
const char name_26[] PROGMEM = "Chase Rainbow"; // HIDE: NOT LIKE
const char name_27[] PROGMEM = "Chase Flash"; // HIDE: NOT LIKE
const char name_28[] PROGMEM = "Chase Flash Random"; // HIDE: NOT LIKE
const char name_29[] PROGMEM = "Chase Rainbow White"; // HIDE: NOT LIKE
const char name_30[] PROGMEM = "Chase Blackout"; // HIDE: NOT LIKE
const char name_31[] PROGMEM = "Chase Blackout Rainbow"; // HIDE: NOT LIKE
const char name_32[] PROGMEM = "Color Sweep Random"; // REMOVE: RGB DONE
const char name_33[] PROGMEM = "Running Color";
const char name_34[] PROGMEM = "Running Red Blue"; // REMOVE: Preset
const char name_35[] PROGMEM = "Running Random"; // REMOVE: RGB DONE
const char name_36[] PROGMEM = "Larson Scanner"; // Slow down ???
const char name_37[] PROGMEM = "Comet"; // HIDE: NOT LIKE
const char name_38[] PROGMEM = "Fireworks";
const char name_39[] PROGMEM = "Merry Christmas";  // REMOVE: Preset
const char name_40[] PROGMEM = "Circus Combustus";  // REMOVE: Preset
const char name_41[] PROGMEM = "Halloween";  // REMOVE: Preset
const char name_42[] PROGMEM = "Bicolor Chase";  // REMOVE: Preset
const char name_43[] PROGMEM = "Tricolor Chase"; // RENAME: Running 3Color
const char name_44[] PROGMEM = "Rain";
const char name_45[] PROGMEM = "Dissolve"; // Ex. "Block Dissolve"
const char name_46[] PROGMEM = "ICU";
const char name_47[] PROGMEM = "Dual Larson"; // SOWDOWN 2x
const char name_48[] PROGMEM = "Running Tails"; // RENAME: "Running Dots"
const char name_49[] PROGMEM = "Filler Up"; // ADD DUO n RGB
const char name_50[] PROGMEM = "Rainbow Larson"; // CHECK ColorModes !!!
const char name_51[] PROGMEM = "Rainbow Fireworks"; // SLOWDOWN 2x
const char name_52[] PROGMEM = "Trifade"; // VERY SPECIAL ???
const char name_53[] PROGMEM = "VU Meter"; 
const char name_54[] PROGMEM = "Heartbeat";
const char name_55[] PROGMEM = "Comets";
const char name_56[] PROGMEM = "Popcorn";
const char name_57[] PROGMEM = "Fire 2012";
const char name_58[] PROGMEM = "Firefly";
const char name_59[] PROGMEM = "UA Flag";
const char name_60[] PROGMEM = "Ocean";
const char name_61[] PROGMEM = "Orbita";
const char name_62[] PROGMEM = "Machina";
const char name_63[] PROGMEM = "PartySky";
const char name_64[] PROGMEM = "Disco";
const char name_65[] PROGMEM = "Meteor";
const char name_66[] PROGMEM = "Party Strobe";
const char name_67[] PROGMEM = "Colorfall";
const char name_68[] PROGMEM = "Split";
const char name_69[] PROGMEM = "VU Meter Peak";
const char name_70[] PROGMEM = "Fliyng Spirit";
const char name_71[] PROGMEM = "Portal";
const char name_72[] PROGMEM = "Custom 0"; // custom modes need to go at the en
const char name_73[] PROGMEM = "Custom 1";
const char name_74[] PROGMEM = "Custom 2";
const char name_75[] PROGMEM = "Custom 3";
const char name_76[] PROGMEM = "Custom 4";
const char name_77[] PROGMEM = "Custom 5";
const char name_78[] PROGMEM = "Custom 6";
const char name_79[] PROGMEM = "Custom 7";
// const char name_80[] PROGMEM = 
// const char name_81[] PROGMEM = 
// const char name_82[] PROGMEM = 
// const char name_83[] PROGMEM = 
// const char name_84[] PROGMEM = 
// const char name_85[] PROGMEM = 
// const char name_86[] PROGMEM = 
// const char name_87[] PROGMEM = 
// const char name_88[] PROGMEM = 

// define static array of member function pointers.
// make sure the order of the _modes array elements matches the FX_MODE_* values
__attribute__ ((unused)) static mode _modes[] = {
  { FSH(name_0),  &WS2812FX::mode_static },
  { FSH(name_1),  &WS2812FX::mode_mood_click },
  { FSH(name_2),  &WS2812FX::mode_breath },
  { FSH(name_3),  &WS2812FX::mode_mood_train },
  { FSH(name_4),  &WS2812FX::mode_xmas_glance },
  { FSH(name_5),  &WS2812FX::mode_xmas_tick },
  { FSH(name_6),  &WS2812FX::mode_xmas_twinkle },
  { FSH(name_7),  &WS2812FX::mode_xmas_glitter },
  { FSH(name_8),  &WS2812FX::mode_mood_shift },
  { FSH(name_9),  &WS2812FX::mode_mood_flow },
  { FSH(name_10), &WS2812FX::mode_scan },
  { FSH(name_11), &WS2812FX::mode_dual_scan },
  { FSH(name_12), &WS2812FX::mode_mood_swing },
  { FSH(name_13), &WS2812FX::mode_theater_chase },
  { FSH(name_14), &WS2812FX::mode_waves },
  { FSH(name_15), &WS2812FX::mode_twinkle },
  { FSH(name_16), &WS2812FX::mode_twinkle_random },
  { FSH(name_17), &WS2812FX::mode_twinkle_fade },
  { FSH(name_18), &WS2812FX::mode_sparkle },
  { FSH(name_19), &WS2812FX::mode_strobe },
  { FSH(name_20), &WS2812FX::mode_strobe_rainbow },
  { FSH(name_21), &WS2812FX::mode_multi_strobe },
  { FSH(name_22), &WS2812FX::mode_blink_rainbow },
  { FSH(name_23), &WS2812FX::mode_chase_white },
  { FSH(name_24), &WS2812FX::mode_chase_color },
  { FSH(name_25), &WS2812FX::mode_chase_random },
  { FSH(name_26), &WS2812FX::mode_chase_rainbow },
  { FSH(name_27), &WS2812FX::mode_chase_flash },
  { FSH(name_28), &WS2812FX::mode_chase_flash_random },
  { FSH(name_29), &WS2812FX::mode_chase_rainbow_white },
  { FSH(name_30), &WS2812FX::mode_chase_blackout },
  { FSH(name_31), &WS2812FX::mode_chase_blackout_rainbow },
  { FSH(name_32), &WS2812FX::mode_color_sweep_random },
  { FSH(name_33), &WS2812FX::mode_running_color },
  { FSH(name_34), &WS2812FX::mode_running_red_blue },
  { FSH(name_35), &WS2812FX::mode_running_random },
  { FSH(name_36), &WS2812FX::mode_larson_scanner },
  { FSH(name_37), &WS2812FX::mode_comet },
  { FSH(name_38), &WS2812FX::mode_fireworks },
  { FSH(name_39), &WS2812FX::mode_merry_christmas },
  { FSH(name_40), &WS2812FX::mode_circus_combustus },
  { FSH(name_41), &WS2812FX::mode_halloween },
  { FSH(name_42), &WS2812FX::mode_bicolor_chase },
  { FSH(name_43), &WS2812FX::mode_tricolor_chase },
  { FSH(name_44), &WS2812FX::mode_rain },
  { FSH(name_45), &WS2812FX::mode_dissolve },
  { FSH(name_46), &WS2812FX::mode_icu },
  { FSH(name_47), &WS2812FX::mode_dual_larson },
  { FSH(name_48), &WS2812FX::mode_running_tails },
  { FSH(name_49), &WS2812FX::mode_filler_up },
  { FSH(name_50), &WS2812FX::mode_rainbow_larson },
  { FSH(name_51), &WS2812FX::mode_rainbow_fireworks },
  { FSH(name_52), &WS2812FX::mode_trifade },
  { FSH(name_53), &WS2812FX::mode_vu_meter },
  { FSH(name_54), &WS2812FX::mode_heartbeat },
  { FSH(name_55), &WS2812FX::mode_comets },
  { FSH(name_56), &WS2812FX::mode_popcorn },
  { FSH(name_57), &WS2812FX::mode_fire2012 },
  { FSH(name_58), &WS2812FX::mode_firefly },
  { FSH(name_59), &WS2812FX::mode_uaflag },
  { FSH(name_60), &WS2812FX::mode_ocean },
  { FSH(name_61), &WS2812FX::mode_orbita },
  { FSH(name_62), &WS2812FX::mode_machina },
  { FSH(name_63), &WS2812FX::mode_party_sky },
  { FSH(name_64), &WS2812FX::mode_disco },
  { FSH(name_65), &WS2812FX::mode_meteor },
  { FSH(name_66), &WS2812FX::mode_party_strobe },
  { FSH(name_67), &WS2812FX::mode_colorfall },
  { FSH(name_68), &WS2812FX::mode_split },
  { FSH(name_69), &WS2812FX::mode_vu_meter_peak },
  { FSH(name_70), &WS2812FX::mode_flying_split },
  { FSH(name_71), &WS2812FX::mode_portal },
  { FSH(name_72), &WS2812FX::mode_custom_0 },
  { FSH(name_73), &WS2812FX::mode_custom_1 },
  { FSH(name_74), &WS2812FX::mode_custom_2 },
  { FSH(name_75), &WS2812FX::mode_custom_3 },
  { FSH(name_76), &WS2812FX::mode_custom_4 },
  { FSH(name_77), &WS2812FX::mode_custom_5 },
  { FSH(name_78), &WS2812FX::mode_custom_6 },
  { FSH(name_79), &WS2812FX::mode_custom_7 }
  // { FSH(name_80), &WS2812FX:: 
};
#endif







