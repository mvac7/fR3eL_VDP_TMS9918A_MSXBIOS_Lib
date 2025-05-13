/* ============================================================================= 
# VDP TMS9918A MSX BIOS Library (fR3eL Project)

Version: 1.3 (1/12/2023)
Author: mvac7/303bcn [mvac7303b@gmail.com] 
Architecture: MSX
Format: C object (SDCC .rel)
Programming language: C and Z80 assembler
Compiler: SDCC 4.1.12 or newer 

## Description:
Open Source library of functions to work with the TMS9918A video processor
using BIOS functions 

## History of versions:
- v1.3 ( 1/12/2023)	update to SDCC (4.1.12) Z80 calling conventions
- v1.2 (22/12/2020)	Conversion to source in C and added Sprite initialization 
					functions.
- v1.1 (14/02/2014) 
- v1.0 (11/02/2014)                             
============================================================================= */ 

#include "../include/msxSystemVariables.h"
#include "../include/msxBIOS.h"

#include "../include/VDP_TMS9918A_MSXBIOS.h"




/* =============================================================================
SCREEN
Description:	Sets the display mode of the screen.
				T1 and G1 modes are initialized the map (Pattern Name Table) 
				with value 0. 
				In G2 and MC mode are initialized in an orderly manner (as in 
				MSX BASIC) to be able to display an image directly.
Input:	[char] number of screen mode
			0 = Text mode 1
			1 = Graphic 1
			2 = Graphic 2
			3 = MultiColor (MC)
Output:	-
============================================================================= */
void SCREEN(char mode) __naked
{
mode;
__asm
	push IX

	or   A
	jr   NZ,setSCR

	;screen 0 > 40 columns mode
	ld   A,#39  ;default value
	ld   (#LINL40),A 

	xor  A
  
setSCR:
	call BIOS_CHGMOD

	pop  IX
	ret
__endasm;
}    


/* =============================================================================
ClearSprites
Description: 
	Initialises the sprite attribute table (OAM). 
	The vertical location of the sprite is set to 209.
	The sprite pattern is cleared to null, the sprite number to the sprite 
	plane number, the sprite colour to the foregtound colour. The vertical 
	location of the sprite is set to 209 (mode 0 to 3) or 217 (mode 4 to 8).
Input:	-
Output:	-
============================================================================= */
void ClearSprites(void) __naked
{
__asm
	push IX
	
	call BIOS_CLRSPR
	
	pop  IX
	ret
__endasm;
} 




/* =============================================================================
SetSpritesSize
Description: Set size type for the sprites.
Input:	[char] size: 0=8x8; 1=16x16
Output:	-
============================================================================= */ 
void SetSpritesSize(char size) __naked
{
size;	//A
__asm
	ld   HL,#RG1SAV ; --- read vdp(1) from mem
	ld   B,(HL)

	cp   #1
	jr   NZ,size8
	set  1,B ; 16x16
	jr   setSize
  
size8:
	res  1,B  ; 8x8    

setSize:  
	ld   C,#0x01
	jp   BIOS_WRTVDP

__endasm;
}
  


/* =============================================================================
SetSpritesZoom
Description: Set zoom type for the sprites.
Input:	[char] or [boolean]/[switcher] zoom: 0/false/OFF = x1; 1/true/ON = x2
Output:	-
============================================================================= */
void SetSpritesZoom(boolean zoom) __naked
{
zoom;	//A
__asm  
	ld   HL,#RG1SAV ; --- read vdp(1) from mem
	ld   B,(HL)

	or   A
	jr   Z,nozoom
	set  0,B ; zoom
	jr   setZoom
  
nozoom:
	res  0,B  ;nozoom    

setZoom:  
	ld   C,#0x01
	jp   BIOS_WRTVDP

__endasm;
}

  


/* =============================================================================
COLOR
Description: Specifies the ink, foreground and background colors. 
Input:	[char] ink color
		[char] background color
		[char] border color
Output:	-     
============================================================================= */
void COLOR(char ink, char background, char border)
{
ink;		//A
background;	//L
border;		//STack
__asm
	push IX
	ld   IX,#0
	add  IX,SP

	ld   (#FORCLR),A	;14c
	ld   A,L          ;5c
	ld   (#BAKCLR),A	;14c
	ld   A,4(IX)     	;21c
	ld   (#BDRCLR),A	;14c
;total ------------->68c

	call BIOS_CHGCLR

	pop  IX
__endasm;
}



/* =============================================================================
 VPOKE
 Description: Writes a byte to the video RAM. 
 Input      : [unsigned int] VRAM address
              [char] value
 Output     : - 
============================================================================= */
void VPOKE(unsigned int vaddr, char value)
{
vaddr;	//HL
value;	//A
__asm
	push IX
	ld   IX,#0
	add  IX,SP

	ld   A,4(IX) 
	call BIOS_WRTVRM

	pop  IX
__endasm;
}



/* =============================================================================
VPEEK
Description: Reads data from the video RAM. 
Input:	[unsigned int] VRAM address
Output:	[char] value
============================================================================= */ 
char VPEEK(unsigned int vaddr) __naked
{
vaddr;	//HL
__asm
	jp BIOS_RDVRM
__endasm;
}



/* =============================================================================
 FillVRAM                               
 Description: Fill a large area of the VRAM of the same byte.
 Input      : [unsigned int] address of VRAM
              [unsigned int] blocklength
              [char] Value to fill.
 Output     : - 
============================================================================= */
void FillVRAM(unsigned int vaddr, unsigned int length, char value)
{
vaddr;  //HL
length; //DE
value;  //STack
__asm
	push IX
	ld   IX,#0
	add  IX,SP

	ld   C,E
	ld   B,D

	ld   A,4(IX) ;value

	call BIOS_FILVRM

	pop  IX
__endasm;
}



/* =============================================================================
 CopyToVRAM
 Description: Block transfer from memory to VRAM 
 Input      : [unsigned int] address of RAM
              [unsigned int] address of VRAM
              [unsigned int] blocklength
 Output     : - 
============================================================================= */
void CopyToVRAM(unsigned int addr, unsigned int vaddr, unsigned int length)
{
addr;	//HL
vaddr;	//DE
length;	//Stack
__asm
	push IX
	ld   IX,#0
	add  IX,SP 

	ld   C,4(IX) ;length
	ld   B,5(IX)

	call BIOS_LDIRVM

	pop  IX
__endasm;
}



/* =============================================================================
 CopyFromVRAM
 Description: Block transfer from VRAM to memory
 Input      : [unsigned int] address of VRAM                     
              [unsigned int] address of RAM
              [unsigned int] blocklength
 Output     : -             
============================================================================= */
void CopyFromVRAM(unsigned int vaddr, unsigned int addr, unsigned int length)
{
vaddr;	//HL
addr;	//DE
length;	//Stack
__asm
	push IX
	ld   IX,#0
	add  IX,SP

	ld   C,4(IX) ;length
	ld   B,5(IX)

	call BIOS_LDIRMV

	pop  IX  
__endasm;
}
  


/* =============================================================================
GetVDP
Description: 	
	Provides the mirror value of a VDP register stored in system variables.
Input:	[char] VDP register              
Output:	[char] Value            
============================================================================= */
char GetVDP(char reg) __naked
{
reg;	//A
__asm 
	ld   IY,#RG0SAV		;Mirror of VDP register 1
	ld   E,A
	ld   D,#0
	add  IY,DE
	ld   A,(IY)
	ret
__endasm;		
}



/* =============================================================================
SetVDP
Description: 
	Writes a value in VDP registers
Input:	[char] VDP register                     
		[char] value
Output:	-             
============================================================================= */
void SetVDP(char reg, char value) __naked
{
reg;	//A
value;	//L
__asm
	ld   C,A ;VDP reg    
	ld   B,L

	call BIOS_WRTVDP

	ret
__endasm;
}