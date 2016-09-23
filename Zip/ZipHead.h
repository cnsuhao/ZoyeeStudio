// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� ZIP_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// ZIP_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef ZIP_EXPORTS
#define ZIP_API __declspec(dllexport)
#else
#define ZIP_API __declspec(dllimport)
#endif

extern "C"{
	ZIP_API void* CreateZipFile(char* pZipFile, char* pPassword = nullptr);
	ZIP_API void* OpenZipFile(char* pZipFile, char* pPassword = nullptr);
	ZIP_API void CloseZipFile(void* pZip);
	ZIP_API int SetUnZipDir(void* pZip, char* pDir);

	ZIP_API int Zip(void* pZip, char* pFromZipFilePath, char* pInZipFilePath, bool bIsDir = false);
	ZIP_API int GetUnZipFileCount(void* pZip);
	ZIP_API int GetUnZipFile(void* pZip, int nIndex, char* pFileName);
	ZIP_API int UnZip(void* pZip, int nIndex);
};
