#include "WMI.h"

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "comsupp.lib")
#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib,"kernel32.lib")
#include <comutil.h>
#include <iostream>
using namespace std;

ZoyeeUtils::CWMI::CWMI()
{

}

int ZoyeeUtils::CWMI::Init()
{
	HRESULT hres;
	//��ʼ�� COM.
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
		cout << "Failed to initialize COM library. "<< "Error code = 0x"<< hex << hres << endl;
		return 1; // Program has failed.
	}

	// ���ý��̰�ȫ����

	hres = CoInitializeSecurity(
		NULL,
		-1, // COM negotiates service
		NULL, // Authentication services
		NULL, // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT, // authentication
		RPC_C_IMP_LEVEL_IMPERSONATE, // Impersonation
		NULL, // Authentication info
		EOAC_NONE, // Additional capabilities
		NULL // Reserved
		);

	if (FAILED(hres))
	{
		cout << "Failed to initialize security. "
			<< "Error code = 0x"
			<< hex << hres << endl;
		CoUninitialize();
		return 1; // Program has failed.
	}

	IWbemLocator *pLoc = 0;
	hres = CoCreateInstance(
		CLSID_WbemLocator,        
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *) &pLoc);
	if (FAILED(hres))
	{
		CoUninitialize();
		return -1;       // Program has failed.
	}
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // WMI namespace
		NULL,                    // User name
		NULL,                    // User password
		0,                       // Locale
		NULL,                    // Security flags 
		0,                       // Authority      
		0,                       // Context object
		&pSvc                    // IWbemServices proxy
		);     
	if (FAILED(hres))
	{
		pLoc->Release();    
		CoUninitialize();
		return -2;                // Program has failed.
	}
	return 0;
}

