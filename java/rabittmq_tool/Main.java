package com.byd;

import com.alibaba.fastjson.JSONObject;
import com.byd.FFmpegCommandManager.FFmpegManager;
import com.byd.FFmpegCommandManager.FFmpegManagerImpl;
import com.byd.rocketmq.Consumer;
import com.byd.timertask.OnceEndTimer;
import com.byd.timertask.OnceStartTimer;
import com.byd.timertask.RepEndTimer;
import com.byd.timertask.RepStartTimer;
import com.byd.utils.CmdUtils;
import com.byd.utils.Loadproperties;
import com.byd.utils.LogUtils;
import com.byd.utils.message.Handler;
import org.apache.commons.io.FileUtils;
import org.apache.log4j.Logger;

import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

import static com.byd.utils.Loadproperties.*;
import static com.byd.utils.UdpUtils.sendLiveMsg;
import static org.apache.log4j.Logger.getLogger;

public class Main {
    public static final int PLAY_NEXT = 1001;//play next stream
    public static final int PLAY_LIVE = 1002;//start play live stream
    public static Handler mStreamHandler;
    public static int mPlayIndex;
    public static ArrayList mPlayList;
    public static FFmpegManager mffmpeg = new FFmpegManagerImpl();
    private static Logger log = getLogger(Main.class);
    public static DefaultStreamingTask mDefaultStreamingTask = null;

    public static void main(String[] args) throws IOException {

        new LogUtils();//init log property
        new Loadproperties();//init properties
//        try {
//            Thread.sleep(60000);
//        } catch (InterruptedException e) {
//            e.printStackTrace();
//        }
        try {
            log.info("start mqtt");
            new Consumer().Subscribe();
//            new Thread(new StationServer_MqttStart()).start();//mqtt监听
//            new Thread(new LiveServer_MqttStart()).start();
        } catch (Exception e) {

        }
//        try {
//            Thread.sleep(10000);
//        } catch (InterruptedException e) {
//            e.printStackTrace();
//        }
//        new Thread(new UdpTask_Server(1, SERVER_PORT)).start();//Listen udp port SERVER_PORT
//        new Thread(new UdpTask_Server_LCD(2, CLIENT_PORT)).start();
//        new UdpTask_Client().start();//The udp client for test
//        netTimer.getInstance().start(true, 10000);
//        File file = new File(FTPPROPERTITY_PATHFILE + PATH_UPDATE_VERSION);
//        if (file.exists()) {
//            String cmd3 = "kill -9 $(ps aux|grep OCCSupgrade| awk '{print $2}')";
//            CmdUtils.runCommand(cmd3, 1);
//            file.delete();
//        } else {
//            System.out.println("updateocc.properties not exsit");
//        }
//        //监听直播流，如果有流，直播没有查找播表
//        //To do
//        String playlist = getCurrPlayList();
//        if (playlist == null) {
//            System.out.println("playlist is null");
//            //playDefaultPlayList();
//            new Thread(DefaultStreamingTask.getInstance()).start();
//        } else {
//            //To do
//            System.out.println("playlist exist");
//            parsePlaylist(playlist);
//        }
//        Looper.prepare();
//        mStreamHandler = new Handler() {
//            @Override
//            public void handleMessage(Message message) {
//                switch (message.what) {
//                    case PLAY_NEXT:
////                        File mfile = new File("/opt/video/vod/001.mp4");
////                        if (mfile.exists()) {
////                            mffmpeg.start("LOCALLIVE", "ffmpeg -re -i " + "/opt/video/vod/001.mp4" + " -c copy -f flv " + ADDR_LIVE);
////                        } else {
////                            log.info("The local live file is not exist");
////                        }
////                        try {
////                            Thread.sleep(5000);
////                        } catch (InterruptedException e) {
////                            e.printStackTrace();
////                        }
//                        startLocalLiveStreaming("LOCALLIVE");
//                        sendLiveMsg(STREAM_LIVE_START, getMessageByID(STREAM_LIVE_START), false);
//                        break;
//                    case PLAY_LIVE:
//                        break;
//                    default:
//                        break;
//                }
//                super.handleMessage(message);
//            }
//        };
//        Looper.loop();
    }
    private final  static  String QUEUE_NAME="test_queue_topic2";
    private final  static String EXCHANGE_NAME="test_exchange_topic" ;

