#ifndef LM35CONFIG_H_
#define LM35CONFIG_H_
#include <avr/io.h>
#include "../../../macrodef.h"

#define LM35_PORT MKPORT(A)
#define LM35_DDR MKDDR(A)
#define LM35_LINE D7

#endif /* LM35CONFIG_H_ */