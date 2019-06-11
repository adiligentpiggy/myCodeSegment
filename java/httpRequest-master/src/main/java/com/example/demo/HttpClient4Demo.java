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

/**
 * Created by ludonglue on 2017/9/19.
 * 使用apache的工具包HttpClient进行http请求
 */
public class HttpClient4Demo {
    public static void main(String[] args) {
		if (args.length < 1) {
			System.out.println("Error: empty args!");
			return;
		}
        get(args[0]);
        post(args[0]);
    }

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

}
