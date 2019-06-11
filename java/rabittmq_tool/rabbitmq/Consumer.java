//package com.byd.rabbitmq;

import com.rabbitmq.client.AMQP.BasicProperties;
import com.rabbitmq.client.Channel;
import com.rabbitmq.client.DefaultConsumer;
import com.rabbitmq.client.Connection;
import com.rabbitmq.client.ConnectionFactory;
import com.rabbitmq.client.QueueingConsumer;
import com.rabbitmq.client.QueueingConsumer.Delivery;
import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;

import java.io.IOException;
import java.net.ConnectException;
import java.util.concurrent.TimeoutException;
import java.util.*;

public class Consumer {
	private final static String SEPARATOR_COMMA = ",";
	private final static String SEPARATOR_COLON = ":";
	
	private final static String PA_EVENT_TYPE_AUTO = "auto";
	private final static String PA_EVENT_TYPE_EMERGENCY = "emergency";
	private final static String PA_EVENT_TYPE_RECORD = "prerecord";
	private final static String PA_EVENT_TYPE_LIVE = "live";
	private final static String PA_EVENT_TYPE_BACKGROUND = "background";
	
	public static enum IconIndex
    {
        IDLE_ICON_INDEX,
        CHECKED_ICON_INDEX,
        LIVE_PA_ICON_INDEX,
        RECORD_PA_ICON_INDEX,
        EMERGENCY_PA_ICON_INDEX,
        AUOTO_PA_ICON_INDEX,
        FAULT_ICON_INDEX,
        ICON_COUNT
    };
	
	public static class PATaskInfo {
		public String audioId;
		public String randStr;
		public String regionIds;
		public String speakerIds;
		public IconIndex iconIndex = IconIndex.RECORD_PA_ICON_INDEX;
		public String paType = PA_EVENT_TYPE_RECORD;
		public int priority = 10;
		
		public void setPATypeByIconIndex() {
			switch (iconIndex)
			{
			case LIVE_PA_ICON_INDEX:
				paType = PA_EVENT_TYPE_LIVE;
				break;
			case EMERGENCY_PA_ICON_INDEX:
				paType = PA_EVENT_TYPE_EMERGENCY;
				break;
			case AUOTO_PA_ICON_INDEX:
				paType = PA_EVENT_TYPE_AUTO;
				break;
			case RECORD_PA_ICON_INDEX:
			default:
				paType = PA_EVENT_TYPE_RECORD;
				break;
			}
		}
		
		public String toString() {
			return "PATaskInfo audioId:" + audioId + " randStr:" + randStr + " regionIds:" + regionIds
			    + " speakerIds:" + speakerIds + " iconIndex:" + iconIndex + " paType:" + paType + " priority:" + priority;
		}
	};
	
	
	
    private final static String EXCHANGE_NAME = "itc-extern";
	
	private static HashMap<String, Boolean> mTerminalStatusMap = new HashMap<String, Boolean>();

    public static void main(String[] args) throws IOException, TimeoutException {
        ConnectionFactory connFactory = null;
        Connection conn = null;
        Channel channel = null;
		while (true) {
        try {
            connFactory = new ConnectionFactory();
            connFactory.setHost("10.4.20.171");
            conn = connFactory.newConnection();
            channel = conn.createChannel();
			
			channel.exchangeDeclare(EXCHANGE_NAME, "direct", false);
			String queueName = channel.queueDeclare().getQueue();
		    //channel.queueBind(queueName, EXCHANGE_NAME, "itc-extern-interface");
			//queueDeclare第一个参数表示队列名称、第二个参数为是否持久化（true表示是，队列将在服务器重启时生存）、第三个参数为是否是独占队列（创建者可以使用的私有队列，断开后自动删除）、第四个参数为当所有消费者客户端连接断开时是否自动删除队列、第五个参数为队列的其他参数
            //channel.queueDeclare(QUEUE_NAME, false, false, false, null);
            System.out.println("listening for event message...");
            channel.queueBind(queueName, EXCHANGE_NAME, "itc-extern-interface");   //使用item.# 匹配所有的以item开头的
            //同一时刻服务器只能发送一条消息给消费者；
            channel.basicQos(1);
            QueueingConsumer consumer = new QueueingConsumer(channel);
            channel.basicConsume(queueName, true, consumer);
            while (true) {
                Thread.sleep(1000);
                System.out.println("waiting msg");
                Delivery delivery = consumer.nextDelivery();
                BasicProperties props = delivery.getProperties();
                BasicProperties reply_props = new BasicProperties.Builder().correlationId(props.getCorrelationId()).build();
                String msg = new String(delivery.getBody(), "utf-8");
                System.out.println("receive msg:" + msg);
				parseServerMsg(msg);
                String retMsg = "ok, give you reply：" + new String(msg.getBytes(), "utf-8");
                System.out.println("Consumer中的返回队列名" + props.getReplyTo());
                if (props.getReplyTo() != null) {
					//basicPublish第一个参数为交换机名称、第二个参数为队列映射的路由key、第三个参数为消息的其他属性、第四个参数为发送信息的主体
                    channel.basicPublish("", props.getReplyTo(), reply_props, retMsg.getBytes());
                }
            }
        } catch (IOException ioe) {
            ioe.printStackTrace();
        } catch (Exception toe) {
            toe.printStackTrace();
        } finally {
			try {
                if (channel != null) channel.close();
                if (conn != null) conn.close();
			} catch (IOException e) {}
        }
		
		try {
		    Thread.sleep(60000);
		} catch (InterruptedException e) {}
		}
		
    }
	
