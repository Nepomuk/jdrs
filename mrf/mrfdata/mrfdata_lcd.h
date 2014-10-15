/*============================================================*/
/* mrfdata_lcd.h                                              */
/* MVD Readout Framework Data Storage                         */
/* Provides generic LCD access functions and types.           */
/*                                                 A. Goerres */
/*============================================================*/

#ifndef MRFDATA_LCD_H
#define MRFDATA_LCD_H

#include <map>
#include <string>
#include "mrfdataadv1d.h"
#include "mrf_confitem.h"

class MrfData_LCD : virtual public TMrfDataAdv1D
{
    public:
        MrfData_LCD(const u_int32_t& blocklength = bitsinablock,
                    const u_int32_t& defaultindex = 0,
                    const u_int32_t& defaultstreamoffset = 0,
                    const u_int32_t& defaultvalueoffset = 0,
                    const bool& defaultreverse = true,
                    const bool& defaultstreamreverse = false);
        virtual ~MrfData_LCD();

        //! Initialize the string-TConfItem map
        virtual void initMaps();

        //! Prepare the data for transmitting to the fifo
        virtual void assemble();

        //! Converts a given character to the corresponding LCD bit pattern
        virtual u_int32_t char2LCD( const char& c );

        //! Converts a given position to the corresponding LCD bit pattern
        virtual u_int32_t pos2LCD( const int& p );

        //! Fills the internal text variable with a new value
        virtual void setText( const std::string& s );
        virtual std::string getText();

        //! Set the current text for the upper/lower LCD line
        virtual void setUpper();
        virtual void setLower();
        virtual bool isUpper();
        virtual bool isLower();

    protected:

    private:
        //! private value declaring upper/lower LCD line
        char _upper_lower;

        //! private value containing the text to display on the LCD
        std::string _text;
};

#endif // MRFDATA_LCD_H
