#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
        setWindowTitle("Chris's donation tracking system!");
        resize(2000,1000);
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
        DisableFields();
        FillAccountList();
        return true;
}

bool MainWindow::InitWidgets()
{
        pCentralWidget          =   new QWidget();
        pDate                   =   new QDate(QDate::currentDate());


        pTransTable             =   new QTableWidget();
        pTransTable->setColumnCount(4);
        pTransTable->setRowCount(7);
        QStringList oTemp = {"Date of Last Give" , "Amount Last Given" ,
                                "Check #" , "Comment"};
        pTransTable->setHorizontalHeaderLabels(oTemp);
        pTransTable->setColumnWidth(1,300);

        pAccountList            =   new QListWidget();

        pTransTableLabel        =   new QLabel("Transactions");
        pTransTableLabel->setFrameStyle(QFrame::Panel  | QFrame::Raised);
        pTransTableLabel->setLineWidth(2);
        pAccountListLabel       =   new QLabel("Accounts");
        pAccountListLabel->setFrameStyle(QFrame::Panel  | QFrame::Raised);
        pAccountListLabel->setLineWidth(2);
        pSelectedAccountLabel   =   new QLabel("Selected Account");
        pBlankLabel             =   new QLabel("");
        pPayeeLabel             =   new QLabel("Payee");
        pPayeeLabel->setFrameStyle(QFrame::Panel  | QFrame::Sunken);
        pPayeeLabel->setLineWidth(2);

        pNewAccountButton       =   new QPushButton("Add New Account");
        pNewTransButton         =   new QPushButton("New Transaction");
        pSaveButton             =   new QPushButton("Save");
        pCancelButton           =   new QPushButton("Cancel");
        pDoneButton             =   new QPushButton("Done");

// Transaction Fields.
        pAmount                 =   new QLineEdit();
        pTransDate              =   new QDateTimeEdit(*pDate);
        pTransDate->setDisplayFormat("MM.dd.yyyy");
        pPaidHow                =   new QComboBox;
        pPaidHow->addItem("Check");
        pPaidHow->addItem("Credit Card");
        pPaidHow->addItem("Cash");
        pComment                =   new QTextEdit;

//  Account Fields.
        pAccountDate            =   new QDateTimeEdit(*pDate);
        pAccountDate->setDisplayFormat("MM.dd.yyyy");
        pAccountName            =   new QLineEdit();
        pAccountDesc            =   new QTextEdit();
        pAccountTotal           =   new QLineEdit();
        pAccountTotal->setReadOnly(true);

        pFileMenu               =   new QMenu("&File");
        pToolMenu               =   new QMenu("&Tool");
        pHelpMenu               =   new QMenu("&Help");

        pMainLayout             =   new QVBoxLayout(this);
        pUpperLayout            =   new QHBoxLayout();
        pUpperLeftLayout        =   new QFormLayout();
        pUpperRightLayout       =   new QVBoxLayout();
        pLowerLayout            =   new QHBoxLayout();
        pLowerLeftLayout        =   new QVBoxLayout();
        pAccountLayout          =   new QFormLayout();
        pLowerRightLayout       =   new QVBoxLayout();
        pButtonLayout           =   new QHBoxLayout();
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

        pButtonLayout->addWidget(pSaveButton);
        pButtonLayout->addWidget(pCancelButton);

        pUpperRightLayout->addWidget(pAccountListLabel);
        pUpperLeftLayout->addRow("Transaction Information -----" , pBlankLabel);
        pUpperLeftLayout->addRow("Date" , pTransDate);
        pUpperLeftLayout->addRow("Who To" , pPayeeLabel);
        pUpperLeftLayout->addRow("Amount" , pAmount);
        pUpperLeftLayout->addRow("Paid by " , pPaidHow);
        pUpperLeftLayout->addRow("Comments" , pComment);
        pUpperLeftLayout->addRow("Finished" , pDoneButton);

        pUpperRightLayout->addWidget(pAccountList);
        pUpperLayout->addLayout(pUpperLeftLayout);
        pUpperLayout->addLayout(pUpperRightLayout);

        pLowerRightLayout->addWidget(pTransTableLabel);
        pAccountLayout->addRow("Account Information -----" , pBlankLabel);
        pAccountLayout->addRow("Date" , pAccountDate);
        pAccountLayout->addRow("Name" , pAccountName);
        pAccountLayout->addRow("Description" , pAccountDesc);
        pAccountLayout->addRow("Total Given" , pAccountTotal);
        pLowerLeftLayout->addLayout(pAccountLayout);
        pLowerLeftLayout->addLayout(pButtonLayout);

//        pLowerRightLayout->addWidget(pTransTableLabel);
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

        connect(pNewAccountButton , &QPushButton::pressed , this , &MainWindow::NewAccount);
        connect(pNewTransButton , &QPushButton::pressed , this , &MainWindow::NewTransaction);

        connect(pCancelButton , &QPushButton::pressed , this , &MainWindow::CancelButton);
        connect(pSaveButton , &QPushButton::pressed , this , &MainWindow::SaveButton);
        connect(pDoneButton , &QPushButton::pressed , this , &MainWindow::DoneButton);

        connect(pAccountList , &QListWidget::itemDoubleClicked , this , &MainWindow::AccountListDoubleClicked);

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
        pPayeeLabel->setEnabled(true);
        pComment->setEnabled(true);
        pDoneButton->setEnabled(true);

        return;
}

