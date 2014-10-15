/*============================================================*/
/* mrfdata.h                                                  */
/* MVD Readout Framework Data Storage                         */
/*                                               M.C. Mertens */
/*============================================================*/


#ifndef __MRFDATA_H__
#define __MRFDATA_H__


// #include "mrfgal.h"
#include <vector>
#include "mrfbase.h"
#include <string>
#include <map>
#include <climits>
//#include "mrf_confitem.h"

//! Error flags set by TMrfData functions.
namespace mrfdata_error {
	static const u_int32_t success = 0;
	static const u_int32_t out_of_bounds = 1;
	static const u_int32_t size_mismatch = 2;
	static const u_int32_t not_found = 4;
}


//! Base interface class for data storage and manipulation. Compatible with IO classes from MRF Suite.
class TMrfData
{
	public:
		TMrfData();

		//TMrfData(const TMrfData& rhs);

		//TMrfData& operator=(const TMrfData& rhs);

		//! Sets the length of the binary data stream.
		/*!
		\param length Length (in bits) of the binary data stream to be set.
		*/
		void setNumBits(const u_int32_t& length);

		//! Retrieves the length of the register in bits.
		/*!
		\return Current length (in bits) of the binary data stream.
		*/
		const u_int32_t& getNumBits() const;

		//! Sets the length of the register to lengths words.
		/*!
		\param length Length (in data words) of the binary data stream to be set.
		*/
		void setNumWords(const u_int32_t& length);

		//! Retrieves the length of the register in words.
		/*!
		\return Current length (in data words) of the binary data stream.
		*/
		const u_int32_t& getNumWords() const;

		//! Sets or resets the bit at position.
		/*!
		\param position Position (unit is bits) of bit in binary data stream to be manipulated.
		\param state Boolean state of the bit to be set to. True sets to 1, False sets to 0.
		*/
		void setBit(const u_int32_t& position, const bool& state);

		//! Determines if bit at position is set.
		/*!
		\param position Position (unit is bits) of bit in binary data stream to be investigated.
		\return \b True if the bit is set (1), \b False if the bit is not set (0).
		*/
		bool getBit(const u_int32_t& position) const;

		//! Appends a bit to the data stream.
		/*!
		\param state Boolean state of the bit to be set to. True sets to 1, False sets to 0.
		\return New length of the binary data stream.
		*/
		const u_int32_t& appendBit(const bool& state);

		//! Sets a complete data word at position to value.
		/*!
		\param position Position (unit is words) of data word to be set.
		\param dataword New value of the data word to be set.
		\param truncate_ok If \b True, the data word is silently truncated in case it exceeds the length of the binary data stream. Otherwise, a flag in the internal error code is set.

		<b>Error codes:</b>
			- \b 0: Everything OK.
			- \b mrfdata_error::out_of_bounds: The data word had to be truncated to fit into the data stream.
		*/
		void setWord(const u_int32_t& position, const mrf::registertype& dataword, const bool& truncate_ok = false);

		//! Sets a subset of bits in a word based on a mask.
		/*!
		\param position Position (unit is words) of data word to be set.
		\param dataword New value of the data word to be set.
		\param mask Manipulation mask. All bits at positions set in the mask will be set/reset, all other bits will remain unchanged.
		\param truncate_ok If \b True, the data word is silently truncated in case it exceeds the length of the binary data stream. Otherwise, a flag in the internal error code is set.

		<b>Error codes:</b>
			- \b 0: Everything OK.
			- \b mrfdata_error::out_of_bounds: The data word had to be truncated to fit into the data stream.
		*/
		void setWordMasked(const u_int32_t& position, const mrf::registertype& dataword, const mrf::registertype& mask = ~0, const bool& truncate_ok = false);

		//! Retrieves the word found at position.
		/*!
		\param position Position (unit is words) of data word to be investigated.
		\return Value of the data word.
		*/
		const mrf::registertype& getWord(const u_int32_t& position) const;
		
		const mrf::registertype& appendWord(const mrf::registertype& dataword);

		//! Sets a bit block of given length to the least significant bits of value
		/*!
		\param position Position (unit is bits) of bit block to be set.
		\param length Length (unit is bits) of the bit block to be set.
		\param value Value the bit block is to be set to, least significant bits are considered first.
		\param offset Optional offset of bits in value to be skipped, starting from LSB
		\param reverse If \b True, the bit order of the value is reversed.
		*/
		void setBitBlock(const u_int32_t& position, const u_int32_t& length, const mrf::registertype& value, const u_int32_t& offset = 0, const bool& reverse = false);

