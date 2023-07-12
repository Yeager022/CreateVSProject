#pragma once

#include <QObject>
#include <QMutex>

class InstanceDemo : public QObject
{
	Q_OBJECT

public:
	static InstanceDemo* getInstance();

private:
	InstanceDemo(QObject* parent = nullptr);
	~InstanceDemo();
	InstanceDemo(const InstanceDemo&) = delete;
	InstanceDemo& operator=(const InstanceDemo&) = delete;

	static InstanceDemo* m_pPtr;
	static QMutex m_mutex;
};
