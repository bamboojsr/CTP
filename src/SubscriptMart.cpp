#include <iostream>
#include <string.h>
#include <thread>
#include <unistd.h>
using namespace std;
#include "ThostFtdcMdApi.h"
#include "ThostFtdcUserApiStruct.h"
#define BROKERID "admin"
#define PASSWORD "admin"
#define USERID  "admin"
CThostFtdcMdApi* pUserApi = NULL;
class QCTPMdSpi : public CThostFtdcMdSpi{
		public:
				QCTPMdSpi(const CThostFtdcMdApi* ipCThostFtdcMdApi):pCThostFtdcMdApi(ipCThostFtdcMdApi){}
				///删除接口对象本身
				///@remark 不再使用本接口对象时,调用该函数删除接口对象
				void Release();

				///初始化
				///@remark 初始化运行环境,只有调用后,接口才开始工作
				void Init();

				///等待接口线程结束运行
				///@return 线程退出代码
				int Join();

				///获取当前交易日
				///@retrun 获取到的交易日
				///@remark 只有登录成功后,才能得到正确的交易日
				const char *GetTradingDay();

				///注册前置机网络地址
				///@param pszFrontAddress：前置机网络地址。
				///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
				///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
				void RegisterFront(char *pszFrontAddress);

				///注册名字服务器网络地址
				///@param pszNsAddress：名字服务器网络地址。
				///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:12001”。 
				///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”12001”代表服务器端口号。
				///@remark RegisterNameServer优先于RegisterFront
				void RegisterNameServer(char *pszNsAddress);

				///注册名字服务器用户信息
				///@param pFensUserInfo：用户信息。
				void RegisterFensUserInfo(CThostFtdcFensUserInfoField * pFensUserInfo);

				///注册回调接口
				///@param pSpi 派生自回调接口类的实例
				void RegisterSpi(CThostFtdcMdSpi *pSpi);

				///订阅行情。
				///@param pInstrumentID 合约ID  
				///@param nCount 要订阅/退订行情的合约个数
				///@remark 
				int SubscribeMarketData(CThostFtdcSpecificInstrumentField *pInstrumentID, int nCount);
				///退订行情。
				///@param pInstrumentID 合约ID  
				///@param nCount 要订阅/退订行情的合约个数
				///@remark 
				int UnSubscribeMarketData(CThostFtdcSpecificInstrumentField * pInstrumentID, int nCount) ;


				///用户登录请求
				int ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID);

				///登出请求
				int ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID);
		private:
				const CThostFtdcMdApi* pCThostFtdcMdApi;
				pthread_t thId;
				static void * workThread(void * arg);
};

///删除接口对象本身
///@remark 不再使用本接口对象时,调用该函数删除接口对象
void QCTPMdSpi :: Release(){}

///初始化
///@remark 初始化运行环境,只有调用后,接口才开始工作
void QCTPMdSpi :: Init(){
		pthread_create(&thId, NULL, workThread, this);
}
void* QCTPMdSpi :: workThread(void* arg){
		//request login
		QCTPMdSpi* pUserSpi= (QCTPMdSpi *)arg;
		CThostFtdcReqUserLoginField userReq;
		memset(&userReq, 0, sizeof(userReq));
		strncpy(userReq.BrokerID, BROKERID, sizeof(userReq.BrokerID) -1);
		strncpy(userReq.UserID, USERID, sizeof(userReq.UserID) -1);
		strncpy(userReq.Password, PASSWORD, sizeof(userReq.Password) -1);
		int requestId = 0;
		int ret = pUserApi->ReqUserLogin(&userReq, ++requestId);
		//subscript market
		//SubscribeMarketData(CThostFtdcSpecificInstrumentField *pInstrumentID, int nCount) = 0;
		CThostFtdcSpecificInstrumentField *pInstrumentID = NULL;
		int nCount = 0;
		ret = pUserApi->SubscribeMarketData(pInstrumentID, nCount);
		return NULL;
}
///等待接口线程结束运行
///@return 线程退出代码
int QCTPMdSpi :: Join() { 
		if( thId != 0)
				pthread_join( thId, NULL);
		return 0;
}

///获取当前交易日
///@retrun 获取到的交易日
///@remark 只有登录成功后,才能得到正确的交易日
const char* QCTPMdSpi :: GetTradingDay(){ return NULL;}

///注册前置机网络地址
///@param pszFrontAddress：前置机网络地址。
///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
void QCTPMdSpi :: RegisterFront(char *pszFrontAddress){}

///注册名字服务器网络地址
///@param pszNsAddress：名字服务器网络地址。
///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:12001”。 
///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”12001”代表服务器端口号。
///@remark RegisterNameServer优先于RegisterFront
void QCTPMdSpi :: RegisterNameServer(char *pszNsAddress) {} 

///注册名字服务器用户信息
///@param pFensUserInfo：用户信息。
void QCTPMdSpi :: RegisterFensUserInfo(CThostFtdcFensUserInfoField * pFensUserInfo) {}

///注册回调接口
///@param pSpi 派生自回调接口类的实例
void QCTPMdSpi :: RegisterSpi(CThostFtdcMdSpi *pSpi) {}

///订阅行情。
///@param pInstrumentID 合约ID  
///@param nCount 要订阅/退订行情的合约个数
///@remark 
int QCTPMdSpi :: SubscribeMarketData(CThostFtdcSpecificInstrumentField *pInstrumentID, int nCount) { return 0;}
///退订行情。
///@param pInstrumentID 合约ID  
///@param nCount 要订阅/退订行情的合约个数
///@remark 
int QCTPMdSpi :: UnSubscribeMarketData(CThostFtdcSpecificInstrumentField * pInstrumentID, int nCount) { return 0;}


///用户登录请求
int QCTPMdSpi :: ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID) { return 0;}

///登出请求
int QCTPMdSpi :: ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID) { return 0;}



int main(int argc, char* argv[]){
		//register user
		pUserApi = CThostFtdcMdApi::CreateFtdcMdApi("./1.txt", true);
		if ( pUserApi == NULL ){
				cout << "call CThostFtdcMdApi::CreateFtdcMdApi" << endl;
				exit(-1);
		}
		QCTPMdSpi* pUserSpi= new QCTPMdSpi(pUserApi);
		pUserApi->RegisterSpi(pUserSpi);
		pUserApi->RegisterFront("tcp://127.0.0.1:17001");
		pUserApi->Init();
		pUserApi->Join();
		return 0;
}
