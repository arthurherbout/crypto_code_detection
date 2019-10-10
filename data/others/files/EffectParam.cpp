#include "EffectParam.h"
#include "PatternRow.h"
#include <algorithm>

EffectParam::EffectParam(int _effect, int _param1, int _param2)
	: effect(_effect), param1(_param1), param2(_param2)
{
}


EffectParam::EffectParam(int _effect, int params)
	: effect(_effect)
{
	setParamsFromByte(params);
}


int EffectParam::getParamsAsByte() const
{
	return param2 + (param1 << 4);
}


void EffectParam::setParamsFromByte(int byte)
{
	param1 = (byte >> 4) & 0xf;
	param2 = byte & 0xf;
}


void EffectParam::setNoteAndOctave(int noteAndOctave)
{
	effect = 'n';
	param1 = noteAndOctave % 12;
	param2 = std::min(15, noteAndOctave / 12);
}

int EffectParam::getNoteWithOctave() const
{
	if (effect != 'n')
		return PatternRow::NoNote;
	
	return param1 + param2 * 12;
}


bool EffectParam::isEmpty() const
{
	return effect == '0' && param1 == 0 && param2 == 0;
}
