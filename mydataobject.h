#ifndef MYDATAOBJECT_H
#define MYDATAOBJECT_H

#include "data_support.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include <cassert>

class MyDataObject : public QObject
{
    Q_OBJECT

public:
//  Constructors and Destructor.
    MyDataObject();
    MyDataObject(stConnection );
    virtual ~MyDataObject();
    bool    OpenDb();
    bool    OpenDb(stConnection );
//  Public Interface methods.
    void    SetDbConnectionData(stConnection);
    bool    OpenDatabase();
    bool    CloseDatabase();
    bool    GetDatabaseTables();
    int     ReturnTableCount();
    bool    IsDatabaseError();
    void    SetDriverName();
    int     ReturnTableColumnCount(std::string& rArg);
    int     ReturnTableRowCount(std::string& rArg);
    std::vector<std::string> ReturnTableNames();
    int     ReturnColumnNames(std::string & , QStringList &);
    bool    GetColumnAndRowData(std::string & rArg);
    int     FindTableIndex(std::string &);
    int     GetRowCount() { return iLastRowsReturnedCount; }
    std::string ReturnDatabaseError();
    inline bool IsOpen() { return oOpenFlag.Is(); }
    bool    GetTableData(std::string &);
    bool    SendDatabaseQuery(const std::string & rS);
    bool    SendDatabaseQuery(const char * pString);
    int     ChangeTableRowCount(std::string & rT, int iT);      //  Second argument is a carry over.

//  Pass throughs from old dataobject.

//  Data handling functions.
    bool    ReturnQueryData(const std::string & , stResultData & );
    bool    ReturnQueryData(const std::string & , std::vector<stTypeData>& );
    bool    ReturnQueryData(const std::string & , std::vector<std::pair<int,int> > & );
    int     ReturnSumQuery(const std::string & );
    int     ReturnLastRecordKey(const std::string & sTableName,const std::string & sColumnName);

//  Private Data.
    QSqlDatabase                oDb;
    stConnection                stConnectionData;
    std::vector<std::string>    vTableList;
    int                         iNumberTables;
    int                         iLastRowsReturnedCount;
    MyFlag                      oOpenFlag;
    MyFlag                      oOpenErrorFlag;
    MyFlag                      oQueryErrorFlag;
    QString                     oErrorString;       //  Hold last error message.
    std::vector<stTableDesc>    vTables;            //  Table Descritions.

//  Private Methods.

//  Slots.
public slots:

//  Signals.
signals:

};

#endif // MYDATAOBJECT_H
