
             ///QUẠT HOẠT ĐỘNG Ở CHẾ ĐỘ FULL-STEP- 4 TRÌNH TỰ
             ///NHIỆT KẾ Ở CHẾ ĐỘ HALF-STEP- 8 TRÌNH TỰ
int buttonPin2 = 2;     // dung de reset cac motor về vị trí home position

int sensorPin[] = { A0,  A1,  A2  };      // Khai báo mang gồm 3 thành phần chứa 3 chan analog cua 3 cam bien LM35
int motorpin1 = 6;                        // 6-9 lan lươt là các chân pin digital cua motor stepper quạt   
int motorpin2 = 7;                        // pin int put cua dong co quạt ( A- B - C- D)
int motorpin3 = 8;
int motorpin4 = 9;

int motorpin6 = 10;
int motorpin7 = 11;       // 10 - 13 la cac pin digital cua dong ho nhiệt kê( A- B - C- D);
int motorpin8 = 12;
int motorpin9 = 13;

int huongcu  = 0;         // Dat huong ban dau cho canh quat la huong 0 
int tempcu   = 0;         // Dat gia tri cho nhiet do ban dau la 0 Celius

 float temp[3];           //  Mang chua nhiet do cua cam bien trong moi lan doc
 int stepsPerDeg = 4;     //  Cho 1 độ C là 2.8125 độ, tức nữa step ở chế độ 64 +> 32 step/1'C
                          //  Nhiệt kế hoạt động ở chế độ half step-8 trình tự
                          //                Vậy để thực hiện 32 step : 32/8 = 4
                         
void setup(void) 
{
    Serial.begin(9600);
    pinMode(buttonPin2, INPUT);    // Khai báo chân Button 2 là đầu vào, khi HIGT thì sẽ quat tự do các động cơ,

    pinMode(sensorPin[0], INPUT);  // Khai báo các chân analog, đọc các giá trị điện  từ các cảm biến 0-1023  
    pinMode(sensorPin[1], INPUT);
    pinMode(sensorPin[2], INPUT);      

    pinMode(motorpin1,OUTPUT);    //  Thiết đặt các chân của động cơ quạt là output
    pinMode(motorpin2,OUTPUT);
    pinMode(motorpin3,OUTPUT);
    pinMode(motorpin4,OUTPUT);

    pinMode(motorpin6,OUTPUT);
    pinMode(motorpin7,OUTPUT);    //Thiết đặt các chân của động cơ nhiệt kế là output
    pinMode(motorpin8,OUTPUT);
    pinMode(motorpin9,OUTPUT);

       Serial.println(" Hay reset nhiet ke ve 0 do C ");
       delay(500);
       resetdongho();  // reset đồng hồ về vị trí ban đầu
       delay(2000);
       Serial.println(" Hay reset dong co quat ve vi tri dau ");
       delay(500);
       resetquat();    // reset  quat ve vi tri ban dau
       delay(2000);
      
}