    /*
    *return the current playlist
    */
    public static String getCurrPlayList() {
        File dir = new File(USR_LOCAL_OCC_PLLIST);
        int nextId = -1;
        long nextDur = 0;
        if (!dir.exists()) {
            dir.mkdirs();
            return null;
        }
        File[] files = dir.listFiles(); // 该文件目录下文件全部放入数组

        //all playlist is empty
        if (files != null && files.length == 0) {
            return null;
        }
        if (files != null) {
            for (int i = 0; i < files.length; i++) {
                if (files[i].isDirectory()) { // 判断是文件还是文件夹
                    // Todo
                } else {
                    try {
                        String strFileName = files[i].getName();
                        System.out.println("---" + strFileName);
                        String datetime[] = strFileName.split("_");

                        if (datetime == null || datetime.length != 2) {
                            continue;
                        }
                        long tsbegin = Long.parseLong(datetime[0]);
                        long tsend = Long.parseLong(datetime[1]);
                        long lplist = 0;

                        lplist = System.currentTimeMillis();
                        System.out.println("SYSTEM CURRENT TIMESTAMP：" + lplist + "\n");

                        if (tsbegin > lplist) {
                            System.out.println("PLAYLIST no start \n");
                            long dur = tsbegin - lplist;
                            if (dur < nextDur || nextDur == 0) {//find the recent playlist
                                nextDur = dur;
                                nextId = i;
                            }
                            continue;
                        } else if (tsend <= lplist) {
                            files[i].delete();
                            System.out.println("PLAYLIST already end \n");
                            continue;
                        } else if (tsbegin <= lplist && tsend > lplist) {
                            log.info("getCurrPlayList the file name:" + files[i].getName());
                            return files[i].getAbsolutePath();
                        }
                    } catch (Exception e) {

                    }
                }
            }
            if (nextId == -1) {
                return null;
            }
            log.info("getCurrPlayList the next file name:" + files[nextId].getName());
            return files[nextId].getAbsolutePath();
        }
        return null;
    }

    /*
    *resetPlayList
    */
    public static void resetPlayList() {
        File dir = new File(USR_LOCAL_OCC_PLLIST);

        if (!dir.exists()) {
            dir.mkdirs();
        }

        File[] files = dir.listFiles(); // 该文件目录下文件全部放入数组
        //all playlist is empty
        if (files != null && files.length == 0) {
            return;
        }
        if (files != null) {
            for (File file : files) {
                if (file.isDirectory()) { // 判断是文件还是文件夹
                    // Todo
                } else {
                    try {
                        String strFileName = file.getName();
                        System.out.println("---" + strFileName);
                        String datetime[] = strFileName.split("_");

                        if (datetime == null || datetime.length != 2) {
                            continue;
                        }
                        long tsbegin = Long.parseLong(datetime[0]);
                        long tsend = Long.parseLong(datetime[1]);
                        long lplist = 0;

                        lplist = System.currentTimeMillis();
                        System.out.println("SYSTEM CURRENT TIMESTAMP：" + lplist + "\n");

                        if (tsend <= lplist) {
                            file.delete();
                            System.out.println("PLAYLIST already end \n");
                            continue;
                        }
                    } catch (Exception e) {

                    }
                }
            }
        }
    }

    /*
    * begintime is the new playlist begin time
    * endtime is the new playlist end time
    * this function is to split the conflict playlist
    */
    public static void PlaylistSplit(long begintime, long endtime) {

        File dir = new File(USR_LOCAL_OCC_PLLIST);
//        File dir_default = new File(USR_LOCAL_OCC_PLLIST_DEFAULT);
//        File dir_rule = new File(USR_LOCAL_OCC_PLLIST_RULE);
//        File dir_inter = new File(USR_LOCAL_OCC_PLLIST_INTER);

        if (!dir.exists()) {
            dir.mkdirs();
        }


        File[] files = dir.listFiles(); // 该文件目录下文件全部放入数组

        if (files != null) {
            for (File file : files) {
                if (file.isDirectory()) { // 判断是文件还是文件夹
                    // Todo
                } else {
                    String strFileName = file.getName();
                    System.out.println("---" + strFileName);
                    System.out.println("PLAYLIST file.getAbsolutePath():" + file.getAbsolutePath());
                    System.out.println("PLAYLIST file.getParent():" + file.getParent());
                    String datetime[] = strFileName.split("_");
                    if (datetime == null || datetime.length != 2) {
                        continue;
                    }
                    long tsbegin = Long.parseLong(datetime[0]);
                    long tsend = Long.parseLong(datetime[1]);

                    if (tsend <= begintime || tsbegin >= endtime) {//no conflict
                        System.out.println("PLAYLIST don't deal");
                    } else if (tsbegin >= begintime && tsend <= endtime) {//complete conflict
                        System.out.println("PLAYLIST delete");
                        file.delete();
                    } else if (tsbegin < begintime && tsend < endtime) {
                        String filePath = file.getParent() + "/" + tsbegin + "_" + begintime;
                        File f = new File(file.getParent() + "/" + tsbegin + "_" + begintime);
                        file.renameTo(f);
                        updatePlaylist(f, 0, begintime);
                        System.out.println("PLAYLIST begin low repath :" + f.getAbsolutePath());
                    } else if (tsbegin > begintime && tsend > endtime) {
                        File f = new File(file.getParent() + "/" + endtime + "_" + tsend);
                        file.renameTo(f);
                        updatePlaylist(f, endtime, 0);
                        System.out.println("PLAYLIST end low repath :" + f.getAbsolutePath());
                    } else if (tsbegin < begintime && tsend > endtime) {
                        File f = new File(file.getParent() + "/" + endtime + "_" + tsend);
                        String filepath = file.getParent() + "/" + tsbegin + "_" + begintime;
                        String cmd = "cp -i " + file.getAbsolutePath() + " " + file.getParent() + "/" + tsbegin + "_" + begintime;
                        CmdUtils.runCommand(cmd, 1);
                        updatePlaylist(new File(filepath), 0, begintime);
                        file.renameTo(f);
                        updatePlaylist(f, endtime, 0);
                        System.out.println("PLAYLIST all repath :" + f.getAbsolutePath());
                    }
                }
            }
        }
    }

