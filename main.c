#include <stdio.h>
#include <string.h>

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
unsigned char gfx[64*32]; //Pixel states 1 or 0, black or white.

/*Timers. If >0 count down to 0*/
unsigned char delay_timer;
unsigned char sound_timer; //Beep if sound_timer hits zero

/*Stack*/
unsigned short stack[16];
/*Stack pointer*/
unsigned short sp;

/*Keypad*/
unsigned char key[16]; /*0x0-0xF*/

/*Clockspeed*/
unsigned int clockspeed = 60;

                            /*Prototypes*/
/*Init emulator*/
int init();

/*one clock cycle*/
void clockcycle();
/*Reset and load rom into memory.*/
int loadROM(const char* path);                                  //Loads the rom into memory.
/*Reset memory.*/
int resetmem();                                                 //Resets memory, and calls init();
/*Memory Dump*/
void memdump(size_t from, size_t to, unsigned int lineSize);    //Dumps memory to std:out for debugging.

void clearscreen();
                            /*Functions*/


/*Main function*/
int main()
{
    loadROM("./ROMS/INVADERS");
    memdump(0,4096,16);
    return 0;
}

/*Init emulator*/
int init()
{
    PC = 0X200;
    return 0;
}

void clockcycle()
{
    switch(memory[PC] & 0x00FF)
    {
        case 0xE0:
            clearscreen();
            break;
        case 0xEE:
            //Return from subroutine call
            break;
        case 0xFB:
            //scroll screen 4 pixels right 	Super only,not implemented 
            break;
        case 0xFC:
            //scroll screen 4 pixels left 	Super only,not implemented 
            break;
        case 0xFE:
            //disable extended screen mode 	Super only 
            break;
        case 0xFF:
            //enable extended screen mode (128 x 64) 	Super only  
            break;
    }

    switch(memory[PC] & 0xF000)
    {
        case 0x01:
            //jump to address xxx 
            break;
        case 0x02:
            //jump to subroutine at address xxx 	16 levels maximum 
            break;
        case 0x03:
            //skip if register r = constant 
            break;
        case 0x04:
            //skip if register r <> constant 
            break;
        case 0x05:
            //skip f register r = register y 
            break;
        case 0x06:
            //move constant to register r 
            break;
        case 0x08:
            switch(memory[PC] & 0x000F)
            {
                case 0x00:
                    //move register vy into vr 
                    break;
                case 0x01:
                    //or register vy into register vx 
                    break;
                case 0x02:
                    //and register vy into register vx 
                    break;
                case 0x03:
                    //exclusive or register ry into register rx 
                    break:
                case 0x04:
                    //add register vy to vr,carry in vf 
                    break;
                case 0x05:
                    //subtract register vy from vr,borrow in vf 	vf set to 1 if borroesws 
                    break;
                case 0x06:
                    //shift register vy right, bit 0 goes into register vf 
                    break;
                case 0x07:
                    //shift register vr left,bit 7 goes into register vf 
                    break;
                case 0x0e:
                    //shift register vr left,bit 7 goes into register vf 
                    break;
            }
            break;
        case 0x09:
            //skip if register rx <> register ry 
            break;
        case 0x0a:
            //Load index register with constant xxx 
            break;
        case 0x0b:
            //Jump to address xxx+register v0 
            break;
        case 0x0c
            //vr = random number less than or equal to xxx 
            break;
        case 0x0d:
            switch(memory[PC] & 0x000F)
             {
                case 0x00:
                    //Draws extended sprite at screen location rx,ry 	As above,but sprite is always 16 x 16. Superchip only, not yet implemented
                    break;
                default:
                    //Draw sprite at screen location rx,ry height s 	Sprites stored in memory at location in index register, maximum 8 bits wide. Wraps around the screen. If when drawn,
                    //clears a pixel, vf is set to 1 otherwise it is zero. All drawing is xor drawing (e.g. it toggles the screen pixels 
                    break;
             }
            break;
        case 0x0e:
             switch(memory[PC] & 0x00FF)
             {
                case 0x9e:
                    //skip if key (register rk) pressed 	The key is a key number, see the chip-8 documentation 
                    break;
                case 0xa1:
                    //skip if key (register rk) not pressed 
                    break;
             }
            break;
        case 0x0f:
            switch(memory[PC] & 0X00ff)
            {
                case 0x07:
                    //get delay timer into vr 
                    break;
                case 0x0a:
                    //wait for for keypress,put key in register vr 
                    break;
                case 0x15:
                    //set the delay timer to vr               
                    break;
                case 0x18:
                    //set the sound timer to vr 
                    break;
                case 0x1e:
                    //add register vr to the index register 
                    break;
                case 0x29:
                    //point I to the sprite for hexadecimal character in vr 	Sprite is 5 bytes high 
                    break;
                case 0x30:
                    //point I to the sprite for hexadecimal character in vr 	Sprite is 10 bytes high,Super only 
                    break;
                case 0x33:
                    //store the bcd representation of register vr at location I,I+1,I+2 	Doesn't change I 
                    break;
                case 0x55:
                    //store registers v0-vr at location I onwards 	I is incremented to point to the next location on. e.g. I = I + r + 1 
                    break;
                case 0x65:
                    //load registers v0-vr from location I onwards 	as above. 
                    break;
            }
            break;
            default:
                //Invalid instruction
                break;
    }
    PC++;
}

/*Memory functions*/
int resetmem()
{
    memset(&memory[0], 0, sizeof(memory));  // Clear the whole memory
    return init();                          // Init everything for a clean start.
}

void memdump(size_t from, size_t to, unsigned int linesize)
{

    printf("MEMDUMP:\r\n");
    printf("0x%03X: ",from);
    for(size_t i = from; i < to; i++)
    {
        if(i % linesize == 0 && i > 0)
            printf("\r\n0x%03X: ", i);
            
         printf(" %02X",memory[i]);
    }
    printf("\r\n");
}

int loadROM(const char* path)
{
    FILE* rom = fopen(path,"rb");
    if(!rom)
        return -1;
    
    char c;
    size_t index = 0x200;                   //Start writing into memory at 0x200
    while((index < 0xFFF)
    {
        memory[index] = getc(rom);
        index++;
    }
    fclose(rom);                            //Close the file.
    return 0;
}
