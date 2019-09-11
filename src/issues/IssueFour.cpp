#include"IssueFour.h"

namespace csc
{
    IssueFour::IssueFour(const string& sqlDatabaseFile)
        :m_sqlDatabaseFile(sqlDatabaseFile)
    {
    }

    bool IssueFour::openSqliteDatabase()
    {
        int rc = sqlite3_open(m_sqlDatabaseFile.c_str(), &m_marketDataDB);//打开数据库文件,没有创建
        if (rc) 
        {
            cout << "不能打开数据库:" << sqlite3_errmsg(m_marketDataDB) << endl;
            return false;
        }
        else 
        {
            cout<<"打开数据库成功"<<endl;
        }
        return true;
    }

    bool IssueFour::closeSqliteDatabase()
    {
        if(m_marketDataDB)
        {
            sqlite3_close(m_marketDataDB);
            cout << "关闭数据库成功" << endl;
            return true;
        }
        return false;
    }

    //创建一个满足条件的行情数据库表
    bool IssueFour::creatTable(const char* sql)
    {
        bool ret = true;
        string errMsg;
        exec(sql, errMsg);
        if (!errMsg.empty())
        {
            cout << "创建数据库表出错:" << errMsg << endl;
            ret = false;
        }
        else
        {
            cout << "创建数据库表成功"<< endl;
        }
        return ret;
    }

    bool IssueFour::deleteTable(const string& tableName)
    {
        string sql = "DROP TABLE " + tableName + "; ";
        bool ret = true;
        string errMsg;
        exec(sql.c_str(), errMsg);
        if (!errMsg.empty())
        {
            cout << "删除数据库表出错:" << errMsg << endl;
            ret = false;
        }
        else
        {
            cout << "删除数据库表成功" << endl;
        }
        return ret;
    }

    bool IssueFour::insertDataToDB(const string& tableName, const string& columns)
    {
        if (!m_stockData.close.empty()
            && m_stockData.close.size() == m_stockData.volume.size()
            && m_stockData.close.size() == m_stockData.date.size())
        {
            stringstream ss;
            int succNum = 0;
            for (size_t index = 0; index < m_stockData.date.size(); ++index)
            {
                ss.str("");
                ss << "'"<<m_stockData.stockCode << "','" << m_stockData.date[index]<< "',"
                    << m_stockData.close[index] << "," << m_stockData.volume[index];
                if (!insertValue(tableName, columns, ss.str()))
                {
                    break;
                }
                ++succNum;
            }
            cout << "成功把" << succNum <<"/"<< m_stockData.date.size() << "条数据插入数据库中" << endl;
            return (succNum != 0);
        }
        return false;
    }

    void IssueFour::readMarketDataFromCSV(const string&dataFile, const string&stockCode)
    {
        m_stockData.stockCode = stockCode;
        ifstream fs(dataFile);
        string line;
        vector<string> fields;
        string field;
        getline(fs, line);//第一条标题数据不要
        while (getline(fs, line))
        {
            istringstream sin(line);
            fields.clear();
            while (getline(sin, field, ','))
            {
                fields.push_back(field);
            }
            if (fields.size() == 3)
            {
                m_stockData.date.emplace_back(fields[0]);
                istringstream sindex(fields[1]);
                double midData;
                sindex >> midData;
                m_stockData.close.emplace_back(midData);
                istringstream sstock(fields[2]);
                sstock >> midData;
                m_stockData.volume.emplace_back(midData);
            }
        }
    }

    bool IssueFour::insertValue(const string& tableName, const string&columns, const string&values)
    {
        string sql = "INSERT INTO " + tableName + "(" + columns + ") "  \
            "VALUES (" + values + "); ";
        bool ret = true;
        string errMsg;
        exec(sql.c_str(), errMsg);
        if (!errMsg.empty())
        {
            cout << "插入数据出错:" << errMsg << endl;
            ret = false;
        }
        else
        {
            //cout << "成功插入一条数据" << endl;
        }
        return ret;
    }

    int IssueFour::exec(const char* sql, string& errMsg, callback callb, void* data )
    {
        char* cErrMsg = NULL;
        int ret = sqlite3_exec(m_marketDataDB, sql, callb, data, &cErrMsg);
        if (NULL != cErrMsg)
        {
            errMsg = cErrMsg;
        }
        return ret;
    }

