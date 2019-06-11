package test.x86;
//与主机位数不符的dll会被无视！！！
import com.sun.jna.Library;
import com.sun.jna.Native;

public interface TestJNA extends Library {

	public abstract int add(int a,int b);
	TestJNA INSTANCE = (TestJNA) Native.loadLibrary("testx86", TestJNA.class);
}