import org.quartz.Scheduler;
  import org.quartz.SchedulerException;
  import org.quartz.impl.StdSchedulerFactory;
  import static org.quartz.JobBuilder.*;
  import static org.quartz.TriggerBuilder.*;
  import static org.quartz.SimpleScheduleBuilder.*;
  import static org.quartz.CronScheduleBuilder.*;
import static org.quartz.DateBuilder.*;
import org.quartz.JobDetail;
import org.quartz.Trigger;


  public class QuartzTest {

      public static void main(String[] args) {
          Object locker = new Object();
          try {
              // Grab the Scheduler instance from the Factory
              Scheduler scheduler = StdSchedulerFactory.getDefaultScheduler();

              // and start it off
              scheduler.start();
			  
			  // define the job and tie it to our HelloQuartz class
			  JobDetail job = newJob(HelloQuartz.class)
				  .withIdentity("job1", "group1")
				  .usingJobData("name", "Hello World!")
				  .build();

			  // Trigger the job to run now, and then repeat every 10 seconds
			  /*Trigger trigger = newTrigger()
				  .withIdentity("trigger1", "group1")
				  .startNow()
						.withSchedule(simpleSchedule()
						  .withIntervalInSeconds(10)
						  .repeatForever())            
				  .build();*/
				  
		      Trigger trigger = newTrigger()
				.withIdentity("trigger3", "group1")
				//.withSchedule(dailyAtHourAndMinute(17, 22))
				.withSchedule(cronSchedule("0 52 18 31 10 ? 2018"))
				//.forJob(myJobKey)
				.build();

			  // Tell quartz to schedule the job using our trigger
			  scheduler.scheduleJob(job, trigger);
			  
			  
			  try {
			     //Thread.sleep(60000);
				 locker.wait();
			  } catch (InterruptedException e) {
			  }

              scheduler.shutdown();

          } catch (SchedulerException se) {
              se.printStackTrace();
          }
      }
  }