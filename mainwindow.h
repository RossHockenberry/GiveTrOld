#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QLabel>
#include <QLayout>
#include <QTableWidget>
#include <QListWidget>
#include "mydataobject.h"
#include "simplemessagebox.h"
#include "settingsdialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSettings               *   ReturnSettings() { return pSettings; };
private:
    bool                    InitObject();
    bool                    InitWidgets();
    void                    InitMenuAndToolbar();
    bool                    SetLayout();
    bool                    SetSlots();
    bool                    LoadSettings();
    void                    EditSettings();

//  Enable and Disable Sections.
    void                    EnableTransFields();
    void                    DisableTransFields();
    void                    EnableAccountFields();
    void                    DisableAccountFields();

//  Other member functions.
    void                    FillAccountList();

//  Database.
    MyDataObject            *   pData;
    bool                    bDbOpenFlag = false;

//  Data Members.
    std::string sDBDriver;                      //  Driver type.
    std::string sDBName;                        //  Database connection strings.
    std::string sDBHost;
    std::string sDBAddress;
    std::string sDBPort;
    std::string sDBUser;
    std::string sDBPassword;

    QRect       * pScreenRect;                  //  Screen and Window stuff.
    QScreen     * pScreen;
    QWindow     * pWindow;

    QWidget                 *   pCentralWidget;
    QDate                   *   pDate;
    QSettings               *   pSettings;
    QAction                 *   pEditSettings;

    QTableWidget            *   pTransTable;
    QListWidget             *   pAccountList;

// Transaction Fields.
    QLineEdit               *   pAmount;
    QLineEdit               *   pCheckNumber;
    QDateTimeEdit           *   pTransDate;
    QComboBox               *   pPaidHow;

//  Account Fields.
    QDateTimeEdit           *   pAccountDate;
    QLineEdit               *   pAccountName;
    QTextEdit               *   pAccountDesc;
    QLineEdit               *   pAccountTotal;

    QToolBar                *   pMainToolBar;
    QPushButton             *   pNewAccountButton;
    QPushButton             *   pNewTransButton;
    QPushButton             *   pLastGivenReport;

    QLabel                  *   pTransTableLabel;
    QLabel                  *   pAccountListLabel;
    QLabel                  *   pSelectedAccountLabel;
    QLabel                  *   pPayee;

    QMenu                   *   pFileMenu;
    QMenu                   *   pToolMenu;
    QMenu                   *   pHelpMenu;

    QAction                 *   pLeave;

    QVBoxLayout             *   pMainLayout;
    QHBoxLayout             *   pUpperLayout;
    QFormLayout             *   pUpperLeftLayout;
    QVBoxLayout             *   pUpperRightLayout;
    QHBoxLayout             *   pLowerLayout;
    QFormLayout             *   pLowerLeftLayout;
    QVBoxLayout             *   pLowerRightLayout;

public slots:
    void                    AccountListDoubleClicked(QListWidgetItem *);
    bool                    NewTransaction();
    bool                    NewAccount();
};
#endif // MAINWINDOW_H
