#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>
#include <vector>

using namespace std;

enum EmCtrlType{
	EM_LABEL,
	EM_EDIT,
	EM_COMBO
};

class CCtrlItem{//the infomation of single ctrl
public:
	CCtrlItem(QString strType);
	CCtrlItem(EmCtrlType emCtrlType);
	CCtrlItem(EmCtrlType emCtrlType, QObject* ctrlHandle);//qobject's parent must be its parent widget
	QString GetText();
	EmCtrlType GetType();
	void SetObject(QObject* pQOjbect);
	void SetDefaultText(QString strText);
	void SetLabel(QString strLabel);
	QString GetLabel();
private:
	QString m_strLabel;
	QString m_strDefaultText;
	QObject* m_qobject;
	EmCtrlType m_emCtrlType;
};

class CMenuCombo{
public:
	CMenuCombo(QString strText, QString strType){
		m_strType = strType;
		m_strText = strText;
		m_pWgt = nullptr;
	};

	QString GetText(){
		return m_strText;
	};

	QString GetType(){
		return m_strType;
	};

	void AddItem(CCtrlItem* pItem){
		m_vecCtrlItems.push_back(pItem);
	}

	vector<CCtrlItem*>& GetVecItems(){
		return m_vecCtrlItems;
	}

	void SetWidget(QWidget* pWgt){
		this->m_pWgt = pWgt;
	};

	QWidget* GetWidget(){
		return m_pWgt;
	};
private:	
	QWidget* m_pWgt;
	vector<CCtrlItem*> m_vecCtrlItems;
	QString m_strType;
	QString m_strText;
};



class XmlParser : public QObject
{
	Q_OBJECT

public:
	XmlParser(QObject *parent);
	~XmlParser();

	void Load();

	std::vector<CMenuCombo>& GetVecMenuCombo(){
		return m_vecMenuCombo;
	};
	CMenuCombo* GetMenuComboByText(const QString& strText);
private:
	std::vector<CMenuCombo> m_vecMenuCombo;
};

#endif // XMLPARSER_H
