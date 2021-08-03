class UInt8 
{
    static int Convert(int value)
    {
        return (value & 255);
    }
	
	static void ToBitArray(int value, out bool result[8])
	{
		for (int i = 0; i < 8; i++) {	
			result[i] = ((value >> i) & 1);
		}
		
		reversearray(result);
	}
}