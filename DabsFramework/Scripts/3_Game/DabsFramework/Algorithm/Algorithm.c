// Static Algorithm class, will contain some useful math functions
class Algorithm
{
	private void Algorithm();
	private void ~Algorithm();
	
	static int BITWISE_XOR(int x, int y)
	{
	    int a = (x | y);
	    int b = ~x;
	    int c = ~y;
	    int d = (b | c);
	    return (a & d);
	}
}