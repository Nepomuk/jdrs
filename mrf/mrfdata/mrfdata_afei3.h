/*============================================================*/
/* mrfdata_afei3.h                                            */
/* MVD Readout Framework Data Storage                         */
/* Specific to Atlas FE-I3 Registers                          */
/*                                               M.C. Mertens */
/*============================================================*/


#ifndef __MRFDATA_AFEI3_H__
#define __MRFDATA_AFEI3_H__

#include "mrfdata.h"
#include <map>
#include <string>
#include "mrf_confitem.h"
#include "mrfbase.h"

//! Length (in bits) of Atlas-FE-I3 registers.
namespace mrf_afei3_reglength {
	//! Length of the command register.
	static const u_int32_t command = 29;
	//! Length of the global register.
	static const u_int32_t global = 231;
	//! Length of one pixel double column (9 double columns total).
	//static const u_int32_t pixrows = 320;
	//! Number of double columns in the pixel register.
	//static const u_int32_t pixcols = 9;
	//! Length of one logical pixel column (18 columns total) according to internal pixel index.
	static const u_int32_t logpixrows = 160;
	//! Number of logical columns in the pixel register according to internal pixel index.
	static const u_int32_t logpixcols = 18;
	//! Number of bits in a single pixel register.
	static const u_int32_t singlepixel = 14;
}

namespace mrf_afei3 {
	static const u_int32_t globregoffset = 29;
	static const u_int32_t pixregoffset = 29;
	static const u_int32_t fdacbits = 3;
	static const u_int32_t tdacbits = 7;
	static const std::string fdac = "FDAC";
	static const std::string tdac = "TDAC";
//	enum assemblymode {
//		command,
//		global,
//		pixel,
//	};
}

//! Data storage and manipulation specific to the Atlas FE-I3. Compatible with IO classes from MRF Suite.
class TMrfData_AFEI3 : virtual public TMrfData
{
	public:
		//! Constructor
		/*!
		\param address Sets the fronted address. Can be changed by calling setAddress.
		\param broadcast Sets the broadcast bit. Can be changed by calling setBroadcast.
		*/
		TMrfData_AFEI3(const mrf::registertype& address = 0, const mrf::registertype& broadcast = 0);

		//! Initializes the command register map.
		/*!
		Resets all values to 0.
		Warning: All previously assigned operators will be invalid after calling this function.
		*/
		void initCommandReg();

		//! Initializes the global register map.
		/*!
		Resets all values to 0.
		Warning: All previously assigned operators will be invalid after calling this function.
		*/
		void initGlobReg();

		//! Initializes the pixel register map.
		/*!
		Resets all values to 0.
		Warning: All previously assigned operators will be invalid after calling this function.
		*/
		void initPixReg();

		//! Resets command register.
		/*!
		Without parameters given, resets all values except Address and Broadcast to 0. Address and Broadcast will be reset to the default values set with setAddress and setBroadcast.
		\param clearpersistent When set to \b true the Address and Broadcast bit are also reset to 0. When set to \b false or not set, the default values are restored. This is the default behaviour.
		*/
		void clearCommandReg(const bool& clearpersistent = false);

		//! Resets all global register values to 0.
		/*!
		Resets all values to 0.
		*/
		void clearGlobReg();

		//! Resets all pixel register values to 0.
		/*!
		Resets all values to 0.
		*/
		void clearPixReg();

		// Deprecated
		// Determines how the binary data stream is built from the configuration values.
		/*
		\param mode Determines which command sequence should be assembled, i.e., which register(s) of the frontend shall be written to.
		*/
		//void assemble(const mrf_afei3::assemblymode mode);

		//! Assembles the part of the binary data stream representing the command register.
		void assembleCommandReg();

		//! Assembles the part of the binary data stream representing the global register.
		void assembleGlobReg();

		//! Assembles the part of the binary data stream representing the pixel register.
		/*!
		\param item Name of the item to be assembled.
		*/
		void assemblePixReg(const std::string& item);

