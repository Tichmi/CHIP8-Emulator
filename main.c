#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>

<<<<<<< HEAD

=======
#define WIDTH 64
#define HEIGHT 32
>>>>>>> SDL
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
const unsigned char width = WIDTH;
const unsigned char height = HEIGHT;
unsigned char gfx[WIDTH*HEIGHT]; //Pixel states 1 or 0, black or white.

/*Timers. If >0 count down to 0*/
unsigned char delay_timer;
unsigned char sound_timer; //Beep if sound_timer hits zero

/*Stack*/
unsigned short stack[16];
/*Stack pointer*/
unsigned short SP;

/*Keypad*/
unsigned char key[16]; /*0x0-0xF*/

/*Clockspeed*/
unsigned int clockspeed = 60;

/*SDL*/
int w_width = 640;
int w_height = 320;

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
/*Push to stack*/
int pushstack(unsigned char value);
/*Pop from stack*/
unsigned char popstack();

void clearscreen();
                            /*Functions*/


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

/*Draw a new frame to the window*/
void drawframe(SDL_Renderer* renderer)
{
    // Clear before drawing
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
    for(int y = 0; y < 32; y++)
    {
        for(int x = 0; x < 64; x++)
        {
            if(gfx[x + y * 64])
            {
                SDL_RenderDrawPoint(renderer, x, y);
            }  
        }
    }
    SDL_RenderPresent(renderer);
    return;
}

void drawingtest(SDL_Renderer* renderer)
{
    for(int i = 0; i < 32; i++)
        {
            gfx[i*64+i]=0x01;
            drawframe(renderer);
            SDL_Delay(16);
        }
    for(int i = 63; i >= 32; i--)
        {
            gfx[(63-i)*64+i]=0x01;
            drawframe(renderer);
            SDL_Delay(16);
        }
}

/*Main function*/
int main()
{
    srand(time(NULL)); /*seed for random*/
    loadROM("./ROMS/INVADERS");
    //memdump(0,4096,16);
    // Initialize SDL2
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO); 
    SDL_Window *window;
    SDL_Renderer* renderer;
    window = SDL_CreateWindow
    (
        "CHIP-8 Emulator", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        w_width, 
        w_height,
        SDL_WINDOW_OPENGL
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, 64, 32);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    
    drawingtest(renderer);


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


int pushstack(unsigned char value)
{
    size_t elements = sizeof(stack) / sizeof(unsigned char);
    if(elements >= sizeof(stack))           //Check if stack is already full
        return -1;                          //Return error
    else
        stack[elements] = value;            //Set top of the stack to the value.
    return 0;                               //Return without error
}

unsigned char popstack()
{
    size_t elements = sizeof(stack) / sizeof(unsigned char);
    unsigned char ret = stack[elements];    //Get element from top and store it.
    stack[elements] = 0;                    //Erase the element
    return ret;                             //Return the element.
}