    void IssueFour::selectOptim(const string& tableName, const string& orderColumn)
    {
        string sql;
        if (!orderColumn.empty())
        {
            sql = "SELECT * from " + tableName + " order by " + orderColumn + ";";
        }
        else
        {
            sql = "SELECT * from " + tableName + ";";
        }
        sqlite3_stmt * stmt = NULL;
        const char *zTail;
        if (sqlite3_prepare_v2(m_marketDataDB, sql.c_str(), -1, &stmt, &zTail) == SQLITE_OK)
        {
            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                m_stockDataSelect.stockCode = std::string((char*)sqlite3_column_text(stmt, 0));
                m_stockDataSelect.date.emplace_back(std::string((char*)sqlite3_column_text(stmt, 1)));
                m_stockDataSelect.close.emplace_back(sqlite3_column_double(stmt, 2));
                m_stockDataSelect.volume.emplace_back(sqlite3_column_int64(stmt, 3));
            }
        }
        sqlite3_finalize(stmt);
    }

    void IssueFour::select(const string& tableName, const string& orderColumn)
    {
        string sql;
        if (!orderColumn.empty())
        {
            sql = "SELECT * from " + tableName + " order by " + orderColumn + ";";
        }
        else
        {
            sql = "SELECT * from " + tableName + ";";
        }
        bool ret = true;
        string errMsg;
        exec(sql.c_str(), errMsg, IssueFour::callBack, (void*)this);
        if (!errMsg.empty())
        {
            cout << "查询失败:" << errMsg << endl;
        }
    }

    int IssueFour::callBack(void *data, int argc, char **argv, char **azColName) 
    {
        //sqlite 每查到一条记录，就调用一次这个回调,效率不高
        int i;
        if (data != NULL)
        {
            IssueFour* pIssueFour = (IssueFour*)data;
            if (pIssueFour == NULL) return -1;
            for (i = 0; i < argc; i++)
            {
                printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
                if (azColName[i] == "StockCode")
                {
                    pIssueFour->m_stockDataSelect.stockCode = argv[i] ? argv[i] : "";
                }
                else if (azColName[i] == "DateTime")
                {
                    pIssueFour->m_stockDataSelect.date.emplace_back(argv[i] ? argv[i] : "");
                }
                else if (azColName[i] == "close")
                {
                    istringstream sindex(argv[i] ? argv[i] : "0");
                    double midData;
                    sindex >> midData;
                    pIssueFour->m_stockDataSelect.close.emplace_back(midData);
                }
                else if (azColName[i] == "volume")
                {
                    istringstream sindex(argv[i] ? argv[i] : "0");
                    r_int64 midData;
                    sindex >> midData;
                    pIssueFour->m_stockDataSelect.volume.emplace_back(midData);
                }
            }
        }
        return 0;
    }

    void IssueFour::showSelectData()
    {
        if (!m_stockDataSelect.isSameSize())
        {
            cout << "查询到的数据列数据长度不一致" << endl;
        }
        cout << "查询结果为:" << endl;
        for (size_t index = 0; index < m_stockDataSelect.close.size(); ++index)
        {
            cout<< m_stockDataSelect.stockCode<<","<< m_stockDataSelect.date[index]<<","<< m_stockDataSelect.close[index]<<","<< m_stockDataSelect.volume[index]<<endl;
        }
    }

    bool IssueFour::showMovingAvg(int period, MOVING_AVG_TYPE type)
    {
        if (!m_stockDataSelect.isSameSize())
        {
            cout << "查询到的数据列数据长度不一致" << endl;
            return false;
        }
        else if (m_stockDataSelect.date.empty())
        {
            cout << "查询到的数据为空" << endl;
            return false;
        }
        if (period <= 0)
        {
            cout << "输入的周期不对" << endl;
            return false;
        }

        switch (type)
        {
        case csc::NORMAL:
        {
            cout << "股票:" << m_stockDataSelect.stockCode << "的移动平均值:" << endl;
            cout << "  日  期  " << "  原  值  " << "移动平均" << endl;
            vector<double> closeVec;
            double sum = 0;
            for (size_t index = 0; index < m_stockDataSelect.close.size(); ++index)
            {
                if (closeVec.size() >= period)
                {
                    closeVec.erase(closeVec.begin());//删去老值
                }
                closeVec.emplace_back(m_stockDataSelect.close[index]);
                if ((int)index + 1 < period)
                {
                    cout << m_stockDataSelect.date[index]<<"   "<< setw(5) << m_stockDataSelect.close[index] << "   nan" << endl;
                }
                else
                {
                    sum = accumulate(std::begin(closeVec), std::end(closeVec), 0.0);
                    cout << m_stockDataSelect.date[index] <<"   " << setw(5) << m_stockDataSelect.close[index] <<"   "<< sum/period << endl;
                }
            }
            break;
        }
        case csc::VOL_WEIGHT:
        {
            cout << "股票:" << m_stockDataSelect.stockCode << "的成交量加权移动平均值:" << endl;
            cout << "  日  期  " << "  原  值  " << "加权移动平均" << endl;
            vector<double> closeVec;
            vector<double> volVec;
            double sumCloseVol = 0;
            double sumVol = 0;
            for (size_t index = 0; index < m_stockDataSelect.close.size(); ++index)
            {
                if (closeVec.size() >= period)
                {
                    closeVec.erase(closeVec.begin());//删去老值
                    volVec.erase(volVec.begin());
                }
                closeVec.emplace_back(m_stockDataSelect.close[index]* m_stockDataSelect.volume[index]);
                volVec.emplace_back(m_stockDataSelect.volume[index]);
                if ((int)index + 1 < period)
                {
                    cout << m_stockDataSelect.date[index] << "   " << setw(5) << m_stockDataSelect.close[index] << "   nan" << endl;
                }
                else
                {
                    sumCloseVol = accumulate(std::begin(closeVec), std::end(closeVec), 0.0);
                    sumVol = accumulate(std::begin(volVec), std::end(volVec), 0.0);
                    
                    cout << m_stockDataSelect.date[index] << "   " << setw(5) << m_stockDataSelect.close[index] << "   " << (abs(sumVol)>FLOAT_ERROR ? sumCloseVol / sumVol:0) << endl;
                }
            }
            break;
        }
        default:
            cout << "错误的移动平均类型" << endl;
            return false;
        }
        return true;
    }

}
