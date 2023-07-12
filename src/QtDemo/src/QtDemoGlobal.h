#pragma once

#include <QPoint>
#include <QTimer>
#include <QRect>

//无边框窗口信息
struct stFrameless
{
	QPoint dragPos;//鼠标点击的坐标,用于无边框窗口拖动
	QTimer timer;//双击后半秒内不能移动
	QRect rect;//用于记录窗口位置
	bool bFullScreen;//是否全屏

	stFrameless()
	{
		bFullScreen = false;
	}
};