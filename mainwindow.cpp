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
        InitWidgets();
        InitMenuAndToolbar();
        SetLayout();
        SetSlots();
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
        pAmount                 =   new QLineEdit();
        pDateEdit               =   new QDateTimeEdit();
        pDateEdit->setDisplayFormat("dd.MM.yyyy");

        pFileMenu               =   new QMenu("&File");
        pToolMenu               =   new QMenu("&Tool");
        pHelpMenu               =   new QMenu("&Help");

        pMainLayout             =   new QVBoxLayout(this);
        pUpperLayout            =   new QHBoxLayout();
        pUpperLeftLayout        =   new QFormLayout();
        pUpperRightLayout       =   new QVBoxLayout();
        pLowerLayout            =   new QHBoxLayout();

        pMainToolBar            =   new QToolBar(this);

        pLeave                  =   new QAction("E&xit");
        return true;
}

bool MainWindow::SetLayout()
{
        setCentralWidget(pCentralWidget);

        pMainLayout->addLayout(pUpperLayout);
        pMainLayout->addLayout(pLowerLayout);

        pUpperLeftLayout->addRow("Date" , pDateEdit);
        pUpperLeftLayout->addRow("Who To" , pPayee);
        pUpperLeftLayout->addRow("Amount" , pAmount);

        pUpperRightLayout->addWidget(pAccountList);

        pUpperLayout->addLayout(pUpperLeftLayout);
        pUpperLayout->addLayout(pUpperRightLayout);

        pCentralWidget->setLayout(pMainLayout);
        return true;
}

bool MainWindow::SetSlots()
{
        connect(pLeave , &QAction::triggered , this  , &MainWindow::close);
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

}

bool MainWindow::LoadSettings()
{
        pSettings->sync();              //  Sink the settings.

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
