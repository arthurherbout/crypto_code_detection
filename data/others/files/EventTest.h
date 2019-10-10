#pragma once

#include "event.h"

//���Բ���������event���ܣ���Ϊϵͳrpc�������ܷ���������

#define EVENT_TASK_COUNT 10000*10000

struct Flower
{
	__int64 nCheckSum; 			//����У��ɼ�����ȷ�ԣ���ֹһЩ�˻���ò��Ի�����
	__int64 nTransferCount;		//���䴫�ݴ���
};


class  FlowerTask : public HereticThread<FlowerTask>
{
public:
	HereticEvent<Flower,true> 	m_FlowerEvent;
	HereticEvent<Flower,true>*	m_NextFlowerEvent;  //��һ��������
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
		//���ü����¼�
		m_NextFlowerEvent->SetEvent();
	}
}
OPTIMIZE_OFF_END

//���񴴽�������
class  FlowerManagerTask : public HereticThread<FlowerManagerTask>
{
public:
	HereticEvent<Flower, true> 	m_FlowerEvent;
	HereticEvent<Flower, true>*	m_NextFlowerEvent;  //��һ��������
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
	//��ʼ�������¼���
	{
		printf("\n��ʼ����(%d)�����Ĵ�������...\n", EVENT_TASK_COUNT);
		m_nLastTime = ::GetTickCount();
		int n = 0;
		for (; n < (EVENT_TASK_COUNT-1); n++)
		{
			CreateHereticThread(&m_FlowerTaskArray[n], TRUE);
			m_FlowerTaskArray[n].m_NextFlowerEvent = &m_FlowerTaskArray[n + 1].m_FlowerEvent;
			m_FlowerTaskArray[n].m_TaskKey = n + 1;
		}
		CreateHereticThread(&m_FlowerTaskArray[n], TRUE);
		//���Լ����봫����
		m_FlowerTaskArray[n].m_NextFlowerEvent = &m_FlowerEvent;
		m_FlowerTaskArray[n].m_TaskKey = n + 1;


		m_NextFlowerEvent= &m_FlowerTaskArray[0].m_FlowerEvent;
		//m_FlowerTaskArray[EVENT_TASK_COUNT].m_TaskKey = EVENT_TASK_COUNT;
		m_TaskKey = EVENT_TASK_COUNT+1;
		m_nCurTime = ::GetTickCount();
		printf("\n����(%d)�����Ĵ��������������ʱ%d ...\n", EVENT_TASK_COUNT,
			(int)(m_nCurTime- m_nLastTime) );
	}
	//�����¼�������ʼ���Ĵ���
	m_FlowerTaskArray[0].m_FlowerEvent.SetEvent();
	for (;;)
	{
		
		XOS_Wait(m_FlowerEvent);

		m_nCircleCount++;
		//printf("FlowerManagerTask %d WaitOver..\n", m_TaskKey);
		//XOS_Sleep(100);
		m_NextFlowerEvent->m_Value.nCheckSum = m_FlowerEvent.m_Value.nCheckSum + m_TaskKey;
		m_NextFlowerEvent->m_Value.nTransferCount = m_FlowerEvent.m_Value.nTransferCount + 1;
		//���ü����¼�
		m_NextFlowerEvent->SetEvent();

		m_nCurTime  = ::GetTickCount();
		if (m_nCurTime - m_nLastTime >= 3000)
		{
			printf("���Ĵ��� ��ʱ(%u) ��תȦ��(%llu) checksum=%llu TransferCount=%llu event IOPS=%u\n", m_nCurTime - m_nLastTime, m_nCircleCount,
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
	//����һ�鲢������
#ifdef USE_LAREG_PAGE
	g_FlowerManager = (FlowerManagerTask*)AllocLargePage(sizeof(FlowerManagerTask));
	new (g_FlowerManager) FlowerManagerTask();
#else
	g_FlowerManager = new FlowerManagerTask;
#endif

	CreateHereticThread(g_FlowerManager, TRUE);

}



