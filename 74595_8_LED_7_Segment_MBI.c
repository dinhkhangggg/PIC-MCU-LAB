///////////////////////////////////////////////////////////////////////////
////   Library for a 74595 Expanded Output Chip                        ////
////                                                                   ////
////   Any number of these chips may be connected in serise to get     ////
////   8 additional outputs per chip.  The cost is 3 I/O pins for      ////
////   any number of chips.                                            ////
////                                                                   ////
////   write_expanded_outputs(eo);  Writes the array eo to the chips   ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2003 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////

#IFNDEF EXP_OUT_ENABLE_8L7D_MBI

#define EXP_OUT_ENABLE_8L7D_MBI     PIN_B0   //ST_CP  = RCK_?
#define EXP_OUT_CLOCK               PIN_B1   //SH_CP  = SSCK
#define EXP_OUT_DO                  PIN_B2   //DS     = SSDO
#define NUMBER_OF_74595_8_LED_7_Segment_MBI    4

#ENDIF


void write_expanded_outputs_8L7D_MBI(BYTE* eo) 
{
  BYTE i;

  output_low(EXP_OUT_CLOCK);
  output_low(EXP_OUT_ENABLE_8L7D_MBI);

  for(i=1;i<=NUMBER_OF_74595_8_LED_7_Segment_MBI*8;i++) 
  {//Clock out bits from the eo array
    if((*(eo+(NUMBER_OF_74595_8_LED_7_Segment_MBI-1))&0x80)==0)
      output_low(EXP_OUT_DO);
    else
      output_high(EXP_OUT_DO);
   shift_left(eo,NUMBER_OF_74595_8_LED_7_Segment_MBI,0);
   output_high(EXP_OUT_CLOCK);
   output_low(EXP_OUT_CLOCK);
  }
  output_high(EXP_OUT_ENABLE_8L7D_MBI);
  output_low(EXP_OUT_ENABLE_8L7D_MBI);

}
