#pragma once

/*
CRsNetCRC16含义是：翻译的瑞星信息中心实现的CRC16算法，原版算法为C#实现
					Rs表示瑞星
					Net表示信息中心
					CRC16算法
*/


class CRsNetCRC16
{
public:
	CRsNetCRC16(void);
	~CRsNetCRC16(void);

	unsigned short CalculateCrc16(CStringA buffer);
};
