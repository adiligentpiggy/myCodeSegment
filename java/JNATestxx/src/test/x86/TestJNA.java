package test.x86;
//������λ��������dll�ᱻ���ӣ�����
import com.sun.jna.Library;
import com.sun.jna.Native;

public interface TestJNA extends Library {

	public abstract int add(int a,int b);
	TestJNA INSTANCE = (TestJNA) Native.loadLibrary("testx86", TestJNA.class);
}