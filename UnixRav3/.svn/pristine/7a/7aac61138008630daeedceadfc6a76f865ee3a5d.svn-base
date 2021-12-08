#include "StdAfx.h"
#include "RsNetRsa.h"

int primesBelow2000[] = 
{
	2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97,
	101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199,
	211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293,
	307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397,
	401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499,
	503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599,
	601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691,
	701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797,
	809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887,
	907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997,
	1009, 1013, 1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069, 1087, 1091, 1093, 1097,
	1103, 1109, 1117, 1123, 1129, 1151, 1153, 1163, 1171, 1181, 1187, 1193,
	1201, 1213, 1217, 1223, 1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291, 1297,
	1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373, 1381, 1399,
	1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451, 1453, 1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499,
	1511, 1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583, 1597,
	1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657, 1663, 1667, 1669, 1693, 1697, 1699,
	1709, 1721, 1723, 1733, 1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789,
	1801, 1811, 1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889,
	1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987, 1993, 1997, 1999 
};






CRsNetRsa::CRsNetRsa(void)
{
	memset(data, 0, sizeof(UINT)*maxLength);
	dataLength = 1;
}

CRsNetRsa::~CRsNetRsa(void)
{
}

CRsNetRsa::CRsNetRsa(CStringA value, int radix)
{
	memset(data, 0, sizeof(UINT)*maxLength);
	dataLength = 1;
	CRsNetRsa multiplier(1);
	CRsNetRsa result;

	value = value.MakeUpper().Trim();
	int limit = 0;
	if(value[0] == '-')
		limit = 1;
	
	for (int i = value.GetLength() - 1; i >= limit; i--)
	{
		int posVal = (int)value[i];

		if (posVal >= '0' && posVal <= '9')
			posVal -= '0';
		else if (posVal >= 'A' && posVal <= 'Z')
			posVal = (posVal - 'A') + 10;
		else
			posVal = 9999999;       // arbitrary large


		if (posVal >= radix)
			throw ( "Invalid string in constructor.");
		else
		{
			if (value[0] == '-')
				posVal = -posVal;

			result = result + (multiplier * posVal);

			if ((i - 1) >= limit)
				multiplier = multiplier * radix;
		}
	}

	if (value[0] == '-')     // negative values
	{
		if ((result.data[maxLength - 1] & 0x80000000) == 0)
			throw ("Negative underflow in constructor.");
	}
	else    // positive values
	{
		if ((result.data[maxLength - 1] & 0x80000000) != 0)
			throw ("Positive overflow in constructor.");
	}

	for (int i = 0; i < result.dataLength; i++)
		data[i] = result.data[i];

	dataLength = result.dataLength;
}

CRsNetRsa::CRsNetRsa(_long value)
{
	memset(data, 0, sizeof(UINT)*maxLength);
	dataLength = 1;

	_long tempVal = value;

	// copy bytes from long to BigInteger without any assumption of
	// the length of the long datatype

	dataLength = 0;
	while (tempVal != 0 && dataLength < maxLength)
	{
		// 32位下会出问题
		data[dataLength] = (UINT)(value & 0xFFFFFFFF);
		tempVal >>= 32;
		dataLength++;
	}

	if (tempVal > 0)         // overflow check for +ve value
	{
		if (value != 0 || (data[maxLength - 1] & 0x80000000) != 0)
			throw ("Positive overflow in constructor.");
	}
	else if (tempVal < 0)    // underflow check for -ve value
	{
		if (value != -1 || (data[dataLength - 1] & 0x80000000) == 0)
			throw ("Negative underflow in constructor.");
	}

	if (dataLength == 0)
		dataLength = 1;
}

