#pragma once

#include <QWidget>
#include <QTimer>
#include "ui_QtDemo.h"
#include "QtDemoGlobal.h"

class QPropertyAnimation;

class QtDemo : public QWidget
{
	Q_OBJECT

public:
	QtDemo(QWidget* parent = nullptr);
	~QtDemo();

private:
	/// <summary>
	/// ��ʼ��
	/// </summary>
	void initial();

	/// <summary>
	/// ������ʽ��
	/// </summary>
	/// <param name="strPath">qss��ʽ�ļ�·��</param>
	/// <returns>�Ƿ���سɹ�</returns>
	bool loadStyleSheet(const QString& strPath);

protected:
	/// <summary>
	/// ��갴��
	/// </summary>
	/// <param name="pEvent">����¼�</param>
	void mousePressEvent(QMouseEvent* pEvent) override;

	/// <summary>
	/// ����ƶ�
	/// </summary>
	/// <param name="pEvent">����¼�</param>
	void mouseMoveEvent(QMouseEvent* pEvent) override;

	/// <summary>
	/// ���˫��
	/// </summary>
	/// <param name="pEvent">����¼�</param>
	void mouseDoubleClickEvent(QMouseEvent* pEvent) override;

private slots:
	/// <summary>
	/// ��С��
	/// </summary>
	void on_pushButton_min_clicked();

	/// <summary>
	/// ���
	/// </summary>
	void on_pushButton_max_clicked();

	/// <summary>
	/// �رմ���
	/// </summary>
	void on_pushButton_close_clicked();

	/// <summary>
	/// ȫ���л�
	/// </summary>
	void fullScreenSwitched();

private:
	Ui::QtDemo ui;

	//�ޱ߿򴰿���Ϣ
	stFrameless m_frameless;
	//����ָ��
	QPropertyAnimation* m_pAnimation;
};
