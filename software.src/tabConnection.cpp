#include "mainwindow.h"

void MainWindow::on_pushButton_Connect_clicked() {
    // check for empty fields
    if ( ui->comboBox_ownIPAdresses->currentText().length() == 0 ||
         ui->spinBox_ownPort->value() == 0 ||
         ui->lineEdit_remoteIPAddress->text().length() == 0 ||
         ui->spinBox_remotePort->value() == 0 ) {
        ui->textEdit_Log->append("One of the required fields for the connection is empty, please check.");
        return;
    }

    QStringList ownIP = ui->comboBox_ownIPAdresses->currentText().split(" ");
    QString connectionParameter = QString("%1,%2,%3,%4")
            .arg(ownIP[0])
            .arg(ui->spinBox_ownPort->value())
            .arg(ui->lineEdit_remoteIPAddress->text())
            .arg(ui->spinBox_remotePort->value());

    _topixcrtl.openDevice( connectionParameter.toLatin1() );

    if (!Error()) {
        ui->pushButton_Connect->setEnabled(false);
        ui->pushButton_Disconnect->setEnabled(true);
        ui->textEdit_Log->append("Device successfully opened!");
    }
}

void MainWindow::on_pushButton_Disconnect_clicked() {
    _topixcrtl.closeDevice();
    if (!Error()) {
        ui->pushButton_Connect->setEnabled(true);
        ui->pushButton_Disconnect->setEnabled(false);
        ui->textEdit_Log->append("Device successfully closed!");
    }
}


void MainWindow::on_toolButton_refreshOwnIP_clicked() {
    _hostAddressMap = _topixcrtl.getIfAddresses();
    QString selectedItem = ui->comboBox_ownIPAdresses->currentText();

    // clean the current list first
    while ( ui->comboBox_ownIPAdresses->count() > 0 ) {
        ui->comboBox_ownIPAdresses->removeItem(0);
    }

    std::map<std::string,std::string>::iterator it;
    QString currentItem;
    for ( it = _hostAddressMap.begin(); it != _hostAddressMap.end(); it++ ) {
        // skip localhost and loopback devices
        if ( (*it).first.compare("127.0.0.1") == 0 ||
             (*it).second.compare("lo0") == 0 ||
             (*it).second.compare("lo") == 0 )
            continue;

        currentItem = QString("%1 (%2)").arg( (*it).first.c_str() ).arg( (*it).second.c_str() );
        ui->comboBox_ownIPAdresses->addItem( currentItem );

        // set the selected item to the old one
        if ( !selectedItem.isEmpty() && currentItem.compare(selectedItem) == 0 )
            ui->comboBox_ownIPAdresses->setCurrentIndex( ui->comboBox_ownIPAdresses->count()-1 );
    }
}


void MainWindow::on_pushButton_Ping_clicked() {
    QIcon pingResultGood(":/icons/Check.png");
    QIcon pingResultBad(":/icons/Delete.png");

    if ( _topixcrtl.deviceIsOnline() ) {
        ui->label_pingResponseIcon->setPixmap( pingResultGood.pixmap(16,16) );

//        _topixcrtl.write(0x4C8, 2);
//        _topixcrtl.write(0x420, 1);
//        _topixcrtl.write(0x420, 2);
//        _topixcrtl.write(0x420, 3);
//        _topixcrtl.write(0x420, 4);
//        _topixcrtl.write(0x420, 5);
//        _topixcrtl.write(0x420, 6);
//        _topixcrtl.write(0x420, 7);
//        _topixcrtl.write(0x420, 8);
//        _topixcrtl.write(0x420, 9);
//        _topixcrtl.write(0x420, 10);
//        _topixcrtl.write(0x008, 10);

    } else {
        ui->label_pingResponseIcon->setPixmap( pingResultBad.pixmap(16,16) );
    }
}
