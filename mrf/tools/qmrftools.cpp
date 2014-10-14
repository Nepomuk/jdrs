/*============================================================*/
/* qmrftools.cpp                                              */
/* Qt <-> MRF Toolbox                                         */
/*                                               M.C. Mertens */
/*============================================================*/


#include "qmrftools.h"
#include "mrfdataadv1d.h"
#include <string>

void qmrftools::assignCommandValues(TMrfData_AFEI3& fe, const QTableWidget& table)
{
	int i;
	fe.clearCommandReg();	
	for (i = 0; i < table.rowCount(); ++i) {
		fe.setCommandRegValue(table.item(i, 0)->text().toStdString(), table.item(i, 5)->text().toInt());
	}
}

void qmrftools::assignGlobalValues(TMrfData_AFEI3& fe, const QTableWidget& table)
{
	int i;
	fe.clearGlobReg();
	for (i = 0; i < table.rowCount(); ++i) {
		fe.setGlobRegValue(table.item(i, 0)->text().toStdString(), table.item(i, 5)->text().toInt());
	}
}

void qmrftools::fillItemComboBox(const std::map<const std::string, TConfItem>::const_iterator& start, const std::map<const std::string, TConfItem>::const_iterator& stop, QComboBox& combobox)
{
	std::map<const std::string, TConfItem>::const_iterator iter;
	combobox.clear();
	for (iter = start; iter != stop; ++iter) {
		combobox.addItem(QString::fromStdString(iter->first));
	}
}

void qmrftools::clearItemTable(const int& itemcount, QTableWidget& table)
{
	table.clear();
	table.setColumnCount(6);
	table.setHorizontalHeaderLabels(QStringList() << "Item" << "Pos" << "Len" << "Min" << "Max" << "Value");
	table.setRowCount(itemcount);
	table.resizeColumnToContents(4);
	table.resizeColumnToContents(3);
	table.resizeColumnToContents(2);
	table.resizeColumnToContents(1);
	table.resizeColumnToContents(0);
	table.resizeRowsToContents();
}

void qmrftools::fillItemTable(const std::map<std::string, TConfItem>::const_iterator& start, const std::map<std::string, TConfItem>::const_iterator& stop, QTableWidget& table)
{
	std::map<const std::string, TConfItem>::const_iterator iter;
	u_int32_t row = 0;
	for (iter = start; iter != stop; ++iter) {
		table.setItem(row, 0, new QTableWidgetItem(QString::fromStdString(iter->first), 0));
		if (iter->second.length == 1) {
			table.setItem(row, 1, new QTableWidgetItem(QString::number(iter->second.position), 0));
		} else {
			table.setItem(row, 1, new QTableWidgetItem(QString::number(iter->second.position) + QString(":") + QString::number(iter->second.position + iter->second.length - 1), 0));
		}
		table.setItem(row, 2, new QTableWidgetItem(QString::number(iter->second.length), 0));
		table.setItem(row, 3, new QTableWidgetItem(QString::number(iter->second.min), 0));
		table.setItem(row, 4, new QTableWidgetItem(QString::number(iter->second.max), 0));
		table.setItem(row, 5, new QTableWidgetItem(QString::number(iter->second.value), 0));
		++row;
	}
	table.resizeColumnToContents(4);
	table.resizeColumnToContents(3);
	table.resizeColumnToContents(2);
	table.resizeColumnToContents(1);
	table.resizeColumnToContents(0);
	table.resizeRowsToContents();
}

void qmrftools::fillItemTable(const TMrfDataAdv1D& data, QTableWidget& table, const int& base)
{
	std::map<std::string, TConfItem>::const_iterator start = data.getConstItemIteratorBegin();
	std::map<std::string, TConfItem>::const_iterator stop = data.getConstItemIteratorEnd();
	std::map<const std::string, TConfItem>::const_iterator iter;
	u_int32_t row = 0;
	for (iter = start; iter != stop; ++iter) {
		table.setItem(row, 0, new QTableWidgetItem(QString::fromStdString(iter->first), 0));
		if (iter->second.length == 1) {
			table.setItem(row, 1, new QTableWidgetItem(QString::number(iter->second.position), 0));
		} else {
			table.setItem(row, 1, new QTableWidgetItem(QString::number(iter->second.position) + QString(":") + QString::number(iter->second.position + iter->second.length - 1), 0));
		}
		table.setItem(row, 2, new QTableWidgetItem(QString::number(iter->second.length), 0));
		table.setItem(row, 3, new QTableWidgetItem(QString::number(iter->second.min), 0));
		table.setItem(row, 4, new QTableWidgetItem(QString::number(iter->second.max), 0));
		if (base == 1) {
			table.setItem(row, 5, new QTableWidgetItem(QString::number(iter->second.value, base).rightJustified(32, '0'), 0));
		} else {
			table.setItem(row, 5, new QTableWidgetItem(QString::number(iter->second.value, base), 0));
		}
		++row;
	}
	table.resizeColumnToContents(4);
	table.resizeColumnToContents(3);
	table.resizeColumnToContents(2);
	table.resizeColumnToContents(1);
	table.resizeColumnToContents(0);
	table.resizeRowsToContents();
}

