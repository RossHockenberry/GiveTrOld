#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLayout>
#include <QTableWidget>
#include <QListWidget>
#include "mydataobject.h"


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

    QTableWidget            *   pTransTable;
    QListWidget             *   pAccountList;

    QLineEdit               *   pAmount;
    QDateTimeEdit           *   pDateEdit;

    QToolBar                *   pMainToolBar;
    QPushButton             *   pNewAccountButton;
    QPushButton             *   pNewTransButton;

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
};
#endif // MAINWINDOW_H
