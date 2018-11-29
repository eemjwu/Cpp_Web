//封装验证码生成函数

#pragma once

#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>

const int gifsize = 17646;

void captcha(unsigned char im[70 * 200], unsigned char l[6]);

void makegif(unsigned char im[70 * 200], unsigned char gif[gifsize]);

static int letter(int n, int pos, unsigned char im[70 * 200], unsigned char swr[200], uint8_t s1, uint8_t s2);

static void line(unsigned char im[70 * 200], unsigned char swr[200], uint8_t s1);

static void dots(unsigned char im[70 * 200]);

static void blur(unsigned char im[70 * 200]);

static void filter(unsigned char im[70 * 200]);

void captcha(unsigned char im[70 * 200], unsigned char l[6]);