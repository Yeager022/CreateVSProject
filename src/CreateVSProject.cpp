#include "CreateVSProject.h"
#include <QMessageBox>
#include <QIcon>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QAction>
#include <QFileDialog>
#include <QDesktopServices>
#include <QDebug>

CreateVSProject::CreateVSProject(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	initial();
}

CreateVSProject::~CreateVSProject()
{}

//初始化
void CreateVSProject::initial()
{
	setWindowTitle(QString(u8"VS2022工程生成工具"));
	QIcon tiTleIcon(":/Icons/TitleIcon.png");
	setWindowIcon(tiTleIcon);

	//加载样式表
	bool bRet = loadStyleSheet(":/Style/StyleSheet.qss");

	//工程名称
	ui.lineEdit_ProjectName->setText(u8"QtApplication");
	//默认路径
	readDefaultPath();

	//基类
	ui.comboBox_Base->addItem(QString(u8"QWidget"));
	ui.comboBox_Base->addItem(QString(u8"QMainWindow"));
	ui.comboBox_Base->addItem(QString(u8"QDialog"));

	//路径选择
	QIcon icon(":/Icons/SelectPath.png");
	QAction* pAction = new QAction;
	pAction->setIcon(icon);
	ui.lineEdit_Path->setReadOnly(true);
	ui.lineEdit_Path->addAction(pAction, QLineEdit::TrailingPosition);

	connect(ui.pushButton_Create, &QPushButton::clicked, this, &CreateVSProject::CreatebuttonClicked);
	connect(pAction, &QAction::triggered, this, [=] {
		QString strPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		if (!strPath.isEmpty())
		{
			m_strPath = strPath;
			ui.lineEdit_Path->setText(m_strPath);
		}
		});
	connect(ui.radioButton_Single, &QRadioButton::clicked, this, &CreateVSProject::SingleClicked);
	connect(ui.radioButton_Common, &QRadioButton::clicked, this, &CreateVSProject::CommonProjectClicked);
}

//加载样式表
bool CreateVSProject::loadStyleSheet(const QString& strPath)
{
	QFile file(strPath);
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}
	qApp->setStyleSheet(file.readAll());
	return true;
}

//拷贝文件夹
bool CreateVSProject::copyRecursively(const QString& sourceFolder, const QString& destFolder)
{
	// 创建目标文件夹
	QDir destDir(destFolder);
	if (destDir.exists())
	{
		qDebug() << "Destination folder already exists!";
		return false;
	}

	if (!destDir.mkpath("."))
	{
		qDebug() << "Failed to create destination directory!";
		return false;
	}

	// 获取源文件夹中的所有项目并进行遍历
	QDir sourceDir(sourceFolder);
	foreach(QString itemName, sourceDir.entryList(QDir::NoDotAndDotDot | QDir::AllEntries))
	{
		QFileInfo fileInfo(sourceDir.absoluteFilePath(itemName));
		QString destinationPath = destDir.absoluteFilePath(itemName);

		// 如果项是一个文件，复制它
		if (fileInfo.isFile())
		{
			// 如果复制失败则打印错误信息并返回 false
			if (!QFile::copy(fileInfo.absoluteFilePath(), destinationPath))
			{
				qDebug() << "Failed to copy file" << fileInfo.absoluteFilePath() << "to" << destinationPath;
				return false;
			}
		}
		// 如果项是一个文件夹，递归地调用 copyRecursively 函数
		else if (fileInfo.isDir())
		{
			if (!copyRecursively(fileInfo.absoluteFilePath(), destinationPath))
			{
				return false;
			}
		}
	}

	return true;
}

//遍历修改文件
void CreateVSProject::recursiveUpdate(QString path, QString oldName, QString newName)
{
	QDir dir(path);
	QStringList fileList = dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
	foreach(QString fileName, fileList) {
		QString filePath = path + "/" + fileName;
		QFileInfo fileInfo(filePath);
		if (fileInfo.isFile()) {
			if (fileInfo.fileName().contains(oldName)) {
				QString newFilePath = QString("%1/%2").arg(fileInfo.path()).arg(fileInfo.fileName().replace(oldName, newName));
				QFile::rename(filePath, newFilePath);
				filePath = newFilePath;
			}

			if (checkFileSuffix(fileInfo.suffix()))
			{
				//替换文件中的字段
				replaceInFile(filePath, oldName, newName);
			}
		}
		else {
			recursiveUpdate(filePath, oldName, newName); // 递归遍历子目录
		}
	}
}

//遍历文件内容进行修改
void CreateVSProject::replaceInFile(const QString& fileName, const QString& oldStr, const QString& newStr)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "Cannot open file for reading: " << fileName;
		return;
	}

	QTextStream in(&file);
	QString content = in.readAll();
	file.close();

	content.replace(oldStr, newStr);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "Cannot open file for writing: " << fileName;
		return;
	}

	QTextStream out(&file);
	out << content;
	file.close();
}

