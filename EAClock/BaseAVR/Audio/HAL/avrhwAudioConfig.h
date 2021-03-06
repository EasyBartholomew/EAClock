#ifndef AVRHWAUDIOCONFIG_H_
#define AVRHWAUDIOCONFIG_H_
#include <avr/io.h>
#include "../../macrodef.h"

#define TARGET_PORT MKPORT(D)
#define TARGET_PIN D4
#define TARGET_DDR MKDDR(D)

#define SET_TH _SETH(TARGET_PORT, TARGET_PIN);
#define SET_TL _SETL(TARGET_PORT, TARGET_PIN);
#define SET_TI _ISH(TARGET_PORT, TARGET_PIN) ? _SETL(TARGET_PORT, TARGET_PIN) : _SETH(TARGET_PORT, TARGET_PIN);

#endif /* AVRHWAUDIOCONFIG_H_ */