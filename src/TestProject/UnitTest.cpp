#include "stdafx.h"
#include "IssueOne.h"
#include "IssueTwo.h"
#include "IssueThree.h"
#include "IssueFour.h"
#include <sqlite\sqlite3.h>

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;
using namespace csc;
namespace TestProject
{
    [TestClass]
    public ref class UnitTest
    {
    private:
        TestContext^ testContextInstance;

    public: 
        /// <summary>
        ///获取或设置测试上下文，该上下文提供
        ///有关当前测试运行及其功能的信息。
        ///</summary>
        property Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ TestContext
        {
            Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ get()
            {
                return testContextInstance;
            }
            System::Void set(Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ value)
            {
                testContextInstance = value;
            }
        };

        #pragma region Additional test attributes
        //
        // 编写测试时，可以使用以下附加特性: 
        //
        // 在运行类中的第一个测试之前使用 ClassInitialize 运行代码
        //[ClassInitialize()]
        //static void MyClassInitialize(TestContext^ testContext) {};
        //
        // 在类中的所有测试都已运行之后使用 ClassCleanup 运行代码
        //[ClassCleanup()]
        //static void MyClassCleanup() {};
        //
        //在运行每个测试之前，使用 TestInitialize 来运行代码
        //[TestInitialize()]
        //void MyTestInitialize() {};
        //
        //在每个测试运行完之后，使用 TestCleanup 来运行代码
        //[TestCleanup()]
        //void MyTestCleanup() {};
        //
        #pragma endregion 
        [TestMethod]
        void TestMethod1()
        {
            //测试IssueOne
            IssueOne issueOne = IssueOne("../datadir/data.csv");
            issueOne.showIssueOneResult();
            Assert::AreEqual(issueOne.isEmpeyOfNeedCalcData(), false);
            issueOne = IssueOne("");
            vector<double> indexData = { 1,1,1,1,1 };
            vector<double> stockData = { -1,-1,-1,-1,-1 };
            issueOne.setNeedCalcData("index", "stock", indexData, stockData);
            Assert::IsFalse(issueOne.isEmpeyOfNeedCalcData());
            Assert::AreEqual(0.0, issueOne.getMathValue(INDEX, STD_DATA));
            Assert::AreEqual(0.0, issueOne.getMathValue(STOCK, STD_DATA));
            Assert::AreEqual(0.0, issueOne.getMathValue(STOCK, CORR_DATA));
            indexData = { 1,2,3,4,5 };
            stockData = { -1,-2,-3,-4,-5 };
            issueOne.setNeedCalcData("index", "stock", indexData, stockData);
            Assert::IsFalse(issueOne.isEmpeyOfNeedCalcData());
            Assert::AreEqual(5.0, issueOne.getMathValue(INDEX, MAX_DATA));
            Assert::AreEqual(1.0, issueOne.getMathValue(INDEX, MIN_DATA));
            Assert::AreEqual(3.0, issueOne.getMathValue(INDEX, MEDIAN_DATA));
            Assert::AreEqual(3.0, issueOne.getMathValue(INDEX, AVG_DATA));
            Assert::AreEqual(-3.0, issueOne.getMathValue(STOCK, MEDIAN_DATA));
            bool corrDiff = abs(issueOne.getMathValue(STOCK, CORR_DATA) +1.0) < FLOAT_ERROR;//-1
            Assert::IsTrue(corrDiff);
            indexData = { 10,8,6,4,2 };
            stockData = { 5,4,3,2,1 };
            issueOne.setNeedCalcData("index", "stock", indexData, stockData);
            Assert::IsFalse(issueOne.isEmpeyOfNeedCalcData());
            corrDiff = abs(issueOne.getMathValue(STOCK, CORR_DATA) - 1.0) < FLOAT_ERROR;//1
            Assert::IsTrue(corrDiff);
        };

        [TestMethod]
        void TestMethod2()
        {
            //测试IssueTwo
            IssueTwo issueTwo;
            vector<double> testlist = { -1,-1,-1,-1,-1 };
            issueTwo.setList(testlist);
            Assert::AreEqual(-1.0, issueTwo.getMaxSubSum());
            Assert::AreEqual(5, issueTwo.getSizeOfMaxSubListPosition());
            testlist = { -1,-1,2,-1,-1,1,1,0};
            issueTwo.setList(testlist);
            Assert::AreEqual(2.0, issueTwo.getMaxSubSum());
            Assert::AreEqual(5, issueTwo.getSizeOfMaxSubListPosition());//2;2 -1 -1 1 1;2 -1 -1 1 1 0;1 1;1 1 0
            testlist = {0,0,0,0};
            issueTwo.setList(testlist);
            Assert::AreEqual(0.0, issueTwo.getMaxSubSum());
            Assert::AreEqual(10, issueTwo.getSizeOfMaxSubListPosition());
            testlist = { 0,1.5,0};
            issueTwo.setList(testlist);
            Assert::AreEqual(1.5, issueTwo.getMaxSubSum());
            Assert::AreEqual(4, issueTwo.getSizeOfMaxSubListPosition());//0 1.5;1.5;1.5 0;0 1.5 0
        };

        [TestMethod]
        void TestMethod3()
        {
            //测试IssueThree
        };

        [TestMethod]
        void TestMethod4()
        {
            //测试IssueFour
            IssueFour issueFour("../datadir/MarketDataCSC.db");
            issueFour.openSqliteDatabase();
            const char* sql = "CREATE TABLE MarketData("\
                "StockCode  CAHR(9)      NOT NULL, "\
                "DateTime   TEXT(20)    NOT NULL, "\
                "close      REAL(11, 4)     NOT NULL, "\
                "volume     INTEGER(11)     NOT NULL );";
            issueFour.deleteTable("MarketData");
            Assert::IsTrue(issueFour.creatTable(sql));
            issueFour.readMarketDataFromCSV("../datadir/SZ000002.csv", "000002.SZ");
            Assert::IsTrue(issueFour.insertDataToDB("MarketData", "StockCode,DateTime,close,volume"));
            issueFour.selectOptim("MarketData","DateTime");
            //issueFour.showSelectData();
            Assert::IsTrue(issueFour.showMovingAvg(5, NORMAL));
            Assert::IsTrue(issueFour.showMovingAvg(5, VOL_WEIGHT));
            //容错测试
            Assert::IsFalse(issueFour.showMovingAvg(-1, NORMAL));//周期不对
            Assert::IsFalse(issueFour.showMovingAvg(-5, VOL_WEIGHT));//周期不对
            Assert::IsFalse(issueFour.creatTable(sql));//创建数据库表失败//重复创建
            sql = "CREATE TABLE MarketDataAnother("\
                "StockCode  CHAR(9)      NOT NULL, "\
                "DateTime   TEXT(20)    NOT NULL, "\
                "close      REAL(11, 4)     NOT NULL, "\
                "volume     INTEGER(11)     NOT NULL );";
            issueFour.deleteTable("MarketDataAnother");//有没有表都尝试删除
            Assert::IsTrue(issueFour.creatTable(sql));//创建数据库表成功//另一张表
            sql = "CREAT TABLE MarketDataOther("\
                "StockCode  CHAR(9)      NOT NULL, "\
                "DateTime   TEXT(20)    NOT NULL, "\
                "close      REAL(11, 4)     NOT NULL, "\
                "volume     INTEGER(11)     NOT NULL );";
            issueFour.deleteTable("MarketDataOther");//有没有表都尝试删除
            Assert::IsFalse(issueFour.creatTable(sql));//创建数据库表成功//sql语句错误
            issueFour.closeSqliteDatabase();
        };
    };
}
