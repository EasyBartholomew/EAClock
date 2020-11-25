#ifndef MACRODEF_H_
#define MACRODEF_H_
#include "globaldef.h"


#define HIGH_STATE TRUE
#define LOW_STATE FALSE

#define DDR_IN 0x00
#define DDR_OUT 0xff

#define DDR_SINGLE_IN LOW_STATE
#define DDR_SINGLE_OUT HIGH_STATE

#define __CLI asm("cli");
#define __SEI asm("sei");


/****************************************************/
/*      Determines high state of specified bit      */
/****************************************************/
#define _ISH(OBJ, NUM) ((OBJ) & (1 << (NUM)))


/****************************************************/
/*      Determines low state of specified bit       */
/****************************************************/
#define _ISL(OBJ, NUM) (!_ISH(OBJ, NUM))


/****************************************************/
/*              Sets high specified bit             */
/****************************************************/
#define _SETH(OBJ, NUM) ((OBJ) |= (HIGH_STATE << (NUM)))


/****************************************************/
/*              Sets low specified bit              */
/****************************************************/
#define _SETL(OBJ, NUM) ((OBJ) &= ~(HIGH_STATE << (NUM)))


/****************************************************/
/*  Sets specified bit to specified logical level   */
/****************************************************/
#define _SETLVL(OBJ, NUM, LVL) \
((LVL) ? _SETH(OBJ, NUM) : _SETL(OBJ, NUM))


/****************************************************/
/*          Sets high specified bit dynamically     */
/****************************************************/
#define _DSETH(OBJ, NUM) do { _SETH(OBJ,NUM); } while(0)


/****************************************************/
/*         Sets low specified bit dynamically       */
/****************************************************/
#define _DSETL(OBJ, NUM) do { _SETL(OBJ,NUM); } while(0)


/****************************************************/
/*                     Sets low                     */
/****************************************************/
#define _CL(OBJ) ((OBJ) = LOW_STATE)


/****************************************************/
/*           Sets value to specified object         */
/****************************************************/
#define _SETVAL(OBJ, VAL) ((OBJ) = (VAL))


/****************************************************/
/*    Sets value to specified object dynamically    */
/****************************************************/
#define _DSETVAL(OBJ, VAL) do { _SETVAL(OBJ, VAL); } while(0)


/****************************************************/
/*                    Data lines                    */
/****************************************************/
typedef enum : u8_t {
	D0,
	D1,
	D2,
	D3,
	D4,
	D5,
	D6,
	D7
} data_line_t;


/****************************************************/
/*                    Makes string                  */
/****************************************************/
#define STR(X) #X

#define SLIME(XX, YY) XX##YY


/****************************************************/
/*                    Makes PORT                    */
/****************************************************/
#define MKPORT(ALPHA) SLIME(PORT, ALPHA)


/****************************************************/
/*                    Makes DDR                     */
/****************************************************/
#define MKDDR(ALPHA) SLIME(DDR, ALPHA)


/****************************************************/
/*                    Makes PIN                     */
/****************************************************/
#define  MKPIN(ALPHA) SLIME(PIN, ALPHA)


/****************************************************/
/*             Casts to byte explicitly             */
/****************************************************/
#define TOBYTE(XVAL) ((u8_t)(XVAL))

#endif /* MACRODEF_H_ */