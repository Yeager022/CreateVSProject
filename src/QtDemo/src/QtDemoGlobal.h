#pragma once

#include <QPoint>
#include <QTimer>
#include <QRect>

//�ޱ߿򴰿���Ϣ
struct stFrameless
{
	QPoint dragPos;//�����������,�����ޱ߿򴰿��϶�
	QTimer timer;//˫��������ڲ����ƶ�
	QRect rect;//���ڼ�¼����λ��
	bool bFullScreen;//�Ƿ�ȫ��

	stFrameless()
	{
		bFullScreen = false;
	}
};