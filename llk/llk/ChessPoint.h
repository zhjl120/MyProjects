#pragma once


// CChessPoint

class CChessPoint : public CWnd
{
	DECLARE_DYNAMIC(CChessPoint)

public:
	CChessPoint(POINT p);
	virtual ~CChessPoint();

protected:
	DECLARE_MESSAGE_MAP()

public:
	POINT m_Point;
	POINT m_upPoint;
	POINT m_downPoint;
	POINT m_leftPoint;
	POINT m_rightPoint;
};


