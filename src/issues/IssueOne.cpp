#include"IssueOne.h"
namespace csc
{
    IssueOne::IssueOne(const string& dataFile)
        :m_dataFile(dataFile)
        , m_index("")
        , m_stock("")
    {
    }

    void IssueOne::showIssueOneResult()
    {
        readMarketDataFromCSV();
        cout << m_stock << " 最大值:" << calcMathValue(STOCK, MAX_DATA)
            << ",最小值:" << calcMathValue(STOCK, MIN_DATA)
            << ",平均值:" << calcMathValue(STOCK, AVG_DATA)
            << ",中位数:" << calcMathValue(STOCK, MEDIAN_DATA)
            << ",标准差:" << calcMathValue(STOCK, STD_DATA)
            <<endl;
        cout << m_index << " 最大值:" << calcMathValue(INDEX, MAX_DATA)
            << ",最小值:" << calcMathValue(INDEX, MIN_DATA)
            << ",平均值:" << calcMathValue(INDEX, AVG_DATA)
            << ",中位数:" << calcMathValue(INDEX, MEDIAN_DATA)
            << ",标准差:" << calcMathValue(INDEX, STD_DATA)
            << endl;
        cout << m_stock << " 和 " << m_index << "的相关系数是:" << calcCorrelation() << endl;
    }

    bool IssueOne::isEmpeyOfNeedCalcData()
    {
        return m_needCalcData.indexClose.empty() || m_needCalcData.stockClose.empty();
    }

    void IssueOne::setNeedCalcData(const string& index, const string& stock, const vector<double> indexData, const vector<double> stockData)
    {
        m_needCalcData.indexClose = indexData;
        m_needCalcData.stockClose = stockData;
        m_index = index;
        m_stock = stock;
    }

    double IssueOne::getMathValue(DATA_TYPE dataType, CALC_TYPE calcType)
    {
        if (calcType == CORR_DATA)
        {
            return calcCorrelation();
        }
        else
        {
            return calcMathValue(dataType, calcType);
        }
    }

    void IssueOne::readMarketDataFromCSV()
    {
        ifstream fs(m_dataFile);
        string line;
        vector<string> fields;
        string field;
        getline(fs, line);
        istringstream sin(line);
        while (getline(sin, field, ','))
        {
            fields.push_back(field);
        }
        if (fields.size() == 3)
        {
            m_index = fields[1];
            m_stock = fields[2];
        }
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
                m_needCalcData.date.emplace_back(fields[0]);
                istringstream sindex(fields[1]);
                double midData;
                sindex >> midData;
                m_needCalcData.indexClose.emplace_back(midData);
                istringstream sstock(fields[2]);
                sstock >> midData;
                m_needCalcData.stockClose.emplace_back(midData);
            }
        }
    }

    double IssueOne::calcMathValue(DATA_TYPE dataType, CALC_TYPE calcType)
    {
        const vector<double>&  dataVec = dataType == INDEX ? m_needCalcData.indexClose : m_needCalcData.stockClose;
        if (dataVec.empty())
        {
            return -1;
        }

        switch (calcType)
        {
        case csc::MAX_DATA:
            return *max_element(dataVec.begin(), dataVec.end());
        case csc::MIN_DATA:
            return *min_element(dataVec.begin(), dataVec.end());
        case csc::AVG_DATA:
        {
            double sum = accumulate(std::begin(dataVec), std::end(dataVec), 0.0);
            return sum / dataVec.size(); 
        }
        case csc::MEDIAN_DATA:
        {
            int size = (int)dataVec.size();
            int halfSize = int(size / 2);
            vector<double> dataHalfSort = dataVec;
            nth_element(dataHalfSort.begin(), dataHalfSort.begin()+ halfSize, dataHalfSort.end());
            if (size % 2 == 0)
            {
                return (dataHalfSort[halfSize - 1] + dataHalfSort[halfSize]) / 2.0;
            }
            else
            {
                return dataHalfSort[halfSize];
            }
        }
        case csc::STD_DATA:
        {
            double sum = accumulate(std::begin(dataVec), std::end(dataVec), 0.0);
            double mean = sum / dataVec.size(); 
            double accum = 0.0;
            std::for_each(std::begin(dataVec), std::end(dataVec), [&](const double d)
            {
                accum += (d - mean)*(d - mean);
            });
            return  sqrt(accum / (dataVec.size() - 1));
        }
        default:
            break;
        }
        return -1;
    }

    double IssueOne::calcCorrelation()
    {
        const vector<double>&  stockVec = m_needCalcData.stockClose;
        const vector<double>&  indexVec = m_needCalcData.indexClose;
        if (stockVec.empty() || indexVec.empty() || stockVec.size() != indexVec.size())
        {
            return -1;
        }
        double meanStock = calcMathValue(STOCK, AVG_DATA);
        double meanIndex = calcMathValue(INDEX, AVG_DATA);
        double cov = 0.0;//协方差
        for (size_t i = 0; i < stockVec.size(); ++i)
        {
            cov += (stockVec[i] - meanStock)*(indexVec[i] - meanIndex);
        }
        cov = cov / double(stockVec.size()-1);
        double stdValue = calcMathValue(STOCK, STD_DATA)*calcMathValue(INDEX, STD_DATA);
        if (abs(stdValue) < FLOAT_ERROR)
        {
            return 0;
        }
        else
        {
            return cov / stdValue;
        }
    }

}
