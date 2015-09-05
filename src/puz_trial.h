/* ---------------------------------------------------------- */
/*  puz_trial.h                                               */
/* ---------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                                                        */
/* SDL puzzle project - for COMIKET62                     */
/*                        Fumi2Kick/LiMo/omamori-2002     */
/*                        1st Maintaner  Rerorero@fumi.   */
/*                                                        */
/*   puz_trial.h                                          */
/*     �ѥ���Ȥ��Ȥ�⡼��                               */
/*                                                        */
/*--------------------------------------------------------*/

#ifndef PUZ_TRIAL_H
#define PUZ_TRIAL_H

/*-------------------------------*/
/* include                       */
/*-------------------------------*/

#include "grp_screen.h"
#include "puz_base.h"
#include "puz_disp.h"
#include "debug.h"

/*-------------------------------*/
/* define                        */
/*-------------------------------*/

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

enum TrialStep {
  TInit,
  TReady,
  TGame,
  TMiss,
  TGameover,
  TEnd
};

/* --- ���������������� */
extern enum {
  LevelEasy = 0,
  LevelNormal,
  LevelHard
} GameLevel;


/*-------------------------------*/
/* struct                        */
/*-------------------------------*/

/* -------------------------------- */
/* --- ���饹���С� */
typedef struct {
  /* - �ƥ��饹 */
  TPuzzleDisp  *super;
  /* - ��������� */
  int  status;

  /* - ��������� */
  TGameScreen  *screen;
  /* - ���ơ��� */
  int  step;
  /* - �������ȥ������ */
  int  readytimer;

  /* - ����� */
  int  game_level;

} TPuzzleTrial, *PTPuzzleTrial;


/* ---------------------------------------------- */
/* --- extern                                  -- */
/* ---------------------------------------------- */

TPuzzleTrial *TPuzzleTrial_Create(TGameScreen *scr, int level);
void TPuzzleTrial_Destroy(TPuzzleTrial *class);

void TPuzzleTrial_LoadTexture(TPuzzleTrial *class);
void TPuzzleTrial_GameMain(TPuzzleTrial *class);
int TPuzzleTrial_GameStat(TPuzzleTrial *class);
void TPuzzleTrial_UserControl(TPuzzleTrial *class);


#endif //PUZ_DISP_H
