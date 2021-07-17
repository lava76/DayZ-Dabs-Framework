class UInt8 
{
    static byte Convert(int value)
    {
        return (value & 255); //00000000000000000000000011111111 (only the first 8 bits will retain their value)
    }
}