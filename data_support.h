#ifndef DATA_SUPPORT_H
#define DATA_SUPPORT_H

#include "myinclude.h"
#include "myflag.h"

//-----------------------------------------------------------------------------------------------------------
// Database data structures.
//-----------------------------------------------------------------------------------------------------------
    struct stTableSchemaBase
    {
        std::string                             sTableName;          //  Name of the table.
        std::vector<std::string>                vColumnName;         //  Names of the column.
        std::vector<int>                        vColumnDataType;     //  Field type of the column.
        int                                     iPrimaryKey;         //  Primay Key Column Position.
                                                                     //  For now we're assuming 1 column - need to change that.
        std::string                             sPrimaryKeyName;     //  Name of the PRIMARY KEY column
        int                                     iRecords;            //  Number of Records.  This will changee. Update?
        int                                     iColumns;            //  Number of Columns in Tables.
    };

    struct stDatabaseSchema
    {
        int                              iNumberTables;         //  Number of tables in database.
        std::vector<stTableSchemaBase>   vTableSchema;          //  It's possible I'm begining to like vectors.
    };

    struct stDB
    {
        const char  *           pszConnInfo;
        bool                    bOpenFlag   =   false;
        int                     number_records;
        int                     number_columns;
//        PGconn      *           conn;
//        PGresult    *           res;
//        ExecStatusType          pgStatus;
    };

    struct stResultData
    {
        int                                      iRowsReturned;      //  Number of rows and columns returned.
        int                                      iColumnsReturned;   //  Need to find out what these are if error.
        std::vector<std::vector<std::string>>    vColumns;           //  Actual column content.
    };

    typedef std::vector<std::pair<int,std::string>> vResultQuery;

    struct stTypeData
    {
        int             iKey;
        std::string     sKeyString;
        int             iPosKey;            //  Position in table or subset of table.
    };

    struct stConnection
    {
        std::string     sDriverName;
        std::string     sDatabaseName;
        std::string     sHostName;
        std::string     sHostAddress;
        std::string     sUserName;
        std::string     sPassword;
        std::string     sPort;
    };

    struct stTableDesc
    {
        std::string                 sTableName;
        std::vector<std::string>    vColumnNames;           //  Column Names.
        int                         iColumnCount = -1;      //  Total number of columns in table.
        bool                        bColumnFlag = false;    //  True if we have a column count.
        int                         iRowCount = -1;         //  Total number of rows in table.
        bool                        bRowFlag = false;       //  True if we have a row count.
        bool                        bDirtyFlag;             //  Need to update structure data.
    };

//----------------------------------------------------------------------------------------------------------
//  End of Database structures.
//----------------------------------------------------------------------------------------------------------

//  This file mainly contains data structures for the character table.

struct stCharacterData
{
    std::string             sID;
    std::string             sAge;
    std::string             sUsed_Name;         //  Length 50.
    std::string             sLast_Name;         //  Length 50.
    std::string             sMiddle_Name;       //  Length 50.
    std::string             sFirst_Name;        //  Length 50.
    std::string             sFamily_Name;       //
    std::string             sJob;
    std::string             sSkills;
    std::string             sPersonality;
    std::string             sPhysical_Desc;     //  Length 100.
    std::string             sLife_Intent;
    std::string             sSeen_As;
    std::string             sHistory;           //  Used to be Quirks.
    std::string             sOrigin_Date;       //  Index in other table.
    std::string             sBirth_Place;       //  Ditto
    std::string             sChar_Type;
    std::string             sDescription;
    std::string             sSex;               //  These two are indexes into
    std::string             sSex_Pref;          //  other tables.
    std::string             sInitial_Story;     //  Initial Story.
};

struct stScreenData
{
    int                     iNumberScreens;
    int                     iCurrentScreen;
    std::vector<std::pair<int,int> > vMaxRes;   //  Maximum resolution of each screen.
    std::vector<std::pair<int,int> > vAvalRes;  //  Available resolution.
};

struct stTimeData
{
    int                     iKey;               //  Scene Primary key value.
    std::string             sKeyString;         //  Scene name.
    int                     iSequence = -1;     //  Seequence Number for each record in
                                                //  reference to the story.
    int                     iLevel = 0;         //  Level number for the tree control.
    float                   fTime =  0.0;       //  Floating point value for what ever
                                                //  time line units we're using.
    bool                    bDirty =  false;    //  If new or changed. Needs written.
    int                     bNew = false;       //  Is new record
};

struct stSettingsData
{
    std::string             sVersion;           //  Version string.  ??
    std::string             sDBTypeName;        //  Database Connection stuff.
    std::string             sDBName;
    std::string             sHostName;
    std::string             sHostAddress;
    std::string             sUserName;
    std::string             sUserPassword;
};

//  Define some private event codes.


#endif // DATA_SUPPORT_H
