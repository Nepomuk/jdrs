/*============================================================*/
/* qmrftools.h                                                */
/* Qt <-> MRF Toolbox                                         */
/*                                               M.C. Mertens */
/*============================================================*/


#ifndef __QMRFTOOLS_H__
#define __QMRFTOOLS_H__

#include "mrfdataadv1d.h"
#include "mrfdata_afei3.h"
#include "mrfcal_afei3.h"
#include "mrfdata_ltc2620.h"
#include <QTableWidget>
#include <QComboBox>
#include <map>
#include <string>

namespace qmrftools {

// Atlas Helpers
void assignCommandValues(TMrfData_AFEI3& fe, const QTableWidget& table);
void assignGlobalValues(TMrfData_AFEI3& fe, const QTableWidget& table);

// Generic Helpers
void fillItemComboBox(const std::map<const std::string, TConfItem>::const_iterator& start, const std::map<const std::string, TConfItem>::const_iterator& stop, QComboBox& combobox);
void clearItemTable(const int& itemcount, QTableWidget& table);
void fillItemTable(const std::map<std::string, TConfItem>::const_iterator& start, const std::map<std::string, TConfItem>::const_iterator& stop, QTableWidget& table);
void fillItemTable(const TMrfDataAdv1D& data, QTableWidget& table, const int& base = 10);
void fillItemTable(const TMrfDataAdv1D::itemMap& data, QTableWidget& table, const int& base = 10);
void assignItemValues(TMrfDataAdv1D& data, const QTableWidget& table, const int& base = 10);

// Topix Helpers
void assignDACValues(TMrfData_LTC2620& data, const QTableWidget& table);
void assignDACActivated(TMrfData_LTC2620& data, const QTableWidget& table);

// Atlas Readout Helpers
void prepareGlobRegTest(TMrfData_AFEI3& frontendimage, const mrf::registertype address, const std::string& startpattern, const std::string& midpattern, const std::string endpattern);
void preparePixRegTest(TMrfData_AFEI3& frontendimage, const mrf::registertype enabledcolumncount, const mrf::registertype address, const std::string& startpattern, const std::string& midpattern, const std::string endpattern);
void readAndResample(TMrfCal_AFEI3& remotedevice, const TMrfData_AFEI3& shiftdata, TMrfData_AFEI3& receivedata, const u_int32_t offset, const u_int32_t factor, const bool reversewords, const u_int32_t truncate);
void prepareThresholdscanGlobal(TMrfData_AFEI3& frontendimage, const u_int32_t& vcaldac);

}


#endif // __QMRFTOOLS_H__
