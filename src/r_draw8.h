#ifndef __R_DRAW8__
#define __R_DRAW8__

void R_DrawColumn_8(void);
void R_DrawSkyColumn_8(void);
void R_DrawShadeColumn_8(void);
void R_DrawTranslucentColumn_8(void);

#ifdef USEASM
void ASMCALL R_DrawColumn_8_ASM(void);
void ASMCALL R_DrawSkyColumn_8_ASM(void);
void ASMCALL R_DrawShadeColumn_8_ASM(void);
void ASMCALL R_DrawTranslucentColumn_8_ASM(void);

void ASMCALL R_DrawColumn_8_Pentium(void); // Optimised for Pentium
void ASMCALL R_DrawColumn_8_NOMMX(void);   // DOSDoom original
void ASMCALL R_DrawColumn_8_K6_MMX(void);  // MMX asm version, optimised for K6
#endif

void R_DrawTranslatedColumn_8(void);
void R_DrawTranslatedTranslucentColumn_8(void);
void R_DrawSpan_8(void);
void R_DrawTranslucentSpan_8(void);

void R_DrawFogSpan_8(void);
void R_DrawFogColumn_8(void);
void R_DrawColumnShadowed_8(void);

#endif