CRsNetRsa CRsNetRsa::operator*(CRsNetRsa bi2)
{
	int lastPos = maxLength - 1;
	bool bi1Neg = false, bi2Neg = false;

	// take the absolute value of the inputs
	try
	{
		if ((data[lastPos] & 0x80000000) != 0)     // bi1 negative
		{
			bi1Neg = true; *this = -(*this);
		}
		if ((bi2.data[lastPos] & 0x80000000) != 0)     // bi2 negative
		{
			bi2Neg = true; bi2 = -bi2;
		}
	}
	catch (...) { }

	CRsNetRsa result ;

	// multiply the absolute values
	try
	{
		for (int i = 0; i < dataLength; i++)
		{
			if (data[i] == 0) continue;

			_ulong mcarry = 0;
			for (int j = 0, k = i; j < bi2.dataLength; j++, k++)
			{
				// k = i + j
				_ulong val = ((_ulong)data[i] * (_ulong)bi2.data[j]) +
					(_ulong)result.data[k] + mcarry;

				result.data[k] = (UINT)(val & 0xFFFFFFFF);
				mcarry = (val >> 32);
			}

			if (mcarry != 0)
				result.data[i + bi2.dataLength] = (UINT)mcarry;
		}
	}
	catch (...)
	{
		throw ("Multiplication overflow.");
	}


	result.dataLength = dataLength + bi2.dataLength;
	if (result.dataLength > maxLength)
		result.dataLength = maxLength;

	while (result.dataLength > 1 && result.data[result.dataLength - 1] == 0)
		result.dataLength--;

	// overflow check (result is -ve)
	if ((result.data[lastPos] & 0x80000000) != 0)
	{
		if (bi1Neg != bi2Neg && result.data[lastPos] == 0x80000000)    // different sign
		{
			// handle the special case where multiplication produces
			// a max negative number in 2's complement.

			if (result.dataLength == 1)
				return result;
			else
			{
				bool isMaxNeg = true;
				for (int i = 0; i < result.dataLength - 1 && isMaxNeg; i++)
				{
					if (result.data[i] != 0)
						isMaxNeg = false;
				}

				if (isMaxNeg)
					return result;
			}
		}

		throw ("Multiplication overflow.");
	}

	// if input has different signs, then result is -ve
	if (bi1Neg != bi2Neg)
		return -result;
	return result;
}

CRsNetRsa CRsNetRsa::operator+(CRsNetRsa bi2)
{
	CRsNetRsa result;

	result.dataLength = (dataLength > bi2.dataLength) ? dataLength : bi2.dataLength;

	_long carry = 0;
	for (int i = 0; i < result.dataLength; i++)
	{
		_long sum = (_long)data[i] + (_long)bi2.data[i] + carry;
		carry = sum >> 32;
		result.data[i] = (UINT)(sum & 0xFFFFFFFF);
	}

	if (carry != 0 && result.dataLength < maxLength)
	{
		result.data[result.dataLength] = (UINT)(carry);
		result.dataLength++;
	}

	while (result.dataLength > 1 && result.data[result.dataLength - 1] == 0)
		result.dataLength--;


	// overflow check
	int lastPos = maxLength - 1;
	if ((data[lastPos] & 0x80000000) == (bi2.data[lastPos] & 0x80000000) &&
		(result.data[lastPos] & 0x80000000) != (data[lastPos] & 0x80000000))
	{
		throw ("error");
	}

	return result;
}

CRsNetRsa& CRsNetRsa::operator-()
{
	// handle neg of zero separately since it'll cause an overflow
	// if we proceed.

	if (dataLength == 1 && data[0] == 0)
		return *this;

	// 1's complement
	for (int i = 0; i < maxLength; i++)
		data[i] = ~(data[i]);

	// add one to result of 1's complement
	_long val, carry = 1;
	int index = 0;

	while (carry != 0 && index < maxLength)
	{
		val = (_long)(data[index]);
		val++;

		data[index] = (UINT)(val & 0xFFFFFFFF);
		carry = val >> 32;

		index++;
	}

	dataLength = maxLength;

	while (dataLength > 1 && data[dataLength - 1] == 0)
		dataLength--;
	return *this;
}


CRsNetRsa::CRsNetRsa(BYTE* inData, int inDataLen)
{
	dataLength = inDataLen >> 2;

	int leftOver = inDataLen & 0x3;
	if (leftOver != 0)         // length not multiples of 4
		dataLength++;


	if (dataLength > maxLength)
		throw ("Byte overflow in constructor.");

	memset(data, 0, sizeof(UINT)*maxLength);

	for (int i = inDataLen - 1, j = 0; i >= 3; i -= 4, j++)
	{
		data[j] = (UINT)((inData[i - 3] << 24) + (inData[i - 2] << 16) +
			(inData[i - 1] << 8) + inData[i]);
	}

	if (leftOver == 1)
		data[dataLength - 1] = (UINT)inData[0];
	else if (leftOver == 2)
		data[dataLength - 1] = (UINT)((inData[0] << 8) + inData[1]);
	else if (leftOver == 3)
		data[dataLength - 1] = (UINT)((inData[0] << 16) + (inData[1] << 8) + inData[2]);


	while (dataLength > 1 && data[dataLength - 1] == 0)
		dataLength--;
}

