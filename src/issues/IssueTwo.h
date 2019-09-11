/**
*  问题二：有一数列，无序，其中元素可正可负，求该数列连续子数列的和的最大值和相应的序列
*
*  @author lishenluo
*  @since  2019-1-12
*/

#pragma once

#include "stdafx.h"

namespace csc
{
    class IssueTwo
    {
    public:
        IssueTwo();
        void setList();             //设置数列
        void showIssueTwoResult();  //展示结果

        //测试接口
        void setList(const vector<double>&listData);
        double getMaxSubSum();
        int getSizeOfMaxSubListPosition();

    private:
        void calcMaxSumSubList();   //计算子数列和最大和
    private:
        vector<double>                  m_list;                  //原始数列
        set<pair<size_t, size_t> >      m_maxSubListPosition;    //满足条件的子数列的开始和结束下标
        double                          m_maxSubSum;             //连续子数列最大和
    };
}
