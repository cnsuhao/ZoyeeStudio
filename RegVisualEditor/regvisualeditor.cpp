#include "regvisualeditor.h"

#include "xmlparser.h"
#include "linewgt.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <Windows.h>

RegVisualEditor::RegVisualEditor(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	xmlParser = new XmlParser(this);
	xmlParser->Load();
	std::vector<CMenuCombo> vecComboMenu = xmlParser->GetVecMenuCombo();
	size_t nSize = vecComboMenu.size();
	for (size_t i = 0; i < nSize; i++){
		this->ui.m_combo->addItem(vecComboMenu[i].GetText());		
	}
	connect(ui.m_btnAdd, SIGNAL(clicked()), this, SLOT(OnSlotClickAdd()));
	pLayout = new QVBoxLayout;
	pLayout->setSpacing(0);
	ui.ContentsLayer->setLayout(pLayout);

}

RegVisualEditor::~RegVisualEditor()
{

}

void RegVisualEditor::OnSlotClickAdd()
{	
	CMenuCombo* pMenuCombo = xmlParser->GetMenuComboByText(ui.m_combo->currentText());
	if (pMenuCombo == nullptr){
		return;
	}else{
		LineWgt* pLineWgt = new LineWgt(ui.ContentsLayer);
		pLineWgt->SetCtrls(pMenuCombo);
		pLayout->addWidget(pLineWgt);
		pLineWgt->show();
		pLineWgt->SetClickDelButtonCallback(OnRecvLineWgtDel);
		m_vecLineWgt.push_back(pLineWgt);
	}


}

void RegVisualEditor::OnRecvLineWgtDel( QWidget* w )
{	
	std::vector<QWidget*>::iterator iter = RegVisualEditor::GetInstance()->m_vecLineWgt.begin();
	for (; iter != RegVisualEditor::GetInstance()->m_vecLineWgt.end(); iter++){
		if (*iter == w){
			RegVisualEditor::GetInstance()->m_vecLineWgt.erase(iter);
			return;
		}
	}
}

RegVisualEditor* RegVisualEditor::GetInstance()
{
	return m_pInstance;
}

void RegVisualEditor::SetInstance( RegVisualEditor* pThis )
{
	m_pInstance = pThis;
}

RegVisualEditor* RegVisualEditor::m_pInstance = nullptr;