CRsNetRsa::CRsNetRsa(UINT* inData, int inDataLen)
{
	dataLength = inDataLen;

	if (dataLength > maxLength)
		throw ("Byte overflow in constructor.");

	memset(data, 0, maxLength*sizeof(UINT));

	for (int i = dataLength - 1, j = 0; i >= 0; i--, j++)
		data[j] = inData[i];

	while (dataLength > 1 && data[dataLength - 1] == 0)
		dataLength--;

	//Console.WriteLine("Len = " + dataLength);
}

CRsNetRsa CRsNetRsa::modPow(CRsNetRsa exp, CRsNetRsa n)
{
	if ((exp.data[maxLength - 1] & 0x80000000) != 0)
		throw ("Positive exponents only.");

	CRsNetRsa resultNum = 1;
	CRsNetRsa tempNum;
	bool thisNegative = false;

	if ((data[maxLength - 1] & 0x80000000) != 0)   // negative this
	{
		tempNum = -(*this) % n;
		thisNegative = true;
	}
	else
		tempNum = (*this) % n;  // ensures (tempNum * tempNum) < b^(2k)

	if ((n.data[maxLength - 1] & 0x80000000) != 0)   // negative n
		n = -n;

	// calculate constant = b^(2k) / m
	CRsNetRsa constant ;

	int i = n.dataLength << 1;
	constant.data[i] = 0x00000001;
	constant.dataLength = i + 1;

	constant = constant / n;
	int totalBits = exp.bitCount();
	int count = 0;

	// perform squaring and multiply exponentiation
	for (int pos = 0; pos < exp.dataLength; pos++)
	{
		UINT mask = 0x01;
		//Console.WriteLine("pos = " + pos);

		for (int index = 0; index < 32; index++)
		{
			if ((exp.data[pos] & mask) != 0)
				resultNum = BarrettReduction(resultNum * tempNum, n, constant);

			mask <<= 1;

			tempNum = BarrettReduction(tempNum * tempNum, n, constant);


			if (tempNum.dataLength == 1 && tempNum.data[0] == 1)
			{
				if (thisNegative && (exp.data[0] & 0x1) != 0)    //odd exp
					return -resultNum;
				return resultNum;
			}
			count++;
			if (count == totalBits)
				break;
		}
	}

	if (thisNegative && (exp.data[0] & 0x1) != 0)    //odd exp
		return -resultNum;

	return resultNum;
}

CRsNetRsa CRsNetRsa::operator%(CRsNetRsa bi2)
{
	CRsNetRsa quotient;//= new BigInteger();
	CRsNetRsa remainder(*this);// = new BigInteger(bi1);

	int lastPos = maxLength - 1;
	bool dividendNeg = false;

	if ((data[lastPos] & 0x80000000) != 0)     // bi1 negative
	{
		*this = -(*this);
		dividendNeg = true;
	}
	if ((bi2.data[lastPos] & 0x80000000) != 0)     // bi2 negative
		bi2 = -bi2;

	if ((*this) < bi2)
	{
		return remainder;
	}

	else
	{
		if (bi2.dataLength == 1)
			singleByteDivide(*this, bi2, quotient, remainder);
		else
			multiByteDivide(*this, bi2, quotient, remainder);

		if (dividendNeg)
			return -remainder;

		return remainder;
	}
}