void loop(void) 
{  /// Đọc nhiệt độ từ các cảm biến, tìm hướng và nhiệt độ của cảm biên có nhiệt độ cao nhất cao nhất 
      temp[0] = Temp(0);           //đọc NHiệt độ của 3 cảm biến lưu vào mảng
      temp[1] = Temp(1);           // Hàm Temp ở bên dưới
      temp[2] = Temp(2);
         
Serial.println( temp[0] );
Serial.println( temp[1] );
Serial.println( temp[2] );      // in ra nhiet do 3 cam bien

     float  tmax  = temp[0];    // cho nhiet do max ban đầu ở vị trí 0 ;
       int  huong = 0;          // Cho huong có nhiệt độ Max  ban đầu là huong 0;
     
  for(int i = 1; i<=2 ; i++)
  {
    if( temp[i] > tmax){       // tim nhiet do max trong mang
          tmax  = temp[i];     // "huong" duoc đặt lại bằng huong co nhiet do cao nhat
          huong = i  ;     
                       }       
          
  }
                Serial.print("Huong quay: ");    //In ra huong động cơ quạt sẽ quay
                Serial.print( huong + 1 );        
                Serial.print(" - Nhiet do:= ");  // In ra nhiệt độ cao nhất
                Serial.println( temp[huong] );
   /// Quay motor stepper quat đến vị trí cảm biến có nhiệt độ cao nhất 
      int diff  = huong - huongcu   ;         // so goc cần thay đổi = goc moi tru goc cu
       huongcu  = huong;                      // set huong cu bằng hướng mới. 
       
     for( int i = 1 ; i<= abs(diff) ; i++)    // Mỗi 1 sự thay đổi về góc thay đổi về góc có nghĩa đông cơ se quay 90'.       
            {     if( diff < 0) quaynguoc();  // Nếu góc thay đổi nhỏ hơn 0 thì quay ngược kim đông hồ
                  else quaythuan();           // Nếu lớn hơn 0 thì quay thuận ....
    
            
   /// Quay motor nhiet kế đến vi trí nhiệt độ         } 

  int newTemp  =  (int)temp[huong];       // doc gia tri nhiet do mới từ cam bien co nhiet do cao nhat
  int tempDiff = newTemp - tempcu;        // temDiff là sự chênh lệch nhiệt độ giữa mới và cũ
        tempcu = newTemp;                 //   dat lai gia tri nhiet do moi

  bool tempDir = true;                     
  if (tempDiff < 0)                        // dat huong chuyen dong, neu nhiet do moi cao hon thi quay thuan, nho hon nhieut do cu thi quay nguowc   
  {    tempDir = false;  }                 // 1`C quay 4 lan tuwc 2.8125' nua step
   
  
   moveMotors(tempDiff, tempDir);         // Ham  goi chay theo nhiet do can thay doi và chiều cần quay
   delay(2000);                           // 2`Celius thì roto quay hết 1 vòng: Tức 2`C = 5.625' : 64 xung;
                                          // 1`Celius thì roto quay hết 1/2 vòng: Tức 1`C = 2.8125 : 34 xung;
}
}
 void quaythuan()     //Ham quay canh quat theo chieu thuan 90'
 {
 for(int i=1; i<=512/4 ; i++){              // huóng quat quay ở chế độ  FULL STEP: 32 step để roto quay hết 1 vong
                                            //               Tỉ lệ bộ giảm tốc là: 1/64  => Quay 360' cần 32*64 = 2048            
                                            //                                           =>       90' cần       = 512 step   
             digitalWrite(motorpin1,HIGH);              //512 xung nhưng vì ta cấp điện theo trình tự 4 bước nên                 
             delay(2);                                  //Cho vòng lặp for từ 1 đến 512/4 =128
             digitalWrite(motorpin1,LOW);               // Mỗi lần thực hiện 4 xung cấp điện theo trình tự từ A -> D
                                                        // Các stato của động cơ  lần lượt HIGH rồi LOW theo chiều thuận kim đồng ho
             digitalWrite(motorpin2,HIGH);              // đê roto quay
             delay(2);
             digitalWrite(motorpin2,LOW);

              digitalWrite(motorpin3,HIGH);              
              delay(2);
              digitalWrite(motorpin3,LOW);

              digitalWrite(motorpin4,HIGH);
              delay(2);
              digitalWrite(motorpin4,LOW);
              delay(2);  }
           }
 
 void quaynguoc() //   quat quay nguoc
 {
 for(int i=1; i<=512/4 ; i++){
             digitalWrite(motorpin4,HIGH);
             delay(2);
             digitalWrite(motorpin4,LOW);
     
             digitalWrite(motorpin3,HIGH);
             delay(2);
             digitalWrite(motorpin3,LOW);

              digitalWrite(motorpin2,HIGH);
              delay(2);
              digitalWrite(motorpin2,LOW);

              digitalWrite(motorpin1,HIGH);
              delay(2);
              digitalWrite(motorpin1,LOW);
              delay(2); 
 
 
 }
 }
                                      // cứ 2 do C la quay 64 xung
                                      // Cap 8 xung lien tục  =>> 2`C lặp vòng lặp 8 xung 8 lần   // voi goc 5.608 do
                                      // Vậy 1 do C thi quay 32 step  // tuc nua vong
                                      // 1o C la 4 xung 2 do C la 8 xung