    public static void updatePlaylist(File file, long startDate, long endDate) {
        String content = null;
        try {
            content = FileUtils.readFileToString(file, Charset.forName("UTF-8"));
            log.info("The playlist content is:" + content);
            JSONObject jsonObject = JSONObject.parseObject(content);
            //转换日期输出格式
            if (startDate != 0) {
                Date date = new Date(startDate);
                SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                System.out.println("update start TIME：" + dateFormat.format(date));
                String strTimeDate = dateFormat.format(date);
                jsonObject.put("startDate", strTimeDate);
            }
            if (endDate != 0) {
                Date date = new Date(endDate);
                SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                System.out.println("update end TIME：" + dateFormat.format(date));
                String strTimeDate = dateFormat.format(date);
                jsonObject.put("endDate", strTimeDate);
            }
            FileUtils.writeStringToFile(file, jsonObject.toJSONString(), Charset.forName("UTF-8"));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    //解析播表推流
    public static void parsePlaylist(String path) {
        String content = null;
        try {
            content = FileUtils.readFileToString(new File(path), Charset.forName("UTF-8"));
            System.out.println("parsePlaylist path:" + path);
            log.info("The playlist content is:" + content);
            JSONObject jsonObject = JSONObject.parseObject(content);
            String mediaLists = jsonObject.getString("nameList");
            String strategy = jsonObject.getString("strategy");
            String areaTypeList = jsonObject.getString("areaTypeList");
            String repeatTimes = jsonObject.getString("repeatTimes");
            String startDate = jsonObject.getString("startDate");
            String endDate = jsonObject.getString("endDate");
            long tsbegin = 0;
            long tsend = 0;
            long lplist = 0;
            System.out.println("parsePlaylist startDate:" + startDate);
            System.out.println("parsePlaylist endDate:" + endDate);
            try {
                tsbegin = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").parse(startDate).getTime();
                tsend = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").parse(endDate).getTime();
                lplist = System.currentTimeMillis();
            } catch (ParseException e) {
                e.printStackTrace();
            }
            long waitbegintimerpl = tsbegin - lplist;
            if (waitbegintimerpl <= 0) {
                System.out.println("PLAYLIST START WAIT TIME waitendtimerpl HAVE ERROR：" + waitbegintimerpl + "\n");
                waitbegintimerpl = 0;
            }
            long waitendtimerpl = tsend - lplist;
            startTimerTask(strategy, waitbegintimerpl, waitendtimerpl, mediaLists);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void startTimerTask(String strategy, long waitbegintimerpl, long waitendtimerpl, String mediaLists) {
        if (strategy.equals("1")) {//只播一次
            OnceStartTimer.getInstance().destroyed();
            OnceEndTimer.getInstance().destroyed();
            try {
                Thread.sleep(500);//to exit thread and give time to recreate thread
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            OnceStartTimer.getInstance().start(true, waitbegintimerpl, mediaLists);
            OnceEndTimer.getInstance().start(true, waitendtimerpl, mediaLists);
        } else {//每天重复
            RepStartTimer.getInstance().destroyed();
            RepEndTimer.getInstance().destroyed();
            try {
                Thread.sleep(500);//to exit thread and give time to recreate thread
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            RepStartTimer.getInstance().start(true, waitbegintimerpl, mediaLists);
            RepEndTimer.getInstance().start(true, waitendtimerpl, mediaLists);
        }
    }

    public static boolean isLiveStreaming() {
        if (mffmpeg.query("LIVETORTMP") != null
                && mffmpeg.query("LIVETORTMP").getProcess() != null
                && mffmpeg.query("LIVETORTMP").getProcess().isAlive()) {
            System.out.println("The live thread is exist");
            return true;
        } else {
            return false;
        }
    }

    public static void stopLiveStreaming() {
        if (mffmpeg.stop("LIVETORTMP")) {
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        } else {
            log.info("REPEAT streaming stop fail!");
        }
    }

    public static boolean isOnceStreaming() {
        if (mffmpeg.query("ONCE") != null
                && mffmpeg.query("ONCE").getProcess() != null
                && mffmpeg.query("ONCE").getProcess().isAlive()) {
            System.out.println("The ONCE streaming thread is exist");
            return true;
        } else {
            return false;
        }
    }

    public static void stopOnceStreaming() {
        if (mffmpeg.stop("ONCE")) {
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        } else {
            log.info("once streaming stop fail!");
        }
    }

    public static boolean isDefaultStreaming() {
        if (mffmpeg.query("DEFAULT") != null
                && mffmpeg.query("DEFAULT").getProcess() != null
                && mffmpeg.query("DEFAULT").getProcess().isAlive()) {
            System.out.println("The DEFAULT streaming thread is exist");
            return true;
        } else {
            return false;
        }
    }

    public static boolean isRepeatStreaming() {
        if (mffmpeg.query("REPEAT") != null
                && mffmpeg.query("REPEAT").getProcess() != null
                && mffmpeg.query("REPEAT").getProcess().isAlive()) {
            System.out.println("The live thread is exist");
            return true;
        } else {
            return false;
        }
    }

    public static void stopRepeatStreaming() {
        if (mffmpeg.stop("REPEAT")) {
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        } else {
            log.info("REPEAT streaming stop fail!");
        }
    }

    public static void resetPlayTimerTask() {
        String playlist = getCurrPlayList();
        try {
            if (playlist == null) {
                System.out.println("resetPlayTimerTask playlist is null");
                DefaultStreamingTask.getInstance().stopDefaultStreaming();
                Thread.sleep(500);
                new Thread(DefaultStreamingTask.getInstance()).start();
                sendLiveMsg(STREAM_LIVE_START, STREAM_LOCAL_VOD, false);
            } else {
                //To do
                System.out.println("playlist exist");
                parsePlaylist(playlist);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static boolean isLocalLiveStreaming() {
        if (mffmpeg.query("LOCALLIVE") != null
                && mffmpeg.query("LOCALLIVE").getProcess() != null
                && mffmpeg.query("LOCALLIVE").getProcess().isAlive()) {
            log.info("The live thread is exist");
            return true;
        } else {
            return false;
        }
    }

    public static void startLiveStreaming(String addr) {
        try {
            if (!isLiveStreaming()) {
                mffmpeg.stop("LIVETORTMP");
                Thread.sleep(1000);
                mffmpeg.start("LIVETORTMP", "ffmpeg -re -i " + addr + " -vcodec copy -acodec aac -ar 44100 -strict -2 -f flv " + ADDR_LIVE);
                Thread.sleep(5000);
            }
        } catch (Exception e) {
        }
    }

    public static void startLocalLiveStreaming(String addr) {
        try {
            File dir = new File(DOWNLOAD_DEFAULT_VOD_MP4);
            File[] files = dir.listFiles(); // 该文件目录下文件全部放入数组
            if (files != null) {
                for (File file : files) {
                    System.out.println("startLocalLiveStreaming:" + file.getAbsolutePath());
                    if (!file.isDirectory() && (file.getName().endsWith("mp4"))) {
                        //mffmpeg.start("LOCALLIVE", "ffmpeg -re -i " + file.getAbsolutePath() + " -c copy -f flv " + STREAM_LOCAL_VOD);
                        String cmd = "ffmpeg -re -i " + file.getAbsolutePath() + " -c copy -f flv " + STREAM_LOCAL_VOD;
                        CmdUtils.runCommand(cmd, 1);
                    } else {
                        log.info("The local live file is not exist");
                    }
                }
            }
        } catch (Exception e) {
        }
    }

    public class StreamHandler extends Handler {

    }

    public static void playDefaultPlayList() {
        try {
            if (isDefaultStreaming()) {//if default streaming exist
                sendLiveMsg(STREAM_LIVE_START, STREAM_LOCAL_VOD, false);
            } else {
                File playfile = new File(DOWNLOAD_DEFAULT_VOD_MP4 + "output.mp4");
                if (playfile.exists()) {//if the default playfile exist
                    mffmpeg.start("DEFAULT", "ffmpeg -threads 2 -re -fflags +genpts -i " + DOWNLOAD_DEFAULT_VOD_MP4 + "output.mp4" + " -c copy -f flv " + STREAM_LOCAL_VOD);
                    sendLiveMsg(STREAM_LIVE_START, STREAM_LOCAL_VOD, false);
                }
            }
        } catch (Exception e) {
        }
    }
}
