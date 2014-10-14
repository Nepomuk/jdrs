/*============================================================*/
/* mrfdata_lcd.cpp                                            */
/* MVD Readout Framework Data Storage                         */
/* Provides generic LCD access functions and types.           */
/*                                                 A. Goerres */
/*============================================================*/

#include "mrfdata_lcd.h"
#include "mrfdataadvbase.h"
#include "mrfdataadv1d.h"
#include <string>
#include <map>


MrfData_LCD::MrfData_LCD( const u_int32_t& blocklength, const u_int32_t& defaultindex, const u_int32_t& defaultstreamoffset, const u_int32_t& defaultvalueoffset, const bool& defaultreverse, const bool& defaultstreamreverse )
    : TMrfDataAdvBase (blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse),
        TMrfDataAdv1D (blocklength, defaultindex, defaultstreamoffset, defaultvalueoffset, defaultreverse, defaultstreamreverse)
{
    //_upper_lower = 'u';
    initMaps();
}

MrfData_LCD::~MrfData_LCD()
{
}


void MrfData_LCD::initMaps()
{
    _localdata.clear();
    _localdata["LCDpos"] = TConfItem(0, 0, 8);   // bits 0-7:  position on LCD
    _localdata["LCDchar"] = TConfItem(0, 8, 8);  // bits 8-15: character to display
}


void MrfData_LCD::assemble()
{
    clearDataStream();
    setNumWords(_text.length());
    for ( uint i = 0; i < _text.length(); i++ ) {
        // fill the data word
        setStreamItemValue( "LCDpos", pos2LCD(i), i );
        setStreamItemValue( "LCDchar", char2LCD(_text.at(i)), i );
    }
}


u_int32_t MrfData_LCD::char2LCD( const char& c )
{
    u_int32_t value = 0x20;  // blank character
    int a = c; // ascii-code

    if ( a >= 0x21 && a <= 0x7D && a != 0x5C ) {
        value = a;
    }

    return value;
}

u_int32_t MrfData_LCD::pos2LCD( const int& p )
{
    // upper line starts with 128, lower one with 192
    u_int32_t value = ( isUpper() ) ? 128 : 192;

    if ( p > 0 && p < 16 ) {
        value += p;
    }

    return value;
}


void MrfData_LCD::setText( const std::string& s )
{
    _text = s;
}
std::string MrfData_LCD::getText()
{
    return _text;
}


void MrfData_LCD::setUpper()
{
    _upper_lower = 'u';
}
void MrfData_LCD::setLower()
{
    _upper_lower = 'l';
}

bool MrfData_LCD::isUpper()
{
    if ( _upper_lower == 'u' ) return true;
    else return false;
}
bool MrfData_LCD::isLower()
{
    if ( _upper_lower == 'l' ) return true;
    else return false;
}
