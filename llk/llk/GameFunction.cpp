#include "stdafx.h"
#include "GameFunction.h"
#include "ChessPoint.h"

BYTE ChessData[11][19];

void StartGame()
{
	HWND hGameHwnd;//��Ŵ��ھ��
	RECT rGameRect;//�����Ϸ���ڴ�С
	POINT lastPoint;//������ԭʼλ��
	int x,y;

	hGameHwnd = ::FindWindow(NULL,_T("QQ��Ϸ - ��������ɫ��"));//��ȡ���ھ��
	::GetWindowRect(hGameHwnd,&rGameRect);//��ȡ���ڵ�ǰ����
	x = rGameRect.left;
	y = rGameRect.top;

	GetCursorPos(&lastPoint);//�������ԭʼλ��

	SetCursorPos(651+rGameRect.left,569+rGameRect.top);//������굽��ʼ��ť�����������Ͻǵ�����������Ͻǵ���ʼ��Ϸ��ť�ľ���

	mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);//ģ�������
	mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);

	SetCursorPos(lastPoint.x,lastPoint.y);//��ԭ���ν֮
	
//	CloseHandle(hGameHwnd);
}

int GetSeatNum()
{
	HWND hWindowHwnd;
	DWORD ProcessID;
	HANDLE processHandle;
	DWORD byRead;
	LPVOID pBaseAddress = (LPVOID)0x00171618;
	int nOutSeatNum;

	hWindowHwnd = FindWindow(NULL,_T("QQ��Ϸ - ��������ɫ��"));//��ȡ���ھ��
	GetWindowThreadProcessId(hWindowHwnd,&ProcessID);//ͨ�����ھ����ȡ����ID
	processHandle = OpenProcess(PROCESS_ALL_ACCESS,false,ProcessID);//ͨ������ID�򿪽���
	ReadProcessMemory(processHandle,pBaseAddress,(LPVOID)&nOutSeatNum,4,&byRead);//��ȡ��������

	return nOutSeatNum;
}

CString GetChessData()
{
	HWND hWindowHwnd;
	DWORD ProcessID;
	HANDLE processHandle;
	DWORD byRead;
	LPVOID pBaseAddress = (LPVOID)0x00189F78;
	BYTE ChessData[11][19];

	hWindowHwnd = FindWindow(NULL,_T("QQ��Ϸ - ��������ɫ��"));//��ȡ���ھ��
	GetWindowThreadProcessId(hWindowHwnd,&ProcessID);//ͨ�����ھ����ȡ����ID
	processHandle = OpenProcess(PROCESS_ALL_ACCESS,false,ProcessID);//ͨ������ID�򿪽���
	ReadProcessMemory(processHandle,pBaseAddress,(LPVOID)ChessData,11*19,&byRead);//��ȡ��������

	CString szChessData;
	char tmp[3];//�����ڴ�������byte���ͣ�����������2���ַ����ټ�һ��������
	for (int nXCount = 0; nXCount < 11; nXCount ++)
	{
		for(int nYCount = 0 ;nYCount < 19 ; nYCount ++)
		{
			itoa(ChessData[nXCount][nYCount] , tmp, 16);
			szChessData += tmp;
			szChessData += "\t";
		}
		szChessData += "\r\n";
	}

	return szChessData;
}

void ClickChessman(int nRow,int nColumn)
{
	int nOriginX = 23;//����ԭʼ����
	int nOriginY = 194;
	HWND hWindowHwnd;
	LPARAM lparam;

	int highParam= nOriginY+35*nRow;//ÿ�����Ӹ���42��������*�߾�����Ҫ������ӵ�Y����
	int lowParam = nOriginX+32*nColumn;//ÿ�����ӵĿ���43��������*�������Ҫ������ӵ�X����

	lparam = (highParam<<16) + lowParam;

	hWindowHwnd = FindWindow(NULL, _T("QQ��Ϸ - ��������ɫ��"));
	SendMessage(hWindowHwnd,WM_LBUTTONDOWN,0,lparam);
	SendMessage(hWindowHwnd,WM_LBUTTONUP,0,lparam);

}

bool CheckLine(POINT p1,POINT p2)
{
	int x,y;

	if (p1.x == p2.x)
	{
		for(y = p1.y ; y <= p2.y; y++)
		{
			if(ChessData[y][p1.x] > 0)
				return false;
		}
	}
	else if(p1.y == p2.y)
	{
		for(x = p1.x ; x <= p2.x; x++)
		{
			if(ChessData[p1.y][x] > 0)
				return false;
		}
	}

	return true;
}

bool Check2point(POINT p1,POINT p2)
{
	CChessPoint pa(p1) , pb(p2);
	CChessPoint pc(p1),  pb(p2);
	int x,y;
	POINT p3,p4;

	if(p1.y == p2.y)
	{
		if(CheckLine(pa.m_rightPoint ,pb.m_leftPoint))
		//	return true;
		p3 =p1;
		p4 =p2;
		for(y = 0; y<11;y++)
		{
			p3.y  = p4.y -y;
		}
	}

	if(p1.x == p2.x)
	{
		if(CheckLine(pa.m_downPoint ,pb.m_upPoint))
			return true;
	}

	return false;
}

void ClearPiar()
{
	GetChessData();

	POINT p1,p2;
	int x1, y1 , x2, y2;

	for (y1 =0 ; y1 < 11; y1++)
		for (x1 = 0; x1 < 19; x1++)
		{
			for(y2=y1 ; y2<11 ;y2++)
				for(x2=x1; x2<19; x2++)
					if (ChessData[y1][x1] == ChessData[y2][x2] 
					&& (!(x1 == x2)&&(y1 == y2)))
					{
						p1.x = x1;
						p1.y = y1;
						p2.x = x2;
						p2.y = y2;
						if (Check2point(p1,p2))
						{
							//	
						}
					}
		}
}