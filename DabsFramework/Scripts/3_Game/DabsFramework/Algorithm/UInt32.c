class UInt32
{
	// Great for debugging bit masks and fancy integer bullshit
	static void ToBitArray(int value, out bool result[32])
	{
		for (int i = 0; i < 32; i++) {	
			result[i] = ((value >> i) & 1);
		}
		
		reversearray(result);
	}
	
    //A + B
    static uint Add(uint a, uint b)
    {
        uint carry = a & b;
        uint result = Algorithm.BITWISE_XOR(a, b);
        while (carry != 0) {
            uint shiftedcarry = carry << 1;
            carry = result & shiftedcarry;
            result = Algorithm.BITWISE_XOR(result, shiftedcarry);
        }
		
        return result;
    }
	
    // A > B
    static bool IsGt(uint a, uint b)
    {
        uint ltb = ~a & b;
        uint gtb = a & ~b;
        ltb |= UInt32.ShiftRight(ltb,1);
        ltb |= UInt32.ShiftRight(ltb,2);
        ltb |= UInt32.ShiftRight(ltb,4);
        ltb |= UInt32.ShiftRight(ltb,8);
        ltb |= UInt32.ShiftRight(ltb,16);
        uint isGt = gtb & ~ltb;
        return (isGt != 0); //non-zero if A > B
    }
	
    // A < B
    static bool IsLt(uint a, uint b)
    {
        return UInt32.IsGt(b, a);
    }
	
    // X >> Amount
    static uint ShiftRight(uint x, int amount)
    {
        if (amount <= 0) {
            return x;
		}

        uint value = (x >> 1) & 0x7FFFFFFF;
        uint extra_shifts = amount - 1;
        if (extra_shifts > 0) {
            value = (value >> extra_shifts);
		}

        return value;
    }
		
    static uint CH(uint x, uint y, uint z)
    {
        uint a = (x & y);
        uint b = ~x;
        uint c = (b & z);
        return Algorithm.BITWISE_XOR(a, c); // a ^ c
    }
	
    static uint MAJ(uint x, uint y, uint z)
    {
        uint a = x & y;
        uint b = x & z;
        uint c = y & z;
        return Algorithm.BITWISE_XOR(a, Algorithm.BITWISE_XOR(b, c)); //a ^ b ^ c (left-right associativity)
    }
	
	/*
	
    static uint SIG0(uint x)
    {
        uint a = UInt32.ROTRIGHT(x, 7);
        uint b = UInt32.ROTRIGHT(x, 18);
        uint c = UInt32.ShiftRight(x, 3);
        return Algorithm.BITWISE_XOR(a, Algorithm.BITWISE_XOR(b, c)); //a ^ b ^ c
    }
	
	
    static uint SIG1(uint x)
    {
        uint a = UInt32.ROTRIGHT(x, 17);
        uint b = UInt32.ROTRIGHT(x, 19);
        uint c = UInt32.ShiftRight(x, 10);
        return Algorithm.BITWISE_XOR(a, Algorithm.BITWISE_XOR(b, c)); //a ^ b ^ c
    }*/
}