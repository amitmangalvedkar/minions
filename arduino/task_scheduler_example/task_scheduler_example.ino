//Refer to this example https://github.com/arkhipenko/TaskScheduler/blob/master/examples/Scheduler_example01/Scheduler_example01.ino 

#include <TaskScheduler.h>

// Callback methods prototypes
void t1_callback();
void t2_callback();
void t3_callback();

//Tasks
Task task1(2000, 10, &t1_callback);
Task task2(2000, TASK_FOREVER, &t2_callback);
Task task3(2000, TASK_FOREVER, &t3_callback);

Scheduler runner;


void t1_callback() {
    Serial.print("task1: ");
    Serial.println(millis());
}

void t2_callback() {
    Serial.print("task2: ");
    Serial.println(millis());  
}

void t3_callback() {
    Serial.print("task3: ");
    Serial.println(millis());
  
}

void setup () {
  Serial.begin(9600);
  Serial.println("Scheduler TEST");
  
  runner.init();
  Serial.println("Initialized scheduler");
  
  runner.addTask(task1);
  Serial.println("added task1");
  
  runner.addTask(task2);
  Serial.println("added task2");

  runner.addTask(task3);
  Serial.println("added task3");
  delay(10000);
  
  task1.enable();
  Serial.println("Enabled task1");
  task2.enable();
  Serial.println("Enabled task2");
  task3.enable();
  Serial.println("Enabled task3");  
}


void loop () {
  runner.execute();
}
