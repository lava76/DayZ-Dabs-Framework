class SHA256_CTX
{
	byte Data[64];
	uint State[8];
	uint BitLen[2];
	uint DataLen;
}