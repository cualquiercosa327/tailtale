/* ---------------------------------------------------------- */
/*  debug.c                                                   */
/* ---------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   debug.c                                             */
/*     �f�o�b�O�p�`�F�b�N���[�`��                         */
/*                                                        */
/*--------------------------------------------------------*/
/* -- $Id: debug.pp,v 1.3 2002/07/11 17:21:47 rero2 Exp $ */


/*------------------------------------------------------------- */
/** @file
    @brief		�f�o�b�O�`�F�b�N���[�`��
    @author		K.Kunikane (rerofumi)
    @since		Jul.27.2005
    $Revision: 1.1.1.1 $
*/
/*-----------------------------------------------------
 Copyright (C) 2002,2005 rerofumi <rero2@yuumu.org>
 All Rights Reserved.
 ------------------------------------------------------*/

/*-------------------------------*/
/* include                       */
/*-------------------------------*/

#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <pspgu.h>
#include <pspgum.h>

#include "debug.h"
#include "grp_screen.h"

/*-------------------------------*/
/* GU packet                     */
/*-------------------------------*/

struct DebugVertex
{
  unsigned short u, v;
  unsigned int color;
  short x,y,z;
};

/*-------------------------------*/
/* local value                   */
/*-------------------------------*/

/* ----- �f�o�b�O���[�`�����L�����ۂ�(ASCII�������o���Ă邩) */
int  UseDebug;

/* ----- ��ʂ̑傫�� */
int  debug_width;
int  debug_height;

/* ----- �A�X�L�[�t�H���g�e�N�X�`���[ */
SDL_Surface *AsciiFont;

/* ----- �A�X�L�[�v���[�� */
SDL_Surface *AsciiPlane;

/* ----- �\�������� */
char StockString[MessageMax][128];
int  StockNum;

/* ----- �A�X�L�[�e�N�X�`���[�� */
char *TextureName = "ASCII.BMP";

/* --- �f�o�b�O�t�H���g������ */
char  debug_line[128];


/* -------------------------------------------------------------- */
/* --- �f�o�b�O�p�`�F�b�N���[�`��                                 */
/* -------------------------------------------------------------- */

/* ---------------------------------------- */
/* --- ���b�Z�[�W�̃��C���\��    */
void  print_msg(char *mes, int disp_x, int disp_y)
{
  int  i, l;
  int  c;
  struct DebugVertex* vertices;

  l = strlen(mes);
  vertices = (struct DebugVertex*)sceGuGetMemory(l * 2 * sizeof(struct DebugVertex));
  for(i=0; i<l; i++) {
    c = mes[i];
    vertices[i*2].u = (c % 16) * 8;
    vertices[i*2].v = (c / 16) * 8;
    vertices[i*2].color = 0;
    vertices[i*2].x = disp_x;
    vertices[i*2].y = disp_y;
    vertices[i*2].z = 0;
    vertices[i*2+1].u = (c % 16) * 8 + 8;
    vertices[i*2+1].v = (c / 16) * 8 + 8;
    vertices[i*2+1].color = 0;
    vertices[i*2+1].x = disp_x + 8;
    vertices[i*2+1].y = disp_y + 8;
    vertices[i*2+1].z = 0;
    disp_x = disp_x + 8;
  }
  sceGuDrawArray(GU_SPRITES,
		 GU_TEXTURE_16BIT|GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D,
		 l*2, 0, vertices);
}