void qmrftools::fillItemTable(const TMrfDataAdv1D::itemMap& data, QTableWidget& table, const int& base)
{
	std::map<std::string, TConfItem>::const_iterator start = data.begin();
	std::map<std::string, TConfItem>::const_iterator stop = data.end();
	std::map<const std::string, TConfItem>::const_iterator iter;
	u_int32_t row = 0;
	for (iter = start; iter != stop; ++iter) {
		table.setItem(row, 0, new QTableWidgetItem(QString::fromStdString(iter->first), 0));
		if (iter->second.length == 1) {
			table.setItem(row, 1, new QTableWidgetItem(QString::number(iter->second.position), 0));
		} else {
			table.setItem(row, 1, new QTableWidgetItem(QString::number(iter->second.position) + QString(":") + QString::number(iter->second.position + iter->second.length - 1), 0));
		}
		table.setItem(row, 2, new QTableWidgetItem(QString::number(iter->second.length), 0));
		table.setItem(row, 3, new QTableWidgetItem(QString::number(iter->second.min), 0));
		table.setItem(row, 4, new QTableWidgetItem(QString::number(iter->second.max), 0));
		if (base == 1) {
			table.setItem(row, 5, new QTableWidgetItem(QString::number(iter->second.value, base).rightJustified(32, '0'), 0));
		} else {
			table.setItem(row, 5, new QTableWidgetItem(QString::number(iter->second.value, base), 0));
		}
		++row;
	}
	table.resizeColumnToContents(4);
	table.resizeColumnToContents(3);
	table.resizeColumnToContents(2);
	table.resizeColumnToContents(1);
	table.resizeColumnToContents(0);
	table.resizeRowsToContents();
}

void qmrftools::assignItemValues(TMrfDataAdv1D& data, const QTableWidget& table, const int& base)
{
	int i;
	for (i = 0; i < table.rowCount(); ++i) {
		data.setLocalItemValue(table.item(i, 0)->text().toStdString(), table.item(i, 5)->text().toInt(0, base));
	}
}

void qmrftools::assignDACValues(TMrfData_LTC2620& data, const QTableWidget& table)
{
	int i;
	for (i = 0; i < table.rowCount(); ++i) {
		data.setDACValue(table.item(i, 0)->text().toStdString(), table.item(i, 5)->text().toInt());
	}
}

void qmrftools::assignDACActivated(TMrfData_LTC2620& data, const QTableWidget& table)
{
	int i;
	for (i = 0; i < table.rowCount(); ++i) {
		data.setDACActivated(table.item(i, 0)->text().toStdString(), table.item(i, 5)->text().toInt());
	}
}

void qmrftools::prepareGlobRegTest(TMrfData_AFEI3& frontendimage, const mrf::registertype address, const std::string& startpattern, const std::string& midpattern, const std::string endpattern)
{	
	//prepare container
	frontendimage.clearDataStream();
	frontendimage.clearCommandReg();
	frontendimage.clearGlobReg();
	frontendimage.setCommandRegValue("Address", address);
	frontendimage.setCommandRegValue("ClockGlobal", 1);
	frontendimage.assembleCommandReg();
	frontendimage.setGlobRegValue("SelectDO", 15);
	//frontendimage.setGlobRegValue("HitBusScaler", 0);
	//generate testpattern
	std::string testpattern = startpattern;
	while (testpattern.length() < mrf_afei3_reglength::global) {
		testpattern.append(midpattern);
	}
	testpattern.erase(mrf_afei3_reglength::global - endpattern.length());
	testpattern.append(endpattern);
	//import testpattern to datastream and correct output mux
	frontendimage.importBinString(testpattern, 29);
	frontendimage.assembleGlobRegValue("SelectDO");
	//frontendimage.assembleGlobRegValue("HitBusScaler");
}

void qmrftools::preparePixRegTest(TMrfData_AFEI3& frontendimage, const mrf::registertype enabledcolumncount, const mrf::registertype address, const std::string& startpattern, const std::string& midpattern, const std::string endpattern)
{
	//prepare container
	frontendimage.clearDataStream();
	frontendimage.clearCommandReg();
	frontendimage.clearPixReg();
	frontendimage.setCommandRegValue("Address", address);
	frontendimage.setCommandRegValue("ClockPixel", 1);
	frontendimage.assembleCommandReg();
	//generate testpattern
	std::string testpattern = startpattern;
	while (testpattern.length() < enabledcolumncount * TMrfData_AFEI3::getNumPixRows()) {
		testpattern.append(midpattern);
	}
	testpattern.erase(enabledcolumncount * TMrfData_AFEI3::getNumPixRows() - endpattern.length());
	testpattern.append(endpattern);
	//import testpattern to datastream and correct output mux
	frontendimage.importBinString(testpattern, 29);
	frontendimage.assembleGlobRegValue("SelectDO");
}

void qmrftools::readAndResample(TMrfCal_AFEI3& remotedevice, const TMrfData_AFEI3& shiftdata, TMrfData_AFEI3& receivedata, const u_int32_t offset, const u_int32_t factor, const bool reversewords, const u_int32_t truncate)
{
	remotedevice.prepareOutputBuffer(shiftdata.getNumWords() * (factor + 1));
	remotedevice.shiftIntoGlobReg(shiftdata);
	remotedevice.readOutputBuffer(receivedata, shiftdata.getNumWords() * (factor + 1));
	receivedata.resample(offset, factor, reversewords, truncate);
}

void qmrftools::prepareThresholdscanGlobal(TMrfData_AFEI3& frontendimage, const u_int32_t& vcaldac)
{
	frontendimage.setGlobRegValue("VCalDAC", vcaldac);
}
