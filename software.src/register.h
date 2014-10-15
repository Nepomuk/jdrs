#ifndef REGISTER_H
#define REGISTER_H

#include <QMainWindow>
#include "ui_mainwindow.h"

//! Register handling for the demo software
/*!
Provides a class with useful functions to handle different registers
*/
class Register
{
    public:

        Register();
        ~Register();

        //! The list of valid register values mapped to a readable name
        std::map<QString, u_int32_t> addressList;

        //! The list of valid DMA register values mapped to a readable name
        std::map<QString, u_int32_t> addressListDMA;


        //! Fills a ComboBox with register values
        /*!
        If there is a ComboBox in the software it can be automatically filled with available register addresses.
        \param comboBox: QComboBox* pointing to the ComboBox to be filled.
        */
        void FillComboBox(QComboBox *comboBox);


        //! Fills a ComboBox with DMA register values
        /*!
        If there is a ComboBox in the software it can be automatically filled with available register addresses.
        \param comboBox: QComboBox* pointing to the ComboBox to be filled.
        */
        void FillComboBoxDMA(QComboBox *comboBox);


        //! Index of the entry for manuel address input
        /*!
        \return Returns the index (int) of the manual address input
        */
        int GetManualAddressIndex();


        //! Gets the matching register address from a string
        /*!
        \param name: A string with the readable name of a register address
        \return Returns a matching register address as an unsigned int.
        */
        u_int32_t GetAddress(QString name);


    protected:

    private:
        int _manualAddressIndex;
};

#endif // REGISTER_H
