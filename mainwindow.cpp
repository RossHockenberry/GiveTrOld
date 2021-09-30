#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
        resize(1000,1000);
        InitObject();
        return;
}

MainWindow::~MainWindow()
{
        return;
}

bool MainWindow::InitObject()
{
    int iCount = 0;     //  Counter for how many times we try database.
    stConnection stConn;

//  First thing is to get the settings.
        pSettings       =   new QSettings("House"  , "GiveTR" ,this);
//        EditSettings();
        QSettings::Status temp = pSettings->status();       //  Temp  error check.
        LoadSettings();

        stConn.sDriverName      =   "QPSQL";
        stConn.sDatabaseName    =   sDBName;
        stConn.sHostName        =   sDBHost;
        stConn.sHostAddress     =   sDBAddress;
        stConn.sPort            =   sDBPort;
        stConn.sUserName        =   sDBUser;
        stConn.sPassword        =   sDBPassword;

        pData = new MyDataObject(stConn);
        pData->OpenDb();

        bDbOpenFlag = pData->IsOpen();

//    MyFlag oTextFlag = bDbOpenFlag;
         while(!bDbOpenFlag)
        {
            ++iCount;
            delete pData;
            EditSettings();
            LoadSettings();
            bDbOpenFlag = pData->IsOpen();
            if(iCount == 3)
            {
                SimpleMessageBox oMB;
//                oMB.ErrorBox(.c_str());
                return false;
            }
        }

        InitWidgets();
        InitMenuAndToolbar();
        SetLayout();
        SetSlots();
        DisableAccountFields();
        DisableTransFields();
        return true;
}

bool MainWindow::InitWidgets()
{
        pCentralWidget          =   new QWidget();
        pDate                   =   new QDate(QDate::currentDate());


        pTransTable             =   new QTableWidget();
        pAccountList            =   new QListWidget();

        pTransTableLabel        =   new QLabel("Transactions");
        pAccountListLabel       =   new QLabel("Accounts");
        pSelectedAccountLabel   =   new QLabel("Selected Account");
        pPayee                  =   new QLabel("Payee");
        pPayee->setFrameStyle(QFrame::Panel  | QFrame::Sunken);
        pPayee->setLineWidth(2);

        pNewAccountButton       =   new QPushButton("Add New Account");
        pNewTransButton         =   new QPushButton("New Transaction");

// Transaction Fields.
        pAmount                 =   new QLineEdit();
        pTransDate              =   new QDateTimeEdit();
        pTransDate->setDisplayFormat("dd.MM.yyyy");
        pPaidHow                =   new QComboBox;
        pPaidHow->addItem("Check");
        pPaidHow->addItem("Credit Card");
        pPaidHow->addItem("Cash");

//  Account Fields.
        pAccountDate            =   new QDateTimeEdit();
        pAccountName            =   new QLineEdit();
        pAccountDesc            =   new QTextEdit();
        pAccountTotal           =   new QLineEdit();

        pFileMenu               =   new QMenu("&File");
        pToolMenu               =   new QMenu("&Tool");
        pHelpMenu               =   new QMenu("&Help");

        pMainLayout             =   new QVBoxLayout(this);
        pUpperLayout            =   new QHBoxLayout();
        pUpperLeftLayout        =   new QFormLayout();
        pUpperRightLayout       =   new QVBoxLayout();
        pLowerLayout            =   new QHBoxLayout();
        pLowerLeftLayout        =   new QFormLayout();
        pLowerRightLayout       =   new QVBoxLayout();
        pMainToolBar            =   new QToolBar(this);

        pLeave                  =   new QAction("E&xit");
        pEditSettings           =   new QAction("Edit &Settings");
        return true;
}

