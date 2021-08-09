//////cnc///////////
#define EN        8     // stepper motor enable , active low
#define X_DIR   5    // X -axis stepper motor direction control
#define Y_DIR   6    // y -axis stepper motor direction control
#define Z_DIR   7    // z axis stepper motor direction control
#define X_STP  2   // x -axis stepper control
#define Y_STP  3   // y -axis stepper control
#define Z_STP  4   // z -axis stepper control
#define limitswxleft 52
#define limitswylow 53
int posi_x=0;
int posi_y=0;
int posi_z=0;
unsigned long last=0;
unsigned long last2=0;
String readforpyserial ;
String bottle ="0";
char layer =0 ;
char positio_n =0 ;
int county;
int countx;
int layer_0 = 0;
int layer_1 = 30;    // 10เท่ากับ1เซนติเมตร
int layer_2 = 193;
int layer_3 = 363;
int position_0 =0 ;
int position_1 =5 ;
int position_2 =197;
int position_3 =396;  //390 max395
int position_4 =4;
int position_5 =199;
int position_6 =387;
int position_7 =7;
int position_8 =203;
int position_9 =387;
int timemoter=1500;//speed
bool chack =false;
/////////relay///////////////////
#include "DHT.h"
# define DHTPIN 14   //14
#define DHTTYPE DHT22
#define lightforcam 15 //in1
#define light5v 16 // in2
#define lightfortree 17 //in3
#define cnc 18    //in4
#define tempcontrol 19//in5
#define hum1  20
#define hum2 21

int state_lightforcam=0;
int state_light5v=0;
int state_lightfortree=0;
int state_cnc=0;
int state_tempcontrol=0;


DHT dht (DHTPIN,DHTTYPE);
unsigned long time=0 ;
double maxtemp;
double mintemp;
double h ;
double t ;
boolean a = false , b= true;
void setup() {
///cnc
  pinMode (Y_DIR, OUTPUT); pinMode (Y_STP, OUTPUT);
   pinMode (X_DIR, OUTPUT); pinMode (X_STP, OUTPUT);
  pinMode (EN, OUTPUT);
 digitalWrite (EN, LOW);
 Serial.begin(9600);
//relay
dht.begin();
  pinMode(lightforcam,OUTPUT);
  pinMode(lightfortree,OUTPUT);
  pinMode(cnc,OUTPUT);
  pinMode(tempcontrol,OUTPUT);
  pinMode(light5v,OUTPUT);
  pinMode(limitswxleft,INPUT);
  pinMode(limitswylow,INPUT);


  digitalWrite(lightforcam,1);
  digitalWrite(cnc,1);
  digitalWrite(tempcontrol,1);
  digitalWrite(light5v,1);
  digitalWrite(lightfortree,1);
  
  settemp();
 
}

void loop() {
  
showdata ();
if(isnan(t) || isnan(h)){
  }
  else{
      if(t>=maxtemp){ //35 >= 39

        digitalWrite(tempcontrol,0); // on
        state_tempcontrol=1;
          }
         if(t<=mintemp) { //35 <= 38
        //Serial.println("off");
          digitalWrite(tempcontrol,1); // off
          state_tempcontrol=0;
          }
  }
    motorx();
    motory();
}
//////////

void findzeroxaxis (){
do{
  x_down();
  }
while(digitalRead(limitswxleft)==0);
    countx=0;
}

void findzeroyaxis (){
do{
  y_down();
  }
while(digitalRead(limitswylow)==0);
    county=0;
}
///////////////////////////////
/////////////
void showdata (){
  if(millis()- last > 1000 ){ //&& chack ==true
    h = dht.readHumidity();
    t = dht.readTemperature();
    if (isnan(t) || isnan(h)) {
Serial.println("dht NAN");
  } else {

Serial.println("bottle:"+String(bottle)+';'+"axisx:"+String(countx)+';'+"axisy:"+String(county)
+';'+"temp:"+String(t)+';'+"humi:"+String(h)+';'+"state_lightforcam:"+String(state_lightforcam)+";"+"state_light5v:"+String(state_light5v)+";"
+"state_lightfortree:"+String(state_lightfortree)+";"+"state_cnc:"+String(state_cnc)+";"+"state_tempcontrol:"+String(state_tempcontrol));

  }
last= millis();
Serial.flush();
}
}

