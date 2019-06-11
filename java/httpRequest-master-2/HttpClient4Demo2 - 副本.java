package com.example.demo;

import org.apache.http.*;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.ContentType;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClientBuilder;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.util.EntityUtils;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.io.BufferedReader;  
import java.io.InputStreamReader;  
import java.io.OutputStreamWriter;  
import java.io.InputStream;  
import java.io.OutputStream;  
import java.io.DataInputStream;  
import java.io.DataOutputStream;  
import java.io.FileInputStream; 
import java.net.HttpURLConnection;  
import java.net.URL;  
import java.util.Map;  
import java.util.Map.Entry;  

import org.apache.http.entity.StringEntity;  
import org.apache.http.impl.client.HttpClientBuilder;  
import org.junit.Test;  
import com.alibaba.fastjson.JSONObject;  
import java.io.File;  
import java.io.UnsupportedEncodingException;  
import java.nio.charset.Charset;  
import org.apache.http.HttpEntity;  
import org.apache.http.HttpResponse;  
import org.apache.http.HttpStatus;  
import org.apache.http.client.HttpClient;  
import org.apache.http.client.methods.HttpPost;  
import org.apache.http.entity.mime.HttpMultipartMode;  
import org.apache.http.entity.mime.MultipartEntityBuilder;  
import org.apache.http.entity.mime.MultipartEntity;  
import org.apache.http.entity.mime.FormBodyPartBuilder;  
import org.apache.http.entity.mime.FormBodyPart;  
import org.apache.http.entity.mime.content.FileBody;  
import org.apache.http.entity.mime.content.StringBody;  
import org.apache.http.impl.client.CloseableHttpClient;  
import org.apache.http.impl.client.DefaultHttpClient;  
import org.apache.http.impl.client.HttpClientBuilder;  
import org.apache.http.util.EntityUtils;  
import org.apache.http.util.CharsetUtils;  

/**
 * Created by ludonglue on 2017/9/19.
 * 使用apache的工具包HttpClient进行http请求
 */
public class HttpClient4Demo2 {
    public static void main(String[] args) {
		/*try {
		    sendFile("C:\\eula.1028.txt");
		} catch (Exception e) {
			e.printStackTrace();
		}*/
	
		uploadFile("C:\\eula.1028.txt");

		if (args.length < 1) {
			System.out.println("Error: empty args!");
			return;
		}
        get(args[0]);
        post(args[0]);
    }
    
    public static String server_url = "http://10.4.20.171/v2/audio/upload_file.json";//"http://183.63.117.154:8010/v2/audio/upload_file.json";

