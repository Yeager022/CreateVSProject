#pragma once

#include <QWidget>
#include <QTimer>
#include <QMutex>
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
	/// 初始化
	/// </summary>
	void initial();

	/// <summary>
	/// 加载样式表
	/// </summary>
	/// <param name="strPath">qss样式文件路径</param>
	/// <returns>是否加载成功</returns>
	bool loadStyleSheet(const QString& strPath);

protected:
	/// <summary>
	/// 鼠标按下
	/// </summary>
	/// <param name="pEvent">鼠标事件</param>
	void mousePressEvent(QMouseEvent* pEvent) override;

	/// <summary>
	/// 鼠标移动
	/// </summary>
	/// <param name="pEvent">鼠标事件</param>
	void mouseMoveEvent(QMouseEvent* pEvent) override;

	/// <summary>
	/// 鼠标双击
	/// </summary>
	/// <param name="pEvent">鼠标事件</param>
	void mouseDoubleClickEvent(QMouseEvent* pEvent) override;

private slots:
	/// <summary>
	/// 最小化
	/// </summary>
	void on_pushButton_min_clicked();

	/// <summary>
	/// 最大化
	/// </summary>
	void on_pushButton_max_clicked();

	/// <summary>
	/// 关闭窗口
	/// </summary>
	void on_pushButton_close_clicked();

	/// <summary>
	/// 全屏切换
	/// </summary>
	void fullScreenSwitched();

private:
	Ui::QtDemo ui;

	//无边框窗口信息
	stFrameless m_frameless;
	//动画指针
	QPropertyAnimation* m_pAnimation;
};
