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

	//初始化
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
	//设置无边框窗口
	setWindowFlags(Qt::FramelessWindowHint);
	//加载样式表
	if (!loadStyleSheet(":style/StyleSheet.qss"))
	{
		//QMessageBox::information(this, QString(u8"提示"), QString(u8"样式文件加载失败!"));
	}
	//为图标设置图片
	ui.pushButton_min->setIcon(QIcon(":/icon/min.png"));
	ui.pushButton_max->setIcon(QIcon(":/icon/max_2.png"));
	ui.pushButton_close->setIcon(QIcon(":/icon/close.png"));
	//用于避免双击时窗口移动
	connect(&m_frameless.timer, &QTimer::timeout, this, [=] {
		m_frameless.timer.stop();
		});

	//初始化动画效果指针
	QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	m_pAnimation = new QPropertyAnimation(this, "geometry");
	m_pAnimation->setDuration(300);// 动画持续时间
	m_pAnimation->setEasingCurve(QEasingCurve::InOutCubic);//动画效果
	connect(m_pAnimation, &QPropertyAnimation::finished, this, &QtDemo::fullScreenSwitched);//动画效果完成后处理
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
		// 记录鼠标相对于窗口的位置
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
		// 移动窗口
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
		m_pAnimation->setStartValue(geometry());//窗口当前的位置和大小
		m_pAnimation->setEndValue(m_frameless.rect);//最大化状态还原到普通状态时窗口应该有的位置和大小
		m_pAnimation->start();
	}
	else
	{
		m_frameless.rect = geometry();
		m_pAnimation->setStartValue(m_frameless.rect);//窗口当前的位置
		m_pAnimation->setEndValue(QApplication::desktop()->availableGeometry());//窗口最大化后的位置
		m_pAnimation->start();
	}
}

void QtDemo::on_pushButton_close_clicked()
{
	QMessageBox::StandardButton button = QMessageBox::question(this, QString(u8"关闭"), QString(u8"确认关闭软件?"));
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