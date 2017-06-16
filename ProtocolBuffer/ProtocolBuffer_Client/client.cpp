#include "hello.pb.h"
#ifdef _DEBUG
#pragma comment(lib, "../lib/debug/libprotobuf.lib")
#pragma comment(lib, "../lib/debug/libprotoc.lib")
#else
#pragma comment(lib, "../lib/release/libprotobuf.lib")
#pragma comment(lib, "../lib/release/libprotoc.lib")
#endif

#include <iostream>
#include <fstream>

int main()
{
	helloPkg::helloMsg msg;
	msg.set_strname("hello");
	msg.set_strmail("hello@Mail.com");
	msg.set_nid(30);

	helloPkg::helloMsg msgelement(msg);
	msgelement.set_strname("H1");
	msg.mutable_msg()->AddAllocated(&msgelement);
	helloPkg::helloMsg* msg2 = msgelement.New();
	//msg2->CopyFrom(msgelement);
	msg2->set_strname("FFF");
	msg2->MergeFrom(msgelement);
	msgelement.set_strname("H2");
	msg.mutable_msg()->AddAllocated(&msgelement);
	msgelement.set_strname("H3");
	msg.mutable_msg()->AddAllocated(&msgelement);
	int nsize = msg.msg().size();
	std::string __strname = msg.msg(0).strname();
	__strname = msg.msg(1).strname();
	__strname = msg.msg(2).strname();
	std::fstream fs("./out.log", std::ios::out | std::ios::binary);
	msg.SerializeToOstream(&fs);
	std::string strMsg;
	msg.SerializeToString(&strMsg);
}