#ifndef LINEWGT_H
#define LINEWGT_H

#include <QWidget>
#include "ui_linewgt.h"

class CMenuCombo;
typedef void(*pfunOnClickDelButton)(QWidget* pW);
class LineWgt : public QWidget
{
	Q_OBJECT

public:
	LineWgt(QWidget *parent = 0);
	~LineWgt();
	void SetCtrls(CMenuCombo* pCombo);
	void SetClickDelButtonCallback(pfunOnClickDelButton pFun);

public slots:
	void OnClickDelButton();
private:
	pfunOnClickDelButton pFunCallback;
	Ui::LineWgt ui;
	CMenuCombo* m_pCombo;
};

#endif // LINEWGT_H
