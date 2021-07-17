class SHA256_CTX
{
	byte Data[64];
	uint BitLen[2];
	uint DataLen;
	
	void SHA256_CTX()
	{
		DataLen = 0;
        BitLen[0] = 0;
        BitLen[1] = 0;
	}
}


// Pulled from https://www.movable-type.co.uk/scripts/sha256.html
// adapted for Enfusion by InclementDab
// original idea from Kegan
// All UInt32 functionality from Kegan
class SHA256
{
	protected uint H[8] = {
		0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19,
	};
	
    protected uint K[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2     
    };
	
    ref SHA256_CTX Context = new SHA256_CTX();
	
	static string Compute(string msg)
	{
		// Preprocess
		int append = 0x80;
		msg += append.AsciiToString();
		
		// convert string msg into 512-bit blocks (array of 16 32-bit integers)
		int l = msg.Length() / 4 + 2;
		int n = Math.Ceil(l / 16);
		array<ref array<int>> m = {};
		
		int i, j, k;
		for (i = 0; i < n; i++) {
			// init array
			m.InsertAt(new array<int>(), i);
			for (j = 0; j < 16; j++) {
				int bit_block = CharCodeAt(msg, i * 64 + j * 4 + 0) << 24;
				bit_block = Algorithm.BITWISE_XOR(bit_block, CharCodeAt(msg, i * 64 + j * 4 + 1) << 16);				
				bit_block = Algorithm.BITWISE_XOR(bit_block, CharCodeAt(msg, i * 64 + j * 4 + 2) << 8);
				bit_block = Algorithm.BITWISE_XOR(bit_block, CharCodeAt(msg, i * 64 + j * 4 + 3) << 0);
				m[i].InsertAt(bit_block, j);
			}
		}
		
		// add length (in bits) into final pair of 32-bit integers (big-endian)
		// note: most significant word would be (len-1)*8 >>> 32
		m[n - 1][14] = Math.Floor(((msg.Length() - 1) * 8) / Math.Pow(32, 2));
		m[n - 1][15] = UInt32.ShiftRight(((msg.Length() - 1) * 8), 0);

		
		// Hash computation
		for (i = 0; i < n; i++) {
			int w[64];
			for (int t = 0; t < 16; t++) {
				w[t] = m[i][t]; // what the fuck (mit pog)
			}
						
			for (t = 16; t < 64; t++) {		
				int s1 = SIG1(w[t - 2]) + w[t - 7];
				int s0 = SIG0(w[t - 15]) + w[t - 16];
				
				//Print(SIG0(w[t - 15]) + w[t - 16]);
				w[t] = UInt32.ShiftRight(s0 + s1, 0);
				bool result[32];
				UInt32.ToBitArray(w[t], result);
			}
		}
		
		return string.Empty;
	}
	
	static int CharCodeAt(string msg, int index)
	{
		if (index > msg.Length() - 1) {
			return 0;
		}
		
		return msg[index].Hash();
	}
		
