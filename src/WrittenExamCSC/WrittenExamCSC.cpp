// WrittenExamCSC.cpp : 定义控制台应用程序的入口点。

#include "IssueOne.h"
#include "IssueTwo.h"
#include "IssueThree.h"
#include "IssueFour.h"
using namespace csc;
int main()
{
    cout << "问题一：计算浦发银行和上证50的最大值，最小值，平均值，中位数，标准差和相关系数" << endl<<endl;
    cout << "问题二：有一数列，无序，其中元素可正可负，求该数列连续子数列的和的最大值和相应的序列" << endl<<endl;
    cout << "问题三：编写一个程序，该程序运行时，CPU的使用率为余弦图像" << endl<<endl;
    cout << "问题四：在数据库中有一张行情表，该表有四列：股票名称，日期，交易量，收盘价." << endl << "我们要求计算某只股票5日移动平均价，和5日交易量加权移动平均价" << endl<<endl;
    cout << "请输出数字1或2或3或4，以回车键结束，开始问题,输入5结束" << endl;
    cout << ":";
    int num;
    while (cin >> num)
    {
        bool end = false;
        switch (num)
        {
            case 1:
            {
                IssueOne issueOne = IssueOne("../datadir/data.csv");
                issueOne.showIssueOneResult();
                break;
            }
            case 2:
            {
                IssueTwo issueTwo;
                issueTwo.setList();
                issueTwo.showIssueTwoResult();
                break;
            }
            case 3:
            {
                cout << "问题3将进入死循环,建议关闭电脑各个软件，打开任务管理器观察cpu使用情况" << endl;
                cout << "进入死循环，需手动停止运行，是否继续(y/n)" << endl;
                cout << "输入y 或 n 后回车:";
                char doit;
                while (1)
                {
                    cin >> doit;
                    if ('y' == doit) break;
                    else if ('n' == doit) break;
                    else
                        cout << "输入错误" << endl << "输入y 或 n 后回车 :";
                }
                if (doit == 'y')
                {
                    IssueThree issueThree;
                    issueThree.showCosGraphOfCPU();
                }
                break;
            }
            case 4:
            {
                IssueFour issueFour("../datadir/MarketDataCSC.db");
                issueFour.openSqliteDatabase();
                const char* sql = "CREATE TABLE MarketData("\
                "StockCode  CAHR(9)      NOT NULL, "\
                "DateTime   TEXT(20)    NOT NULL, "\
                "close      REAL(11, 4)     NOT NULL, "\
                "volume     INTEGER(11)     NOT NULL );";
                issueFour.deleteTable("MarketData");
                issueFour.creatTable(sql);
                issueFour.readMarketDataFromCSV("../datadir/SZ000002.csv", "000002.SZ");
                issueFour.insertDataToDB("MarketData","StockCode, DateTime, close, volume");
                issueFour.selectOptim("MarketData","DateTime");
                issueFour.closeSqliteDatabase();
                issueFour.showMovingAvg(5, NORMAL);
                cout << endl;
                issueFour.showMovingAvg(5, VOL_WEIGHT);
                break;
            }
            case 5:
            {
                end = true;
                break;
            }
            default:
                cout << "输入不对" << endl;
        }
        if (end)
        {
            cout << "结束！" << endl;
            break;
        }
        cout << "*****************************************************************************************" << endl;
        cout << endl<<"请输出数字1或2或3或4，以回车键结束，继续问题,输入5结束" << endl;
        cout << ":";
    }
    return 0;
}