std::string ZoyeeUtils::CWMI::GetSelectString( emWmiInfo wmInfo )
{
	string strTable;
	switch(wmInfo)
	{
	case Win32_SoundDevice:                          // ��Ƶ�豸  
		strTable = "Win32_SoundDevice";
		break;
	case Win32_Product:    
		strTable = "Win32_Product";
		break;
	case Win32_Registry:                             // ע���  
		strTable = "Win32_Registry";
		break;
	case Win32_OnBoardDevice:                        // �����ϵ�Ƕ���豸  
		strTable = "Win32_OnBoardDevice";
		break;
	case Win32_MotherboardDevice:                    // ĸ��  
		strTable  = "Win32_MotherboardDevice";
		break;
	case Win32_BaseBoard:                            // ����  
		strTable = "Win32_BaseBoard";
		break;
	case Win32_DMAChannel:                           // DMAͨ��  
		strTable = "Win32_DMAChannel";
		break;
	case Win32_Bus:                                  // ����  
		strTable = "Win32_Bus";
		break;
	case Win32_BIOS:                                 // BIOSϵͳ  
		strTable = "Win32_BIOS";
		break;
	case Win32_SystemBIOS:  
		strTable = "Win32_SystemBIOS";
		break;
	case Win32_Processor:                            // ������  
		strTable = "Win32_Processor";
		break;
	case Win32_SystemProcesses:                      //   
		strTable = "Win32_SystemProcesses";
		break;
	case Win32_Process:                              // ϵͳ����  
		strTable = "Win32_Process";
		break;
	case Win32_Thread:                               // ϵͳ�߳�  
		strTable = "Win32_Thread";
		break;
	case Win32_AssociatedProcessorMemory:            // CACHE  
		strTable = "Win32_AssociatedProcessorMemory";
		break;
	case Win32_CacheMemory:                          // ���������ڴ�  
		strTable = "Win32_CacheMemory";
		break;
	case Win32_PhysicalMedia:                        // ����ý����Ϣ (Ӳ��)  
		strTable = "Win32_PhysicalMedia";
		break;
	case Win32_LogicalDisk :                         // �߼�������  
		strTable = "Win32_LogicalDisk";
		break;
	case Win32_DiskDrive:                            // ����������  
		strTable = "Win32_DiskDrive";
		break;
	case Win32_MemoryDevice:                         // �ڴ��豸  
		strTable = "Win32_MemoryDevice";
		break;
	case Win32_PhysicalMemoryArray:                  // �����ڴ�����  
		strTable = "Win32_PhysicalMemoryArray";
		break;
	case Win32_PhysicalMemoryLocation:               // �����ڴ�λ��  
		strTable = "Win32_PhysicalMemoryLocation";
		break;
	case CIM_NumericSensor:                          // ���ִ�����  
		strTable = "CIM_NumericSensor";
		break;
	case Win32_VoltageProbe:                         // ���ִ�����   
		strTable = "Win32_VoltageProbe";
		break;
	case Win32_TemperatureProbe:                     // �¶ȴ�����  
		strTable = "Win32_TemperatureProbe";
		break;
	case Win32_CurrentProbe:       
		strTable = "Win32_CurrentProbe";
		break;
	case Win32_OperatingSystem:                      // ����ϵͳ      
		strTable = "Win32_OperatingSystem";
		break;
	case Win32_UserAccount:                          // �û��˺�  
		strTable = "Win32_UserAccount";
		break;
	case Win32_SerialPort:                           // ���нӿ�  
		strTable = "Win32_SerialPort";
		break;
	case Win32_ParallelPort:                         // ���нӿ�  
		strTable = "Win32_ParallelPort";
		break;
	case Win32_SCSIController:                       // С�ͼ����ϵͳ�ӿ�  
		strTable = "Win32_SCSIController";
		break;
	case Win32_PortResource:                         // I/O �˿�  
		strTable = "Win32_PortResource";
		break;
	case Win32_PNPDevice:                            // ���弴���豸  
		strTable = "Win32_PNPDevice";
		break;
	case Win32_NetworkAdapter:                       // ����������  
		strTable = "Win32_NetworkAdapter";
		break;
	case Win32_NetworkAdapterConfiguration:  
		strTable = "Win32_NetworkAdapterConfiguration";
		break;
	case Win32_NetworkAdapterSetting:  
		strTable = "Win32_NetworkAdapterSetting";
		break;
	case Win32_AssociatedBattery:  
		strTable = "Win32_AssociatedBattery";
		break;
	case Win32_Battery:                              // �ڲ����  
		strTable = "Win32_Battery";
		break;
	case Win32_PortableBattery:  
		strTable = "Win32_PortableBattery";
		break;
	case Win32_PowerManagementEvent:  
		strTable = "Win32_PowerManagementEvent";
		break;
	case Win32_UninterruptiblePowerSupply:  
		strTable = "Win32_UninterruptiblePowerSupply";
		break;
	case Win32_DriverForDevice:  
		strTable = "Win32_DriverForDevice";
		break;
	case Win32_Printer:                              // ��ӡ��  
		strTable = "Win32_Printer";
		break;
	case Win32_TCPIPPrinterPort:  
		strTable = "Win32_TCPIPPrinterPort";
		break;
	case Win32_POTSModem:  
		strTable = "Win32_POTSModem";
		break;
	case Win32_DesktopMonitor:                       // ��ʾ��  
		strTable = "Win32_DesktopMonitor";
		break;
	case Win32_VideoController:                      // �Կ�  
		strTable = "Win32_VideoController";
		break;
	case Win32_CDROMDrive:  
		strTable = "Win32_CDROMDrive";
		break;
	case Win32_Keyboard:                             // ����  
		strTable = "Win32_Keyboard";
		break;
	case Win32_AutochkSetting:  
		strTable = "Win32_AutochkSetting";
		break;
	case Win32_PointingDevice:                       // ����豸����ꡢ������  
		strTable = "Win32_PointingDevice";
		break;
	case Win32_Fan:                                  // ����  
		strTable = "Win32_Fan";
		break;
	case Win32_WMISetting:  
		strTable = "Win32_WMISetting";
		break;
	case Win32_TimeZone:       
		strTable = "Win32_TimeZone";
		break;
	case Win32_Environment:                          // ����·��  
		strTable = "Win32_Environment";
		break;
	case Win32_QuotaSetting:  
		strTable = "Win32_QuotaSetting";
		break;
	case Win32_NetworkProtocol:                      // ����װ������Э��  
		strTable = "Win32_NetworkProtocol";
		break;
	default:
		break;
	}
	char sz[128] = {0};
	sprintf(sz, "SELECT * FROM %s", strTable.c_str());
	return sz;
}