/* ---------------------------------------- */
/* --- �f�o�b�O�t�F�C�X������               */
/* ---------------------------------------- */
void  TDebugInit(TGameScreen *screen,
		 int Width, int Height, int Depth)
{
  SDL_Surface *plane;
  void *nonalign;
  int  msize, i;
  unsigned long *pix;

  debug_width = Width;
  debug_height = Height;
  StockNum = 0;
  UseDebug = 1;
  plane = SDL_LoadBMP(TextureName);
  if (plane == NULL) {
    UseDebug = 0;
    return;
  }
  AsciiPlane = SDL_CreateRGBSurface(SDL_SWSURFACE,
				    Width, Height, 32,
				    DRmask, DGmask, DBmask, DAmask);
  if (AsciiPlane == NULL) {
    UseDebug = 0;
  }
  AsciiFont = SDL_ConvertSurface(plane, AsciiPlane->format, SDL_SWSURFACE);
  if (AsciiFont == NULL) {
    UseDebug = 0;
  }
  nonalign = AsciiFont->pixels;
  msize = (AsciiFont->w * AsciiFont->h) * AsciiFont->format->BytesPerPixel;
  AsciiFont->pixels = (void*)memalign(16, msize);
  memcpy(AsciiFont->pixels, nonalign, msize);
  free(nonalign);
  SDL_SetColorKey(AsciiFont, SDL_SRCCOLORKEY, 0x000000);
  SDL_FreeSurface(plane);
  /* --- ColorKey �C�R�[�����l����Ȃ��悤�Ȃ̂� */
  /*    ���O�Ń��l�ݒ� */
  pix = (unsigned long*)AsciiFont->pixels;
  msize = (AsciiFont->w * AsciiFont->h);
  for(i=0; i<msize; i++) {
    if (((*pix) & 0xffffff) == 0x007d00) {
      *pix &= 0x00ffffff;
    }
    else {
      *pix |= 0xff000000;
    }
    pix++;
  }
}


/* ---------------------------------------- */
/* --- �f�o�b�O�t�F�C�X���                 */
/* ---------------------------------------- */
void  TDebugFree()
{
  SDL_FreeSurface(AsciiPlane);
  SDL_FreeSurface(AsciiFont);
  UseDebug = 0;
}


/* ---------------------------------------- */
/* --- �f�o�b�O�t�F�C�X�̒���`��           */
/* ---------------------------------------- */
void  TDebugDisp(TGameScreen *screen)
{
  int  disp_x, disp_y;
  int  i;
  void *pixels;

  /* --- �������o���Ă��Ȃ��������� */
  if (UseDebug == 0) {
    return;
  }

  /* --- �A�X�L�[�v���[�����X�N���[���� */
  if (StockNum > 0) {
    pixels = AsciiFont->pixels;

    /* --- �e�N�X�`���̐ݒ� */
    sceGuEnable(GU_TEXTURE_2D);
    sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
    sceGuTexMode(GU_PSM_8888,0,0,0);
    sceGuTexImage(0, 128, 64, 128, pixels);
    sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
    sceGuTexFilter(GU_NEAREST, GU_NEAREST);
    sceGuTexScale(1.0f,1.0f);
    sceGuTexOffset(0.0f,0.0f);
    sceGuAmbientColor(0xffffffff);
    //sceGuTexSync();
    sceGuColor(0xffffffff);

    /* --- �\���L���[�ɐς܂�Ă��镪�����\�� */
    disp_x = 0;
    disp_y = 0;
    for(i=0; i<StockNum; i++) {
      print_msg(StockString[i], disp_x, disp_y);
      disp_y = disp_y + 8;
    }

    /* --- ���̂��߂ɃN���A */
    //SDL_FillRect(AsciiPlane, 0, 0x00000000);
  }
  StockNum = 0;  
}


/* ---------------------------------------- */
/* --- �f�o�b�O���b�Z�[�W�̃L���[�C���O     */
/* ---------------------------------------- */
void  TDebugPrint(char *mes)
{
  if (StockNum < MessageMax) {
    strncpy(StockString[StockNum], mes, 127);
    StockNum = StockNum + 1;
  }
}


/* ---------------------------------------- */
/* --- �����𕶎���ɕϊ�                   */
/* ---------------------------------------- */
void  IntToStr(int num, char *buf)
{
#ifndef NO_SNPRINTF
  snprintf(buf, 127, "%d", num);
#endif
}


/* ---------------------------------------- */
/* --- 16�i���\���p                         */
/* ---------------------------------------- */
void  IntToHex(int num, int length, char *buf)
{
#ifndef NO_SNPRINTF
  snprintf(buf, 127, "%x", num);
#endif
}