    public static void get(String url){
        /**
         * client和response需要关闭资源
         */
        //创建client,放入try()中自动释放,不需要finally
        try (CloseableHttpClient client = HttpClientBuilder.create().build()){

            //执行得到response
            try (CloseableHttpResponse response = client.execute(new HttpGet(url))){
                //获取statusCode
                Integer statusCode = response.getStatusLine().getStatusCode();
                HttpEntity entity = response.getEntity();
                if (entity != null) {
                    //body
                    String bodyAsString = EntityUtils.toString(entity);
                    //Media Type
                    String contentMimeType = ContentType.getOrDefault(entity).getMimeType();
                    //head
                    Header[] headers = response.getHeaders(HttpHeaders.CONTENT_TYPE);
                    //输出
                    System.out.println("statusCode:"+statusCode);
                    System.out.println("contentMinmeType:"+contentMimeType);
                    System.out.println("body:"+bodyAsString);
                    System.out.println("head"+headers);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void post(String url){
        //创建client
        try (CloseableHttpClient client = HttpClientBuilder.create().build()){
            HttpPost postRequest = new HttpPost(url);
            //添加请求参数
            List<NameValuePair> params = new ArrayList<>();
            params.add(new BasicNameValuePair("key1", "value1"));
            params.add(new BasicNameValuePair("key2", "value2"));
            postRequest.setEntity(new UrlEncodedFormEntity(params, Consts.UTF_8));
            //执行并获取返回结果到response
            try (CloseableHttpResponse response = client.execute(postRequest)) {
                //获取statusCode
                Integer statusCode = response.getStatusLine().getStatusCode();
                //Media Type
                String contentMimeType = ContentType.getOrDefault(response.getEntity()).getMimeType();
                //body
                String bodyAsString = EntityUtils.toString(response.getEntity());
                //head
                Header[] headers = response.getHeaders(HttpHeaders.CONTENT_TYPE);
                System.out.println("statusCode:"+statusCode);
                System.out.println("contentMinmeType:"+contentMimeType);
                System.out.println("body:"+bodyAsString);
                System.out.println("head"+headers);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

public static void sendFile(String filePath) throws Exception {  
    HttpClient httpClient = HttpClientBuilder.create().build();  
    HttpPost httpPost = new HttpPost(server_url);  
    //加上该header访问会404,不知道原因...  
    httpPost.setHeader("Content-Type", "multipart/form-data; boundary=-----ZR8KqAYJyI2jPdddL");  
    
    System.out.println("sendFile url: " + server_url);
      
    FileBody fileBody = new FileBody(new File(filePath));  
    StringBody stringBody = new StringBody("application/text", Charset.defaultCharset()); 
    StringBody stringBody2 = new StringBody("application/text", Charset.defaultCharset()); 
    StringBody stringBody3 = new StringBody("faffgfferwrwrd", Charset.defaultCharset()); 	
	
	FormBodyPart t1 = FormBodyPartBuilder.create().setName("authenticity_token").setBody(stringBody3).build(); 
  
    JSONObject json = new JSONObject();  
    json.put("utf8", "true");  
    json.put("authenticity_token", "faffgfferwrwrd");  

    StringBody contentBody = new StringBody(json.toJSONString(), Charset.defaultCharset());  
  
    //以浏览器兼容模式访问,否则就算指定编码格式,中文文件名上传也会乱码  
    /*HttpEntity reqEntity = MultipartEntityBuilder.create().setMode(HttpMultipartMode.BROWSER_COMPATIBLE)
            .addPart("file", fileBody)
            .addPart(t1).setCharset(CharsetUtils.get("UTF-8")).build(); */
			
			/*HttpEntity reqEntity = MultipartEntityBuilder.create().addBinaryBody("medium[virtual_file][]", new File(filePath), 
			ContentType.MULTIPART_FORM_DATA, "1.mp3")
			.addTextBody("method", "aaa")
			.addTextBody("method2", "bbb")
			.build();
			*/HttpEntity reqEntity = MultipartEntityBuilder.create().build();

  
    httpPost.setEntity(reqEntity);  
    HttpResponse response = httpClient.execute(httpPost);  
    if(HttpStatus.SC_OK==response.getStatusLine().getStatusCode()){  
  
        HttpEntity entitys = response.getEntity();  
        if (entitys != null) {  
            System.out.println(EntityUtils.toString(entitys));  
        }  
    }  
    httpClient.getConnectionManager().shutdown();  
}  


public static void uploadFile(String fileName) {  
        try {  
  
            // 换行符  
            final String newLine = "\r\n";  
            final String boundaryPrefix = "--";  
            // 定义数据分隔线  
            String BOUNDARY = "========7d4a6d158c9";  
            // 服务器的域名  
            URL url = new URL(server_url);  
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();  
            // 设置为POST情  
            conn.setRequestMethod("POST");  
            // 发送POST请求必须设置如下两行  
            conn.setDoOutput(true);  
            conn.setDoInput(true);  
            conn.setUseCaches(false);  
            // 设置请求头参数  
            conn.setRequestProperty("connection", "Keep-Alive");  
            conn.setRequestProperty("Charsert", "UTF-8");  
            conn.setRequestProperty("Content-Type", "multipart/form-data; boundary=" + BOUNDARY);  
  
            OutputStream out = new DataOutputStream(conn.getOutputStream());  
            
            System.out.println("uploadFile url: " + server_url);
  
            // 上传文件  
            File file = new File(fileName);  
            StringBuilder sb = new StringBuilder();  
            sb.append(boundaryPrefix);  
            sb.append(BOUNDARY);  
            sb.append(newLine);  
            // 文件参数,photo参数名可以随意修改  
            sb.append("Content-Disposition: form-data;name=\"photo\";filename=\"" + fileName  
                    + "\"" + newLine);  
            sb.append("Content-Type:application/octet-stream");  
            // 参数头设置完以后需要两个换行，然后才是参数内容  
            sb.append(newLine);  
            sb.append(newLine);  
  
            // 将参数头的数据写入到输出流中  
            out.write(sb.toString().getBytes());  
  
            // 数据输入流,用于读取文件数据  
            DataInputStream in = new DataInputStream(new FileInputStream(  
                    file));  
            byte[] bufferOut = new byte[1024];  
            int bytes = 0;  
            // 每次读1KB数据,并且将文件数据写入到输出流中  
            while ((bytes = in.read(bufferOut)) != -1) {  
                out.write(bufferOut, 0, bytes);  
            }  
            // 最后添加换行  
            out.write(newLine.getBytes());  
            in.close();  
  
            // 定义最后数据分隔线，即--加上BOUNDARY再加上--。  
            byte[] end_data = (newLine + boundaryPrefix + BOUNDARY + boundaryPrefix + newLine)  
                    .getBytes();  
            // 写上结尾标识  
            out.write(end_data);  
            out.flush();  
            out.close();  
  
            // 定义BufferedReader输入流来读取URL的响应  
            BufferedReader reader = new BufferedReader(new InputStreamReader(  
                    conn.getInputStream()));  
            String line = null;  
            while ((line = reader.readLine()) != null) {  
                System.out.println(line);  
            }  
  
        } catch (Exception e) {  
            System.out.println("发送POST请求出现异常！" + e);  
            e.printStackTrace();  
        }  
    }  
}
