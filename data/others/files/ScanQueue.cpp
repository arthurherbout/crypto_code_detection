#include "ScanQueue.h"
INT32 IdentifyTaskCount = 0;
SCAN_INFORMATION IdentifyTemp; //���ټ���Temp
PSCAN_QUEUE_INFORMATION IdentifyQueue = NULL; //���ټ�������
PSCAN_QUEUE_INFORMATION IdentifyQueueTop = NULL; //���ټ������еĶ���
DWORD WINAPI ScanThreadPool(PVOID arg)
{
	SCAN_INFORMATION IdentifyInformation;
	VIRUS_DESCRIPTION VirusDescription;
	PSCAN_QUEUE_INFORMATION Queue;
	BOOLEAN IsQueueTask = false;
	VirusType VType;
	while(1)
	{
	WaitForSingleObject(ThreadPool_Scan_ThreadEvent,INFINITE);
	memmove(&IdentifyInformation,&IdentifyTemp,sizeof(SCAN_INFORMATION));
	SetEvent(ThreadPool_Scan_MainEvent);
Deal:
	//=================
	VType = IdentifyFile(IdentifyInformation.Path,&VirusDescription,&IdentifyInformation);
	//=================
	if(!IsQueueTask)
	IdentifyTaskCount --;
	else
	IsQueueTask = false;
	if(IdentifyQueue != NULL)
	{
		WaitForSingleObject(ThreadPool_Scan_QueueEvent,INFINITE);
		if(IdentifyQueue != NULL)
		{
		Queue = IdentifyQueue;
		IdentifyQueue = (PSCAN_QUEUE_INFORMATION)IdentifyQueue->Next;//�ı�ͷ
		if(IdentifyQueue == NULL)
			IdentifyQueueTop = NULL;
		memmove(&IdentifyInformation,&Queue->Info,sizeof(SCAN_INFORMATION));
		free(Queue);
		IsQueueTask = true;
		SetEvent(ThreadPool_Scan_QueueEvent);//�������̻߳�ȡ������Ϣ
		goto Deal; //��ת���������
		}
	}
	}
}
VirusType IdentifyFile(LPWSTR FileName,PVIRUS_DESCRIPTION VirusDescription,PSCAN_INFORMATION Info)
{
	bool VirusFlag = false;
	VirusType Type;
	if(Engines[JavHeur].FunctionCall2(FileName))
	{
		VirusFlag = true;
		lstrcpy(VirusDescription->VirusName,L"HEUR\\Malware.Gen01");
		lstrcpy(VirusDescription->EngineName, L"Jav");
	}
	
	if(VirusFlag)
	{
		if(!Engines[FileVerify].FunctionCall2(FileName))
		{
		Type = VirusType::Virus;
		goto CallBack;
		}
	}
	Type = VirusType::Safe;
CallBack:
	if(Info!=NULL)
	Info->ScanCallBack(FileName,VirusDescription,Type,Info->CallBackPtr32,Info->CallBackValue);
	return Type;
}