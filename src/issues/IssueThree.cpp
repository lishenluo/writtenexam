#include"IssueThree.h"

namespace csc
{
    IssueThree::IssueThree()
    {}

    void IssueThree::showCosGraphOfCPU()
    {
        calcCosValue();
        SYSTEM_INFO info;
        GetSystemInfo(&info);//得到cpu数目
        int num = info.dwNumberOfProcessors;
        cout << "本计算cpu核数" << num << endl;

        HANDLE *handle;
        handle = new HANDLE[num];
        DWORD *threadId;
        threadId = new DWORD[num];
        for (int i = 0; i< num; ++i)
        {
            handle[i] = CreateThread(NULL, 0, startFunc, (LPVOID)this, 0, &threadId[i]);//创建新线程
            SetThreadAffinityMask(handle[i], i + 1);//限定线程运行在哪个cpu上
            cout << "线程id:" << threadId[i] << endl;
        }
        cout << "打开资源管理器，关闭别的软件和浏览器等，观看cpu使用率曲线" << endl;
        cout << "观察结束直接关闭exe程序窗口" << endl;
        WaitForSingleObject(handle[0], INFINITE);//等待线程结束
        delete[] handle;
        delete[] threadId;
    }

    void IssueThree::calcCosValue()
    {
        double x = 0.0;
        for (int i = 0; i < COUNT; ++i, x += GAP)
        {
            m_cosValue[i] = (1 + cos(FACTOR * x)) / 2; //y=(1/2)*(1+cos(a*x))
        }
    }

    DWORD WINAPI IssueThree::startFunc(LPVOID p)
    {
        int count = 0;
        IssueThree* pIssueThree = (IssueThree*)p;
        if (pIssueThree == NULL)
        {
            cout << "指针为空" << endl;
            return -1;
        }
        while (1)
        {
            DWORD start = GetTickCount();
            if (count >= COUNT)
            {
                count = 0;
            }
            double r = pIssueThree->m_cosValue[count++];
            DWORD busy = DWORD(r * GAP);
            while (GetTickCount() - start < busy) {}
            Sleep(DWORD(GAP - busy));
        }
        return DWORD(0);
    }
}
