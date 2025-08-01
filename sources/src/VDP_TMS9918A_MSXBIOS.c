/* ============================================================================= 
# VDP TMS9918A MSX BIOS Library (fR3eL Project)

- Version: 1.5 (22/07/2025)
- Author: mvac7/303bcn
- Architecture: MSX
- Format: SDCC Relocatable object file (.rel)
- Programming language: C and Z80 assembler
- Compiler: SDCC 4.4 or newer 

## Description:
C function library for working with the TMS9918A/28A/29A video processor.

This library contains a collection of functions for basic access to VDP 
capabilities, such as: initializing display modes, accessing registers, 
accessing video memory, and displaying sprites.

## History of versions (dd/mm/yyyy):
- 1.5 (22/07/2025) Changes for the new version of the VDP_SPRITES library.
	- Added GetSPRattrVRAM function
	- Added GetSpritePattern routine for Assembler Inline
	- Added ReadByteFromVRAM and WriteByteToVRAM labels for Assembler Inline
- v1.4 (12/06/2025) 
	- Added PUTSPRITE function
- v1.3 ( 1/12/2023)	Update to SDCC (4.1.12) Z80 calling conventions
- v1.2 (22/12/2020)	Conversion to source in C and added Sprite initialization 
					functions.
- v1.1 (14/02/2014) 
- v1.0 (11/02/2014) First version.
============================================================================= */ 

#include "../include/msxSystemVariables.h"
#include "../include/msxBIOS.h"

#include "../include/VDP_TMS9918A_MSXBIOS.h"



char SPRITE_BF[4];		//buffer with sprite data (Y,X,Pattern number,Color)



