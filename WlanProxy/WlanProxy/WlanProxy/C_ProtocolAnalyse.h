#pragma once

class C_ProtocolAnalyse{
public:
	C_ProtocolAnalyse(void);
	~C_ProtocolAnalyse(void);

public:
	void HandleNextPacket();

private:
	FILE* m_pFile;
	FILE* m_pFilter;
};