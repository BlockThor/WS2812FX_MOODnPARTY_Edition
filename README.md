<p align="center"> <img src="https://github.com/BlockThor/WS2812FX_MOODnPARTY_Edition/blob/main/WS2812FX_MnP_logo.png" align="center" width="500"/> </p>

# WS2812FX MOOD'n'PARTY - Updated, Upgraded, and Remastered

WS2812FX MOOD'n'PARTY is a modified version of the WS2812FX library, enhanced for use with the "**MOOD'n'PARTY Lights**" single-page web interface.

## Features

- All effects now look good with the default speed set to 1000 (one move per second, or 60 bpm).
- Many effects now have three sub-modes: **MonoColor** (if the second color is the same as the background color), **DuoColor** (default), and **RainbowColor** (if the first color is the same as the background color).
- Non-symmetrical effects can be Reversed.
- Many effects utilize `SIZE_OPTION` and `FADE_RATE`.

### Example
Mode **Static**: 
- **MonoMode**: Filled with solid color - classic static mode.
- **DuoMode**: 
  * `DIRECT`: 
    + `SIZE_SMALL` : filled with gradient from Main Color to Second Color;
    + `SIZE_MEDIUM` : filled with 2 sections of gradient;
    + `SIZE_LARGE` : filled with 4 sections of gradient;
    + `SIZE_XLARGE` : filled with 8 sections of gradient;
  * `REVERSE` : 
    + `SIZE_SMALL` : filled with gradient from Second Color to Main Color;
    + `SIZE_MEDIUM` : filled with 2 sections of gradient;
    + `SIZE_LARGE` : filled with 4 sections of gradient;
    + `SIZE_XLARGE` : filled with 8 sections of gradient;
- **RGBMode**: 
  * `DIRECT`: 
    + `SIZE_SMALL` : filled with rainbow gradient from Red to Violet;
    + `SIZE_MEDIUM` : filled with 2 sections of gradient;
    + ...
  * `REVERSE` : 
    + `SIZE_SMALL` : filled with gradient from Violet ro Red;
    + `SIZE_MEDIUM` : filled with 2 sections of gradient;
    + ...

## New Effects

### Upgraded Effect

- **VU Meter Peak**: Similar to the old VU Meter but now includes Peak. _Mono:no; Due:ok; RGB:no; Reverse:ok;_
- **Xmas Glitter**: Similar to **Xmas Twinkle** (old name: TwinkleFOX) but changes Hue Color instead of brightness. _Mono:ok; Due:ok; RGB:ok; Reverse:ok;_

### CopyPasta Effect

- **Fire2012**: Adaptation from Fire2012 by Mark Kriegsman. _Mono:ok; Due:no*; RGB:ok; Reverse:ok; (* default: CoolWhite color changed to WarmWhite)_.
- **Meteor**: Inspired by [Tweaking4All.com](https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectMeteorRain) – "It’s a kind of meteor falling from the sky." _Mono:ok; Due:ok; RGB:ok; Reverse:ok;_

### Original Effect

- **FireFly**: Flying light spot. _Mono:ok; Due:no*; RGB:ok; Reverse:no (*special effect: default WarmWhite color)_.
- **Ocean**: Imitating ocean waves. _Mono:ok; Due:no*; RGB:ok; Reverse:ok (*special effect: default Ocean color)_.
- **Orbita**: Three flying objects on their orbit with a small variety of speed. _Mono:ok; Due:no*; RGB:ok; Reverse:ok (*Three objects with two colors, mid object combines Main and Second colors)_.
- **Machina**: Three objects in Red, Green, and Blue moving randomly fore and back, mixing with each other. _Mono:ok; Due:no*; RGB:ok; Reverse:ok; (* default colors are saturated, looks more pastel)_.
- **Party Sky**: Group of random LEDs light up in the same color and fade out with rare, brighter stars in the same color. _Mono:ok; Due:ok; RGB:ok; Reverse:no;_
- **Disco**: Line of LEDs randomly light up and fade out. _Mono:ok; Due:ok; RGB:ok; Reverse:ok;_
- **Party Strob**: Classic strob with random delay and random color. _Mono:no; Due:no; RGB:ok; Reverse:no;_
- **Colorfall**: Waves of colors falling down like a waterfall. _Mono:no; Due:no; RGB:ok; Reverse:ok;_
- **Split**: Same as Colorfall but split in the middle. _Mono:no; Due:no; RGB:ok; Reverse:no;_
- **Flying Spirit**: Modification of Split with a moved core and fading out wings. _Mono:no; Due:no; RGB:ok; Reverse:no;_

### Special Effect

- **UA Flag**: Ukrainian Flag waved in the wind. _Mono:no; Due:no*; RGB:ok; Reverse:ok; (*default colors are LightBlue and WarmYellow)_

### Template

>- **Mode** - Description. _Mono:ok; Due:ok; RGB:ok; Reverse:ok;_

## ToDo

- [ ] Change DuoMode to PaletteMode (Maybe, not sure yet).
- [ ] Add marks (categories) for effects to indicate if they can be used for Mono/Duo/Rainbow/Special modes, Reversible/Element size/Fade rate.

## Download and Install

1. Install the [Adafruit NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel) (v1.1.7 or newer).
2. Download this repository.
3. Extract the contents into your Arduino libraries directory.
4. Open the Arduino IDE.
