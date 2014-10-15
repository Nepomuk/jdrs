/*============================================================*/
/* mrfdata.cpp                                                */
/* MVD Readout Framework Data Storage                         */
/*                                               M.C. Mertens */
/*============================================================*/


#include "mrfdata.h"
#include "mrftools.h"
#include <string>
#include <sstream>

using mrftools::getIntBit;
using mrftools::setIntBit;
using mrftools::shiftBy;

const u_int32_t TMrfData::zeroval;
const u_int32_t TMrfData::bitsinablock;

TMrfData::TMrfData()
: reglengthbits(0), reglengthwords(0), lastreglengthbits(0),  errcode(0) //, bitsinablock(sizeof(mrf::registertype) * CHAR_BIT)
{
}

/*
TMrfData(const TMrfData& rhs)
{

}
*/
/*
TMrfData& TMrfData::operator=(const TMrfData& rhs)
{
	setNumBits(rhs.reglengthbits);
	regdata = rhs.regdata;
	return *this;
}
*/

void TMrfData::setNumBits(const u_int32_t& length)
{
	reglengthbits = length;
	reglengthwords = (length + bitsinablock - 1) / bitsinablock;
	regdata.resize(reglengthwords);
	lastreglengthbits = length % bitsinablock;
	if (lastreglengthbits) {
		regdata.at(reglengthwords-1) &= (~((~0) << lastreglengthbits));
	}
}

const u_int32_t& TMrfData::getNumBits() const
{
	return reglengthbits;
}

void TMrfData::setNumWords(const u_int32_t& length)
{
	reglengthwords = length;
	reglengthbits = length * bitsinablock;
	regdata.resize(reglengthwords);
}

const u_int32_t& TMrfData::getNumWords() const
{
	return reglengthwords;
}

void TMrfData::setBit(const u_int32_t& position, const bool& state)
{
	if ((position < reglengthbits) && (reglengthbits > 0)) {
		/*
		if (state) {
			regdata.at(position / bitsinablock) |= ((mrf::registertype) 1<<(position % bitsinablock));
		} else {
			regdata.at(position / bitsinablock) &= (~((mrf::registertype) 1<<(position % bitsinablock))) ;
		}
		*/
		setIntBit(position % bitsinablock, regdata.at(position / bitsinablock), state);
		errcode = 0;
	} else {
		errcode |= mrfdata_error::out_of_bounds;
	}
}

bool TMrfData::getBit(const u_int32_t& position) const
{
	if (position < reglengthbits) {
		errcode = 0;
		return bool (regdata.at(position / bitsinablock) & ((mrf::registertype) 1<<(position % bitsinablock)));
	} else {
		errcode |= mrfdata_error::out_of_bounds;
		return false;
	}
}

const u_int32_t& TMrfData::appendBit(const bool& state)
{
	setNumBits(reglengthbits + 1);
	setBit(reglengthbits - 1, state);
	return reglengthbits;
}

void TMrfData::setWord(const u_int32_t& position, const mrf::registertype& dataword, const bool& truncate_ok)
{
	// Noch aendern, um stattdessen die masked funktion mit konstanter einsmaske zu benutzen
	if (((position < (reglengthwords - 1)) || ((position < reglengthwords) && ((reglengthbits % bitsinablock) == 0))) && (reglengthbits > 0)) {
		regdata.at(position) = dataword;
		errcode = 0;
	} else if (position < reglengthwords) {
		mrf::registertype bitmask = 1;
		for (uint i = 0; i < ((reglengthbits % bitsinablock) - 1); ++i) {
			bitmask |= bitmask << 1;
		}
		regdata.at(position) = (dataword & bitmask);
		if (!(dataword & (~bitmask)) || truncate_ok) {
			//regdata.at(position) = (dataword & bitmask);
			errcode = 0;
		} else {
			errcode |= mrfdata_error::out_of_bounds;
		}
	} else {
		errcode |= mrfdata_error::out_of_bounds;
	}
}

void TMrfData::setWordMasked(const u_int32_t& position, const mrf::registertype& dataword, const mrf::registertype& mask, const bool& truncate_ok)
{
	if (((position < (reglengthwords - 1)) || ((position < reglengthwords) && ((reglengthbits % bitsinablock) == 0))) && (reglengthbits > 0)) {
		regdata.at(position) = dataword;
		errcode = 0;
	} else if (position < reglengthwords) {
		mrf::registertype bitmask = 1;
		for (uint i = 0; i < ((reglengthbits % bitsinablock) - 1); ++i) {
			bitmask |= bitmask << 1;
		}
		//regdata.at(position) = (((dataword & mask) | (regdata.at(position) & (~ mask))) & bitmask);
		regdata.at(position) &= (~ mask);
		regdata.at(position) |= (dataword & mask);
		regdata.at(position) &= bitmask;
		if (!((dataword & mask) & (~bitmask)) || truncate_ok) {
			//regdata.at(position) = (dataword & bitmask);
			errcode = 0;
		} else {
			errcode |= mrfdata_error::out_of_bounds;
		}
	} else {
		errcode |= mrfdata_error::out_of_bounds;
	}
}

const mrf::registertype& TMrfData::getWord(const u_int32_t& position) const
{
	if (position < reglengthwords) {
		errcode = 0;
		return regdata.at(position);
	} else {
		errcode |= mrfdata_error::out_of_bounds;
		return zeroval;
	}
}

const mrf::registertype& TMrfData::appendWord(const mrf::registertype& dataword)
{
	setNumWords(getNumWords() + 1);
	setWord(getNumWords()-1, dataword);
	return getNumWords();
}


