#include "QtDemo.h"

#include <QMessageBox>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QDesktopWidget>

QtDemo::QtDemo(QWidget* parent)
	: QWidget(parent)
	, m_pAnimation(nullptr)
{
	ui.setupUi(this);

	//��ʼ��
	initial();
}

QtDemo::~QtDemo()
{
	if (!m_pAnimation)
	{
		delete m_pAnimation;
		m_pAnimation = nullptr;
	}
}

void QtDemo::initial()
{
	//�����ޱ߿򴰿�
	setWindowFlags(Qt::FramelessWindowHint);
	//������ʽ��
	if (!loadStyleSheet(":style/StyleSheet.qss"))
	{
		//QMessageBox::information(this, QString(u8"��ʾ"), QString(u8"��ʽ�ļ�����ʧ��!"));
	}
	//Ϊͼ������ͼƬ
	ui.pushButton_min->setIcon(QIcon(":/icon/min.png"));
	ui.pushButton_max->setIcon(QIcon(":/icon/max_2.png"));
	ui.pushButton_close->setIcon(QIcon(":/icon/close.png"));
	//���ڱ���˫��ʱ�����ƶ�
	connect(&m_frameless.timer, &QTimer::timeout, this, [=] {
		m_frameless.timer.stop();
		});

	//��ʼ������Ч��ָ��
	QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	m_pAnimation = new QPropertyAnimation(this, "geometry");
	m_pAnimation->setDuration(300);// ��������ʱ��
	m_pAnimation->setEasingCurve(QEasingCurve::InOutCubic);//����Ч��
	connect(m_pAnimation, &QPropertyAnimation::finished, this, &QtDemo::fullScreenSwitched);//����Ч����ɺ���
}

bool QtDemo::loadStyleSheet(const QString& strPath)
{
	QFile file(strPath);
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}
	setStyleSheet(file.readAll());
	return true;
}

void QtDemo::mousePressEvent(QMouseEvent* pEvent)
{
	if (pEvent->button() == Qt::LeftButton && pEvent->pos().y() <= ui.Layout_Title->geometry().bottomRight().y())
	{
		// ��¼�������ڴ��ڵ�λ��
		m_frameless.dragPos = pEvent->globalPos() - frameGeometry().topLeft();
		pEvent->accept();
	}
}

void QtDemo::mouseMoveEvent(QMouseEvent* pEvent)
{
	if (pEvent->buttons() & Qt::LeftButton && pEvent->pos().y() <= ui.Layout_Title->geometry().bottomRight().y())
	{
		if (m_frameless.timer.isActive())
		{
			return;
		}
		// �ƶ�����
		move(pEvent->globalPos() - m_frameless.dragPos);
		pEvent->accept();
	}
}

void QtDemo::mouseDoubleClickEvent(QMouseEvent* pEvent)
{
	if (pEvent->button() == Qt::LeftButton && pEvent->pos().y() <= ui.Layout_Title->geometry().bottomRight().y())
	{
		m_frameless.timer.start(500);
		on_pushButton_max_clicked();
		pEvent->accept();
	}
}

void QtDemo::on_pushButton_min_clicked()
{
	showMinimized();
}

void QtDemo::on_pushButton_max_clicked()
{
	if (isMaximized())
	{
		m_pAnimation->setStartValue(geometry());//���ڵ�ǰ��λ�úʹ�С
		m_pAnimation->setEndValue(m_frameless.rect);//���״̬��ԭ����ͨ״̬ʱ����Ӧ���е�λ�úʹ�С
		m_pAnimation->start();
	}
	else
	{
		m_frameless.rect = geometry();
		m_pAnimation->setStartValue(m_frameless.rect);//���ڵ�ǰ��λ��
		m_pAnimation->setEndValue(QApplication::desktop()->availableGeometry());//������󻯺��λ��
		m_pAnimation->start();
	}
}

void QtDemo::on_pushButton_close_clicked()
{
	QMessageBox::StandardButton button = QMessageBox::question(this, QString(u8"�ر�"), QString(u8"ȷ�Ϲر����?"));
	if (button == QMessageBox::Yes)
	{
		close();
	}
	else if (button == QMessageBox::No)
	{
		return;
	}
}

void QtDemo::fullScreenSwitched()
{
	if (!m_frameless.bFullScreen)
	{
		setWindowState(Qt::WindowMaximized);
		ui.pushButton_max->setIcon(QIcon(":/icon/max_1.png"));
		m_frameless.bFullScreen = true;
	}
	else
	{
		ui.pushButton_max->setIcon(QIcon(":/icon/max_2.png"));
		m_frameless.bFullScreen = false;
	}
}