void MainWindow::DisableTransFields()
{
        pAmount->setEnabled(false);
        pTransDate->setEnabled(false);
        pPaidHow->setEnabled(false);
        pPayeeLabel->setEnabled(false);
        pComment->setEnabled(false);
        pDoneButton->setEnabled(true);

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
    QString sBuild;

        vAccounts.clear();          //  Clear the list first.

        sBuild = "SELECT account_id , name FROM account ORDER BY name";
        pData->ReturnQueryData(sBuild.toStdString() , vAccounts);

        for(auto & it : vAccounts)
            pAccountList->addItem(it.sKeyString.c_str());

        return;
}

void MainWindow::FillTransactionTable()
{
    QString sBuild;

        sBuild = QString("Select origin_date , amount , check # , comment"
                            " FROM transaction WHERE account_id = %1")
                                .arg(iSelectedAccount);
        return;
}

void MainWindow::FillAccountFields()
{
    QString sBuild;
    stResultData stResult;

        sBuild = QString("SELECT * FROM account WHERE account_id = %1").arg(iSelectedAccount);
        pData->ReturnQueryData(sBuild.toStdString() , stResult);

        pAccountName->setText(stResult.vColumns[0][1].c_str());
        pAccountDesc->setText(stResult.vColumns[0][2].c_str());
        pAccountTotal->setText(stResult.vColumns[0][4].c_str());

        return;
}

bool MainWindow::WriteNewAccountRecord()
{
    QString sBuild;

//  Escape single quotes.
        QString sName       =   QString(pAccountName->text()).replace("'","''");
        QString sDesc       =   QString(pAccountDesc->toPlainText()).replace("'","''");

//  First we need to read back the data and build a query string.
        sBuild.append("INSERT INTO account ");
        sBuild.append("(name , description , total_given) ");

//  Double single quotes work for Postgresql - not sure about others.
//  I'll have to check on it.   Backspace as an escape doesn't work.
        sBuild.append("VALUES ('");
        sBuild += sName + "','";
        sBuild += sDesc + "','0.0'";

        pData->SendDatabaseQuery(sBuild.toStdString());
        return true;
}

bool MainWindow::WriteNewTransactionRecord()
{
    QString sBuild;
    QDate   oDate;

//  Escape Single Quotes.

        QString sHow_Paid       =   pPaidHow->currentText();
        oDate                   =   pTransDate->date();
        QString sTransDate      =   oDate.toString();
        QString sAmount         =   pAmount->text();
        QString sComment        =   pComment->toPlainText();
        QString sCheck_Number   =   pCheckNumber->text();

        sBuild = "INSERT INTO transaction "
                    "(origin_date , amount , how_paid , check_number "
                    " comment , account_id";

        return true;
}

void MainWindow::ClearTransFields()
{
        pAmount->clear();
        pTransDate->clear();

        return;
}

void MainWindow::ClearAccountFields()
{
        pAccountDate->clear();
        pAccountName->clear();
        pAccountDesc->clear();
        pAccountTotal->clear();

        return;
}

bool MainWindow::NewTransaction()
{
        if(iSelectedAccount == -1)
            return false;
        EnableTransFields();
        return true;
}

bool MainWindow::NewAccount()
{
        EnableAccountFields();

        return true;
}

void MainWindow::AccountListDoubleClicked(QListWidgetItem * pItem)
{
        sSelectedAccount = pItem->text().toStdString();

        for(auto & it : vAccounts)
            if(it.sKeyString == sSelectedAccount)
            {
                iSelectedAccount = it.iKey;
                pPayeeLabel->setText(sSelectedAccount.c_str());
                EnableFields();
                FillAccountFields();
                break;
            }
        return;
}

bool MainWindow::CancelButton()
{
        ClearFields();
        DisableFields();
        return true;
}

bool MainWindow::SaveButton()
{
    SimpleMessageBox oMB;

        if(pAccountName->text().isEmpty())
        {
            oMB.ErrorBox("Name must be entered!");
            return false;
        }
        WriteNewAccountRecord();
        FillAccountList();
        return true;
}

bool MainWindow::DoneButton()
{

        return true;
}