	public static void parseServerMsg(String msg) {
	    Object json = JSON.parse(msg);
		if (json instanceof JSONObject) {
			JSONObject jObj = (JSONObject)json;
			if (jObj.containsKey("job_type")) {
				PATaskInfo paTask = new PATaskInfo();
				String type = jObj.getString("job_type");
				String triggerType = jObj.getString("trigger_type");
				String status = jObj.getString("job_status");
				String sequenceId = jObj.getString("sequence_id");
				boolean isFireAlarmMsg = "fire_alarm".equals(triggerType);
				boolean isJobStart = "start".equals(triggerType);
				
				if ("broadcast_event".equals(type) || "talkbackevent".equals(type)) {
					paTask.paType = PA_EVENT_TYPE_LIVE;
					paTask.randStr = sequenceId;
					paTask.iconIndex = IconIndex.LIVE_PA_ICON_INDEX;
				} else if (("fire_alarm".equals(type) || "media".equals(type)) && isFireAlarmMsg) {
					paTask.paType = PA_EVENT_TYPE_EMERGENCY;
					paTask.randStr = sequenceId;
					paTask.iconIndex = IconIndex.EMERGENCY_PA_ICON_INDEX;
				} else {
					String[] strParts = sequenceId.split("_"); // sequenceId content eg: "temp_media_3_2534765"
					if (strParts.length > 3) {
						paTask.iconIndex = getEnumTypeByInt(Integer.valueOf(strParts[2]));
					}
					paTask.setPATypeByIconIndex();
					
					paTask.randStr = sequenceId;
					int lastUnderscorePos = sequenceId.lastIndexOf('_');
					if (lastUnderscorePos != -1) {
						if (lastUnderscorePos > 0) lastUnderscorePos -= 1;
						
						paTask.randStr = sequenceId.substring(lastUnderscorePos); // "3_2534765" is the rand string.
					}
				}
				
				JSONObject calleObj = jObj.getJSONObject("callee");
				String terminalExtens = calleObj.getString("telephone");
				terminalExtens.replace(SEPARATOR_COMMA, SEPARATOR_COLON);
				paTask.speakerIds = terminalExtens;
				System.out.println("paTask:" + paTask);
			}
		} else if (json instanceof JSONArray) {
			JSONArray jArray = (JSONArray)json;
			for (int i = 0; i < jArray.size(); i++) {
				JSONObject jItem = jArray.getJSONObject(i);
				if (jItem.containsKey("terminal")) {
					JSONObject jterminal = jItem.getJSONObject("terminal");
					String exten = jterminal.getString("exten");
					boolean onlineStatus = jterminal.getBooleanValue("status");
					
					if (!mTerminalStatusMap.containsKey(exten) || mTerminalStatusMap.get(exten) != onlineStatus) {
						mTerminalStatusMap.put(exten, onlineStatus);
						System.out.println("exten status exten:" + exten + " onlineStatus:" + onlineStatus);
					}
				}
			}
		}
        //MqttJsonId=json.getString("id");
	}
	
	public static IconIndex getEnumTypeByInt(int i) {
		IconIndex ret = IconIndex.ICON_COUNT;
		IconIndex[] array = {IconIndex.IDLE_ICON_INDEX,
			IconIndex.CHECKED_ICON_INDEX,
			IconIndex.LIVE_PA_ICON_INDEX,
			IconIndex.RECORD_PA_ICON_INDEX,
			IconIndex.EMERGENCY_PA_ICON_INDEX,
			IconIndex.AUOTO_PA_ICON_INDEX,
			IconIndex.FAULT_ICON_INDEX
			};
			
		if (i >= 0 && i < array.length) {
			ret = array[i];
		}
		
		return ret;
	}
}