		//! Assembles the part of the binary data stream representing the pixel register.
		/*!
		\param item Name of the item to be assembled.
		\param rhs Data source to use for assembly.
		*/
		void assemblePixReg(const std::string& item, const TMrfData_AFEI3& rhs);

		//! Assembles a single value from the command register storage into its position within the binary data stream.
		void assembleCommandRegValue(const std::string& item);

		//! Assembles a single value from the global register storage into its position within the binary data stream.
		void assembleGlobRegValue(const std::string& item);

		//! Disassembles the binary data stream to configuration items.
		void disassemble();

		//! Extracts the configuration values of the command register from the binary data stream.
		/*!
		All command register values are updated when this function is called.
		*/
		void disassembleCommandReg();

		//! Extracts the configuration values of the global register from the binary data stream.
		/*!
		All global register values are updated when this function is called.
		*/
		void disassembleGlobReg();

		//! Extracts the contents of the pixel register from the binary data stream.
		/*!
		Data update is done for a single pixel register bit due to protocol restrictions. Global register contents stored in the configuration variables determine which bit is updated.
		TODO: Overload for manual configuration possibility.
		*/
		void disassemblePixReg(const u_int32_t& activecolumns, const std::string& item);

		//! Returns an iterator to the start of the command register map.
		/*!
		Can be used to iterate over all configuration values.
		\return Iterator to the start of the command register map.
		*/
		const std::map<const std::string, TConfItem>::const_iterator getCommandRegIteratorBegin() const;

		//! Returns an iterator to the start of the global register map.
		/*!
		Can be used to iterate over all configuration values.
		\return Iterator to the start of the global register map.
		*/
		const std::map<const std::string, TConfItem>::const_iterator getGlobRegIteratorBegin() const;

		//! Returns an iterator to the start of the pixel register map.
		/*!
		Can be used to iterate over all configuration values.
		\return Iterator to the start of the pixel register map.
		*/
		const std::map<const std::string, TConfItem>::const_iterator getPixRegIteratorBegin(const u_int32_t& i, const u_int32_t& j) const;

		//! Returns an iterator right past the end of the command register map.
		/*!
		Can be used to iterate over all configuration values.
		\return Iterator past the end of the command register map.
		*/
		const std::map<const std::string, TConfItem>::const_iterator getCommandRegIteratorEnd() const;

		//! Returns an iterator right past the end of the global register map.
		/*!
		Can be used to iterate over all configuration values.
		\return Iterator past the end of the global register map.
		*/
		const std::map<const std::string, TConfItem>::const_iterator getGlobRegIteratorEnd() const;

		//! Returns an iterator right past the end of the pixel register map.
		/*!
		Can be used to iterate over all configuration values.
		\param i Pixel double column index.
		\param j Pixel row index.
		\return Iterator past the end of the pixel register map.
		*/
		const std::map<const std::string, TConfItem>::const_iterator getPixRegIteratorEnd(const u_int32_t& i, const u_int32_t& j) const;

		//! Determines which pixel double columns are enabled in the global register.
		/*!
		\return A mask with bits 0 to 8 representing the enabled state of the 9 pixel register double columns.
		*/
		const u_int32_t& getActivePixCols() const;

		//! Determines how many pixel double columns are enabled in the global register.
		/*!
		\return The amount of activated pixel double columns.
		*/
		const u_int32_t& getActivePixColCount() const;

		//! Determines if a pixel column is active or not.
		/*!
		\param column Column index of the pixel column to check.
		\return Returns \b true when the selected column is set active, \b false otherwise.
		*/
		bool getPixColActivated(const u_int32_t& column) const;

		//! Returns the total number of pixel columns.
		/*!
		\return Returns the total number of pixel columns.
		*/
		static const u_int32_t& getNumPixCols();

		//! Returns the total number of pixel rows.
		/*!
		\return Returns the total number of pixel rows.
		*/
		static const u_int32_t& getNumPixRows();

		//! Sets a value in the command register by name.
		/*!
		\param item Configuration item to be set.
		\param value New value of the configuration item.
		*/
		void setCommandRegValue(const std::string& item, const mrf::registertype& value);

