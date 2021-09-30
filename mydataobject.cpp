/*
    This is the second iteration of a data manager.
    Unlike the first one this time I will use the Qt Sql classes.
    This will use the same data structures of the first - mdataobject.h for now.
    Ross Edward Hockenberry 09/15/2021
    Part of the WriterData system.
    Using Qt open source.
*/

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include "mydataobject.h"


MyDataObject::MyDataObject()    :   QObject()
{
        return;
}

MyDataObject::MyDataObject(stConnection stConn) : QObject()
{
        SetDbConnectionData(stConn);
}

MyDataObject::~MyDataObject()
{
        return;
}

bool MyDataObject::OpenDb()
{
    bool bFlag;

        bFlag = OpenDatabase();
//        GetDatabaseTables();
        return bFlag;
}

bool MyDataObject::OpenDb(stConnection stConn)
{
        SetDbConnectionData(stConn);
        return OpenDatabase();
}

//--------------------------------------------------------------------------------------
//  Database functions.
void MyDataObject::SetDbConnectionData(stConnection stConn)
{
        stConnectionData = stConn;      //  Copy connection data.

        oDb = QSqlDatabase::addDatabase(stConn.sDriverName.c_str());
        oDb.setHostName(stConn.sHostAddress.c_str());                    //  Use the ip for now.
        oDb.setDatabaseName(stConn.sDatabaseName.c_str());
        oDb.setPort(atoi(stConn.sPort.c_str()));
        oDb.setUserName(stConn.sUserName.c_str());
        oDb.setPassword(stConn.sPassword.c_str());

        return;
}

void MyDataObject::SetDriverName()
{
        stConnectionData.sDriverName = "QPSQL";
        return;
}

bool MyDataObject::OpenDatabase()
{
        oOpenFlag.Set(oDb.open());
        if(oOpenFlag.Is())
            GetDatabaseTables();

        return oOpenFlag.Is();
}

bool MyDataObject::CloseDatabase()
{
        if(oOpenFlag.Is())
        {
            oDb.close();
            oOpenFlag.SetFalse();
        }
        return oOpenFlag.Is();
}

bool MyDataObject::GetDatabaseTables()
{
    QStringList oTableList;
    struct      stTableDesc stDesc;

        oTableList = oDb.tables();      //  Might want a list of tables in the future.

        iNumberTables = oTableList.length();

//  Put table names in our structure.
        for(int iIndex = 0 ; iIndex < iNumberTables ; iIndex++)
        {
            stDesc.sTableName = oTableList.at(iIndex).toStdString();
            vTables.push_back(stDesc);
        }
        return true;
}

int MyDataObject::ReturnTableCount()
{
        return iNumberTables;
}

bool MyDataObject::IsDatabaseError()
{
        oOpenErrorFlag.Set(oDb.isOpenError());
        if(oOpenErrorFlag.Is() || oQueryErrorFlag.Is())
        {
            oOpenErrorFlag.SetFalse();
            oQueryErrorFlag.SetFalse();
            return true;
        }

        return false;
}

std::string MyDataObject::ReturnDatabaseError()
{
        return oErrorString.toStdString();
}

std::vector<std::string> MyDataObject::ReturnTableNames()
{
        return vTableList;
}

int MyDataObject::ReturnColumnNames(std::string& rArg , QStringList & rList)
{
    int         iVectorIndex = -1;      //  Index into our vTables array.
    int         iIndex;

        iVectorIndex = FindTableIndex(rArg);
        if(!vTables[iVectorIndex].bColumnFlag)
            GetColumnAndRowData(rArg);
        for( iIndex = 0 ; iIndex < vTables[iVectorIndex].iColumnCount ; iIndex++)
        {
            std::string sString = vTables[iVectorIndex].vColumnNames[iIndex];
            rList.push_back(sString.c_str());
        }
        return vTables[iVectorIndex].iColumnCount;
}

bool MyDataObject::GetColumnAndRowData(std::string & rArg)
{
    int         iIndex = 0;
    int         iVectorIndex = 0;
    bool        bFillFlag = false;
    QString     sBuild;
    QSqlRecord  oRecord;

        iVectorIndex = FindTableIndex(rArg);
        if(vTables[iVectorIndex].vColumnNames.empty())
            bFillFlag = true;

        sBuild = QString("SELECT * FROM %1").arg(rArg.c_str());     //  Make a query string.
        QSqlQuery oQ(sBuild);                                       //  Make the query.

        if(oQ.first())                                              //  Execute the query.
        {
            oRecord = oQ.record();                              //  Get record and field count.
            vTables[iVectorIndex].iColumnCount = oRecord.count();
            for( iIndex = 0 ; iIndex < vTables[iVectorIndex].iColumnCount ; iIndex++)
            {
                std::string sString = oRecord.fieldName(iIndex).toStdString();
                 if(bFillFlag)
                    vTables[iVectorIndex].vColumnNames.push_back(sString);
            }
            vTables[iVectorIndex].bColumnFlag = true;
        }
        vTables[iVectorIndex].iRowCount = oQ.size();        //  Set the row count.
        return true;
}

