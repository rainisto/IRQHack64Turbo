#ifndef _IRQHACK64_
#include <avr/pgmspace.h>


#define _IRQHACK64_
#define IRQ 2 
#define EXROM 3
#define NMI 8
#define RESET 9
#define SEL 18

#define TYPE_MENU 0
#define TYPE_STANDARD_PRG 1
#define TYPE_CARTRIDGE 2
#define TYPE_BOOTER 3
#define TYPE_PRG_TRANSMISSION 4

//#define DEBUG

//#define NOOUT
#define USERAMLAUNCHER
#define PATCHPROCESSORPORTACCESS

#endif
