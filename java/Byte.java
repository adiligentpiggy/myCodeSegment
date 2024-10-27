public class Byte {
	static String mStr = "222";
static void f(String arg)	{
	System.out.println(arg);
	arg = "1111";
	System.out.println(arg);
	System.out.println(mStr);
}

public static void main(String[] args) {
  byte b=-1;
  System.out.println((int)(char)b);
  System.out.println((int)(char)(b & 0xff));
  System.out.println(0xffffffff);
  
  System.out.println(mStr);
  f(mStr);
  System.out.println(mStr);
}
}