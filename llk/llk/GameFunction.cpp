#include "stdafx.h"
#include "GameFunction.h"
#include "ChessPoint.h"

BYTE ChessData[11][19];

void StartGame()
{
	HWND hGameHwnd;//存放窗口句柄
	RECT rGameRect;//存放游戏窗口大小
	POINT lastPoint;//存放鼠标原始位置
	int x,y;

	hGameHwnd = ::FindWindow(NULL,_T("QQ游戏 - 连连看角色版"));//获取窗口句柄
	::GetWindowRect(hGameHwnd,&rGameRect);//获取窗口当前坐标
	x = rGameRect.left;
	y = rGameRect.top;

	GetCursorPos(&lastPoint);//保存鼠标原始位置

	SetCursorPos(651+rGameRect.left,569+rGameRect.top);//设置鼠标到开始按钮处，窗口左上角的坐标加上左上角到开始游戏按钮的距离

	mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);//模拟鼠标点击
	mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);

	SetCursorPos(lastPoint.x,lastPoint.y);//还原鼠标谓之
	
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

	hWindowHwnd = FindWindow(NULL,_T("QQ游戏 - 连连看角色版"));//获取窗口句柄
	GetWindowThreadProcessId(hWindowHwnd,&ProcessID);//通过窗口句柄获取进程ID
	processHandle = OpenProcess(PROCESS_ALL_ACCESS,false,ProcessID);//通过进程ID打开进程
	ReadProcessMemory(processHandle,pBaseAddress,(LPVOID)&nOutSeatNum,4,&byRead);//读取进程数据

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

	hWindowHwnd = FindWindow(NULL,_T("QQ游戏 - 连连看角色版"));//获取窗口句柄
	GetWindowThreadProcessId(hWindowHwnd,&ProcessID);//通过窗口句柄获取进程ID
	processHandle = OpenProcess(PROCESS_ALL_ACCESS,false,ProcessID);//通过进程ID打开进程
	ReadProcessMemory(processHandle,pBaseAddress,(LPVOID)ChessData,11*19,&byRead);//读取进程数据

	CString szChessData;
	char tmp[3];//由于内存数据是byte类型，所以最多会有2个字符，再加一个结束符
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
	int nOriginX = 23;//棋盘原始坐标
	int nOriginY = 194;
	HWND hWindowHwnd;
	LPARAM lparam;

	int highParam= nOriginY+35*nRow;//每个棋子高是42，用行数*高就是需要点击棋子的Y坐标
	int lowParam = nOriginX+32*nColumn;//每个棋子的宽是43，用列数*宽就是需要点击棋子的X坐标

	lparam = (highParam<<16) + lowParam;

	hWindowHwnd = FindWindow(NULL, _T("QQ游戏 - 连连看角色版"));
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