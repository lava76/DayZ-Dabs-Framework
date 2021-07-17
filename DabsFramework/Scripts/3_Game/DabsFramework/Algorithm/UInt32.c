class UInt32
{
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
	
    //--- unsigned bitwise operations
    static uint ROTLEFT(uint a, byte b)
    {
        b = UInt8.Convert(b);

        uint y = (a << b);
        uint x = (32 - b);
        uint z = UInt32.ShiftRight(a,x);
        return (y | z);
    }
	
    static uint ROTRIGHT(uint a, byte b)
    {
        b = UInt8.Convert(b); //ensure the byte value is clamped

        uint y = UInt32.ShiftRight(a, b);
        uint x = (32 - b);
        uint z = (a << x); //left shift works on Unsigned Integers
        return (y | z);
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
	
    static uint EP0(uint x)
    {
        uint a = UInt32.ROTRIGHT(x, 2);
        uint b = UInt32.ROTRIGHT(x, 13);
        uint c = UInt32.ROTRIGHT(x, 22);
        return Algorithm.BITWISE_XOR(a, Algorithm.BITWISE_XOR(b, c)); //a ^ b ^ c
    }
	
    static uint EP1(uint x)
    {
        uint a = UInt32.ROTRIGHT(x, 6);
        uint b = UInt32.ROTRIGHT(x, 11);
        uint c = UInt32.ROTRIGHT(x, 25);
        return Algorithm.BITWISE_XOR(a, Algorithm.BITWISE_XOR(b, c)); //a ^ b ^ c
    }
	
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
    }
}