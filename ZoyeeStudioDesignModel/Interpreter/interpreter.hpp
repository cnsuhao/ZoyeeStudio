#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include <string>

class Context
{
public:
	Context(std::string strText) : m_strText(strText){

	};
	std::string GetText(){
		return m_strText;
	}
protected:
	std::string m_strText;
};

class iInterpreter
{
public:
	iInterpreter(Context* pContext) : m_pContext(pContext){

	};
	virtual void ShowText(){};
protected:
	Context* m_pContext;
};

class MobileInterpreter : public iInterpreter
{
public:
	MobileInterpreter(Context* pContext) : iInterpreter(pContext){};
	void ShowText(){
		printf("the text [%s] is showed on mobile\n", m_pContext->GetText().c_str());
	}
};

class PCInterpreter : public iInterpreter
{
public:
	PCInterpreter(Context* pContext) : iInterpreter(pContext){};
	void ShowText(){
		printf("the text [%s] is showed on PC\n", m_pContext->GetText().c_str());
	}
};

#endif