//生成工程
bool CreateVSProject::createProject(QString& strInfo)
{
	//目标路径
	QString strTargetPath = QString("%1/%2").arg(ui.lineEdit_Path->text()).arg(ui.lineEdit_ProjectName->text());

	QDir dir;
	if (dir.exists(strTargetPath))
	{
		strInfo = QString(u8"该目录下已存在名为%1的文件夹！").arg(ui.lineEdit_ProjectName->text());
		return false;
	}

	if (ui.radioButton_Common->isChecked())
	{
		//先将本地的QtDemo拷贝到指定路径
		if (!copyRecursively(QDir("./QtDemo").absolutePath(), strTargetPath))
		{
			strInfo = QString(u8"拷贝文件到指定路径失败!");
			return false;
		}

		//遍历文件夹所有文件修改基类
		if (ui.comboBox_Base->currentText() != "QWidget")
		{
			recursiveUpdate(strTargetPath, "QWidget", ui.comboBox_Base->currentText());
		}

		//遍历文件夹中的所有文件
		recursiveUpdate(strTargetPath, "QtDemo", ui.lineEdit_ProjectName->text());
	}
	else
	{
		//先将本地的QtDemo拷贝到指定路径
		if (!copyRecursively(QDir("./InstanceDemo").absolutePath(), strTargetPath))
		{
			strInfo = QString(u8"拷贝文件到指定路径失败!");
			return false;
		}

		//遍历文件夹中的所有文件
		recursiveUpdate(strTargetPath, "InstanceDemo", ui.lineEdit_ProjectName->text());
	}

	return true;
}

//获取文件后缀类型
eSuffix CreateVSProject::getSuffixType(const QString& strSuffix)
{
	if (strSuffix == "h")
	{
		return eSuffix_h;
	}
	else if (strSuffix == "cpp")
	{
		return eSuffix_cpp;
	}
	else if (strSuffix == "sln")
	{
		return eSuffix_sln;
	}
	else if (strSuffix == "ui")
	{
		return eSuffix_ui;
	}
	else if (strSuffix == "vcxproj")
	{
		return eSuffix_vcxproj;
	}
	else if (strSuffix == "filters")
	{
		return eSuffix_filters;
	}
	else if (strSuffix == "user")
	{
		return eSuffix_user;
	}
	return eSuffix_unknown;
}

//文件后缀是否匹配
bool CreateVSProject::checkFileSuffix(const QString& strSuffix)
{
	if (getSuffixType(strSuffix) == eSuffix_unknown)
	{
		return false;
	}
	return true;
}

//读取默认路径
void CreateVSProject::readDefaultPath()
{
	QFile file("./config.log");
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	m_strPath = file.readAll();
	ui.lineEdit_Path->setText(m_strPath);
}

//关闭软件
void CreateVSProject::closeEvent(QCloseEvent* pEvent)
{
	QFile file("./config.log");
	if (!file.open(QIODevice::ReadWrite))
	{
		QMessageBox::information(this, QString(u8"提示"), QString(u8"保存默认路径失败!"));
		return;
	}

	file.write(m_strPath.toLocal8Bit());
	file.close();
	return;
}

//单例模式按钮
void CreateVSProject::SingleClicked(bool bChecked)
{
	if (bChecked)
	{
		ui.label_ProjectName->setText(u8"单例名称:");
		ui.label_Base->setVisible(!bChecked);
		ui.comboBox_Base->setVisible(!bChecked);
	}
}

//通用Qt工程按钮
void CreateVSProject::CommonProjectClicked(bool bChecked)
{
	if (bChecked)
	{
		ui.label_ProjectName->setText(u8"工程名称:");
		ui.label_Base->setVisible(bChecked);
		ui.comboBox_Base->setVisible(bChecked);
	}
}

//生成按钮按下
void CreateVSProject::CreatebuttonClicked()
{
	if (ui.lineEdit_ProjectName->text().isEmpty())
	{
		QMessageBox::information(this, QString(u8"提示"), QString(u8"请输入工程名称!"));
		return;
	}

	if (ui.lineEdit_Path->text().isEmpty())
	{
		QMessageBox::information(this, QString(u8"提示"), QString(u8"请输入工程路径!"));
		return;
	}

	//生成工程是否成功的信息
	QString strInfo;
	//生成工程
	if (!createProject(strInfo))
	{
		QMessageBox::warning(this, QString(u8"提示"), QString(u8"%1").arg(strInfo));
		return;
	}

	QMessageBox::information(this, QString(u8"提示"), QString(u8"%1工程生成成功!").arg(strInfo));

	QString strPath = QString("%1/%2").arg(ui.lineEdit_Path->text()).arg(ui.lineEdit_ProjectName->text());
	QUrl folderUrl = QUrl::fromLocalFile(strPath);
	QDesktopServices::openUrl(folderUrl);
}