import java.util.Iterator;  
import java.util.Set; 
import java.nio.charset.Charset;
public class Utf8 {    
    public static void main(String[] args) {
        String str = "张";
        final String HEADER_ID = "工单ID";//"\uE55D\u5553\u4900\u4400";//"工单ID"
        final String HEADER_TIME = "\u1B52\uFA5E\uF665\uF495";//创建时间
        final String HEADER_SUBMITTER = "\u3375\uF78B\uBA4E";//申请人
        final String HEADER_VEHICLE = "\u668F\u868F\uE14F\u6F60";//车辆信息
        final String HEADER_MODULE = "\u4565\u9C96\u216A\u5757";//故障模块
        final String HEADER_ASSIGNEE = "\u535F\u4D52\u0459\u0674\uBA4E";//当前处理人
        final String HEADER_DESC = "\uEE95\u9898\uCF63\uF08F";//问题描述

        System.out.println(Integer.toHexString(str.codePointAt(0)).toUpperCase());
        System.out.println(Charset.defaultCharset());
        String[] arr = {HEADER_ID,HEADER_TIME,HEADER_SUBMITTER,HEADER_VEHICLE,HEADER_MODULE,HEADER_ASSIGNEE};
        for (String i : arr) {
        	System.out.println(i);
        }
        //System.out.println(charsetName);
        Set<String> charsetNames = Charset.availableCharsets().keySet();  
        System.out.println("-----the number of jdk1.67's charset is "+charsetNames.size()+"-----");  
        /*for (Iterator it = charsetNames.iterator(); it.hasNext();)  
        {  
            String charsetName = (String) it.next();   
            System.out.println(charsetName);  
        }  */
    }
}