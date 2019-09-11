/**
*  问题四：在数据库中有一张行情表，该表有四列：股票名称，日期，交易量，收盘价。我们要求计算某只股票5日移动平均价，和5日交易量加权移动平均价。
*  (移动平均概念请参见https://zh.wikipedia.org/wiki/%E7%A7%BB%E5%8B%95%E5%B9%B3%E5%9D%87，请附上建表语句和查询语句的SQL)
*  
*  思路：1.用sqlite建立本地数据库和数据库表
*        2.把000002.SZ.csv中数据读入内存并把数据插入数据库表中
*        3.查表，并计算5日移动平均价
*        4.直接通过sql计算移动平均过慢，先查后计算
*  
*  @author lishenluo
*  @since  2019-01-12
*/

#pragma once

#include "stdafx.h"
#include <sqlite\sqlite3.h>

namespace csc
{
    typedef struct _st
    {
        string stockCode;
        vector<string> date;       //日期
        vector<double> close;      //收盘价
        vector<r_int64> volume;    //收盘价
        bool isSameSize() {
            return (date.size() == close.size() && close.size() == volume.size());
        };
    }StockData;

    enum MOVING_AVG_TYPE
    {
        NORMAL,
        VOL_WEIGHT,
    };

    typedef int(*callback)(void*, int, char**, char**);

    class IssueFour
    {
    public:
        IssueFour(const string& sqlDatabaseFile);
        bool openSqliteDatabase();
        bool closeSqliteDatabase();
        bool creatTable(const char* sql);
        bool deleteTable(const string& tableName);
        bool insertDataToDB(const string& tableName, const string& columns);
        void readMarketDataFromCSV(const string&dataFile,const string&stockCode); //读取csv中数据
        void select(const string& tableName, const string& orderColumn="");
        void selectOptim(const string& tableName, const string& orderColumn="");
        bool showMovingAvg(int period, MOVING_AVG_TYPE type);
        void showSelectData();
    private:
        int exec(const char* sql, string& errMsg, callback callb = NULL, void* data=NULL);
        bool insertValue(const string& tableName, const string&columns, const string&values);
        static int callBack(void *data, int argc, char **argv, char **azColName);
    private:
        string              m_sqlDatabaseFile;
        sqlite3*            m_marketDataDB;
        StockData           m_stockData;
        StockData           m_stockDataSelect;
    };
}
