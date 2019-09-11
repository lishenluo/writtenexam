/**
*  问题三：编写一个程序，该程序运行时，CPU的使用率为余弦图像
*
*  @author lishenluo
*  @since  2019-01-12
*/

#pragma once

#include "stdafx.h"
#include <Windows.h>
#include<tchar.h>

namespace csc
{
    static int PERIOD = 25 * 1000; //周期ms
    const int COUNT = 300;  //一个周期计算次数
    const double PI = 3.1415926535898; //PI
    const double GAP = (double)PERIOD / COUNT; //时间间隔
    const double FACTOR = 2 * PI / PERIOD;  //函数的系数
    class IssueThree
    {
    public:
        IssueThree();
        void showCosGraphOfCPU();
    private:
        static DWORD WINAPI  startFunc(LPVOID);
        void                 calcCosValue();
        double               m_cosValue[COUNT];
    };
}