void CRsNetRsa::singleByteDivide(CRsNetRsa bi1, CRsNetRsa bi2,	CRsNetRsa& outQuotient, CRsNetRsa& outRemainder)
{
	UINT result[maxLength];
	memset(result, 0, sizeof(UINT)*maxLength);
	int resultPos = 0;

	// copy dividend to reminder
	for (int i = 0; i < maxLength; i++)
		outRemainder.data[i] = bi1.data[i];
	outRemainder.dataLength = bi1.dataLength;

	while (outRemainder.dataLength > 1 && outRemainder.data[outRemainder.dataLength - 1] == 0)
		outRemainder.dataLength--;

	_ulong divisor = (_ulong)bi2.data[0];
	int pos = outRemainder.dataLength - 1;
	_ulong dividend = (_ulong)outRemainder.data[pos];

	//Console.WriteLine("divisor = " + divisor + " dividend = " + dividend);
	//Console.WriteLine("divisor = " + bi2 + "\ndividend = " + bi1);

	if (dividend >= divisor)
	{
		_ulong quotient = divisor ? (dividend / divisor) : dividend;
		result[resultPos++] = (UINT)quotient;

		outRemainder.data[pos] = (UINT)(divisor ? (dividend % divisor) : dividend);
	}
	pos--;

	while (pos >= 0)
	{
		//Console.WriteLine(pos);

		dividend = ((_ulong)outRemainder.data[pos + 1] << 32) + (_ulong)outRemainder.data[pos];
		_ulong quotient = dividend / divisor;
		result[resultPos++] = (UINT)quotient;

		outRemainder.data[pos + 1] = 0;
		outRemainder.data[pos--] = (UINT)(dividend % divisor);
		//Console.WriteLine(">>>> " + bi1);
	}

	outQuotient.dataLength = resultPos;
	int j = 0;
	for (int i = outQuotient.dataLength - 1; i >= 0; i--, j++)
		outQuotient.data[j] = result[i];
	for (; j < maxLength; j++)
		outQuotient.data[j] = 0;

	while (outQuotient.dataLength > 1 && outQuotient.data[outQuotient.dataLength - 1] == 0)
		outQuotient.dataLength--;

	if (outQuotient.dataLength == 0)
		outQuotient.dataLength = 1;

	while (outRemainder.dataLength > 1 && outRemainder.data[outRemainder.dataLength - 1] == 0)
		outRemainder.dataLength--;
}

void CRsNetRsa::multiByteDivide(CRsNetRsa bi1, CRsNetRsa bi2,	CRsNetRsa& outQuotient, CRsNetRsa& outRemainder)
{
	UINT result[maxLength];
	memset(result, 0, sizeof(UINT)*maxLength);	

    int remainderLen = bi1.dataLength + 1;
    
	UINT* remainder	 = new UINT[remainderLen];
	memset(remainder, 0, sizeof(UINT)*remainderLen);

    UINT mask = 0x80000000;
    UINT val = bi2.data[bi2.dataLength - 1];
    int shift = 0, resultPos = 0;

    while (mask != 0 && (val & mask) == 0)
    {
        shift++; mask >>= 1;
    }

	for (int i = 0; i < bi1.dataLength; i++)
		remainder[i] = bi1.data[i];
	shiftLeft(remainder, remainderLen, shift);
	bi2 = bi2 << shift;

    int j = remainderLen - bi2.dataLength;
    int pos = remainderLen - 1;

    _ulong firstDivisorByte = bi2.data[bi2.dataLength - 1];
    _ulong secondDivisorByte = bi2.data[bi2.dataLength - 2];

    int divisorLen = bi2.dataLength + 1;
	UINT* dividendPart = new UINT[divisorLen];
	memset(dividendPart, 0, sizeof(UINT)*divisorLen);
   

        while (j > 0)
        {
            unsigned long long dividend = ((_ulong)remainder[pos] << 32) + (_ulong)remainder[pos - 1];
            //Console.WriteLine("dividend = {0}", dividend);

            unsigned long long q_hat = dividend / firstDivisorByte;
            unsigned long long r_hat = dividend % firstDivisorByte;

            //Console.WriteLine("q_hat = {0:X}, r_hat = {1:X}", q_hat, r_hat);

            bool done = false;
            while (!done)
            {
                done = true;

                if (q_hat == 0x100000000LL ||
                   (q_hat * secondDivisorByte) > ((r_hat << 32) + remainder[pos - 2]))
                {
                    q_hat--;
                    r_hat += firstDivisorByte;

                    if (r_hat < 0x100000000LL)
                        done = false;
                }
            }

            for (int h = 0; h < divisorLen; h++)
                dividendPart[h] = remainder[pos - h];

			CRsNetRsa kk(dividendPart, divisorLen);
			CRsNetRsa ss = bi2 * (_long)q_hat;
   
            //Console.WriteLine("ss before = " + ss);
            while (ss > kk)
            {
                q_hat--;
                ss = ss - bi2;
                //Console.WriteLine(ss);
            }
            CRsNetRsa yy = kk - ss;

            //Console.WriteLine("ss = " + ss);
            //Console.WriteLine("kk = " + kk);
            //Console.WriteLine("yy = " + yy);

            for (int h = 0; h < divisorLen; h++)
                remainder[pos - h] = yy.data[bi2.dataLength - h];

            /*
            Console.WriteLine("dividend = ");
            for(int q = remainderLen - 1; q >= 0; q--)
                    Console.Write("{0:x2}", remainder[q]);
            Console.WriteLine("\n************ q_hat = {0:X}\n", q_hat);
            */

            result[resultPos++] = (UINT)q_hat;

            pos--;
            j--;
        }

        outQuotient.dataLength = resultPos;
        int y = 0;
        for (int x = outQuotient.dataLength - 1; x >= 0; x--, y++)
            outQuotient.data[y] = result[x];
        for (; y < maxLength; y++)
            outQuotient.data[y] = 0;

        while (outQuotient.dataLength > 1 && outQuotient.data[outQuotient.dataLength - 1] == 0)
            outQuotient.dataLength--;

        if (outQuotient.dataLength == 0)
            outQuotient.dataLength = 1;

        outRemainder.dataLength = shiftRight(remainder, remainderLen, shift);

        for (y = 0; y < outRemainder.dataLength; y++)
            outRemainder.data[y] = remainder[y];
        for (; y < maxLength; y++)
            outRemainder.data[y] = 0;

	delete[] dividendPart;
	dividendPart = NULL;
	delete[] remainder;
	remainder = NULL;
}

