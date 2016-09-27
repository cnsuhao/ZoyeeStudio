#ifdef MD5_EXPORTS
#define MD5_API __declspec(dllexport)
#else
#define MD5_API __declspec(dllimport)
#endif

extern "C"{
	MD5_API void md5(char* pInput, int nInputLen, __out char* pOutput32);
};
