//encoding functionality
class Encoding 
{
	static const ref TStringArray HEX_BYTES = {
		"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"
	};
	
    static array<byte> GetBytes(string str)
    {
        array<byte> result = {};
        for (int i = 0; i < str.Length(); i++) {
            result.Insert(Encoding.GetByte(str[i]));
        }
		
        return result;
    }
	
    static byte GetByte(string character)
    {
        return UInt8.Convert(character[0].Hash());
    }
	
    static string ToHex(int ascii)
    {
        return (HEX_BYTES[Math.Floor(ascii / 16)] + HEX_BYTES[ascii % 16]);
    }
	
    static string FromBytesHex(notnull array<byte> data)
    {
        string hexStr = "";
        for (int i = 0; i < data.Count(); i++) {
            hexStr += Encoding.ToHex(data[i]);
        }
		
        return hexStr;
    }
}