// ChessPoint.cpp : 实现文件
//

#include "stdafx.h"
#include "llk.h"
#include "ChessPoint.h"


// CChessPoint

IMPLEMENT_DYNAMIC(CChessPoint, CWnd)

CChessPoint::CChessPoint(POINT p)
{
	m_Point = p;

	m_upPoint = p;
	m_upPoint.y -= 1;

	m_downPoint = p;
	m_downPoint.y += 1;

	m_leftPoint = p;
	m_leftPoint.x -= 1;

	m_rightPoint = p;
	m_rightPoint.x +=1;

}

CChessPoint::~CChessPoint()
{
}


BEGIN_MESSAGE_MAP(CChessPoint, CWnd)
END_MESSAGE_MAP()



// CChessPoint 消息处理程序