/* =============================================================================
SCREEN
Description:	
		Initializes the display to one of the four standardized modes on the MSX.
		Same as the SCREEN instruction in MSX BASIC.

Input:	[char] number of screen mode
			0 = TextMode1
			1 = Graphic1
			2 = Graphic2
			3 = MultiColor
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
	ld   A,#40  ;default value
	ld   (#LINL40),A 

	xor  A
  
setSCR:
	call BIOS_CHGMOD

	pop  IX
	ret
__endasm;
}    
  


/* =============================================================================
COLOR
Description: 
		Set the foreground, background, and border screen colors. 
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

	ld   (#FORCLR),A	//14c
	ld   A,L          	//5c
	ld   (#BAKCLR),A	//14c
	ld   A,4(IX)     	//21c
	ld   (#BDRCLR),A	//14c
//total ----------------->68c

	call BIOS_CHGCLR

	pop  IX
__endasm;
}



/* =============================================================================
CLS 
Description: 
		 Clear Screen
		 Fills the Name Table with the value 0
		 Note: Does not hide Sprite planes.
Input:	-
Output:	-
============================================================================= */
void CLS(void) __naked
{
__asm	

	ld   A,(#RG1SAV)	; --- read vdp(1) from mem
	bit  4,A			;M1
	jr   Z,CLS_NOTEXTMODE

;Clear TEXT mode
	xor  A
	ld   HL,#T1_MAP
	ld   BC,#0x3C0
	jp   BIOS_FILVRM
	
CLS_NOTEXTMODE:
	xor  A
	ld   HL,#G1_MAP
	ld   BC,#0x300
	jp   BIOS_FILVRM
	
__endasm;
}



/* =============================================================================
VPOKE
Description: 
		Writes a value to VRAM 
Input:	[unsigned int] VRAM address
		[char] value
Output:	- 
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
Description: 
		Reads a value from VRAM
Input:	[unsigned int] VRAM address
Output:	[char] value
============================================================================= */ 
char VPEEK(unsigned int vaddr) __naked
{
vaddr;	//HL
__asm
ReadByteFromVRAM::
	jp   BIOS_RDVRM


/* ------------------------------------------------
Label:	WriteByteToVRAM
Description: 
		Writes a value to VRAM 
Input:	[HL] VRAM address
		[A]  value
Output:	-
Note:	I add this routine here, since including it 
		in the VPOKE function would make it heavier
--------------------------------------------------- */
WriteByteToVRAM::
	jp   BIOS_WRTVRM	
__endasm;
}



/* =============================================================================
FillVRAM                               
Description: 
		Fill a large area of the VRAM of the same byte
Input:	[unsigned int] address of VRAM
		[unsigned int] blocklength
		[char] Value to fill.
Output:	- 
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
		Gets the value in a VDP register.
		Provides the mirror value stored in system variables.
Input:	[char] VDP register number             
Output:	[char] Value            
============================================================================= */
char GetVDP(char reg) __naked
{
reg;	//A
__asm 
	ld   IY,#RG0SAV		;Mirror of VDP register 0
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
		Writes a value to a VDP register
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

	jp   BIOS_WRTVDP
__endasm;
}




//############################################################################## SPRITEs functions



/* =============================================================================
ClearSprites
Description: 
		Initialises the Sprite Attribute Table (OAM) and Sprite Pattern Table.
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
	jr   NZ,SetSprSize8
	set  1,B ; 16x16
	jr   setVDPREG01
  
SetSprSize8:
	res  1,B  ; 8x8
	jr   setVDPREG01

__endasm;
}
  


/* =============================================================================
SetSpritesZoom
Description: Set zoom type for the sprites.
Input:	[char] or [boolean]/[switcher] zoom: 0/false/OFF = x1; 1/true/ON = x2
Output:	-
============================================================================= */
void SetSpritesZoom(char zoom) __naked
{
zoom;	//A
__asm  
	ld   HL,#RG1SAV ; --- read vdp(1) from mem
	ld   B,(HL)

	or   A
	jr   Z,SetSprNOzoom
	set  0,B ; zoom
	jr   setVDPREG01
  
SetSprNOzoom:
	res  0,B  ;nozoom    

setVDPREG01:  
	ld   C,#0x01
	jp   BIOS_WRTVDP
__endasm;
}



/* =============================================================================
PUTSPRITE
Description: 
		Displays a Sprite on the screen.
Input:	[char] sprite plane (0-31) 
		[char] X coordinate 
		[char] Y coordinate
		[char] color (0-15)
		[char] pattern number
Output:	-
============================================================================= */
void PUTSPRITE(char plane, char x, char y, char color, char pattern)
{
plane;x;y;color;pattern;
__asm
	push IX
	ld   IX,#0
	add  IX,SP

	ld   B,A	//plane
	ld   D,L	//x
	
	ld   HL,#_SPRITE_BF
	ld   C,4(IX)
	ld   (HL),C		//Y
	
	inc  HL
	ld   (HL),D		//X
	
	inc  HL
	ld   E,6(IX)	//Sprite pattern
	call GetSpritePattern	//Input:E; Output:A-->pattern position according to sprite size
	ld   (HL),A
	
	inc  HL
	ld   C,5(IX)
	ld   (HL),C		//color

//write Sprite Buffer to VRAM
	ld   A,B		//plane
	call GetSPRattrVADDR	//Input: A-->Sprite plane; Output: HL-->VRAM addr
	ld   DE,#_SPRITE_BF
	ex   DE,HL
	ld   BC,#4
	call BIOS_LDIRVM	//Block transfer to VRAM from memory
	
	pop  IX
	
__endasm;
}



/* =============================================================================
GetSPRattrVRAM
Description: 
		Gets the address in video memory of the Sprite attributes of specified 
		plane.
Input:	[char] sprite plane (0-31) 
Output:	[unsigned int] VRAM address
============================================================================= */
unsigned int GetSPRattrVRAM(char plane) __naked
{
plane;		//A
__asm
	call GetSPRattrVADDR	//Input: A-->Sprite plane; Output: HL-->VRAM addr
	ex   DE,HL
	ret						//return DE

GetSPRattrVADDR::
	jp   0x0087	
	
	
/* =============================================================================
GetSpritePattern
Description: 
		Returns the pattern value according to the Sprite size 
		(multiplied by 4 when its 16x16).
Input:	[E] sprite pattern 
Output: [A] pattern position
Regs:	A
============================================================================= */
GetSpritePattern::

	ld   A,(#RG0SAV+1)	// read vdp(1) from mem

	bit  1,A			//Sprite size; 1=16x16
	ld   A,E
	ret  Z				//same value

//if spritesize = 16x16 then E*4
    add  A				//(5ts)
	add  A				//multiply x 4	
	ret
	
__endasm;
}