bool MainWindow::SetLayout()
{
        setCentralWidget(pCentralWidget);

        pMainLayout->addLayout(pUpperLayout);
        pMainLayout->addLayout(pLowerLayout);

        pUpperLeftLayout->addRow("Date" , pTransDate);
        pUpperLeftLayout->addRow("Who To" , pPayee);
        pUpperLeftLayout->addRow("Amount" , pAmount);
        pUpperLeftLayout->addRow("Paid by " , pPaidHow);
        pUpperRightLayout->addWidget(pAccountListLabel);
        pUpperRightLayout->addWidget(pAccountList);
        pUpperLayout->addLayout(pUpperLeftLayout);
        pUpperLayout->addLayout(pUpperRightLayout);

        pLowerLeftLayout->addRow("Date" , pAccountDate);
        pLowerLeftLayout->addRow("Name" , pAccountName);
        pLowerLeftLayout->addRow("Description" , pAccountDesc);
        pLowerLeftLayout->addRow("Total Given" , pAccountTotal);
        pLowerRightLayout->addWidget(pTransTableLabel);
        pLowerRightLayout->addWidget(pTransTable);
        pLowerLayout->addLayout(pLowerLeftLayout);
        pLowerLayout->addLayout(pLowerRightLayout);

        pCentralWidget->setLayout(pMainLayout);
        return true;
}

bool MainWindow::SetSlots()
{
        connect(pLeave , &QAction::triggered , this  , &MainWindow::close);
        connect(pEditSettings , &QAction::triggered , this , &MainWindow::EditSettings);

        return true;
}

void MainWindow::InitMenuAndToolbar()
{
        QMainWindow::menuBar()->addMenu(pFileMenu);
        QMainWindow::menuBar()->addMenu(pToolMenu);
        QMainWindow::menuBar()->addMenu(pHelpMenu);

        pFileMenu->addAction(pLeave);

        this->addToolBar(pMainToolBar);
        pMainToolBar->addWidget(pNewAccountButton);
        pMainToolBar->addSeparator();
        pMainToolBar->addWidget(pNewTransButton);

        pFileMenu->addAction(pLeave);
        pHelpMenu->addAction(pEditSettings);
}

bool MainWindow::LoadSettings()
{
        pSettings->sync();              //  Sink the settings.
        QStringList oList = pSettings->allKeys();

        sDBDriver   = "QPSQL";
        sDBName     = pSettings->value("DBName").toString().toStdString();
        sDBHost     = pSettings->value("DBHost").toString().toStdString();
        sDBAddress  = pSettings->value("DBAddress").toString().toStdString();
        sDBPort     = pSettings->value("DBPort").toString().toStdString();
        sDBUser     = pSettings->value("DBUser").toString().toStdString();
        sDBPassword = pSettings->value("DBPassword").toString().toStdString();

//  Add Menus.
        return true;
}

void MainWindow::EditSettings()
{
    SettingsDialog      *   pSettingsDialog;

        pSettingsDialog =   new SettingsDialog(this);
        pSettingsDialog->OnCreate();
        pSettingsDialog->exec();
        return;
}

void MainWindow::EnableTransFields()
{
        pAmount->setEnabled(true);
        pTransDate->setEnabled(true);
        pPaidHow->setEnabled(true);
        pPayee->setEnabled(true);

        return;
}

void MainWindow::DisableTransFields()
{
        pAmount->setEnabled(false);
        pTransDate->setEnabled(false);
        pPaidHow->setEnabled(false);
        pPayee->setEnabled(false);

        return;
}

void MainWindow::EnableAccountFields()
{
        pAccountDate->setEnabled(true);
        pAccountName->setEnabled(true);
        pAccountDesc->setEnabled(true);
        pAccountTotal->setEnabled(true);

        return;
}

void MainWindow::DisableAccountFields()
{
        pAccountDate->setEnabled(false);
        pAccountName->setEnabled(false);
        pAccountDesc->setEnabled(false);
        pAccountTotal->setEnabled(false);

        return;
}

void MainWindow::FillAccountList()
{


        return;
}

bool MainWindow::NewTransaction()
{

        return true;
}

bool MainWindow::NewAccount()
{

        return true;
}

void MainWindow::AccountListDoubleClicked(QListWidgetItem * pItem)
{

        return;
}



