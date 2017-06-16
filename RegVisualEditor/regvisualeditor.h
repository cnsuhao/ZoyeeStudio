#ifndef REGVISUALEDITOR_H
#define REGVISUALEDITOR_H


#include <QtWidgets/QWidget>
#include "ui_regvisualeditor.h"
#include <vector>
using namespace std;

class XmlParser;
class QFormLayout;
class QVBoxLayout;

class RegVisualEditor : public QWidget
{
	Q_OBJECT

public:

	RegVisualEditor(QWidget *parent = 0);
	~RegVisualEditor();

	static void OnRecvLineWgtDel(QWidget* w);
	static RegVisualEditor* GetInstance();
	void SetInstance(RegVisualEditor* pThis);
public slots:
	void OnSlotClickAdd();

private:
	XmlParser* xmlParser;
	QVBoxLayout* pLayout;	
	std::vector<QWidget*> m_vecLineWgt;
private:
	static RegVisualEditor* m_pInstance;
	Ui::RegVisualEditorClass ui;
};

#endif // REGVISUALEDITOR_H
