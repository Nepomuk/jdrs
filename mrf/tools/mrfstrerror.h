#ifndef __MRFSTRERROR_H__
#define __MRFSTRERROR_H__

#include <iostream>
#include <map>
#include <string>
#include "mrfgal.h"

class TMrfStrError
{
	public:
		TMrfStrError();
		~TMrfStrError();
		//! Translates the integer error code into a readable string
		/*!
		\return A string of all errors occured after last successful command. Success returns SuccessString.
		
		<b>Implementations notes:</b>
		The standard implementation should work in most cases.
		*/
		const std::string& getErrorStr(u_int32_t errornum);
		void setDelimiter(std::string delimiter = "\n");
		void setSuccessString(std::string successstring="");
		std::string getDelimiter();
		std::string getSuccessString();
		
	private:
		std::map<u_int32_t,std::string> _error;
		std::map<u_int32_t,std::string>::reverse_iterator rit;
		std::string _errorstr;
		std::string _delimiter;
};
#endif // __MRFSTRERROR_H__