int CRsNetRsa::shiftRight(UINT* buffer, int nBufferLen, int shiftVal)
{
	int shiftAmount = 32;
	int invShift = 0;
	int bufLen = nBufferLen;

	while (bufLen > 1 && buffer[bufLen - 1] == 0)
		bufLen--;

	//Console.WriteLine("bufLen = " + bufLen + " buffer.Length = " + buffer.Length);

	for (int count = shiftVal; count > 0; )
	{
		if (count < shiftAmount)
		{
			shiftAmount = count;
			invShift = 32 - shiftAmount;
		}

		//Console.WriteLine("shiftAmount = {0}", shiftAmount);

		_ulong carry = 0;
		for (int i = bufLen - 1; i >= 0; i--)
		{
			_ulong val = ((_ulong)buffer[i]) >> shiftAmount;
			val |= carry;

			carry = ((_ulong)buffer[i]) << invShift;
			buffer[i] = (UINT)(val);
		}

		count -= shiftAmount;
	}

	while (bufLen > 1 && buffer[bufLen - 1] == 0)
		bufLen--;

	return bufLen;
}

int CRsNetRsa::shiftLeft(UINT* buffer, int nBufferLen, int shiftVal)
{
	int shiftAmount = 32;
	int bufLen = nBufferLen;

	while (bufLen > 1 && buffer[bufLen - 1] == 0)
		bufLen--;

	for (int count = shiftVal; count > 0; )
	{
		if (count < shiftAmount)
			shiftAmount = count;

		//Console.WriteLine("shiftAmount = {0}", shiftAmount);

		_ulong carry = 0;
		for (int i = 0; i < bufLen; i++)
		{
			_ulong val = ((_ulong)buffer[i]) << shiftAmount;
			val |= carry;

			buffer[i] = (UINT)(val & 0xFFFFFFFF);
			carry = val >> 32;
		}

		if (carry != 0)
		{
			if (bufLen + 1 <= nBufferLen)
			{
				buffer[bufLen] = (UINT)carry;
				bufLen++;
			}
		}
		count -= shiftAmount;
	}
	return bufLen;
}


CRsNetRsa CRsNetRsa::operator/(CRsNetRsa bi2)
{
	CRsNetRsa quotient;// = new BigInteger();
	CRsNetRsa remainder;// = new BigInteger();

	int lastPos = maxLength - 1;
	bool divisorNeg = false, dividendNeg = false;

	if ((data[lastPos] & 0x80000000) != 0)     // bi1 negative
	{
		*this = -(*this);
		dividendNeg = true;
	}
	if ((bi2.data[lastPos] & 0x80000000) != 0)     // bi2 negative
	{
		bi2 = -bi2;
		divisorNeg = true;
	}

	if (*this < bi2)
	{
		return quotient;
	}

	else
	{
		if (bi2.dataLength == 1)
			singleByteDivide(*this, bi2, quotient, remainder);
		else
			multiByteDivide(*this, bi2, quotient, remainder);

		if (dividendNeg != divisorNeg)
			return -quotient;

		return quotient;
	}
}

