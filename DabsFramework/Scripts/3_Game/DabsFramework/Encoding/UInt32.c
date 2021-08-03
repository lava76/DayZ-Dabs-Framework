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
}