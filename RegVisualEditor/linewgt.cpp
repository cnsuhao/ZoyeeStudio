#include "linewgt.h"
#include "xmlparser.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>

const QString strWgtQss = "*{border: 3px solid rgb(255, 0, 0);}";


LineWgt::LineWgt(QWidget *parent)
	: QWidget(parent), m_pCombo(nullptr), pFunCallback(nullptr)
{
	ui.setupUi(this);
	//setStyleSheet(strWgtQss);
}

LineWgt::~LineWgt()
{

}

void LineWgt::SetCtrls( CMenuCombo* pCombo )
{
	m_pCombo = pCombo;
	QHBoxLayout* pLayout = new QHBoxLayout;
	QWidget* pFather = this->ui.groupBox;
	this->ui.groupBox->setFixedHeight(50);
	vector<CCtrlItem*>& vec = pCombo->GetVecItems();
	int nSize = vec.size();
	for (int i = 0; i < nSize; i++){
		switch(vec[i]->GetType()){
		case EM_COMBO:
			break;
		case EM_EDIT:{
			QLineEdit* pEdit = new QLineEdit(pFather);
			if (vec[i]->GetLabel().isEmpty() == false){
				QLabel* pLabel = new QLabel(pFather);
				pLabel->setText(vec[i]->GetLabel());
				pLabel->setBuddy(pEdit);
				pLayout->addWidget(pLabel);
			}
			pLayout->addWidget(pEdit);
			vec[i]->SetObject(pEdit);
			 }
			break;
		case EM_LABEL:{
			QLabel* pLabel = new QLabel(pFather);
			pLabel->setText(vec[i]->GetLabel());
			pLayout->addWidget(pLabel);
			}			
			break;
		default:
			break;
		}
	}
	QPushButton* btn = new QPushButton(QStringLiteral("Çå³ý") , pFather);
	btn->setFixedHeight(25);
	btn->setFixedWidth(70);
	connect(btn, SIGNAL(clicked()), this, SLOT(OnClickDelButton()));
	pLayout->addWidget(btn);
	pFather->setLayout(pLayout);
	
}

void LineWgt::OnClickDelButton()
{
	if (pFunCallback){
		pFunCallback(this);
	}
	close();
}

void LineWgt::SetClickDelButtonCallback( pfunOnClickDelButton pFun )
{
	pFunCallback = pFun;
}
