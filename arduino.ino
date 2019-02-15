#include <Ultrasonic.h>
#include <Bounce2.h>
#define NUM_BUTTONS 5
const uint8_t BUTTON_PINS[NUM_BUTTONS] = {2, 3, 4, 5, 6};
Bounce* buttons = new Bounce[NUM_BUTTONS];	//宣告有 NUM_BUTTONS 個 button

#define positive_electrode_direction 8  //正極方向控制腳位
#define negative_electrode_direction 9  //正極方向控制腳位
#define echo_pin 10  //超音波感測器input腳位
#define trigger_pin 11//超音波感測器output腳位
#define floor_height 15 //樓層高度15cm

Ultrasonic ultrasonic(trigger_pin, echo_pin); //定義超音波感測器

float get_distance(){//測量距離，可參考網站 http://www.weichess.tw/wordpress/2188/alc07-arduino-ultrasonic-hc-sr04-module/
  digitalWrite(trigger_pin,LOW);
  delayMicroseconds(5);
  digitalWrite(trigger_pin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin,LOW);
  pinMode(echo_pin,INPUT);
  return ((pulseIn(echo_pin,HIGH))/2)/(29.1);
}

void motor_action(bool is_up){//若 is_up 是 true, 馬達會讓電梯向上
  if(is_up == true){
    digitalWrite(positive_electrode_direction,HIGH);
    digitalWrite(negative_electrode_direction,LOW);
  }
  else{
    digitalWrite(positive_electrode_direction,LOW);
    digitalWrite(negative_electrode_direction,HIGH);
  }
  Serial.println(get_distance());
}

void move_to_floor(int distance){ //傳入想要去樓層之離地表的距離
  while(distance < get_distance())
    motor_action(false);  //go down
  while(distance > get_distance())
    motor_action(true);   //go up

  digitalWrite(positive_electrode_direction,LOW);	//關閉正反電流的電位差
  digitalWrite(negative_electrode_direction,LOW);
}

void listen_buttom(){
  for (int i = 0; i < NUM_BUTTONS; i++)  {
    buttons[i].update();  // 載入按鈕狀態
    if(buttons[i].read()==LOW){	//按鈕被按下
      move_to_floor(floor_height*i+4);
      Serial.print(i);Serial.print("  ");Serial.println(get_distance());
    }
  }
}

void setup(){
  Serial.begin(9600);
  pinMode(trigger_pin,OUTPUT);
  pinMode(echo_pin,INPUT);
  
  pinMode(positive_electrode_direction,OUTPUT);
  pinMode(negative_electrode_direction,OUTPUT);

  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].attach(BUTTON_PINS[i] , INPUT_PULLUP);       //setup the bounce instance for the current button
    buttons[i].interval(25);              // interval in ms
  }
}

void loop(){
  listen_buttom();
}
