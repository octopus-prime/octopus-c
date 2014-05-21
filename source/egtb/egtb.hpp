/* -------------------------------------------------------------------- */
/*                                                                      */
/*              Probe chess endgame database ("tablebase")              */
/*                                                                      */
/*               Copyright (c) 1998--2001 Eugene Nalimov                */
/*                                                                      */
/* The code listed below should not be used in any product (software or */
/* hardware,  commercial or not,  and so on) without written permission */
/* from the author.                                                     */
/*                                                                      */
/* -------------------------------------------------------------------- */

#ifndef EGTB_hpp
#define EGTB_hpp

// Some constants from SJE program

#define pageL 256

/* tablebase byte entry semispan length */

#define tbbe_ssL ((pageL - 4) / 2)

/* tablebase signed byte entry values */

#define bev_broken  (tbbe_ssL + 1)  /* illegal or busted */

#define bev_mi1     tbbe_ssL        /* mate in 1 move */
#define bev_mimin   1               /* mate in 126 moves */

#define bev_draw    0               /* draw */

#define bev_limax   (-1)            /* mated in 125 moves */
#define bev_li0     (-tbbe_ssL)     /* mated in 0 moves */

#define bev_limaxx  (-tbbe_ssL - 1) /* mated in 126 moves */
#define bev_miminx  (-tbbe_ssL - 2) /* mate in 127 moves */

// Some constants for 16-bit tables

#define L_pageL 65536

/* tablebase short entry semispan length */

#define L_tbbe_ssL ((L_pageL - 4) / 2)

/* tablebase signed short entry values */

#define L_bev_broken  (L_tbbe_ssL + 1)      /* illegal or busted */

#define L_bev_mi1     L_tbbe_ssL            /* mate in 1 move */
#define L_bev_mimin   1                     /* mate in 32766 moves */

#define L_bev_draw    0                     /* draw */

#define L_bev_limax   (-1)                  /* mated in 32765 moves */
#define L_bev_li0     (-L_tbbe_ssL)         /* mated in 0 moves */

#define L_bev_limaxx    (-L_tbbe_ssL - 1)   /* mated in 32766 moves */
#define L_bev_miminx    (-L_tbbe_ssL - 2)   /* mate in 32767 moves */

#  define  C_PIECES  3

// Define functions to use:
int IInitializeTb(char *pszPath);
int IDescFindFromCounters(int *piCount);

int FTbSetCacheSize(void *pv, unsigned long cbSize);
int FRegisteredFun(int iTb, int side);

#if !defined (PFNCALCINDEX_DECLARED)
typedef unsigned long long (*PfnCalcIndex) (unsigned int *psqW, unsigned int *psqB, unsigned int sqEnP, int fInverse);
#define PFNCALCINDEX_DECLARED
#endif

PfnCalcIndex PfnIndCalcFun(int iTb, int side);
int L_TbtProbeTable(int iTb, int side, unsigned long long indOffset);

#endif
