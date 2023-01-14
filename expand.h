#ifndef EXPAND_H
#define EXPAND_H

#define EXPAND_1_OF_2(A, B) A
#define EXPAND_2_OF_2(A, B) B

#define EXPAND_1_EQ_2_OF_3( A, B, C) A = B
#define EXPAND_STR_1_OF_3(  A, B, C) #A
#define EXPAND_3_SUB_1_OF_3(A, B, C) C - 1

#endif
