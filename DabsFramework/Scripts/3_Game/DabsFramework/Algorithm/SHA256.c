class SHA256_CTX
{
	byte Data[64];
	uint State[8];
	uint BitLen[2];
	uint DataLen;
	
	void SHA256_CTX()
	{
		State[0] = 0x6a09e667;
        State[1] = 0xbb67ae85;
        State[2] = 0x3c6ef372;
        State[3] = 0xa54ff53a;
        State[4] = 0x510e527f;
        State[5] = 0x9b05688c;
        State[6] = 0x1f83d9ab;
        State[7] = 0x5be0cd19;
		DataLen = 0;
        BitLen[0] = 0;
        BitLen[1] = 0;
	}
}

class SHA256
{
    protected ref array<uint> K = {
        0x428a2f98 ,0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2     
    };
	
    ref SHA256_CTX Context = new SHA256_CTX();
    byte Hash[32];
    bool Finished;

    array<byte> Compute(byte data[], uint length)
    {
		array<byte> hash = {};
		
        if (Finished) {
            Error("SHA256 Object Already Computed! Construct a new object to calculate a new Hash! Use the static helper functions!");
			copyarray(hash, Hash);
            return hash;
        }

        Update(data, length);
        Final();

		Print(Hash);
		copyarray(hash, Hash);
        return hash;
    }

	
    private void Update(byte data[], uint len)
    {
        for (uint i = 0; i < len; ++i)
        {
            Context.Data[Context.DataLen] = data[i];
            Context.DataLen++;

            if (Context.DataLen == 64)
            {
                Transform(Context.Data);

                array<uint> results = DBL_INT_ADD(Context.BitLen[0], Context.BitLen[1], 512);
                Context.BitLen[0] = results[0];
                Context.BitLen[1] = results[1];

                Context.DataLen = 0;
            }
        }
    }
	
    private void Transform(byte data[])
    {
        uint a, b, c, d, e, f, g, h, i, j, t1, t2;
        //initialize M to be filled with 64 0s
        array<uint> m = {};
        for(int m_itr = 0; m_itr < 64; m_itr++) m.Insert(0);

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
        a = Context.State[0];
        b = Context.State[1];
        c = Context.State[2];
        d = Context.State[3];
        e = Context.State[4];
        f = Context.State[5];
        g = Context.State[6];
        h = Context.State[7];
		
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
        Context.State[0] = Context.State[0] + a;
        Context.State[1] = Context.State[1] + b;
        Context.State[2] = Context.State[2] + c;
        Context.State[3] = Context.State[3] + d;
        Context.State[4] = Context.State[4] + e;
        Context.State[5] = Context.State[5] + f;
        Context.State[6] = Context.State[6] + g;
        Context.State[7] = Context.State[7] + h;
    }
    private void Final()
    {
        uint i = Context.DataLen;
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
        Transform(Context.Data);
        for (i = 0; i < 4; i++) {
            int shift = 24 - (i * 8);
            Hash[i] = UInt8.Convert(UInt32.ShiftRight(Context.State[0], shift));
            Hash[i + 4] = UInt8.Convert(UInt32.ShiftRight(Context.State[1], shift));
            Hash[i + 8] = UInt8.Convert(UInt32.ShiftRight(Context.State[2], shift));
            Hash[i + 12] = UInt8.Convert(UInt32.ShiftRight(Context.State[3], shift));
            Hash[i + 16] = UInt8.Convert(UInt32.ShiftRight(Context.State[4], shift));
            Hash[i + 20] = UInt8.Convert(UInt32.ShiftRight(Context.State[5], shift));
            Hash[i + 24] = UInt8.Convert(UInt32.ShiftRight(Context.State[6], shift));
            Hash[i + 28] = UInt8.Convert(UInt32.ShiftRight(Context.State[7], shift));
        }
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
    
    static array<byte> ComputeArray(byte data[], uint length)
    {
        SHA256 crypto = new SHA256();
        return crypto.Compute(data, length);
    }
	
	static array<byte> ComputeArray(array<byte> data)
	{
		byte _data[64];
		copyarray(_data, data);
		Print(data.Count());
		return ComputeArray(_data, data.Count());
	}
	
    static string ComputeString(string data)
    {
        array<byte> hash = SHA256.ComputeArray(Encoding.GetBytes(data)); //data.Length()
        return Encoding.FromBytesHex(hash);
    }
}