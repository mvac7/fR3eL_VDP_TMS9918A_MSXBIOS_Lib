# VDP TMS9918A MSXBIOS SDCC Library (fR3eL Project)

<table>
<tr><td>Architecture</td><td>MSX</td></tr>
<tr><td>Format</td><td>C Object (SDCC .rel)</td></tr>
<tr><td>Programming language</td><td>C and Z80 assembler</td></tr>
<tr><td>Compiler</td><td>SDCC v4.4 or newer</td></tr>
</table>
 

## Description

Open Source library with basic functions to work with the TMS9918A video processor.

It uses the functions from the MSX BIOS, so it is designed to create applications in ROM or MSXBASIC environments.

If you need specific functions to manage Sprites, you can use any of the following libraries:
- [SPRITES Small](https://github.com/mvac7/SDCC_VDP_SPRITES_S_MSXROM_Lib) - Basic functions for managing Sprites. It is more compact so it takes up less space in our application.
- [SPRITES](https://github.com/mvac7/SDCC_VDP_SPRITES_MSXROM_Lib) - It includes the same functions as the Small version and adds to access specific parameters (positioning, color, pattern, visibility and EarlyClock).
- [SPRITES 1/2](https://github.com/mvac7/SDCC_VDP_SPRITES_12_MSXROM_Lib) - Same as SPRITES but in the G3 screen mode (V9938), it treats the color parameters in a simplified way. Assign the color parameters and EarlyClock to all the lines of the Sprites.
- SPRITES DUMP (In development) - Uses a buffer in RAM that has to be dumped in each interruption. Includes a Sprite Flicker, to allow viewing up to 8 sprites per line.

You also have the [VDP PRINT MSX SDCC Library](https://github.com/mvac7/SDCC_VDP_PRINT_Lib), with functions for display text strings in the graphic modes of the TMS9918A (G1 and G2).

Use them for developing MSX applications using [Small Device C Compiler (SDCC)](http://sdcc.sourceforge.net/) cross compiler.

You can access the documentation here with [`How to use the library`](docs/HOWTO.md).

These libraries are part of the [MSX fR3eL Project](https://github.com/mvac7/SDCC_MSX_fR3eL).

This project is an Open Source. 
You can add part or all of this code in your application development or include it in other libraries/engines.

Enjoy it!   

<br/>

---

## History of versions

- v1.3 ( 1/12/2023) update to SDCC (4.1.12) Z80 calling conventions
- v1.2 (22/12/2020) Conversion to source in C and added Sprite initialization functions.
- v1.1 (14/02/2014)
- v1.0 (11/02/2014)

<br/>

---

## Requirements

- [Small Device C Compiler (SDCC) v4.4](http://sdcc.sourceforge.net/)
- [Hex2bin v2.5](http://hex2bin.sourceforge.net/)

<br/>

---

## Functions

- void **SCREEN**(char) - Sets the display mode of the screen.
- void **ClearSprites**() - Initialises the sprite attribute table.
- void **SetSpritesSize**(char size) - Set size type for the sprites.
- void **SetSpritesZoom**(boolean zoom) - Set zoom type for the sprites.
- void **COLOR**(char, char, char) - Specifies the ink, foreground and background colors.
- void **VPOKE**(unsigned int, char) - Writes a byte to the video RAM.
- char **VPEEK**(unsigned int) - Reads data from the video RAM.
- void **FillVRAM**(unsigned int, unsigned int, char) - Fill a large area of the VRAM of the same byte.
- void **CopyToVRAM**(unsigned int, unsigned int, unsigned int) - Block transfer from memory to VRAM.
- void **CopyFromVRAM**(unsigned int, unsigned int, unsigned int) - Block transfer from VRAM to memory.
- void **SetVDP**(char, char) - Writes a value in VDP registers.

<br/>

---

## Documentation

- Texas Instruments TMS9918A application manual [`PDF`](http://map.grauw.nl/resources/video/texasinstruments_tms9918.pdf)
- Texas Instruments VDP Programmer’s Guide [`PDF`](http://map.grauw.nl/resources/video/ti-vdp-programmers-guide.pdf)
- Texas Instruments TMS9918A VDP by Sean Young [`TXT`](http://bifi.msxnet.org/msxnet/tech/tms9918a.txt)
- The MSX Red Book · [2 Video Display Processor](https://github.com/gseidler/The-MSX-Red-Book/blob/master/the_msx_red_book.md#chapter_2)

<br/>

---

## Examples
 
The project includes several examples that I have used to test the library and that can help you learn how to use this library.

You can find them in this project in the [`../examples/`](examples/) folder.

<br/>

### Example 1

This example is very simple. Shows a use case for initializing a screen in graphics mode 1.

<br/>

### Example 2

This example tests all of the library's functions in the four screen modes of the TMS9918A.
Test the functionality of the library in a system environment for ROM (BIOS+ROM+RAM+RAM).
