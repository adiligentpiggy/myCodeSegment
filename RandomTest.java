import java.util.Iterator;  
import java.util.Set; 
import java.nio.charset.Charset;
public class RandomTest {    
    public static void main(String[] args) {
        for(int j = 0; j< 100; j++){
            System.out.println((int)((Math.random()*9+1)*100000));
        }

        //System.out.println(charsetName);

    }
}