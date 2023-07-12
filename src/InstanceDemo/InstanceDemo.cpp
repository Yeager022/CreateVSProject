#include "InstanceDemo.h"

InstanceDemo* InstanceDemo::m_pPtr = nullptr;
QMutex InstanceDemo::m_mutex;

InstanceDemo* InstanceDemo::getInstance()
{
	if (!m_pPtr)
	{
		m_mutex.lock();
		if (!m_pPtr)
		{
			m_pPtr = new InstanceDemo();
		}
		m_mutex.unlock();
	}
}

InstanceDemo::InstanceDemo(QObject* parent)
	: QObject(parent)
{}

InstanceDemo::~InstanceDemo()
{}