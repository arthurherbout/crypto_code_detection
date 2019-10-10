#pragma once

#include "event.h"

//测试并发任务下event性能，作为系统rpc串行性能分析的依据

#define EVENT_TASK_COUNT 10000*10000

struct Flower
{
	__int64 nCheckSum; 			//用于校验成绩的正确性，防止一些人会觉得测试会作假
	__int64 nTransferCount;		//花朵传递次数
};


class  FlowerTask : public HereticThread<FlowerTask>
{
public:
	HereticEvent<Flower,true> 	m_FlowerEvent;
	HereticEvent<Flower,true>*	m_NextFlowerEvent;  //下一个接收者
	int m_TaskKey;

	FlowerTask() { 
		m_FlowerEvent.m_Value.nCheckSum = 0;
		m_FlowerEvent.m_Value.nTransferCount = 0;
	};
	~FlowerTask() {};
	
	
	void Init() {};
	void Close() {};
	void Loop();

};

OPTIMIZE_OFF
void FlowerTask::Loop()
{
	USING_HERETICOS_THREAD;

	for (;;)
	{
		XOS_Wait(m_FlowerEvent);
		
		//XOS_Sleep(100);
		//printf("Task %d WaitOver checksum=%llu TransferCount=%llu ..\n", m_TaskKey,
		//	m_FlowerEvent.m_Value.nCheckSum, m_FlowerEvent.m_Value.nTransferCount);
		//XOS_WaitTimeOut(m_FlowerEvent, 1000);
		m_NextFlowerEvent->m_Value.nCheckSum = m_FlowerEvent.m_Value.nCheckSum+m_TaskKey;
		m_NextFlowerEvent->m_Value.nTransferCount= m_FlowerEvent.m_Value.nTransferCount+1;
		//设置激活事件
		m_NextFlowerEvent->SetEvent();
	}
}
OPTIMIZE_OFF_END

//任务创建管理者
class  FlowerManagerTask : public HereticThread<FlowerManagerTask>
{
public:
	HereticEvent<Flower, true> 	m_FlowerEvent;
	HereticEvent<Flower, true>*	m_NextFlowerEvent;  //下一个接收者
	FlowerTask m_FlowerTaskArray[EVENT_TASK_COUNT];
	DWORD  m_nCurTime;
	DWORD m_nLastTime;
	__int64 m_nLastTransferCount;
	__int64 m_nCircleCount;
	int m_TaskKey;
	FlowerManagerTask() {
		m_nCircleCount=m_nLastTransferCount = 0;
		m_nCurTime = m_nLastTime = ::GetTickCount();
	}
	~FlowerManagerTask() {}
	void Init() {};
	void Close() {};
	void Loop();
	
};

OPTIMIZE_OFF
void FlowerManagerTask::Loop()
{
	USING_HERETICOS_THREAD;
	//初始化任务事件环
	{
		printf("\n开始创建(%d)个击鼓传花任务...\n", EVENT_TASK_COUNT);
		m_nLastTime = ::GetTickCount();
		int n = 0;
		for (; n < (EVENT_TASK_COUNT-1); n++)
		{
			CreateHereticThread(&m_FlowerTaskArray[n], TRUE);
			m_FlowerTaskArray[n].m_NextFlowerEvent = &m_FlowerTaskArray[n + 1].m_FlowerEvent;
			m_FlowerTaskArray[n].m_TaskKey = n + 1;
		}
		CreateHereticThread(&m_FlowerTaskArray[n], TRUE);
		//将自己加入传话环
		m_FlowerTaskArray[n].m_NextFlowerEvent = &m_FlowerEvent;
		m_FlowerTaskArray[n].m_TaskKey = n + 1;


		m_NextFlowerEvent= &m_FlowerTaskArray[0].m_FlowerEvent;
		//m_FlowerTaskArray[EVENT_TASK_COUNT].m_TaskKey = EVENT_TASK_COUNT;
		m_TaskKey = EVENT_TASK_COUNT+1;
		m_nCurTime = ::GetTickCount();
		printf("\n创建(%d)个击鼓传花任务结束，用时%d ...\n", EVENT_TASK_COUNT,
			(int)(m_nCurTime- m_nLastTime) );
	}
	//启动事件链，开始击鼓传花
	m_FlowerTaskArray[0].m_FlowerEvent.SetEvent();
	for (;;)
	{
		
		XOS_Wait(m_FlowerEvent);

		m_nCircleCount++;
		//printf("FlowerManagerTask %d WaitOver..\n", m_TaskKey);
		//XOS_Sleep(100);
		m_NextFlowerEvent->m_Value.nCheckSum = m_FlowerEvent.m_Value.nCheckSum + m_TaskKey;
		m_NextFlowerEvent->m_Value.nTransferCount = m_FlowerEvent.m_Value.nTransferCount + 1;
		//设置激活事件
		m_NextFlowerEvent->SetEvent();

		m_nCurTime  = ::GetTickCount();
		if (m_nCurTime - m_nLastTime >= 3000)
		{
			printf("击鼓传花 用时(%u) 轮转圈数(%llu) checksum=%llu TransferCount=%llu event IOPS=%u\n", m_nCurTime - m_nLastTime, m_nCircleCount,
				m_FlowerEvent.m_Value.nCheckSum, m_FlowerEvent.m_Value.nTransferCount, (unsigned int)((1000*(m_FlowerEvent.m_Value.nTransferCount - m_nLastTransferCount)) / (m_nCurTime - m_nLastTime))
			);
			m_nLastTransferCount = m_FlowerEvent.m_Value.nTransferCount;
			m_nLastTime = m_nCurTime;
		}
		

	}
}
OPTIMIZE_OFF_END

FlowerManagerTask * g_FlowerManager;

void TestEventIo()
{
	//创建一组并发任务
#ifdef USE_LAREG_PAGE
	g_FlowerManager = (FlowerManagerTask*)AllocLargePage(sizeof(FlowerManagerTask));
	new (g_FlowerManager) FlowerManagerTask();
#else
	g_FlowerManager = new FlowerManagerTask;
#endif

	CreateHereticThread(g_FlowerManager, TRUE);

}