int CRsNetRsa::bitCount()
{
	while (dataLength > 1 && data[dataLength - 1] == 0)
		dataLength--;

	UINT value = data[dataLength - 1];
	UINT mask = 0x80000000;
	int bits = 32;

	while (bits > 0 && (value & mask) == 0)
	{
		bits--;
		mask >>= 1;
	}
	bits += ((dataLength - 1) << 5);

	return bits;
}

CRsNetRsa CRsNetRsa::BarrettReduction(CRsNetRsa x, CRsNetRsa n, CRsNetRsa constant)
{
	int k = n.dataLength,
		kPlusOne = k + 1,
		kMinusOne = k - 1;

	CRsNetRsa q1;

	// q1 = x / b^(k-1)
	for (int i = kMinusOne, j = 0; i < x.dataLength; i++, j++)
		q1.data[j] = x.data[i];
	q1.dataLength = x.dataLength - kMinusOne;
	if (q1.dataLength <= 0)
		q1.dataLength = 1;


	CRsNetRsa q2 = q1 * constant;
	CRsNetRsa q3;

	// q3 = q2 / b^(k+1)
	for (int i = kPlusOne, j = 0; i < q2.dataLength; i++, j++)
		q3.data[j] = q2.data[i];
	q3.dataLength = q2.dataLength - kPlusOne;
	if (q3.dataLength <= 0)
		q3.dataLength = 1;


	// r1 = x mod b^(k+1)
	// i.e. keep the lowest (k+1) words
	CRsNetRsa r1 ;
	int lengthToCopy = (x.dataLength > kPlusOne) ? kPlusOne : x.dataLength;
	for (int i = 0; i < lengthToCopy; i++)
		r1.data[i] = x.data[i];
	r1.dataLength = lengthToCopy;


	// r2 = (q3 * n) mod b^(k+1)
	// partial multiplication of q3 and n

	CRsNetRsa r2;
	for (int i = 0; i < q3.dataLength; i++)
	{
		if (q3.data[i] == 0) continue;

		_ulong mcarry = 0;
		int t = i;
		for (int j = 0; j < n.dataLength && t < kPlusOne; j++, t++)
		{
			// t = i + j
			_ulong val = ((_ulong)q3.data[i] * (_ulong)n.data[j]) +
				(_ulong)r2.data[t] + mcarry;

			r2.data[t] = (UINT)(val & 0xFFFFFFFF);
			mcarry = (val >> 32);
		}

		if (t < kPlusOne)
			r2.data[t] = (UINT)mcarry;
	}
	r2.dataLength = kPlusOne;
	while (r2.dataLength > 1 && r2.data[r2.dataLength - 1] == 0)
		r2.dataLength--;

	r1 = r1 - r2;
	if ((r1.data[maxLength - 1] & 0x80000000) != 0)        // negative
	{
		CRsNetRsa val;
		val.data[kPlusOne] = 0x00000001;
		val.dataLength = kPlusOne + 1;
		r1 = r1 + val;
	}

	while (r1 >= n)
		r1 = r1 - n;

	return r1;
}

bool CRsNetRsa::operator <(CRsNetRsa bi2)
{
	int pos = maxLength - 1;

	// bi1 is negative, bi2 is positive
	if ((data[pos] & 0x80000000) != 0 && (bi2.data[pos] & 0x80000000) == 0)
		return true;

	// bi1 is positive, bi2 is negative
	else if ((data[pos] & 0x80000000) == 0 && (bi2.data[pos] & 0x80000000) != 0)
		return false;

	// same sign
	int len = (dataLength > bi2.dataLength) ? dataLength : bi2.dataLength;
	for (pos = len - 1; pos >= 0 && data[pos] == bi2.data[pos]; pos--) ;

	if (pos >= 0)
	{
		if (data[pos] < bi2.data[pos])
			return true;
		return false;
	}
	return false;
}

CRsNetRsa CRsNetRsa::operator <<(int shiftVal)
{
	CRsNetRsa result(*this);
	result.dataLength = shiftLeft(result.data, maxLength, shiftVal);

	return result;
}

