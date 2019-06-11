import java.util.Iterator;  
import java.util.Set; 
import java.nio.charset.Charset;


public class NoVisibility {  
private static class ThreadSafeCache {
	int number;  
	public int getResult(){
		return number;
	}

	public synchronized void setResult(int result){
		this.number = result;
	}
}
private static boolean ready;
private static  int number;
private static ThreadSafeCache cache = new ThreadSafeCache();

private static class ReaderThread extends Thread {
    public void run(){
          while(!ready)
                Thread.yield();
          System.out.println(number);
    }
}

private static class ReaderThread2 extends Thread {
	private ThreadSafeCache  mCache;
	public ReaderThread2(ThreadSafeCache c) {
		mCache = c;
	}
    public void run(){
          while(true) {
          System.out.println(mCache.getResult());
		  try {
		  sleep(10);
		  } catch (InterruptedException e) {}
		  }
    }
}

 
    public static void main(String[] args) {
        new ReaderThread2(cache).start();
		cache.setResult(23);
		//number=42;
		//ready=true;

    }
}