#include "myinclude.h"
#include "mainwindow.h"
#include "data_support.h"
#include "settingsdialog.h"


SettingsDialog::SettingsDialog(QWidget * pParent) : QDialog(pParent,Qt::Tool)
{
        pMain       =   dynamic_cast<MainWindow *>(pParent);
        pSettings   =   pMain->ReturnSettings();
        return;
}

SettingsDialog::~SettingsDialog()
{

        return;
}

void SettingsDialog::OnCreate()
{
//        oDefaultFont =  pMain->ReturnDefaultFont();     //  Get the initial default font.
        InitWidgets();
        SetLayout();
        SetSlots();
        LoadKeys();
        FillEditFields();
        return;
}

void SettingsDialog::InitWidgets()
{
        pKeyList    =   new QStringList();
        *pKeyList   =   pSettings->allKeys();

//  Init layouts.

        pWindowLayout   =   new QVBoxLayout();
        pFormLayout     =   new QFormLayout();
        pButtonLayout   =   new QHBoxLayout();

//  Buttons.
        pCancelButton       =   new QPushButton("&Cancel");
        pCloseButton        =   new QPushButton("C&lose");
        pDefaultsButton     =   new QPushButton("Default  Font");
        pSaveButton         =   new QPushButton("Save");

//  Line Edits.
        pDBName             =   new QLineEdit();
        pHostName           =   new QLineEdit();
        pHostAddress        =   new QLineEdit();
        pPortNumber         =   new QLineEdit();
        pUserName           =   new QLineEdit();
        pUserPassword       =   new QLineEdit();
//        pEditorFont         =   new QLineEdit();

        return;
}

void SettingsDialog::DumpSettings()
{
        for(int iIdx = 0; iIdx < pKeyList->size() ; iIdx++)
        {
            POUTNLF(pKeyList->at(iIdx).toStdString());
            POUT(ReturnValue(pKeyList->at(iIdx).toStdString()));
        }
        return;
}

//-------------------------------------------------------------------------------------------
//  Currently store items in the settings.
//      Database Name
//      Host Name
//      Host Address
//      Port Number
//      User Name
//      Password
//-------------------------------------------------------------------------------------------

void SettingsDialog::SetLayout()
{
//  Add the widgets.
        pFormLayout->addRow("Database Name" , pDBName);
        pFormLayout->addRow("Host Name" , pHostName);
        pFormLayout->addRow("Host Address" , pHostAddress);
        pFormLayout->addRow("Port Number" , pPortNumber);
        pFormLayout->addRow("User Name" , pUserName);
        pFormLayout->addRow("Password" , pUserPassword);

        pButtonLayout->addWidget(pSaveButton);
        pButtonLayout->addWidget(pDefaultsButton);
        pButtonLayout->addWidget(pCancelButton);
        pButtonLayout->addWidget(pCloseButton);

//  Layouts.
        pWindowLayout->addLayout(pFormLayout);
        pWindowLayout->addLayout(pButtonLayout);

        setLayout(pWindowLayout);
        return;
}

void SettingsDialog::SetSlots()
{
        connect(pCloseButton , &QPushButton::pressed , this , &SettingsDialog::CloseAction);
        connect(pSaveButton , &QPushButton::pressed , this , &SettingsDialog::SaveAction);
        connect(pDefaultsButton , &QPushButton::pressed , this , &SettingsDialog::DefaultAction);
        connect(pCancelButton , &QPushButton::pressed , this , &SettingsDialog::CancelAction);

        return;
}

bool SettingsDialog::LoadKeys()
{
        *pKeyList = pSettings->allKeys();
        if(pKeyList->isEmpty())         //  No keys!
        {
            WriteDefaults();            //  Should only need to do this once.
            return false;
        }
        return true;
}

bool SettingsDialog::SetKeyPair(std::string sKey , std::string sValue)
{
        pSettings->setValue(sKey.c_str() , sValue.c_str());
        return true;
}

bool SettingsDialog::SetKeyPair(std::string sKey , bool bFlag)
{
        pSettings->setValue(sKey.c_str() , bFlag);
        return true;
}

bool SettingsDialog::SetKeyPair(std::string sKey , QFont oFont)
{
        pSettings->setValue(sKey.c_str() , oFont);
        return true;
}

bool SettingsDialog::RemoveKeyPair(std::string sKey)
{
        if(pKeyList->isEmpty())
            return false;               //  Nothing to remove.

        pSettings->remove(sKey.c_str());        //  Not tested yet.
        return true;
}

std::string SettingsDialog::ReturnValue(std::string sKey)
{
    std::string sValue;

        sValue = pSettings->value(sKey.c_str()).toString().toStdString();
        return sValue;
}

bool SettingsDialog::ReturnBoolValue(std::string sKey)
{
        return pSettings->value(sKey.c_str()).toBool();
}

QFont SettingsDialog::ReturnFontValue(std::string sKey)
{
    QFont oFont;

        oFont = pSettings->value(sKey.c_str()).value<QFont>();
        return oFont;
}

bool SettingsDialog::WriteDefaults()
{
        SetKeyPair("DatabaseName","none");
        SetKeyPair("DBName" , "none");
        SetKeyPair("DBHost" , "none");
        SetKeyPair("DBAddress" , "none");
        SetKeyPair("DBPort" , "none");
        SetKeyPair("DBUser" , "none");
        SetKeyPair("DBPassword" , "none");
        UpdateSettings();
        return true;
}

bool SettingsDialog::UpdateSettings()
{
        pSettings->sync();
        auto it = pSettings->status();
        if(it != QSettings::NoError)
            return false;

        return true;
}

void SettingsDialog::CloseAction()
{
        close();
        return;
}

void SettingsDialog::CancelAction()
{
        FillEditFields();   //  For right now.  Maybe change later.
        return;
}

void SettingsDialog::DefaultAction()
{

        FillEditFields();
        return;
}

/*
 * Key Names.
        DBName      string
        DBHost
        DBAddress
        DBPort
        DBUser
        DBPassword
        EditorFont
        FullScreen
*/
void SettingsDialog::SaveAction()
{
        SetKeyPair("DBName" , pDBName->text().toStdString());
        SetKeyPair("DBHost" , pHostName->text().toStdString());
        SetKeyPair("DBAddress" , pHostAddress->text().toStdString());
        SetKeyPair("DBPort" , pPortNumber->text().toStdString());
        SetKeyPair("DBUser" , pUserName->text().toStdString());
        SetKeyPair("DBPassword" , pUserPassword->text().toStdString());

        return;
}

void SettingsDialog::FillEditFields()
{
    std::string sType = "String";

        pDBName->setText(ReturnValue("DBName").c_str());

        pHostName->setText(ReturnValue("DBHost").c_str());
        pHostAddress->setText(ReturnValue("DBAddress").c_str());
        pPortNumber->setText(ReturnValue("DBPort").c_str());
        pUserName->setText(ReturnValue("DBUser").c_str());
        pUserPassword->setText(ReturnValue("DBPassword").c_str());

         return;
}