bool CRsNetRsa::operator >(CRsNetRsa bi2)
{
	int pos = maxLength - 1;

	// bi1 is negative, bi2 is positive
	if ((data[pos] & 0x80000000) != 0 && (bi2.data[pos] & 0x80000000) == 0)
		return false;

	// bi1 is positive, bi2 is negative
	else if ((data[pos] & 0x80000000) == 0 && (bi2.data[pos] & 0x80000000) != 0)
		return true;

	// same sign
	int len = (dataLength > bi2.dataLength) ? dataLength : bi2.dataLength;
	for (pos = len - 1; pos >= 0 && data[pos] == bi2.data[pos]; pos--) ;

	if (pos >= 0)
	{
		if (data[pos] > bi2.data[pos])
			return true;
		return false;
	}
	return false;
}

CRsNetRsa CRsNetRsa::operator-(CRsNetRsa bi2)
{
	CRsNetRsa result;

	result.dataLength = (dataLength > bi2.dataLength) ? dataLength : bi2.dataLength;

	long carryIn = 0;
	for (int i = 0; i < result.dataLength; i++)
	{
		_long diff;

		diff = (_long)data[i] - (_long)bi2.data[i] - carryIn;
		result.data[i] = (UINT)(diff & 0xFFFFFFFF);

		if (diff < 0)
			carryIn = 1;
		else
			carryIn = 0;
	}

	// roll over to negative
	if (carryIn != 0)
	{
		for (int i = result.dataLength; i < maxLength; i++)
			result.data[i] = 0xFFFFFFFF;
		result.dataLength = maxLength;
	}

	// fixed in v1.03 to give correct datalength for a - (-b)
	while (result.dataLength > 1 && result.data[result.dataLength - 1] == 0)
		result.dataLength--;

	// overflow check
	int lastPos = maxLength - 1;
	if ((data[lastPos] & 0x80000000) != (bi2.data[lastPos] & 0x80000000) &&
		(result.data[lastPos] & 0x80000000) != (data[lastPos] & 0x80000000))
	{
		throw ("ERROR");
	}

	return result;
}

bool CRsNetRsa::operator>=(CRsNetRsa bi2)
{
	return (*this == bi2 || *this > bi2);
}

bool CRsNetRsa::operator ==(CRsNetRsa bi2)
{
	return this->Equals(bi2);
}

bool CRsNetRsa::Equals(CRsNetRsa o)
{
	CRsNetRsa& bi = o;

	if (dataLength != bi.dataLength)
		return false;

	for (int i = 0; i < dataLength; i++)
	{
		if (data[i] != bi.data[i])
			return false;
	}
	return true;
}

CStringA  CRsNetRsa::ToHexString()
{
	char temp[9] = {0};
	//char result[maxLength*8+1] = {0};
	sprintf(temp, "%X", data[dataLength-1]);
	std::string result;
	result = temp;
	for(int i = dataLength - 2 ; i >=0; i--)
	{
		sprintf(temp, "%8X", data[i]);
		result += temp;
	}

	return CStringA(result);
}









/// <summary>
/// 用公钥解密
/// </summary>
/// <param name="bytes">加密后的字节数组</param>
/// <param name="n">公钥中的n</param>
/// <param name="e">公钥中的d</param>
/// <returns>解密后的字节数组</returns>
BOOL CRsNetRsaHelp::DecryptByPublicKey(BYTE* bytes, int nBytesLen, CStringA n, CStringA e, BYTE* out_bytes, int& out_byteslen)
{
	int length = nBytesLen - 1;
// 	if(length < out_byteslen) 为什么写他？
// 		return FALSE;

	//大整数N
	CRsNetRsa biN(n, 16);

	//私钥大素数
	CRsNetRsa biE(e, 16);

	CRsNetRsa biEncrypted(bytes, nBytesLen);
	CRsNetRsa biDecrypted = biEncrypted.modPow(biE, biN);

	CStringA resultStr = biDecrypted.ToHexString();

	if (resultStr.GetLength() < length * 2)
	{
		while (resultStr.GetLength() != length * 2)
		{
			resultStr.mString.insert(resultStr.mString.begin(), '0');
			//resultStr = "0" + resultStr;
		}
	}

	out_byteslen = length;
	for (int i = 0; i < length; i++)
	{
		CStringA str = resultStr.Mid(i*2,2);
		out_bytes[i] = (BYTE)strtol(str.GetBuffer(), NULL, 16);
	}
	return TRUE;
}
