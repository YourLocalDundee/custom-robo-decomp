/* Custom Robo: from bfbbdecomp/bfbb @ d8e8eac38dd617ee72161f4cb2da4498bc89aad6, src/PowerPC_EABI_Support/src/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_sqrt.c */ extern double __ieee754_sqrt();

double sqrt(double __x) { return (double)__ieee754_sqrt(); }

/*
.loc_0x0:
  stwu      r1, -0x10(r1)
  mflr      r0
  stw       r0, 0x14(r1)
  bl        -0x29C
  lwz       r0, 0x14(r1)
  mtlr      r0
  addi      r1, r1, 0x10
  blr
*/
