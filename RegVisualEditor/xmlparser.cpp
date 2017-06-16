#include "xmlparser.h"
#include <QFile>
#include <QtXml/QtXml>

#include <QLineEdit>
#include <QComboBox>
#include <QLabel>

#include <Windows.h>

static std::string GetExePath(){
	static char szPath[1024] = "";
	GetModuleFileNameA(NULL, szPath, 1024);
	char* ptr = strrchr(szPath, '\\');
	*ptr = 0;
	return szPath;
}

XmlParser::XmlParser(QObject *parent)
	: QObject(parent)
{

}

XmlParser::~XmlParser()
{

}

void XmlParser::Load()
{
	QString strXmlPath;
	strXmlPath.sprintf("%s\\regCtrl.xml", GetExePath().c_str());

	QString strXml;
	QFile qFile(strXmlPath);
	if ( ! qFile.open(QFile::ReadOnly | QFile::Text)){
		return;
	}

	strXml = qFile.readAll();
	qFile.close();

	QDomDocument doc;
	doc.setContent(strXml);
	QDomElement root = doc.documentElement();

	QDomElement itemGroup = root.firstChildElement();
	for (; itemGroup.isNull() == false; itemGroup = itemGroup.nextSiblingElement()){
		CMenuCombo combo(itemGroup.attribute("text"), itemGroup.attribute("type"));		
		for (QDomElement itemEle = itemGroup.firstChildElement(); itemEle.isNull() == false; itemEle = itemEle.nextSiblingElement()){
			CCtrlItem* pItem = new CCtrlItem(itemEle.attribute("type"));
			pItem->SetDefaultText(itemEle.attribute("value"));
			pItem->SetLabel(itemEle.attribute("label"));
			combo.AddItem(pItem);
		}
		m_vecMenuCombo.push_back(combo);
	}
}

CMenuCombo* XmlParser::GetMenuComboByText( const QString& strText )
{
	int nSize = m_vecMenuCombo.size();
	for (int i = 0; i < nSize; i++){
		if (m_vecMenuCombo[i].GetText() == strText){
			return &m_vecMenuCombo[i];
		}
	}
	return nullptr;
}

CCtrlItem::CCtrlItem( EmCtrlType emCtrlType, QObject* ctrlHandle ) : m_emCtrlType(emCtrlType), m_qobject(ctrlHandle)
{

}

CCtrlItem::CCtrlItem( EmCtrlType emCtrlType ) : m_emCtrlType(emCtrlType), m_qobject(nullptr)
{

}

CCtrlItem::CCtrlItem( QString strType ) : m_qobject(nullptr)
{
	if (strType == "label"){
		m_emCtrlType = EM_LABEL;
	}else if (strType == "edit"){
		m_emCtrlType = EM_EDIT;
	}else if (strType == "combo"){
		m_emCtrlType = EM_COMBO;
	}
}

QString CCtrlItem::GetText()
{
	switch(m_emCtrlType){
	case EM_EDIT:
		return ((QLineEdit*)m_qobject)->text();
	case EM_COMBO:
		return ((QComboBox*)m_qobject)->currentText();
	case EM_LABEL:
		return ((QLabel*)m_qobject)->text();
	default:
		return "";
	}
}

EmCtrlType CCtrlItem::GetType()
{
	return m_emCtrlType;
}

void CCtrlItem::SetObject( QObject* pQOjbect )
{
	m_qobject = pQOjbect;	
}

void CCtrlItem::SetDefaultText( QString strText )
{
	m_strDefaultText = strText;
}

void CCtrlItem::SetLabel( QString strLabel )
{
	m_strLabel = strLabel;
}

QString CCtrlItem::GetLabel()
{
	return m_strLabel;
}
