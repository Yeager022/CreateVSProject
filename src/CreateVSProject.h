#pragma once

#include <QWidget>
#include "ui_CreateVSProject.h"

enum eSuffix
{
	eSuffix_unknown,
	eSuffix_h,
	eSuffix_cpp,
	eSuffix_sln,
	eSuffix_ui,
	eSuffix_vcxproj,
	eSuffix_filters,
	eSuffix_user
};

class CreateVSProject : public QWidget
{
	Q_OBJECT

public:
	CreateVSProject(QWidget* parent = nullptr);
	~CreateVSProject();

private:
	//��ʼ��
	void initial();

	//������ʽ��
	bool loadStyleSheet(const QString& strPath);

	//�����ļ���
	bool copyRecursively(const QString& sourceFolder, const QString& destFolder);

	//�����޸��ļ�
	void recursiveUpdate(QString path, QString oldName, QString newName);

	//�����ļ����ݽ����޸�
	void replaceInFile(const QString& fileName, const QString& oldStr, const QString& newStr);

	//���ɹ���
	bool createProject(QString& strInfo);

	//��ȡ�ļ���׺����
	eSuffix getSuffixType(const QString& strSuffix);

	//�ļ���׺�Ƿ�ƥ��
	bool checkFileSuffix(const QString& strSuffix);

	//��ȡĬ��·��
	void readDefaultPath();

private slots:
	//���ɰ�ť����
	void CreatebuttonClicked();

	//�ر����
	void closeEvent(QCloseEvent* pEvent);

private:
	Ui::CreateVSProjectClass ui;

	//���ڱ���·��
	QString m_strPath;
};
