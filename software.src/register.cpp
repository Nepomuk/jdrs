#include "register.h"
#include "mrftal_rbudp.h"
#include "mainwindow.h"

// constructor
Register::Register()
{
    addressList[QString("Control")] = rb_address::control;
    addressList[QString("Dummy Data")] = 0x420;
    addressList[QString("LED Register")] = 0x4C8;

    addressListDMA[QString("DMA")] = 0x400;
}

// destructor
Register::~Register()
{
}

// put the register addresses into the ComboBox
void Register::FillComboBox(QComboBox *comboBox)
{
    std::map<QString, unsigned int>::iterator it = addressList.begin();
    while ( it != addressList.end() ) {
        comboBox->addItem(QString("%1: %2")
                          .arg(it->second,3,16,QChar('0'))
                          .arg(it->first));
        it++;
    }

    comboBox->addItem(QString("...: Manual Input"));
    _manualAddressIndex = addressList.size();
}

// put the register addresses into the ComboBox
void Register::FillComboBoxDMA(QComboBox *comboBox)
{
    std::map<QString, u_int32_t>::iterator it = addressListDMA.begin();
    while ( it != addressListDMA.end() ) {
        comboBox->addItem(QString("%1: %2")
                          .arg(it->second,3,16,QChar('0'))
                          .arg(it->first));
        it++;
    }
}

int Register::GetManualAddressIndex()
{
    return _manualAddressIndex;
}

// get the address
u_int32_t Register::GetAddress(QString name)
{
    return name.left(3).toUInt(0,16);
}
