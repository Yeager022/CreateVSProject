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
	//初始化
	void initial();

	//加载样式表
	bool loadStyleSheet(const QString& strPath);

	//拷贝文件夹
	bool copyRecursively(const QString& sourceFolder, const QString& destFolder);

	//遍历修改文件
	void recursiveUpdate(QString path, QString oldName, QString newName);

	//遍历文件内容进行修改
	void replaceInFile(const QString& fileName, const QString& oldStr, const QString& newStr);

	//生成工程
	bool createProject(QString& strInfo);

	//获取文件后缀类型
	eSuffix getSuffixType(const QString& strSuffix);

	//文件后缀是否匹配
	bool checkFileSuffix(const QString& strSuffix);

	//读取默认路径
	void readDefaultPath();

private slots:
	//生成按钮按下
	void CreatebuttonClicked();

	//关闭软件
	void closeEvent(QCloseEvent* pEvent);

private:
	Ui::CreateVSProjectClass ui;

	//用于保存路径
	QString m_strPath;
};