    static uint ROTR(int n, int x)
    {
		return (UInt32.ShiftRight(x, n) | ( x << (32 - n));
    }
	
	static int EP0(int x)
	{
		return (Algorithm.BITWISE_XOR(Algorithm.BITWISE_XOR(ROTR(2, x), ROTR(13, x)), ROTR(22, x)));
	}
	
	static int EP1(int x)
	{
		return (Algorithm.BITWISE_XOR(Algorithm.BITWISE_XOR(ROTR(6, x), ROTR(11, x)), ROTR(25, x)));
	}
	
	static int SIG0(int x)
	{
		return (Algorithm.BITWISE_XOR(Algorithm.BITWISE_XOR(ROTR(7, x), ROTR(18, x)), UInt32.ShiftRight(x, 3)));
	}
	
	static int SIG1(int x)
	{
		return (Algorithm.BITWISE_XOR(Algorithm.BITWISE_XOR(ROTR(17, x), ROTR(19, x)), UInt32.ShiftRight(x, 10)));
	}
	
	
/*
    array<byte> Compute(uint data[64], uint length)
    {
		array<byte> hash = {}; // max 32
		int i;
        for (i = 0; i < length; i++) {
            Context.Data[Context.DataLen] = data[i];
            Context.DataLen++;

            if (Context.DataLen == 64) {
                Transform(Context.Data);
                array<uint> results = DBL_INT_ADD(Context.BitLen[0], Context.BitLen[1], 512);
                Context.BitLen[0] = results[0];
                Context.BitLen[1] = results[1];

                Context.DataLen = 0;
            }
        }
		
		// Final
        i = Context.DataLen;
        if (Context.DataLen < 56) {
            Context.Data[i++] = 0x80;

            while (i < 56) {
                Context.Data[i++] = 0x00;
			}
        } else {
            Context.Data[i++] = 0x80;
            while (i < 64) {
                Context.Data[i++] = 0x00;
			}

            Transform(Context.Data);
        }
		
        array<uint> result = DBL_INT_ADD(Context.BitLen[0], Context.BitLen[1], Context.DataLen * 8);
        Context.BitLen[0] = result[0];
        Context.BitLen[1] = result[1];
        Context.Data[63] = UInt8.Convert(Context.BitLen[0]);
        Context.Data[62] = UInt8.Convert(UInt32.ShiftRight(Context.BitLen[0], 8));
        Context.Data[61] = UInt8.Convert(UInt32.ShiftRight(Context.BitLen[0], 16));
        Context.Data[60] = UInt8.Convert(UInt32.ShiftRight(Context.BitLen[0], 24));
        Context.Data[59] = UInt8.Convert(Context.BitLen[1]);
        Context.Data[58] = UInt8.Convert(UInt32.ShiftRight(Context.BitLen[1], 8));
        Context.Data[57] = UInt8.Convert(UInt32.ShiftRight(Context.BitLen[1], 16));
        Context.Data[56] = UInt8.Convert(UInt32.ShiftRight(Context.BitLen[1], 24));
		Print(Context.Data);
        Transform(Context.Data);
		Print(Context.Data);
        for (i = 0; i < 4; i++) {
            int shift = 24 - (i * 8);
            hash[i] = UInt8.Convert(UInt32.ShiftRight(H[0], shift));
            hash[i + 4] = UInt8.Convert(UInt32.ShiftRight(H[1], shift));
            hash[i + 8] = UInt8.Convert(UInt32.ShiftRight(H[2], shift));
            hash[i + 12] = UInt8.Convert(UInt32.ShiftRight(H[3], shift));
            hash[i + 16] = UInt8.Convert(UInt32.ShiftRight(H[4], shift));
            hash[i + 20] = UInt8.Convert(UInt32.ShiftRight(H[5], shift));
            hash[i + 24] = UInt8.Convert(UInt32.ShiftRight(H[6], shift));
            hash[i + 28] = UInt8.Convert(UInt32.ShiftRight(H[7], shift));
        }

        return hash;
    }
	
    private void Transform(byte data[64])
    {
        uint a, b, c, d, e, f, g, h, i, j, t1, t2;
        //initialize M to be filled with 64 0s
        array<uint> m = {};
        for (int m_itr = 0; m_itr < 64; m_itr++) m.Insert(0);

        j = 0;
        for (i = 0; i < 16; ++i)
        {
            uint l1_a = data[j] << 24;
            uint l1_b = data[j + 1] << 16;
            uint l1_c = data[j + 2] << 8;
            uint l1_d = data[j + 3];
            m[i] = (l1_a | l1_b | l1_c | l1_d);
            j += 4;
        }
		
        for (; i < 64; ++i)
        {
            uint l2_a = m[i - 2];
            uint l2_b = UInt32.SIG1(l2_a);
            uint l2_c = m[i - 7];
            uint l2_d = m[i - 15];
            uint l2_e = UInt32.SIG0(l2_d);
            uint l2_f = m[i - 16];
            m[i] = l2_b + l2_c + l2_e + l2_f;
        }
        a = H[0];
        b = H[1];
        c = H[2];
        d = H[3];
        e = H[4];
        f = H[5];
        g = H[6];
        h = H[7];
		
        for (i = 0; i < 64; ++i)
        {
            t1 = UInt32.Add(UInt32.Add(UInt32.Add(UInt32.Add(h, UInt32.EP1(e)), UInt32.CH(e, f, g)), K[i]), m[i]);//h + EP1(e) + CH(e, f, g) + k[i] + m[i];
            t2 = UInt32.Add(UInt32.EP0(a), UInt32.MAJ(a, b, c));//EP0(a) + MAJ(a, b, c);
            h = g;
            g = f;
            f = e;
            e = UInt32.Add(d,t1);//d + t1;
            d = c;
            c = b;
            b = a;
            a = UInt32.Add(t1, t2);//t1 + t2;
        }
		
        H[0] = H[0] + a;
        H[1] = H[1] + b;
        H[2] = H[2] + c;
        H[3] = H[3] + d;
        H[4] = H[4] + e;
        H[5] = H[5] + f;
        H[6] = H[6] + g;
        H[7] = H[7] + h;
    }
	
    //This function treats A and B as one 64bit unsigned integer and adds C to it
    private array<uint> DBL_INT_ADD(uint a, uint b, uint c)
    {
        uint max_int = 0xffffffff;
        uint test = max_int - c;
		 //if a > test (unsigned comparison)
        if (UInt32.IsGt(a, test)) {
            ++b;
        }

        a = UInt32.Add(a, c); // A + C using unsigned math
        return { a, b };
    }
    
    static array<byte> ComputeArray(byte data[64], uint length)
    {
        SHA256 crypto = new SHA256();
        return crypto.Compute(data, length);
    }
	
	static array<byte> ComputeArray(array<byte> data)
	{
		byte _data[64];
		for (int i = 0; i < 64; i++) {
			_data[i] = data[i]; // copyarray no worky :(
		}
				
		return ComputeArray(_data, 64);
	}
	
    static string ComputeString(string data)
    {
        array<byte> hash = SHA256.ComputeArray(Encoding.GetBytes(data));
		Print(hash.Count());
        return Encoding.FromBytesHex(hash);
    }
	*/
}