int MyDataObject::ReturnTableColumnCount(std::string& rArg)
{
        int iVectorIndex = FindTableIndex(rArg);
        if(!vTables[iVectorIndex].bColumnFlag)
            GetColumnAndRowData(rArg);
        return vTables[FindTableIndex(rArg)].iColumnCount;
}

int MyDataObject::ReturnTableRowCount(std::string &rArg)
{
        int iVectorIndex = FindTableIndex(rArg);
        if(!vTables[iVectorIndex].bRowFlag)
            GetColumnAndRowData(rArg);
        return vTables[iVectorIndex].iRowCount;
}

//  Return the tables index in the table desc struct.
int MyDataObject::FindTableIndex(std::string & rArg)
{
        for(int iIndex = 0 ; iIndex < iNumberTables ; iIndex++)
            if(vTables[iIndex].sTableName == rArg)
                return iIndex;

        return -1;      //  Table not found.
}

bool MyDataObject::SendDatabaseQuery(const std::string & rS)
{
        QSqlQuery oQ(rS.c_str());

        if(oQ.lastError().isValid())
        {
            oErrorString = oQ.lastError().text();
            oQueryErrorFlag.SetTrue();
            return false;
        }

        return true;
}

bool MyDataObject::SendDatabaseQuery(const char * pString)
{
        QSqlQuery oQ(pString);

        if(oQ.lastError().isValid())
        {
            oErrorString = oQ.lastError().text();
            oQueryErrorFlag.SetTrue();
        }

        return true;
}

int MyDataObject::ChangeTableRowCount(std::string & rT, int iT)
{
    Q_UNUSED(iT);
        return ReturnTableRowCount(rT);
}

//------------------------------------
//  Data Handling Functions.
bool MyDataObject::ReturnQueryData(const std::string & sQuery , stResultData & rResult)
{
    QString     sTemp;

        QSqlQuery   oQ(sQuery.c_str());             //  Pass the query.

        if(oQ.lastError().isValid())
        {
            oErrorString = oQ.lastError().text();
            oQueryErrorFlag.SetTrue();
        }

        rResult.iColumnsReturned = oQ.record().count();
        int         iRowIndex = 0;
        int         iSize = rResult.iRowsReturned = oQ.size();

// Read the requested data.
        (rResult.vColumns).resize(iSize);
        while(oQ.next())
        {
            for(int iIndex = 0 ; iIndex < rResult.iColumnsReturned ; iIndex++)
            {
                sTemp = oQ.value(iIndex).toString();
                rResult.vColumns[iRowIndex].push_back(sTemp.toStdString());
            }
            iRowIndex++;                //  Increment the row.
        }
//  Set the number of rows and column
        return true;
}

bool MyDataObject::ReturnQueryData(const std::string & sQuery , std::vector<stTypeData> &vType)
{
    stTypeData stType;
    QSqlQuery oQ(sQuery.c_str());       //  Create and pass Query statement.

        if(oQ.lastError().isValid())
        {
            oErrorString = oQ.lastError().text();
            oQueryErrorFlag.SetTrue();
            return false;
        }

        while(oQ.next())
        {
            stType.iKey = oQ.value(0).toInt();
            stType.sKeyString = oQ.value(1).toString().toStdString();
            vType.push_back(stType);
        }

        return true;
}

bool MyDataObject::ReturnQueryData(const std::string & rQuery, std::vector<std::pair<int,int> > & vQuery)
{
    stTypeData stType;
    QSqlQuery oQ(rQuery.c_str());       //  Create and pass Query statement.
    std::pair<int , int> vPair;

        if(oQ.lastError().isValid())
            {
            oErrorString = oQ.lastError().text();
            oQueryErrorFlag.SetTrue();
            return false;
        }

        while(oQ.next())
        {
            vPair.first     = oQ.value(0).toInt();
            vPair.second    = oQ.value(1).toInt();
            vQuery.push_back(vPair);
        }

        return true;
}

int MyDataObject::ReturnSumQuery(const std::string & rQuery)
{
        QSqlQuery oQ(rQuery.c_str());

        if(oQ.lastError().isValid())
        {
            oErrorString = oQ.lastError().text();
            oQueryErrorFlag.SetTrue();
        }
        oQ.next();
        if(!IsDatabaseError())
            return oQ.value(0).toInt();

        return -1;
}

//  Used to get the primary key of the last record written.
int MyDataObject::ReturnLastRecordKey(const std::string & sTableName,const std::string & sColumnName)
{
    QString      sBuild;
    stResultData stResult;

        sBuild = QString("SELECT MAX( %1 ) FROM  %2").arg(sColumnName.c_str() , sTableName.c_str());
        QSqlQuery oQ(sBuild);
        if(!IsDatabaseError())
        {
            return oQ.value(sColumnName.c_str()).toInt();
        }
        return -1;      //  Error or not found.
}
