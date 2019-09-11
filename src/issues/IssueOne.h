/**
*  问题一：计算浦发银行和上证50的最大值，最小值，平均值，中位数，标准差和相关系数。(数据可参见data.csv)
*
*  @author lishenluo
*  @since  2019-01-12
*/

#pragma once

#include "stdafx.h"

namespace csc
{
    
    enum CALC_TYPE
    {
        MAX_DATA = 0, //最大值
        MIN_DATA,     //最小值
        AVG_DATA,     //平均值
        MEDIAN_DATA,  //中位数
        STD_DATA,     //标准差
        CORR_DATA = 5,//相关系数
    };

    enum DATA_TYPE
    {
        INDEX,        //指数
        STOCK,        //股票
    };

    typedef struct _mt
    {
        vector<string> date;       //日期
        vector<double> indexClose; //指数数据
        vector<double> stockClose; //股票数据

    }MarketData;

    class IssueOne
    {
    public:
        IssueOne(const string& dataFile); //传入数据文件构造对象
        void showIssueOneResult();        //显示计算结果

        //测试接口
        bool  isEmpeyOfNeedCalcData();
        void setNeedCalcData(const string& index, const string& stock, const vector<double> indexData, const vector<double> stockData);
        double getMathValue(DATA_TYPE dataType, CALC_TYPE calcType);
    private:
        void readMarketDataFromCSV(); //读取data.csv中数据
        double calcMathValue(DATA_TYPE dataType, CALC_TYPE calcType);//计算各个统计值
        double calcCorrelation();     //计算相关系数
    private:
        MarketData m_needCalcData; //data.csv中数据
        string m_dataFile;         //data.csv文件路径及文件
        string m_index;            //指数代码
        string m_stock;            //股票代码
    };
}
