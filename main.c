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
unsigned char V[16];

/*Index register*/
unsigned short I;
/*Program counter*/
unsigned short PC;

/*Screen*/ 
unsigned char gfx[64*32]; /*Pixel states 1 or 0, black or white.*/

/*Timers. If >0 count down to 0*/
unsigned char delay_timer;
unsigned char sound_timer; /*Beep if sound_timer hits zero*/

/*Stack*/
unsigned short stack[16];
/*Stack pointer*/
unsigned short SP;

/*Keypad*/
unsigned char key[16]; /*0x0-0xF*/

/*Clockspeed*/
unsigned int clockspeed = 60;

/*Init emulator*/
int init();
/*Reset and load rom into memory.*/
int loadROM(char* path);

/*Print screen to stdout*/
void printscreen()
{
    for(int y = 0; y < 32; y++)
    {
        for(int x = 0; x < 64; x++)
        {
            if(gfx[x + y * 64])
                putchar('#');
            else
                putchar(' ');
        }
        putchar('\n');
    }
    return;
}

/*Main function*/
int main()
{

    return 0;
}

/*Init emulator*/
int init()
{

}
