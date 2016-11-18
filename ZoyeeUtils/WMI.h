#ifndef __WMI_H__
#define __WMI_H__

#include <Wbemidl.h>
#include <iostream>
#include <string>
struct IWbemServices;
namespace ZoyeeUtils{
	class CWMI{
	public:
		CWMI();
		~CWMI();
		int Init();
		enum emWmiInfo{
			Win32_SoundDevice,                          // ��Ƶ�豸  
			Win32_Product,    
			Win32_Registry,                             // ע���  
			Win32_OnBoardDevice,                        // �����ϵ�Ƕ���豸  
			Win32_MotherboardDevice,                    // ĸ��  
			Win32_BaseBoard,                            // ����  
			Win32_DMAChannel,                           // DMAͨ��  
			Win32_Bus,                                  // ����  
			Win32_BIOS,                                 // BIOSϵͳ  
			Win32_SystemBIOS,  
			Win32_Processor,                            // ������  
			Win32_SystemProcesses,                      //   
			Win32_Process,                              // ϵͳ����  
			Win32_Thread,                               // ϵͳ�߳�  
			Win32_AssociatedProcessorMemory,            // CACHE  
			Win32_CacheMemory,                          // ���������ڴ�  
			Win32_PhysicalMedia,                        // ����ý����Ϣ (Ӳ��)  
			Win32_LogicalDisk ,                         // �߼�������  
			Win32_DiskDrive,                            // ����������  
			Win32_MemoryDevice,                         // �ڴ��豸  
			Win32_PhysicalMemoryArray,                  // �����ڴ�����  
			Win32_PhysicalMemoryLocation,               // �����ڴ�λ��  
			CIM_NumericSensor,                          // ���ִ�����  
			Win32_VoltageProbe,                         // ���ִ�����   
			Win32_TemperatureProbe,                     // �¶ȴ�����  
			Win32_CurrentProbe,       
			Win32_OperatingSystem,                      // ����ϵͳ      
			Win32_UserAccount,                          // �û��˺�  
			Win32_SerialPort,                           // ���нӿ�  
			Win32_ParallelPort,                         // ���нӿ�  
			Win32_SCSIController,                       // С�ͼ����ϵͳ�ӿ�  
			Win32_PortResource,                         // I/O �˿�  
			Win32_PNPDevice,                            // ���弴���豸  
			Win32_NetworkAdapter,                       // ����������  
			Win32_NetworkAdapterConfiguration,  
			Win32_NetworkAdapterSetting,  
			Win32_AssociatedBattery,  
			Win32_Battery,                              // �ڲ����  
			Win32_PortableBattery,  
			Win32_PowerManagementEvent,  
			Win32_UninterruptiblePowerSupply,  
			Win32_DriverForDevice,  
			Win32_Printer,                              // ��ӡ��  
			Win32_TCPIPPrinterPort,  
			Win32_POTSModem,  
			Win32_DesktopMonitor,                       // ��ʾ��  
			Win32_VideoController,                      // �Կ�  
			Win32_CDROMDrive,  
			Win32_Keyboard,                             // ����  
			Win32_AutochkSetting,  
			Win32_PointingDevice,                       // ����豸����ꡢ������  
			Win32_Fan,                                  // ����  
			Win32_WMISetting,  
			Win32_TimeZone,       
			Win32_Environment,                          // ����·��  
			Win32_QuotaSetting,  
			Win32_NetworkProtocol                    // ����װ������Э��  
		};
		std::string GetSystemInfo(emWmiInfo wmInfo);		
		std::string ExecWQL(std::string strWQL);
	protected:
		std::string GetSelectString(emWmiInfo wmInfo);
	private:
		IWbemServices *pSvc;
		IWbemLocator *pLoc;
	};
}

#endif