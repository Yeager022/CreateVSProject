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

//��ʼ��
void CreateVSProject::initial()
{
	setWindowTitle(QString(u8"VS2022�������ɹ���"));
	QIcon tiTleIcon(":/Icons/TitleIcon.png");
	setWindowIcon(tiTleIcon);

	//������ʽ��
	bool bRet = loadStyleSheet(":/Style/StyleSheet.qss");

	//��������
	ui.lineEdit_ProjectName->setText(u8"QtApplication");
	//Ĭ��·��
	readDefaultPath();

	//����
	ui.comboBox_Base->addItem(QString(u8"QWidget"));
	ui.comboBox_Base->addItem(QString(u8"QMainWindow"));
	ui.comboBox_Base->addItem(QString(u8"QDialog"));

	//·��ѡ��
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

//������ʽ��
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

//�����ļ���
bool CreateVSProject::copyRecursively(const QString& sourceFolder, const QString& destFolder)
{
	// ����Ŀ���ļ���
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

	// ��ȡԴ�ļ����е�������Ŀ�����б���
	QDir sourceDir(sourceFolder);
	foreach(QString itemName, sourceDir.entryList(QDir::NoDotAndDotDot | QDir::AllEntries))
	{
		QFileInfo fileInfo(sourceDir.absoluteFilePath(itemName));
		QString destinationPath = destDir.absoluteFilePath(itemName);

		// �������һ���ļ���������
		if (fileInfo.isFile())
		{
			// �������ʧ�����ӡ������Ϣ������ false
			if (!QFile::copy(fileInfo.absoluteFilePath(), destinationPath))
			{
				qDebug() << "Failed to copy file" << fileInfo.absoluteFilePath() << "to" << destinationPath;
				return false;
			}
		}
		// �������һ���ļ��У��ݹ�ص��� copyRecursively ����
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

//�����޸��ļ�
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
				//�滻�ļ��е��ֶ�
				replaceInFile(filePath, oldName, newName);
			}
		}
		else {
			recursiveUpdate(filePath, oldName, newName); // �ݹ������Ŀ¼
		}
	}
}

//�����ļ����ݽ����޸�
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

//���ɹ���
bool CreateVSProject::createProject(QString& strInfo)
{
	//Ŀ��·��
	QString strTargetPath = QString("%1/%2").arg(ui.lineEdit_Path->text()).arg(ui.lineEdit_ProjectName->text());

	QDir dir;
	if (dir.exists(strTargetPath))
	{
		strInfo = QString(u8"��Ŀ¼���Ѵ�����Ϊ%1���ļ��У�").arg(ui.lineEdit_ProjectName->text());
		return false;
	}

	if (ui.radioButton_Common->isChecked())
	{
		//�Ƚ����ص�QtDemo������ָ��·��
		if (!copyRecursively(QDir("./QtDemo").absolutePath(), strTargetPath))
		{
			strInfo = QString(u8"�����ļ���ָ��·��ʧ��!");
			return false;
		}

		//�����ļ��������ļ��޸Ļ���
		if (ui.comboBox_Base->currentText() != "QWidget")
		{
			recursiveUpdate(strTargetPath, "QWidget", ui.comboBox_Base->currentText());
		}

		//�����ļ����е������ļ�
		recursiveUpdate(strTargetPath, "QtDemo", ui.lineEdit_ProjectName->text());
	}
	else
	{
		//�Ƚ����ص�QtDemo������ָ��·��
		if (!copyRecursively(QDir("./InstanceDemo").absolutePath(), strTargetPath))
		{
			strInfo = QString(u8"�����ļ���ָ��·��ʧ��!");
			return false;
		}

		//�����ļ����е������ļ�
		recursiveUpdate(strTargetPath, "InstanceDemo", ui.lineEdit_ProjectName->text());
	}

	return true;
}

//��ȡ�ļ���׺����
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

//�ļ���׺�Ƿ�ƥ��
bool CreateVSProject::checkFileSuffix(const QString& strSuffix)
{
	if (getSuffixType(strSuffix) == eSuffix_unknown)
	{
		return false;
	}
	return true;
}

//��ȡĬ��·��
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

//�ر����
void CreateVSProject::closeEvent(QCloseEvent* pEvent)
{
	QFile file("./config.log");
	if (!file.open(QIODevice::ReadWrite))
	{
		QMessageBox::information(this, QString(u8"��ʾ"), QString(u8"����Ĭ��·��ʧ��!"));
		return;
	}

	file.write(m_strPath.toLocal8Bit());
	file.close();
	return;
}

//����ģʽ��ť
void CreateVSProject::SingleClicked(bool bChecked)
{
	if (bChecked)
	{
		ui.label_ProjectName->setText(u8"��������:");
		ui.label_Base->setVisible(!bChecked);
		ui.comboBox_Base->setVisible(!bChecked);
	}
}

//ͨ��Qt���̰�ť
void CreateVSProject::CommonProjectClicked(bool bChecked)
{
	if (bChecked)
	{
		ui.label_ProjectName->setText(u8"��������:");
		ui.label_Base->setVisible(bChecked);
		ui.comboBox_Base->setVisible(bChecked);
	}
}

//���ɰ�ť����
void CreateVSProject::CreatebuttonClicked()
{
	if (ui.lineEdit_ProjectName->text().isEmpty())
	{
		QMessageBox::information(this, QString(u8"��ʾ"), QString(u8"�����빤������!"));
		return;
	}

	if (ui.lineEdit_Path->text().isEmpty())
	{
		QMessageBox::information(this, QString(u8"��ʾ"), QString(u8"�����빤��·��!"));
		return;
	}

	//���ɹ����Ƿ�ɹ�����Ϣ
	QString strInfo;
	//���ɹ���
	if (!createProject(strInfo))
	{
		QMessageBox::warning(this, QString(u8"��ʾ"), QString(u8"%1").arg(strInfo));
		return;
	}

	QMessageBox::information(this, QString(u8"��ʾ"), QString(u8"%1�������ɳɹ�!").arg(strInfo));

	QString strPath = QString("%1/%2").arg(ui.lineEdit_Path->text()).arg(ui.lineEdit_ProjectName->text());
	QUrl folderUrl = QUrl::fromLocalFile(strPath);
	QDesktopServices::openUrl(folderUrl);
}