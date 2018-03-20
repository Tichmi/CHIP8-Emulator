#include <stdio.h>

/*4k Memory*/
unsigned char memory[4096];
/*
Memory map:
0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
0x200-0xFFF - Program ROM and work RAM
*/

/*General purpose registers
/* V[0x0]-V[0xE]. 
/* V[0x10]=Carry flag.
*/
unsigned char Vregisters[16];

/*Index register*/
unsigned short index;
/*Program counter*/
unsigned short pc;

/*Screen*/ 
unsigned char gfx[64*32]; /*Pixel states 1 or 0, black or white.*/

/*Timers. If >0 count down to 0*/
unsigned char delay_timer;
unsigned char sound_timer; /*Beep if sound_timer hits zero*/

/*Stack*/
unsigned short stack[16];
/*Stack pointer*/
unsigned short sp;

/*Keypad*/
unsigned char key[16]; /*0x0-0xF*/

/*Clockspeed*/
unsigned int Clockspeed = 60;

/*Init emulator*/
int init();
/*Reset and load rom into memory.*/
int loadROM(char* path);

/*Main function*/
int main()
{
    
    return 0;
}

/*Init emulator*/
int init()
{

}