void TMrfData::setBitBlock(const u_int32_t& position, const u_int32_t& length, const mrf::registertype& value, const u_int32_t& offset, const bool& reverse)
{
	u_int32_t i;
	if (reverse) {
		for (i = 0; i < length; ++i) {
			setBit(position + length - 1 - i, getIntBit(offset + i, value));
		}
	} else {
		for (i = 0; i < length; ++i) {
			setBit(position + i, getIntBit(offset + i, value));
		}
	}
}

const mrf::registertype& TMrfData::getBitBlock(const u_int32_t& position, const u_int32_t& length, const u_int32_t& offset, const bool& reverse) const
{
	u_int32_t i;
	_bitblock = 0;
	if (reverse) {
		for (i = 0; i < length; ++i) {
			//setBit(position + length - 1 - i, getIntBit(value, offset + i));
			setIntBit(offset + i, _bitblock, getBit(position + length - 1 - i));
		}
	} else {
		for (i = 0; i < length; ++i) {
			//setBit(position + i, getIntBit(value, offset + i));
			setIntBit(offset + i, _bitblock, getBit(position + i));
		}
	}
	return _bitblock;
}

/*
void TMrfData::copyFragment(const u_int32_t sourcestart, const u_int32_t destposition, const u_int32_t size, const mrf::registertype value)
{
	if (destposition + size < reglengthbits) {
		u_int32_t mask;
		mrf::registertype pattern;
		// Data fits in one block.
		if ((destposition % bitsinablock) + size < bitsinablock) {
			pattern = shiftBy(sourcestart - (destposition % bitsinablock), value);
			mask = 0;
			for (u_int32_t i = (destposition % bitsinablock); i < ((destposition % bitsinablock) + size); ++i) {
				setIntBit(mask, i, true);
			}
			setWordMasked(bitInBlock(destposition), pattern, mask, false);
		} else {
			// Data goes to two adjacent blocks.
			// Left pattern

			//linkes pattern:
			//Nach rechts shiften: destpos % bitsinablock - sourcestart
			//Soviel Nullen links: destpos % bitsinablock

			pattern = shiftBy((destposition % bitsinablock) - sourcestart, value);
			mask = (~ 0);
			for (u_int32_t i = 0; i < (destposition % bitsinablock); ++i) {
				setIntBit(mask, i, false);
			}
			setWordMasked(bitInBlock(destposition), pattern, mask, false);
			// Right Pattern
			pattern = shiftBy(-(bitsinablock + sourcestart - (destposition % bitsinablock)), value);
			mask = 0;
			for (u_int32_t i = 0; i < (size + (destposition % bitsinablock) - bitsinablock); ++i) {
				setIntBit(mask, i, true);
			}
			setWordMasked(bitInBlock(destposition) + 1, pattern, mask, false);
			//linkes pattern:
			//Nach rechts shiften: destpos % bitsinablock - sourcestart
			//Soviel Nullen links: destpos % bitsinablock
			//rechtes pattern:
			//Nach links shiften: bitsinablock - leftshifts
			//					= bitsinablock + sourcestart - destpos % bitsinablock
			//Soviel Einsen links: size - linkebits
			//					= size - (bitsinablock - destpos % bitsinablock)
			//					= size + destpos % bitsinablock - bitsinablock
		}
	} else {
		errcode |= mrfdata_error::out_of_bounds;
	}
}
*/

u_int32_t TMrfData::bitInBlock(const u_int32_t& position) const
{
	return (position / bitsinablock);
}

void TMrfData::clearDataStream()
{
	//setNumBits(0);
	setNumWords(0);
}

bool TMrfData::sameDataStream(const TMrfData& other) const
{
	return (regdata == other.regdata);
}

void TMrfData::importBinString(const std::string& data, const u_int32_t& offset)
{
	u_int32_t i;
	u_int32_t reglength = data.length();
	if (getNumBits() < (offset + reglength)) {
		setNumBits(offset + reglength);
	}
	for (i = 0; i < reglength; ++i) {
		setBit(offset + i, (data.compare(i, 1, "1") == 0));
	}
}

//void TMrfData::importHexString(const std::string data, const u_int32_t offset)
//{
//	//setBitBlock(position + offset);
//}

const std::string& TMrfData::exportBinString() const
{
	u_int32_t i;
	_data.clear();
	for (i = 0; i < getNumBits(); ++i) {
		if (getBit(i)) {
			_data.append("1");
		} else {
			_data.append("0");
		}
	}
	return _data;
}


//const std::string TMrfData::exportHexString() const
//{
//	// throw "Not implemented yet.";
//	return "nix";
//}

void TMrfData::resample(const u_int32_t& offset, const u_int32_t& factor, const bool& reverse, const u_int32_t& cutoff)
{
	std::vector<mrf::registertype> tmp = regdata;
	u_int32_t newlength = 0;
	if (reverse) {
		for (u_int32_t i = offset; i < getNumBits(); i += factor) {
			setBit((i - offset) / factor, getIntBit(bitsinablock - 1 - (i % bitsinablock), tmp.at(i / bitsinablock)));
			++newlength;
		}
	} else {
		for (u_int32_t i = offset; i < getNumBits(); i += factor) {
			setBit((i - offset) / factor, getIntBit(i % bitsinablock, tmp.at(i / bitsinablock)));
			++newlength;
		}
	}
	if (cutoff == 0) {
		setNumBits(newlength);
	} else {
		setNumBits(cutoff);
	}
}

const u_int32_t& TMrfData::getLastError() const
{
	return errcode;
}

bool TMrfData::lastActionSuccessful() const
{
	return !(errcode);
}








