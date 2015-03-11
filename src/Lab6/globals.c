/*
 * globals.c
 *
 *  Created on: Nov 11, 2014
 *      Author: tchambs
 */

#include "globals.h"

uint8_t globals_sequence[GLOBALS_MAX_FLASH_SEQUENCE];
uint8_t globals_sequenceLength = 0;
uint8_t globals_currentSequenceLength = 0;

// This is the length of the complete sequence at maximum length.
// You must copy the contents of the sequence[] array into the global variable that you maintain.
// Do not just grab the pointer as this will fail.
void globals_setSequence(const uint8_t sequence[], uint16_t length)
{
	for (int i = 0; i < length; i++)
	{
		globals_sequence[i] = sequence[i];
	}
	globals_sequenceLength = length;
}

// This returns the value of the sequence at the index.
uint8_t globals_getSequenceValue(uint16_t index)
{
	return globals_sequence[index];
}

// Retrieve the sequence length.
uint16_t globals_getSequenceLength()
{
	return globals_sequenceLength;
}

//set a value in our sequence (used for incrementing the sequence)
void globals_setSequenceValue(uint16_t index, uint8_t value)
{
	globals_sequence[index] = value;
}
// This is the length of the sequence that you are currently working on.
void globals_setSequenceIterationLength(uint16_t length)
{
	globals_currentSequenceLength = length;
}

// This is the length of the sequence that you are currently working on (not the maximum length but the interim length as
// the use works through the pattern one color at a time.
uint16_t globals_getSequenceIterationLength()
{
	return globals_currentSequenceLength;
}
