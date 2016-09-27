#define TINYHTTPD_EXPORTS
#ifdef TINYHTTPD_EXPORTS
#define TINYHTTPD_API __declspec(dllexport)
#else
#define TINYHTTPD_API __declspec(dllimport)
#endif
typedef void(*pOutput)(char* pAction, char* pMsg);
extern "C"{
	TINYHTTPD_API int Start(int nPort, pOutput pFunc);
	TINYHTTPD_API int Stop();
};