// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� MD5_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// MD5_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef MD5_EXPORTS
#define MD5_API __declspec(dllexport)
#else
#define MD5_API __declspec(dllimport)
#endif

extern "C"{
	MD5_API void md5(char* pInput, int nInputLen, __out char* pOutput32);
};

//MD5_API int fnMD5(void);
