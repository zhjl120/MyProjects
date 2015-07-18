#pragma once

class C_DBService
{
public:
	C_DBService(void);
	virtual ~C_DBService(void);

public:
	BOOL Connect();
	void Disconnect();
};

