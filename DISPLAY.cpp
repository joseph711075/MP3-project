// DISPLAY.cpp

#include <stdlib.h>
#include "DISPLAY.h"
#include "stdio.h"
#include "ff.h"
/*
Graphic LCD Font (Ascii Charaters)
creddit: https://github.com/adafruit/monochron/blob/master/firmware/font5x7.h
*/

static unsigned char letterBank[96][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00},// _
    {0x00, 0x00, 0x5F, 0x00, 0x00},// !
    {0x00, 0x07, 0x00, 0x07, 0x00},// "
    {0x14, 0x7F, 0x14, 0x7F, 0x14},// #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12},// $
    {0x23, 0x13, 0x08, 0x64, 0x62},// %
    {0x36, 0x49, 0x55, 0x22, 0x50},// &
    {0x00, 0x05, 0x03, 0x00, 0x00},// '
    {0x00, 0x1C, 0x22, 0x41, 0x00},// (
    {0x00, 0x41, 0x22, 0x1C, 0x00},// )
    {0x08, 0x2A, 0x1C, 0x2A, 0x08},// *
    {0x08, 0x08, 0x3E, 0x08, 0x08},// +
    {0x00, 0x50, 0x30, 0x00, 0x00},// ,
    {0x08, 0x08, 0x08, 0x08, 0x08},// -
    {0x00, 0x60, 0x60, 0x00, 0x00},// .
    {0x20, 0x10, 0x08, 0x04, 0x02},// /
    {0x3E, 0x51, 0x49, 0x45, 0x3E},// 0
    {0x00, 0x42, 0x7F, 0x40, 0x00},// 1
    {0x42, 0x61, 0x51, 0x49, 0x46},// 2
    {0x21, 0x41, 0x45, 0x4B, 0x31},// 3
    {0x18, 0x14, 0x12, 0x7F, 0x10},// 4
    {0x27, 0x45, 0x45, 0x45, 0x39},// 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30},// 6
    {0x01, 0x71, 0x09, 0x05, 0x03},// 7
    {0x36, 0x49, 0x49, 0x49, 0x36},// 8
    {0x06, 0x49, 0x49, 0x29, 0x1E},// 9
    {0x00, 0x36, 0x36, 0x00, 0x00},// :
    {0x00, 0x56, 0x36, 0x00, 0x00},// ;
    {0x00, 0x08, 0x14, 0x22, 0x41},// <
    {0x14, 0x14, 0x14, 0x14, 0x14},// =
    {0x41, 0x22, 0x14, 0x08, 0x00},// >
    {0x02, 0x01, 0x51, 0x09, 0x06},// ?
    {0x32, 0x49, 0x79, 0x41, 0x3E},// @
    {0x7E, 0x11, 0x11, 0x11, 0x7E},// A
    {0x7F, 0x49, 0x49, 0x49, 0x36},// B
    {0x3E, 0x41, 0x41, 0x41, 0x22},// C
    {0x7F, 0x41, 0x41, 0x22, 0x1C},// D
    {0x7F, 0x49, 0x49, 0x49, 0x41},// E
    {0x7F, 0x09, 0x09, 0x01, 0x01},// F
    {0x3E, 0x41, 0x41, 0x51, 0x32},// G
    {0x7F, 0x08, 0x08, 0x08, 0x7F},// H
    {0x00, 0x41, 0x7F, 0x41, 0x00},// I
    {0x20, 0x40, 0x41, 0x3F, 0x01},// J
    {0x7F, 0x08, 0x14, 0x22, 0x41},// K
    {0x7F, 0x40, 0x40, 0x40, 0x40},// L
    {0x7F, 0x02, 0x04, 0x02, 0x7F},// M
    {0x7F, 0x04, 0x08, 0x10, 0x7F},// N
    {0x3E, 0x41, 0x41, 0x41, 0x3E},// O
    {0x7F, 0x09, 0x09, 0x09, 0x06},// P
    {0x3E, 0x41, 0x51, 0x21, 0x5E},// Q
    {0x7F, 0x09, 0x19, 0x29, 0x46},// R
    {0x46, 0x49, 0x49, 0x49, 0x31},// S
    {0x01, 0x01, 0x7F, 0x01, 0x01},// T
    {0x3F, 0x40, 0x40, 0x40, 0x3F},// U
    {0x1F, 0x20, 0x40, 0x20, 0x1F},// V
    {0x7F, 0x20, 0x18, 0x20, 0x7F},// W
    {0x63, 0x14, 0x08, 0x14, 0x63},// X
    {0x03, 0x04, 0x78, 0x04, 0x03},// Y
    {0x61, 0x51, 0x49, 0x45, 0x43},// Z
    {0x00, 0x00, 0x7F, 0x41, 0x41},// [
    {0x02, 0x04, 0x08, 0x10, 0x20},// "\"
    {0x41, 0x41, 0x7F, 0x00, 0x00},// ]
    {0x04, 0x02, 0x01, 0x02, 0x04},// ^
    {0x40, 0x40, 0x40, 0x40, 0x40},// _
    {0x00, 0x01, 0x02, 0x04, 0x00},// `
    {0x20, 0x54, 0x54, 0x54, 0x78},// a
    {0x7F, 0x48, 0x44, 0x44, 0x38},// b
    {0x38, 0x44, 0x44, 0x44, 0x20},// c
    {0x38, 0x44, 0x44, 0x48, 0x7F},// d
    {0x38, 0x54, 0x54, 0x54, 0x18},// e
    {0x08, 0x7E, 0x09, 0x01, 0x02},// f
    {0x08, 0x14, 0x54, 0x54, 0x3C},// g
    {0x7F, 0x08, 0x04, 0x04, 0x78},// h
    {0x00, 0x44, 0x7D, 0x40, 0x00},// i
    {0x20, 0x40, 0x44, 0x3D, 0x00},// j
    {0x00, 0x7F, 0x10, 0x28, 0x44},// k
    {0x00, 0x41, 0x7F, 0x40, 0x00},// l
    {0x7C, 0x04, 0x18, 0x04, 0x78},// m
    {0x7C, 0x08, 0x04, 0x04, 0x78},// n
    {0x38, 0x44, 0x44, 0x44, 0x38},// o
    {0x7C, 0x14, 0x14, 0x14, 0x08},// p
    {0x08, 0x14, 0x14, 0x18, 0x7C},// q
    {0x7C, 0x08, 0x04, 0x04, 0x08},// r
    {0x48, 0x54, 0x54, 0x54, 0x20},// s
    {0x04, 0x3F, 0x44, 0x40, 0x20},// t
    {0x3C, 0x40, 0x40, 0x20, 0x7C},// u
    {0x1C, 0x20, 0x40, 0x20, 0x1C},// v
    {0x3C, 0x40, 0x30, 0x40, 0x3C},// w
    {0x44, 0x28, 0x10, 0x28, 0x44},// x
    {0x0C, 0x50, 0x50, 0x50, 0x3C},// y
    {0x44, 0x64, 0x54, 0x4C, 0x44},// z
    {0x00, 0x08, 0x36, 0x41, 0x00},// {
    {0x00, 0x00, 0x7F, 0x00, 0x00},// |
    {0x00, 0x41, 0x36, 0x08, 0x00},// }
    {0x08, 0x08, 0x2A, 0x1C, 0x08},// ->
    {0x08, 0x1C, 0x2A, 0x08, 0x08} // <-
};