		//! Sets the frontend default address.
		/*!
		Sets the frontend address in the command register and stores this as the default frontend address to use.
		\param address The new frontend address.
		*/
		void setDefaultAddress(const mrf::registertype& address);

		//! Sets the broadcast bit default value.
		/*!
		Sets the broadcasat bit in the command register and stores this value as default.
		\param broadcast The new value of the broadcast bit.
		*/
		void setDefaultBroadcast(const mrf::registertype& broadcast);

		//! Retrieves the frontend default address.
		/*!
		\return Default address.
		*/
		const mrf::registertype& getDefaultAddress() const;

		//! Retrieves the broadcast bit default value.
		/*!
		\return Broadcast bit default value.
		*/
		const mrf::registertype& getDefaultBroadcast() const;

		//! Sets a value in the global register by name.
		/*!
		\param item Configuration item to be set.
		\param value New value of the configuration item.
		*/
		void setGlobRegValue(const std::string& item, const mrf::registertype& value);

		//! Sets a value in the pixel register by name.
		/*!
		\param item Configuration item to be set.
		\param i Column index of the pixel.
		\param j Row index of the pixel.
		\param value New value of the configuration item.
		*/
		void setPixRegValue(const std::string& item, const u_int32_t& i, const u_int32_t& j, const mrf::registertype& value);

		//! Gets a value in the command register by name.
		/*!
		\param item Configuration item to be set.
		\return Current value of the configuration item.
		*/
		const u_int32_t& getCommandRegValue(const std::string& item) const;

		//! Gets a value in the command register by name.
		/*!
		\param item Configuration item to be set.
		\return Current value of the configuration item.
		*/
		const u_int32_t& getGlobRegValue(const std::string& item) const;

		//! Gets a value in the command register by name.
		/*!
		\param item Configuration item to be set.
		\param i Column index of the pixel.
		\param j Row index of the pixel.
		\return Current value of the configuration item.
		*/
		const u_int32_t& getPixRegValue(const std::string& item, const u_int32_t& i, const u_int32_t& j) const;

		void updatePixRegPosCache() const;
		bool pixRegPosCacheExpired() const;
		u_int32_t getPixRegPositionFromCoords(const u_int32_t& col, const u_int32_t& row) const;
		const u_int32_t& getPixRegPositionFromCoordsFast(const u_int32_t& col, const u_int32_t& row) const;
		u_int32_t getPixColFromPixRegPosition(const u_int32_t& position) const;
		const u_int32_t& getPixColFromPixRegPositionFast(const u_int32_t& position) const;
		u_int32_t getPixRowFromPixRegPosition(const u_int32_t& position) const;
		const u_int32_t& getPixRowFromPixRegPositionFast(const u_int32_t& position) const;


		//! Initializes all Atlas FE-I3 configuration maps.
		void initMaps();

	protected:
		//! The map holding the representation of Atlas FE-I3 command register contents.

		std::map<const std::string, TConfItem> cmdreg;

		//! The map holding the representation of Atlas FE-I3 global register contents.
		std::map<const std::string, TConfItem> globreg;

		//! The map holding the representation of Atlas FE-I3 pixel register contents.
		std::map<const std::string, TConfItem> pixreg[mrf_afei3_reglength::logpixcols][mrf_afei3_reglength::logpixrows];

	private:
		mrf::registertype feaddress;
		mrf::registertype febroadcast;
		mutable u_int32_t _activecolumns;
		mutable u_int32_t _activecolcount;
		mutable u_int32_t _pixregvalue;
		mutable u_int32_t _pixregpos[mrf_afei3_reglength::logpixcols][mrf_afei3_reglength::logpixrows];
		mutable u_int32_t _pixregcol[mrf_afei3_reglength::logpixcols*mrf_afei3_reglength::logpixrows];
		mutable u_int32_t _pixregrow[mrf_afei3_reglength::logpixcols*mrf_afei3_reglength::logpixrows];
		mutable u_int32_t _activecolcache;
};



#endif // __MRFDATA_AFEI3_H__

