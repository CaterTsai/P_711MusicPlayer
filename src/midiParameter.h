#pragma once

enum cMIDI_PITCH_CODE : int
{
	//Pitch
	cMIDI_PITCH_C = 0
	, cMIDI_PITCH_Db
	, cMIDI_PITCH_D
	, cMIDI_PITCH_Eb
	, cMIDI_PITCH_E
	, cMIDI_PITCH_F
	, cMIDI_PITCH_Gb
	, cMIDI_PITCH_G
	, cMIDI_PITCH_Ab
	, cMIDI_PITCH_A
	, cMIDI_PITCH_Bb
	, cMIDI_PITCH_B
	, cMIDI_PITCH_BASE
};

enum cMIDI_CTRL_CODE : int
{
	cMIDI_CTRL_PITCH = 0
	, cMIDI_CTRL_MOD = 1
	, cMIDI_SUSTAIN = 64
};

static const int cMIDI_KEY_NUM = 24;

static const int cMIDI_MIN_PITCH_V = 0;
static const int cMIDI_MAX_PITCH_V = 127;

static const int cMIDI_MIN_CTRL_PITCH = 0;
static const int cMIDI_MAX_CTRL_PITCH = 16383;