///////funtion for relay
void settemp (){

        //Serial.println("max temp");
            while(Serial.available() == 0) {
            showdata ();
        }
        readforpyserial=Serial.readStringUntil('\n');
        maxtemp =readforpyserial.toDouble();
        Serial.println("maxtemp "+String(maxtemp));

        //////////////////
             //   Serial.println("min temp");
             while(Serial.available() == 0) {
             showdata ();
        }
        readforpyserial=Serial.readStringUntil('\n');
        mintemp =readforpyserial.toDouble();
        Serial.println("mintemp "+String(mintemp));
  }
void serialEvent() 
{

  //relay
  if(Serial.available()>0){

//////////////
   chack = true;
  
  readforpyserial=Serial.readStringUntil('\n');
 
      if(readforpyserial== "7")
      {
       layer = '3';
       positio_n='7';
       bottle="7";
        
      }
     else if(readforpyserial== "8")
      {
        layer = '3';
        positio_n ='8';
         bottle="8";
      }
      else  if(readforpyserial== "9")
      {
        layer = '3';
        positio_n='9';
        bottle="9";
      }
     else if(readforpyserial=="4")
      {
       layer= '2';
       positio_n ='4';
       bottle="4"; 
       }
     else if(readforpyserial== "5")
      {
       layer='2';
       positio_n ='5';
       bottle="5"; 
       }
       else if(readforpyserial== "6")
       {
        layer = '2';
        positio_n ='6';
        bottle="6";
        }
      else  if(readforpyserial== "1")//1
      {
        layer= '1';
        positio_n = '1';
        bottle="1";
        }
      else  if(readforpyserial== "2")//2
      {
        layer= '1';
        positio_n = '2';
        bottle="2";
        }   
    else   if(readforpyserial== "3")//3
      {
        layer='1';
        positio_n= '3';
        bottle="3";
      }
     else if(readforpyserial== "0")
      {
      layer='0';
      positio_n= '0';
      bottle="0";
        }    
   if(readforpyserial == "oncnc")
    {
      
    digitalWrite(cnc,0);
    state_cnc=1;
    findzeroxaxis ();
    findzeroyaxis ();
    digitalWrite(light5v,0);
    state_light5v=1;
    
    }
    if(readforpyserial == "offcnc")
    {
    digitalWrite(cnc,1);
    state_cnc=0;
    digitalWrite(light5v,1);
    state_light5v=0;
    
    }
    if(readforpyserial =="onlightforcam")
    {
      digitalWrite(lightforcam , 0);
      state_lightforcam=1;
      }
    if(readforpyserial =="offlightforcam")
    {
      digitalWrite(lightforcam , 1);
      state_lightforcam=0;
      
      
      
      }
    if(readforpyserial =="onlight5v")
    {
      digitalWrite(light5v,0);
      state_light5v=1;
      }
    if(readforpyserial =="offlight5v")
    {
      digitalWrite(light5v,1);
      state_light5v=0;
      }
    if(readforpyserial =="onlightfortree")
    {
      digitalWrite(lightfortree,0);
      state_lightfortree=1;
      }
    if(readforpyserial =="offlightfortree")
    {
      digitalWrite(lightfortree,1);
      state_lightfortree=0;
      }

    if(readforpyserial== "findzero"){
      findzeroxaxis();
      findzeroyaxis();
      layer='0';
      positio_n= '0';
       bottle="0";

      }

      //////////////////////////////////////
    if(readforpyserial =="settemp")
    {
      settemp();
      }
  }
}
///////////////////cnc
/////////////////////////////////////////////////////////////////////////////////////////// 

