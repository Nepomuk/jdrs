#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mrfstrerror.h"
#include "mrfcal_topix.h"
#include "mrfdata/mrfdata_ltc.h"
#include "mrfdata/mrfdata_ltc2604.h"
#include "register.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
  private:
    Ui::MainWindow *ui;
    TMrfCal_Topix _topixcrtl;
    TMrfStrError* _errorstrcon;
    std::map<std::string,std::string> _hostAddressMap;

    Register _registers;

    bool Error();
    void Print(const QString& str);

  private slots:

    // ----------------------------
    // -- Tab: Connection        --
    // ----------------------------

    void on_pushButton_Connect_clicked();
    void on_pushButton_Disconnect_clicked();
    void on_toolButton_refreshOwnIP_clicked();
    void on_pushButton_Ping_clicked();


    // ----------------------------
    // -- Tab: Register          --
    // ----------------------------

    // register handling
    void on_pushButton_registerRead_clicked();
    void on_pushButton_registerWrite_clicked();
    void on_comboBox_registerAddress_currentIndexChanged(int comboBoxIndex);

    // DMA register handling
    void on_pushButton_DMAread_clicked();
    void on_pushButton_DMAclear_clicked();

};

#endif // MAINWINDOW_H
