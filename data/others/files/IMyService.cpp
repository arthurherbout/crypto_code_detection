#include "IMyService.h"
namespace android
{
    IMPLEMENT_META_INTERFACE(MyService, "android.demo.IMyService"); //ʹ�ú꣬���MyService����
   
  //�ͻ���
	BpMyService::BpMyService(const sp<IBinder>& impl) :
			BpInterface<IMyService>(impl) {
	}
	
	// ʵ�ֿͻ���sayHello����
	void BpMyService::sayHello() {
		printf("BpMyService::sayHello\n");
		Parcel data, reply;
		data.writeInterfaceToken(IMyService::getInterfaceDescriptor());
		remote()->transact(HELLO, data, &reply);
		printf("get num from BnMyService: %d\n", reply.readInt32());
	}
	
	//����ˣ�����Զ����Ϣ������onTransact����
	status_t BnMyService::onTransact(uint_t code, const Parcel& data,
			Parcel* reply, uint32_t flags) {
		switch (code) {
		case HELLO: {    //�յ�HELLO����Ĵ�������
			printf("BnMyService:: got the client hello\n");
			CHECK_INTERFACE(IMyService, data, reply);
			sayHello();
			reply->writeInt32(2015);
			return NO_ERROR;
		}
			break;
		default:
			break;
		}
		return NO_ERROR;
	}
	
	// ʵ�ַ����sayHello����
	void BnMyService::sayHello() {
		printf("BnMyService::sayHello\n");
	};
}
