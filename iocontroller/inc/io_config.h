#ifndef IOCONFIG_H
#define IOCONFIG_H

#define INPUT_DEBOUNCE_TIME 100000
#define INPUT_LONG_HOLD 2.0f
#define INPUT_SHORT_PRESS 0.25f

#define INPUT_BTTN_NEXT				23
#define INPUT_BTTN_NEXT_STATE		0	// Expected depressed pin state
#define INPUT_BTTN_PREVIOUS			24
#define INPUT_BTTN_PREVIOUS_STATE	0	// Expected depressed pin state
#define INPUT_BTTN_GENERAL			25
#define INPUT_BTTN_GENERAL_STATE	0	// Expected depressed pin state
#define INPUT_ENCODER_A				5
#define INPUT_ENCODER_B				6
#define INPUT_BTTN_ENCODER			13
#define INPUT_BTTN_ENCODER_STATE	0	// Expected depressed pin state

#define ENCODER_PPR (360.0f / 48.0f)
#define ENCODER_LINEAR_INC 0.05f

#endif