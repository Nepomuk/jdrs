#include "mainwindow.h"

void MainWindow::on_pushButton_registerRead_clicked() {
    int comboBoxIndex = ui->comboBox_registerAddress->currentIndex();
    mrf::addresstype address;
    mrf::registertype value;

    if ( _registers.GetManualAddressIndex() == comboBoxIndex ) {
        address = ui->lineEdit_registerAddressManual->text().toUInt(0,16);
    } else {
        address = _registers.GetAddress(ui->comboBox_registerAddress->currentText());
    }

    value = _topixcrtl.read(address);
    ui->lineEdit_registerValue->setText(QString::number(value, 16));

    if ( Error() ) {
        ui->textEdit_Log->append("Register read failed!");
    }
}

void MainWindow::on_pushButton_registerWrite_clicked() {
    int comboBoxIndex = ui->comboBox_registerAddress->currentIndex();
    mrf::addresstype address;
    mrf::registertype value = ui->lineEdit_registerValue->text().toUInt(0, 16);

    if ( _registers.GetManualAddressIndex() == comboBoxIndex ) {
        address = ui->lineEdit_registerAddressManual->text().toUInt(0,16);
    } else {
        address = _registers.GetAddress(ui->comboBox_registerAddress->currentText());
    }

    _topixcrtl.write(address, value);

    if ( Error() ) {
        ui->textEdit_Log->append("Register write failed!");
    }
}

void MainWindow::on_comboBox_registerAddress_currentIndexChanged(int comboBoxIndex) {
    if ( _registers.GetManualAddressIndex() == comboBoxIndex ) {
        ui->lineEdit_registerAddressManual->setEnabled(true);
        ui->label_registerAddressManual->setEnabled(true);
        ui->label_registerAddress->setEnabled(false);
    } else {
        ui->lineEdit_registerAddressManual->setEnabled(false);
        ui->label_registerAddressManual->setEnabled(false);
        ui->label_registerAddress->setEnabled(true);
    }
}


//
// DMA register handling
//

void MainWindow::on_pushButton_DMAread_clicked() {
    ui->textEdit_DMAoutput->clear();

    //mrf::addresstype address = _registers.GetAddress(ui->comboBox_DMAaddress->currentText());
    u_int32_t length = ui->spinBox_DMAwordCount->value();

    TMrfData DMAdata;
    _topixcrtl.readOutputBuffer(DMAdata, length);
    ui->label_DMAwordcount->setText(QString("%1").arg(DMAdata.getNumWords()));

    if ( DMAdata.getNumWords() > 0 ) {
        QString line;
        int number = DMAdata.getNumWords()-1;
        int digits = 0;
        if ( number <= 0 )
            digits = 1;
        else
            while ( number > 0 ) { number /= 10; digits++; }

        for ( unsigned int i = 0; i < DMAdata.getNumWords(); i++ ) {
            line = QString("%1: 0x%2").arg(i,digits,10,QLatin1Char('0')).arg((unsigned int)DMAdata.getWord(i),0,16);
            ui->textEdit_DMAoutput->append( line );
        }
    }
}

void MainWindow::on_pushButton_DMAclear_clicked() {
    ui->textEdit_DMAoutput->clear();
}