void resetquat()     // resset canh quat
{
  while(  digitalRead(buttonPin2) == HIGH )
  {   
      digitalWrite(motorpin1,HIGH);
      delay(2);
      digitalWrite(motorpin1,LOW);

     digitalWrite(motorpin2,HIGH);
     delay(2);
     digitalWrite(motorpin2,LOW);

     digitalWrite(motorpin3,HIGH);
     delay(2);
     digitalWrite(motorpin3,LOW);

     digitalWrite(motorpin4,HIGH);
     delay(2);
     digitalWrite(motorpin4,LOW);
     delay(2);
  }
}
void resetdongho()     // resset canh quat
{
  while( digitalRead(buttonPin2) == HIGH )
  {   
       digitalWrite(motorpin6,HIGH);                   
      delay(2);
      digitalWrite(motorpin6,LOW);

     digitalWrite(motorpin7,HIGH);
     delay(2);
     digitalWrite(motorpin7,LOW);

     digitalWrite(motorpin8,HIGH);
     delay(2);
     digitalWrite(motorpin8,LOW);

     digitalWrite(motorpin9,HIGH);
     delay(2);
     digitalWrite(motorpin9,LOW);
     delay(2);
  }
}
                                                
void moveMotors(int tempDiff , bool tempDir) //Ham goi chay kim nhiệt kế theo so nhiệt đọ thay đổi vaf chieu cần quay
{   
    
                                                  // Kim nhiệt kế quay theo chế độ Half Step: Rotor quay 1 vòng cần 64 step: 5.625`                                           
                                                  //           Tỉ lệ bộ giảm tốc là: 1/64 => Quay 360' cần 64*64 = 4096 step 
                                                  //                          1' Celius cần:   2.8125' cần       = 32 step       
                                             
                                                 // P/s nếu 2.8125'/1'C là hợp lý vì dễ tinn toán, Lúc này ta có thể biểu dien được 128 Giá trị tren 360;
    int tempSteps = abs(stepsPerDeg * tempDiff); // So step can có đe thực hiện sự thay đổi tempDiff                                                                          
    if(tempDir){                                 // true thì quay thuận chiều kim đồng hồ
  for(int i = 1; i <= tempSteps  ;  i++ ){       // 1 vòng lặp for có 8 lần thay đổi xung: mà 1 do C lần thay đỏi 32 xung lần tempSteps = nhiệt đọ *4    
  digitalWrite(motorpin6,HIGH);                  // A HIGH
  delay(2);                                                                    
  digitalWrite(motorpin7,HIGH);                  // A-B  HIGH    
  delay(2);                               
  digitalWrite(motorpin6,LOW);                   // B HIGH
  delay(2);
  digitalWrite(motorpin8,HIGH);                  // B-C HIGH
  delay(2);
  digitalWrite(motorpin7,LOW);                   // C HIGH 
  delay(2);                            
  digitalWrite(motorpin9,HIGH);                  // C-D HIGH
  delay(2);
  digitalWrite(motorpin8,LOW);                   // D HIGH
   delay(2); 
  digitalWrite(motorpin6,HIGH);                  // D-A HIGH 
  delay(2);
   digitalWrite(motorpin9,LOW);                  //ABCD LOW
   digitalWrite(motorpin6,LOW);
 }

   delay(5);
  }
  else{
  for(int i=0; i < tempSteps   ; i++ ){           
  digitalWrite(motorpin9,HIGH);
  delay(2);
  digitalWrite(motorpin8,HIGH);
  delay(2);
  digitalWrite(motorpin9,LOW);
  delay(2);
  
  digitalWrite(motorpin7,HIGH);
  delay(2);
  digitalWrite(motorpin8,LOW);
  delay(2);
  digitalWrite(motorpin6,HIGH); 
  delay(2);
  digitalWrite(motorpin7,LOW);
   delay(2);
 digitalWrite(motorpin9,HIGH);
   delay(2);
   
  digitalWrite(motorpin6,LOW); 
  digitalWrite(motorpin9,LOW);
   delay(2);
    }
            delay(5);
  }
}

  float Temp(int i)
{
  float reading = analogRead(sensorPin[i]);   // đọc giá trị điện áp từ cảm biến LM35 // trả về 1 giá trị trong khoảng 0 -1023 tương ứng điện áp mặc định ( 0 - 5V)
  float voltage = reading *5.0/1023.0;        // tính giá trị hiệu điện thế (Volt)từ giá trị cảm biến = giá trị điện áp từ cảm biến chia cho mức analog cao nhất (1023) rồi nhân với 5V ( nguồn cấp)
  float tempc = voltage *100.0;               //đổi giá trị điện thế sang độ C // cảm biến có nhiệt độ thay đổi tuyến tính 10mV/C
  return tempc;
}