void clearscreen()
{
     memset(&gfx[0], 0, sizeof(gfx));
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
            PC+=2;
            break;
        case 0xEE:
            PC = popstack(); //Return from subroutine call
            PC+=2;
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

    switch((memory[PC] & 0xF000) >> 12)
    {
        case 0x01:
            PC = memory[PC] & 0x0FFF; //jump to address xxx 
            break;
        case 0x02:
            //jump to subroutine at address xxx 	16 levels maximum 
            pushstack(PC + 1);
            PC = memory[PC] & 0x0FFF;
            break;
        case 0x03:
            //skip if register r = constant 
            if(V[(memory[PC] & 0x0F00) >> 8] == memory[PC] & 0x00FF)
                PC += 4;
            else
                PC+=2;
            break;
        case 0x04:
            //skip if register r != constant 
            if(V[(memory[PC] & 0x0F00) >> 8] != memory[PC] & 0x00FF)
                PC += 4;
            else
                PC+=2;
            break;
        case 0x05:
            //skip if register r = register y 
            if(V[(memory[PC] & 0x0F00) >> 8] == V[(memory[PC] & 0x0F0) >> 4])
                PC += 4; //Skip
            else
                PC+=2;
            break;
        case 0x06:
            //move constant to register r 
            V[(memory[PC] & 0x0F00) >> 8] = memory[PC] & 0x00FF;
            PC+=2;
            break;
        case 0x07:
            //add constant to register r 	No carry generated 
            V[(memory[PC] & 0x0F00) >> 8] += memory[PC] & 0x00FF;
            PC+=2;
            break;
        case 0x08:
            switch(memory[PC] & 0x000F)
            {
                case 0x00:
                    //move register vy into vr 
                    V[(memory[PC] & 0x0F00) >> 8] = V[(memory[PC] & 0x00F0) >> 4];
                    PC+=2;
                    break;
                case 0x01:
                    //or register vy into register vx 
                    V[(memory[PC] & 0x0F00) >> 8] = V[(memory[PC] & 0x0F00) >> 8] | V[(memory[PC] & 0x00F0) >> 4];
                    PC+=2;
                    break;
                case 0x02:
                    //and register vy into register vx 
                    V[(memory[PC] & 0x0F00) >> 8] = V[(memory[PC] & 0x0F00) >> 8] & V[(memory[PC] & 0x00F0) >> 4];
                    PC+=2;
                    break;
                case 0x03:
                    //exclusive or register ry into register rx 
                    V[(memory[PC] & 0x0F00) >> 8] = V[(memory[PC] & 0x0F00) >> 8] ^ V[(memory[PC] & 0x00F0) >> 4];
                    PC+=2;
                    break;
                case 0x04:
                    //add register vy to vr,carry in vf 
                    if(V[(memory[PC] & 0x00F0) >> 4] > (0xFF - V[(memory[PC] & 0x0F00) >> 8]))
                        V[0xF] = 1; //carry
                    else
                        V[0xF] = 0;
                    V[(memory[PC] & 0x0F00) >> 8] += V[(memory[PC] & 0x00F0) >> 4];              
                    PC+=2;
                    break;
                case 0x05:
                    //subtract register vy from vr,borrow in vf     vf set to 1 if borroesws 
                    if(V[(memory[PC] & 0x00F0) >> 4] > (V[(memory[PC] & 0x0F00) >> 8]))
                        V[0xF] = 1; //borrow
                    else
                        V[0xF] = 0;
                    V[(memory[PC] & 0x0F00) >> 8] -= V[(memory[PC] & 0x00F0) >> 4];              
                    PC+=2;
                    break;
                case 0x06:
                    //shift register vy right, bit 0 goes into register vf 
                    V[0xF] = V[(memory[PC] & 0x0F00) >> 8] & 0b00000001;
                    V[(memory[PC] & 0x0F00) >> 8] = V[(memory[PC] & 0x0F00) >> 8] >> 1;
                    PC += 2;
                    break;
                case 0x07:
                    //subtract register vr from register vy, result in vr 	vf set to 1 if borrows 
                        if(V[(memory[PC] & 0x0F00) >> 8] > V[(memory[PC] & 0x00F0) >> 4])
                        V[0xF] = 1; //borrow
                    else
                        V[0xF] = 0;
                    V[(memory[PC] & 0x00F0) >> 4] -= V[(memory[PC]& 0x0F00) >> 8];              
                    PC+=2;
                    break;
                case 0x0e:
                    //shift register vr left,bit 7 goes into register vf 
                    V[0xF] = ((V[(memory[PC] & 0x0F00) >> 8] & 0b10000000) >> 7);
                    V[(memory[PC] & 0x0F00) >> 8] = V[(memory[PC] & 0x0F00) >> 8] >> 1;
                    PC += 2;
                    break;
            }
            break;
        case 0x09:
            //skip if register rx <> register ry 
            if(V[(memory[PC] & 0x0F00) >> 8] != V[(memory[PC] & 0x00F0) >> 4])
                PC+= 4;
            else
                PC+= 2;
            break;
        case 0x0a:
            //Load index register with constant xxx 
            I = (memory[PC] & 0x0FFF);
            PC+=2;
            break;
        case 0x0b:
            //Jump to address xxx+register v0 
            PC = (memory[PC] & 0x0FFF) + V[0];
            break;
        case 0x0c:
            //vr = random number less than or equal to xxx 
            V[(memory[PC] & 0x0F00) >> 8] = rand() % ((memory[PC] & 0x00FF)+1);
            PC+=2;
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
            switch(memory[PC] & 0X00FF)
            {
                case 0x07:
                    //get delay timer into vr 
                    V[memory[PC] & 0x0F00 >> 8] = delay_timer;
                    PC+=2;
                    break;
                case 0x0a:
                    //wait for for keypress,put key in register vr 

                    break;
                case 0x15:
                    //set the delay timer to vr   
                    delay_timer =  V[memory[PC] & 0x0F00 >> 8];     
                    PC+=2;      
                    break;
                case 0x18:
                    //set the sound timer to vr 
                    sound_timer =  V[memory[PC] & 0x0F00 >> 8];     
                    PC+=2;
                    break;
                case 0x1e:
                    //add register vr to the index register 
                    I +=  V[memory[PC] & 0x0F00 >> 8];     
                    break;
                case 0x29:
                    //point I to the sprite for hexadecimal character in vr 	Sprite is 5 bytes high 
                    break;
                case 0x30:
                    //point I to the sprite for hexadecimal character in vr 	Sprite is 10 bytes high,Super only 
                    break;
                case 0x33:
                    //store the bcd representation of register vr at location I,I+1,I+2 	Doesn't change I 
                    memory[I]     = (V[(memory[PC] & 0x0F00) >> 8] / 100);
                    memory[I + 1] = (V[(memory[PC] & 0x0F00) >> 8] / 10) % 10;
                    memory[I + 2] = (V[(memory[PC] & 0x0F00) >> 8] % 100) % 10;
                    PC += 2;
                    break;
                case 0x55:
                    //store registers v0-vr at location I onwards 	I is incremented to point to the next location on. e.g. I = I + r + 1 
                    for(size_t i = 0; i <= (memory[PC] & 0x0F00) >> 8; i++)
                    {
                        memory[I] = V[i];
                        I+=1;
                    }
                    break;
                case 0x65:
                    //load registers v0-vr from location I onwards 	as above. 
                    for(size_t i = 0; i <= (memory[PC] & 0x0F00) >> 8; i++)
                    {
                        V[i] = memory[I];
                        I+=1;
                    }
                    break;
            }
            break;
            default:
                //Invalid instruction
                break;
    }
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
    printf("0x%03X: ",(unsigned int)from);
    for(size_t i = from; i < to; i++)
    {
        if(i % linesize == 0 && i > 0)
            printf("\r\n0x%03X: ", (unsigned int)i);
            
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
    while((c = getc(rom)) != EOF)
    {
        memory[index] = getc(rom);
        index++;
    }
    fclose(rom);                            //Close the file.
    return 0;
}