// initially BLACK SCREEN buffer
static uint8_t buffer[(128 * 64)] =
{
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


//default constructor init

DISPLAY::DISPLAY(){
    i2c.init(200); //speed = 100 KHz
    if(i2c.checkDeviceResponse(I2C_dev_addr)){

        initDisplay();              //Set up the display

    }
    else{
        u0_dbg_printf("Display not ready yet! \n");
    }
}

//initial display


void DISPLAY::initDisplay(){

    I2C_dev_addr = 0x78;             //7 bit i2c register // slave address is  0x3C*2


    CMD(0xD5);           //cmd set clock divide ratio to generate display clock from clk
    CMD(0x80);                //clock divide ratio = 0x80

    CMD(0xA8);           //cmd - set multiplex ration range16-63
    CMD(63);         // set display height = 64-1

    CMD(0xD3);            //vertical display offset
    CMD(0x00);                  //0x00

    CMD(0x40 | 0x0);  //cmd - display startline to line 0



    CMD(0x20); //Set Horizontal addressing mode
    CMD(0x00); //Lower Column Start Address for page addr mode (8 bit column)

    CMD(0xDA); // cmd - COM Pins Hardware
    CMD(0x12); //sequential COM config from COM63 to COM0
    CMD(0xA0 | 0x1);// seg remap
    CMD(0xC8); // com output scan dir from com63 to com0
    CMD(0X81); // set contrast (0~255)

    CMD(0xCF);


    CMD(0xD9); //cmd - set  pre-charge period by # od DCLK


    CMD(0xF1);//pre-charge period = F1h


    CMD(0XD8); // display start line
    CMD(0x40);

    CMD(0XA4); //cmd - enable display outputs
    CMD(0XA6); //cmd - set normal display (1==on pixel)

    CMD(0X8D);    //Charge Pump Setting
    CMD(0x14);   //Enable Charge Pump

    CMD(0xAF);  //cmd - set Display ON
}


//send command
void DISPLAY::CMD(uint8_t cmd){
    //bool I2C_Base::writeReg(uint8_t deviceAddress, uint8_t registerAddress, uint8_t value)
        //sets to write mode
        //then transfers the data to dev
    i2c.getInstance().writeReg(I2C_dev_addr, 0, cmd);

}



//send data
void DISPLAY::DATA(uint8_t data)
{
    //bool I2C_Base::writeReg(uint8_t deviceAddress, uint8_t registerAddress, uint8_t value)
    //sets to write mode
    //then transfers the data to dev
    i2c.getInstance().writeReg(I2C_dev_addr, 0x40, data);
}

// set display address
void DISPLAY::setledaddr()
{
    //Set the 128 column
    CMD(0x21);         //set column address cmd=0x21
    CMD(0);             //start of col
    CMD(127);             //end of col 128-1
    //Set the display to contain 8 pages (rows)
    CMD(0x22);        //set page address cmd= 0x22h
    CMD(0);             //start of page
    CMD(63);      //end of page   64-1 //8 pages 8 bits
}


// show on display
void DISPLAY::showDisplay(){
    // prep display by setting display address
    setledaddr();
    //Display the buffer
    buffer2display(buffer);
}


// displaying buff on display
void DISPLAY::buffer2display(uint8_t *buff){
    uint16_t i;

    for(i = 0; i < ((128*64)/8); i++){
        DATA(buff[i]);
    }
}

//send  a string to buffer  page = starting row  column = starting column
void DISPLAY::stringtobuffer(uint8_t page, uint8_t column, char *str){
    uint8_t offset = 32;        //32 offset b/c ASCII
    uint8_t x = 0;
    uint8_t y = 0 ;
    unsigned char ch;

    //Go through entire string char by char
    while(str[x] != 0){
        ch = str[x];
        //put the letter into the buffer
        for(y = 0; y < 5; y++){
            buffer[y + ((page * 128) + (column + (5 * x)))] = letterBank[ch - offset][y];
        }
        x++;
    }
}


// clear display

void DISPLAY::clearscreen(){
    memset(buffer, 0,(128 * 64 / 8));
    showDisplay();
}