void stepYre(boolean dir, byte dirPin, byte stepperPin, int steps)
{
  digitalWrite (dirPin,dir);

  for (int i = 0;i<steps;i++){  
      digitalWrite (stepperPin, HIGH);

      delayMicroseconds(timemoter);
      digitalWrite (stepperPin,LOW);

      delayMicroseconds(timemoter);
  }

}
void stepYfor(boolean dir, byte dirPin, byte stepperPin, int steps)
{
  digitalWrite (dirPin,dir);

  for (int i = 0;i<steps;i++){  
      digitalWrite (stepperPin, HIGH);

      delayMicroseconds(timemoter);
      digitalWrite (stepperPin,LOW);

      delayMicroseconds(timemoter);
  }

}
//////////////////////////////////////////////////
void stepXre(boolean dir, byte dirPin, byte stepperPin, int steps)
{
  digitalWrite (dirPin,dir);

  for (int i = 0;i<steps;i++){  
      digitalWrite (stepperPin, HIGH);

      delayMicroseconds(timemoter);
      digitalWrite (stepperPin,LOW);

      delayMicroseconds(timemoter);
  }

}
void stepXfor(boolean dir, byte dirPin, byte stepperPin, int steps)
{
  digitalWrite (dirPin,dir);

  for (int i = 0;i<steps;i++){  
      digitalWrite (stepperPin, HIGH);

      delayMicroseconds(timemoter);
      digitalWrite (stepperPin,LOW);

      delayMicroseconds(timemoter);
  }

}
/////////////////////////////////////////////////
void y_up(){
  
  stepYfor(true,Y_DIR,Y_STP,5); // Y axis motor forward 1 ring, the 200 step is a circle.
  }                                //50 =1cm
                                  //200 = 4 cm
void y_down(){
  
  stepYre(false,Y_DIR,Y_STP,5); // X axis motor reverse 1 ring, the 200 step is a circle.
  }
  /////////////////////////////////////////////////
void x_down(){ //right

  
  stepXfor(true,X_DIR,X_STP,5); // Y axis motor forward 1 ring, the 200 step is a circle.
  }