std::string ZoyeeUtils::CWMI::GetSystemInfo( emWmiInfo wmInfo )
{
	HRESULT hres;
	hres	= CoSetProxyBlanket(
		pSvc,
		// the proxy to set 
		RPC_C_AUTHN_WINNT,            // authentication service
		RPC_C_AUTHZ_NONE,             // authorization service
		NULL,                         // Server principal name
		RPC_C_AUTHN_LEVEL_CALL,       // authentication level
		RPC_C_IMP_LEVEL_IMPERSONATE,  // impersonation level
		NULL,                         // client identity
		EOAC_NONE                     // proxy capabilities    
		);

	if (FAILED(hres))
	{
		return "";               // Program has failed.
	}
	IEnumWbemClassObject* pEnumerator = NULL;     

	hres = pSvc->ExecQuery(     
		_bstr_t("WQL"),   
		_bstr_t(GetSelectString(wmInfo).c_str()),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,      
		NULL,     
		&pEnumerator);   
	IWbemClassObject* pclsObj = NULL;     
	ULONG uReturn = 0;     
	std::string strvtProBSTR;
	while (pEnumerator)     
	{     
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);     
		if ((0 == uReturn) || (0 == pclsObj))     
		{     
			break;     
		}     
		VARIANT vtProp; 
		// Get the value of the Name property      
		VariantInit(&vtProp);   
		vtProp.bstrVal = 0;// NULL   
		hr = pclsObj->GetObjectText(0, &vtProp.bstrVal);  
		if (vtProp.bstrVal != 0)     
		{     
			/*wcout.imbue(locale(locale(""), "" ,LC_CTYPE));  
			wcout << L"Ӳ����Ϣ: " << vtProp.bstrVal << endl;     */
			strvtProBSTR += _com_util::ConvertBSTRToString(vtProp.bstrVal);
		}     
		else     
		{     
			
		}  
		VariantClear(&vtProp);     
	}       
	if (pEnumerator)     
	{     
		pEnumerator->Release();     
	}     	
	if (pclsObj){
		pclsObj->Release();
	}  
	return strvtProBSTR;
}

ZoyeeUtils::CWMI::~CWMI()
{
	pSvc->Release();
	pLoc->Release();    
	CoUninitialize();
}

std::string ZoyeeUtils::CWMI::ExecWQL( std::string strWQL )
{
	HRESULT hres;
	hres	= CoSetProxyBlanket(
		pSvc,
		// the proxy to set 
		RPC_C_AUTHN_WINNT,            // authentication service
		RPC_C_AUTHZ_NONE,             // authorization service
		NULL,                         // Server principal name
		RPC_C_AUTHN_LEVEL_CALL,       // authentication level
		RPC_C_IMP_LEVEL_IMPERSONATE,  // impersonation level
		NULL,                         // client identity
		EOAC_NONE                     // proxy capabilities    
		);

	if (FAILED(hres))
	{
		return "";               // Program has failed.
	}
	IEnumWbemClassObject* pEnumerator = NULL;     

	hres = pSvc->ExecQuery(     
		_bstr_t("WQL"),   
		_bstr_t(strWQL.c_str()),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,      
		NULL,     
		&pEnumerator);   
	IWbemClassObject* pclsObj = NULL;     
	ULONG uReturn = 0;     
	std::string strvtProBSTR;
	while (pEnumerator)     
	{     
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);     
		if ((0 == uReturn) || (0 == pclsObj))     
		{     
			break;     
		}     
		VARIANT vtProp; 
		// Get the value of the Name property      
		VariantInit(&vtProp);   
		vtProp.bstrVal = 0;// NULL   
		hr = pclsObj->GetObjectText(0, &vtProp.bstrVal);  
		if (vtProp.bstrVal != 0)     
		{     
			/*wcout.imbue(locale(locale(""), "" ,LC_CTYPE));  
			wcout << L"Ӳ����Ϣ: " << vtProp.bstrVal << endl;     */
			strvtProBSTR += _com_util::ConvertBSTRToString(vtProp.bstrVal);
		}     
		else     
		{     
			
		}  
		VariantClear(&vtProp);     
	}       
	if (pEnumerator)     
	{     
		pEnumerator->Release();     
	}     	
	if (pclsObj){
		pclsObj->Release();
	}  
	return strvtProBSTR;
}
