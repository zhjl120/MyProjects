#include "stdafx.h"
#include "C_DataQueue.h"

C_DataQueue* C_DataQueue::m_pInstance = NULL;
CMutex* C_DataQueue::m_pObjMutex = new CMutex;

C_DataQueue::C_DataQueue( void )
{
}

C_DataQueue::~C_DataQueue( void )
{
}

C_DataQueue* C_DataQueue::GetInstance()
{
	if(m_pInstance == NULL)
	{
		m_pObjMutex->Lock();

			if (m_pInstance == NULL)
				m_pInstance = new C_DataQueue;

		m_pObjMutex->Unlock();
	}

	return m_pInstance;
}

void C_DataQueue::DestoryInstance()
{
	if(m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}

	if(m_pObjMutex != NULL)
	{
		delete m_pObjMutex;
		m_pObjMutex = NULL;
	}
}

void C_DataQueue::AddData( PacketInfo Packet )
{
	m_pObjMutex->Lock();
	m_DataQueue.push(Packet);
	m_pObjMutex->Unlock();
}

PacketInfo C_DataQueue::PopData()
{
	PacketInfo packet;
	packet.pPacket = NULL;

	m_pObjMutex->Lock();
	if(m_DataQueue.size() > 0)
	{
		packet = m_DataQueue.front();
		m_DataQueue.pop();
	}
	m_pObjMutex->Unlock();
	
	return packet;
}