		const mrf::registertype& getBitBlock(const u_int32_t& position, const u_int32_t& length, const u_int32_t& offset = 0, const bool& reverse = false) const;

		// Copies a size Bits long Data fragment to position.
		//void copyFragment(const u_int32_t sourcestart, const u_int32_t destposition, const u_int32_t size, const mrf::registertype value);

		//! Returns the word index which holds bit position.
		/*!
		\param position Position (unit is bits) of bit in binary data stream to be investigated.
		\return Position (unit is words) of bit in binary data stream.
		*/
		u_int32_t bitInBlock(const u_int32_t& position) const;

		//! Sets a bitfield within the data storage based on the configuration stored in the map parameter bitmap.
		//void setItemValue(const std::string& item, const std::map<const std::string, TConfItem>& bitmap, const mrf::registertype& value, const u_int32_t& offset = 0);

		//! Retrieves a bitfield from the data storage based on the configuration stored in the map parameter bitmap.
		//u_int32_t getItemValue(const std::string& item, const std::map<const std::string, TConfItem>& bitmap, const u_int32_t& offset = 0) const;

		//! Clears the data stream.
		void clearDataStream();

		//! Checks data streams for equality.
		bool sameDataStream(const TMrfData& other) const;

		//! Imports a data stream from a string representing binary digits
		/*!
		\param data String consiting only of characters "0" and "1" to be imported as binary data stream.
		\param offset Optional offset (unit is bits) to change the import start position in the binary data stream.
		*/
		void importBinString(const std::string& data, const u_int32_t& offset = 0);

		// Imports a data stream from a string representing hexadecimal digits
		/*
		\param data String consiting only of characters representing hexadecimal digits to be imported as binary data stream.
		\param offset Optional offset (unit is bits) to change the import start position in the binary data stream.
		*/
		//void importHexString(const std::string data, const u_int32_t offset = 0);

		//! Exports a data stream to a string representing binary digits
		/*!
		\return A string containing a binary representation of the binary data stream.
		*/
		const std::string& exportBinString() const;

		//! Extracts binary data from returned oversampled data
		/*!
		\param offset Initial offset of oversampled data, after which sampling starts.
		\param factor Oversampling factor.
		\param reverse When set to \b true the order in which the bits of each data word are processed, is revesed. Default is \b false.
		\param cutoff Optional maximum length of the resulting data stream. The default is to not cut the data stream.
		*/
		void resample(const u_int32_t& offset, const u_int32_t& factor, const bool& reverse = false, const u_int32_t& cutoff = 0);

		// Exports a data stream to a string representing hexadecimal digits
		/*
		\return A string containing a hexadecimal representation of the binary data stream.
		*/
		//const std::string exportHexString() const;

		//! Returns an integer with errorflags of all errors occured after last successful command.
		/*!
		\return An integer with errorflags of all errors occured after last successful command. Zero returned means success.
		*/
		const u_int32_t& getLastError() const;

		//! Returns \b True if the last action completed successfullly, \b False otherwise.
		/*!
		\return \b True if the last action completed successfullly, \b False if any error occured.
		*/
		bool lastActionSuccessful() const;
//	private:
		u_int32_t reglengthbits;
		u_int32_t reglengthwords;
		u_int32_t lastreglengthbits;
		static const u_int32_t zeroval = 0;
		mutable u_int32_t _bitblock;
		mutable std::string _data;
//	protected:
		//! Internal error code.
		/*!
		The internal error code is a binary OR of all error flags which were set after the last successful command, which resets the error code to 0.
		*/
		mutable u_int32_t errcode;

		//! Number of bits stored in each data word.
		static const u_int32_t bitsinablock = sizeof(mrf::registertype) * CHAR_BIT;

		//! Internal storage for data structure.
		/*!
		Do not access directly unless you are absolutely sure you need to do this. If you are absolutely sure, think about it once again.
		*/
		std::vector<mrf::registertype> regdata;
		//void copyBlock(const u_int32_t sourcestart, const u_int32_t length, const u_int32_t deststart, const u_int32_t value);
};



#endif // __MRFDATA_H__

