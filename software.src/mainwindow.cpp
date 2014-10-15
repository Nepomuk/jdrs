#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
     _topixcrtl()
{
    _errorstrcon = new TMrfStrError();
    ui->setupUi(this);

    on_toolButton_refreshOwnIP_clicked();

    // get the possible register addresses and put it into the comboBox
    _registers = Register();
    _registers.FillComboBox(ui->comboBox_registerAddress);
    //_registers.FillComboBoxDMA(ui->comboBox_DMAaddress);
}

MainWindow::~MainWindow() {
    delete ui;
}


bool MainWindow::Error() {
    if ( (_topixcrtl.getLastError() == 0)) {
        return false;
    }
    else {
        _topixcrtl.getLastError();
        _errorstrcon->getErrorStr(_topixcrtl.getLastError());

        QString prefixMsg = "<span style=\"font-weight: bold; color: red;\">[ERROR]</span> ";
        QString errorMsg = QString::fromStdString(_errorstrcon->getErrorStr(_topixcrtl.getLastError()));
        Print(prefixMsg + errorMsg);
        return true;
    }
}

void MainWindow::Print(const QString& str) {
    if (str != ""){
        ui->textEdit_Log->append(str);
        //statusbar->showMessage(str, 5000);
    }
}

