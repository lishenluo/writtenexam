#include"IssueTwo.h"

namespace csc
{
    IssueTwo::IssueTwo()
    {
    }

    void IssueTwo::setList()
    {
        cout << "请输出一列数据以空格隔开，如 -1 0 5 -2 3 5 0 0，以回车键结束:" << endl;
        cout << ":";
        double num;
        while (cin >> num)
        {
            m_list.emplace_back(num);
            if (cin.get() == '\n')
                break;
        }
    }

    void IssueTwo::showIssueTwoResult()
    {
        calcMaxSumSubList();
        cout << "您输入的数列为:" << endl;
        for (size_t index = 0; index < m_list.size(); ++index)
        {
             if (index == m_list.size()-1)
             {
                 cout << m_list[index] << endl;
             }
             else
             {
                 cout << m_list[index] << " ";
             }
        }
        cout <<endl << "连续子数列的和的最大值为:" << m_maxSubSum << endl;
        cout << "相应的子序列有:" << endl;
        int j = 1;
        for (set<pair<size_t, size_t> >::iterator it = m_maxSubListPosition.begin(); it != m_maxSubListPosition.end(); ++it)
        {
            cout <<j++<< ".位置:[" << (*it).first << "," << (*it).second << "],子序列:";
            for (size_t ind = (*it).first; ind <= (*it).second; ++ind)
            {
                cout << m_list[ind];
                if (ind !=(*it).second)
                {
                    cout << " ";
                }
            }
            cout << endl;
        }
    }

    void IssueTwo::setList(const vector<double>&listData)
    {
        m_list = listData;
        calcMaxSumSubList();
    }

    double IssueTwo::getMaxSubSum()
    {
        return m_maxSubSum;
    }

    int IssueTwo::getSizeOfMaxSubListPosition()
    {
        return (int)m_maxSubListPosition.size();
    }

    void IssueTwo::calcMaxSumSubList()
    {
        //算法说明dp[n] = max(0, dp[n-1]) + num[n]
        //并考虑一些特殊情况
        //查找最大子序和值与对应的满足条件的所有子序列，时间复杂度O(2N)
        //最坏情况O(N^2):全部是0的情况

        m_maxSubListPosition.clear();
        if (m_list.empty())
        {
            return;
        }

        //特殊情况判定
        if (m_list.size() == 1)
        {
            m_maxSubListPosition.insert(pair<size_t, size_t>(0, 0));
            m_maxSubSum = m_list[0];
            return;
        }

        vector<double> midList(m_list);
        m_maxSubSum = midList[1];
        //midList: -1 0 1 2 -1 -1 -1 1 1 1 0
        for (size_t index = 1; index < midList.size(); ++index) 
        {
            //寻找最大和
            if (midList[index - 1] > 0)
            {
                midList[index] += midList[index - 1];
            }
            else if(midList[index - 1] > midList[index])
            {
                midList[index] = midList[index - 1];
            }

            if (midList[index] > m_maxSubSum)
            {
                m_maxSubSum = midList[index];
            }
        }

        size_t start = -1;
        size_t end = -1;
        size_t all_start = -1;
        vector<size_t> startVec;
        //after 寻找最大和 now midList: -1 0 1 3 2 1 0 1 2 3 3
        for (size_t index = 0; index < midList.size(); ++index)
        {
            //寻找所有满足条件的子序列
            if (all_start == -1 && (midList[index] >= 0))//序列中第一个非负的位置//处理开始是负数的情况
            {
                all_start = index;
            }
            if (midList[index] == 0)//达到0的位置：本身就是0或前面的值累计得到0
            {
                startVec.emplace_back(index);
            }
            if (midList[index] != m_maxSubSum)
            {
                continue;
            }

            //达到最大值的位置

            //无为0的位置
            if (startVec.empty())
            {
                size_t st = ((all_start == -1) ? index : all_start);
                m_maxSubListPosition.insert(pair<size_t, size_t>(st, index));
                continue;
            }

            //有为0的位置
            bool hasOriZero = false;//原始数据位置就是0
            for (size_t ind = 0; ind < startVec.size(); ++ind)
            {
                if (m_list[startVec[ind]] == 0)//原始数列的该位置上就是0
                {
                    m_maxSubListPosition.insert(pair<size_t, size_t>(startVec[ind], index));
                    hasOriZero = true;
                }
                else if(!hasOriZero && all_start !=-1 )
                {
                    m_maxSubListPosition.insert(pair<size_t, size_t>(all_start, index));
                }
                if (startVec[ind] + 1 <= index)
                {
                    m_maxSubListPosition.insert(pair<size_t, size_t>(startVec[ind] + 1, index));
                }
            }
        }
    }

}