void x_up(){//left


  stepXre(false,X_DIR,X_STP,5); // X axis motor reverse 1 ring, the 200 step is a circle.

  }

 void motory(){
    /////////////////////////////////////////////////////////////////////

  if(layer=='0')
  {
        if(county<=layer_3||county>=layer_0){
    
    while(county<=layer_3&&county>layer_0){ // เริ่ม150 
      
      relayinterrupt ();
      county--;
      y_down();
      }
      }
  }
  if(layer=='1')
  {
  if(county>=layer_0){
    
    while(county<layer_1&&county>=layer_0){ // เริ่ม0 
      //showdata ();
      relayinterrupt ();
      county++;
      //showdata ();
      y_up();
      }
  }
        if(county==layer_3||county==layer_2){
    
    while(county<=layer_3&&county>layer_1){ // เริ่ม150 
      //showdata ();
      relayinterrupt ();
      county--;
      //showdata ();
      y_down();
      }
      }
  }
 

///////////////////////////////////////////
if(layer=='2')
  {
  if(county>=layer_0||county==layer_1){
    
    while(county<layer_2&&county>=layer_0){ // เริ่ม0 
      //showdata ();
      relayinterrupt ();
      county++;
      //showdata ();
      y_up();
      
      }
  }

        if(county==layer_3){
    
    while(county>layer_2&&county>=layer_0){ // เริ่ม150 
      //showdata ();
      relayinterrupt ();
      county--;
     //showdata ();
      y_down();
      }
      }  
  }

  //////////////////////////////////////////////////////////////
  if(layer=='3')
  {
  if(county>=layer_0||county==layer_1||county==layer_2){
    
    while(county<layer_3&&county>=layer_0){ // เริ่ม0 
      //showdata ();
      relayinterrupt ();
      county++;
      //showdata ();
      y_up();
      }
  } }
  
  }

  void motorx ()
  {

      if(positio_n=='0'){

      if( countx>position_0){//150 100 >>>50
        
        while(countx<=position_3&&countx>position_0){
        //showdata ();
        relayinterrupt ();          
        countx--;
      //showdata ();
        x_down();
        }
        }
    
    }
    //////////////////////////////////////////
       if(positio_n=='1'){
    if(countx>=position_0 ||countx==position_1){

      while(countx>=position_0&& countx<position_1){
        //showdata ();
        relayinterrupt ();
        countx++;
       //showdata ();
        x_up();
        }
      
      }
      
      if( countx>position_0){//150 100 >>>50
        
        while((countx<=position_6 ||countx<=position_3 ||countx<=position_9)&&countx>position_1){
        //showdata ();
        relayinterrupt ();  
        countx--;
       //showdata ();
        x_down();
        }
        }
    
    }
    ////////////////////////////////////////////////////////////////
   if(positio_n=='2'){
    if(countx>=position_0 ||countx==position_2){

      while(countx>=position_0&& countx<position_2){
        //showdata ();
        relayinterrupt ();
        countx++;
      //showdata ();
        x_up();
        }
      
      }
      
      if( countx>position_0){//150 100 >>>50
        
        while((countx<=position_6 ||countx<=position_3 ||countx<=position_9)&&countx>position_2){
         //showdata ();
         relayinterrupt (); 
        countx--;
      //showdata ();
        x_down();
        }
        }
    
    }
  //////////////////////////////////////////////////////////  
     if(positio_n=='3'){
    if(countx>=position_0 ||countx==position_3){

      while(countx>=position_0&& countx<position_3){
        //showdata ();
        relayinterrupt ();
        countx++;
      //showdata ();
        x_up();
        }
      }
    }  
    ////////////////////////////
        //////////////////////////////////////////
       if(positio_n=='4'){
    if(countx>=position_0 ||countx==position_4){
      
      while(countx>=position_0&& countx<position_4){
        //showdata ();
        relayinterrupt ();
        countx++;
       //showdata ();
        x_up();
        }
      
      }
      
      if( countx>position_0){//150 100 >>>50
        
        while((countx<=position_6 ||countx<=position_3 ||countx<=position_9)&&countx>position_4){
         //showdata ();
         relayinterrupt (); 
        countx--;
       //showdata ();
        x_down();
        }
        }
    
    }
    ////////////////////////////////////////////////////////////////
   if(positio_n=='5'){
    if(countx>=position_0 ||countx==position_5){

      while(countx>=position_0&& countx<position_5){
        //showdata ();
        relayinterrupt ();
        countx++;
       //showdata ();
        x_up();
        }
      
      }
      
      if( countx>position_0){//150 100 >>>50
        
        while((countx<=position_6 ||countx<=position_3 ||countx<=position_9)&&countx>position_5){
        //showdata ();
        relayinterrupt ();  
        countx--;
      //showdata ();
        x_down();
        }
        }
    
    }
  //////////////////////////////////////////////////////////  
     if(positio_n=='6'){
    if(countx>=position_0 ||countx==position_6){

      while(countx>=position_0&& countx<position_6){
        //showdata ();
        relayinterrupt ();
        countx++;
      //showdata ();
        x_up();
        }
      }
      
    }  
    ////////////////////////////
        //////////////////////////////////////////
       if(positio_n=='7'){
    if(countx>=position_0 ||countx==position_7){

      while(countx>=position_0&& countx<position_7){
        //showdata ();
        relayinterrupt ();
        countx++;
       //showdata ();
        x_up();
        }
      
      }
      
      if( countx>position_0){//150 100 >>>50
        
        while((countx<=position_6 ||countx<=position_3 ||countx<=position_9)&&countx>position_7){
          //showdata ();
          relayinterrupt ();
        countx--;
       //showdata ();
        x_down();
        }
        }
    
    }
    ////////////////////////////////////////////////////////////////
   if(positio_n=='8'){
    if(countx>=position_0 ||countx==position_8){

      while(countx>=position_0&& countx<position_8){
        //showdata ();
        relayinterrupt ();
        countx++;
       //showdata ();
        x_up();
        }
      
      }
      
      if( countx>position_0){//150 100 >>>50
        
        while((countx<=position_6 ||countx<=position_3 ||countx<=position_9)&&countx>position_8){
       //showdata ();
       relayinterrupt ();   
        countx--;
      //showdata ();
        x_down();
        }
        }
    
    }
  //////////////////////////////////////////////////////////  
     if(positio_n=='9'){
    if(countx>=position_0 ||countx==position_9){

      while(countx>=position_0&& countx<position_9){
        //showdata ();
        relayinterrupt ();
        countx++;
      //showdata ();
        x_up();
        }
      }
    }  
    ////////////////////////////
    }

 void relayinterrupt (){
    
    
    
        if(t>=maxtemp){ //35 >= 39
          //Serial.println("on");
        digitalWrite(tempcontrol,0); // on

        }
       //else
       if(t<=(mintemp+0.5)) { //35 <= 38
        //Serial.println("off");
          digitalWrite(tempcontrol,1); // off
          }
  
  if(Serial.available()>0){

//////////////
   chack = true;
  
  readforpyserial=Serial.readStringUntil('\n');
     if(readforpyserial== "7")
      {
       layer = '3';
       positio_n='7';
       bottle="7";
        
      }
     else if(readforpyserial== "8")
      {
        layer = '3';
        positio_n ='8';
         bottle="8";
      }
      else  if(readforpyserial== "9")
      {
        layer = '3';
        positio_n='9';
        bottle="9";
      }
     else if(readforpyserial=="4")
      {
       layer= '2';
       positio_n ='4';
       bottle="4"; 
       }
     else if(readforpyserial== "5")
      {
       layer='2';
       positio_n ='5';
       bottle="5"; 
       }
       else if(readforpyserial== "6")
       {
        layer = '2';
        positio_n ='6';
        bottle="6";
        }
      else  if(readforpyserial== "1")//1
      {
        layer= '1';
        positio_n = '1';
        bottle="1";
        }
      else  if(readforpyserial== "2")//2
      {
        layer= '1';
        positio_n = '2';
        bottle="2";
        }   
    else   if(readforpyserial== "3")//3
      {
        layer='1';
        positio_n= '3';
        bottle="3";
      }
     else if(readforpyserial== "0")
      {
      layer='0';
      positio_n= '0';
      bottle="0";
        }    
   if(readforpyserial == "oncnc")
    {
      
    digitalWrite(cnc,0);
    state_cnc=1;
    findzeroxaxis ();
    findzeroyaxis ();
    digitalWrite(light5v,0);
    state_light5v=1;
    
    }
    if(readforpyserial == "offcnc")
    {
    digitalWrite(cnc,1);
    state_cnc=0;
    digitalWrite(light5v,1);
    state_light5v=0;
    
    }
    if(readforpyserial =="onlightforcam")
    {
      digitalWrite(lightforcam , 0);
      state_lightforcam=1;
      }
    if(readforpyserial =="offlightforcam")
    {
      digitalWrite(lightforcam , 1);
      state_lightforcam=0;
      
      
      
      }
    if(readforpyserial =="onlight5v")
    {
      digitalWrite(light5v,0);
      state_light5v=1;
      }
    if(readforpyserial =="offlight5v")
    {
      digitalWrite(light5v,1);
      state_light5v=0;
      }
    if(readforpyserial =="onlightfortree")
    {
      digitalWrite(lightfortree,0);
      state_lightfortree=1;
      }
    if(readforpyserial =="offlightfortree")
    {
      digitalWrite(lightfortree,1);
      state_lightfortree=0;
      }

    if(readforpyserial== "findzero"){
      findzeroxaxis();
      findzeroyaxis();
      layer='0';
      positio_n= '0';
       bottle="0";

      }

      //////////////////////////////////////
    if(readforpyserial =="settemp")
    {
      settemp();
      }
  }
  
  }
