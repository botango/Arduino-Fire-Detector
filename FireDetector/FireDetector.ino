#define STOP_SW_PIN     2
#define DOOR_SW_PIN     3
#define SPK_PIN         4
#define LAMP_PIN        5
#define L_PIN           13

#define THRESHOLD  50

bool fDetected=false;
int c1=0;

void setup()
{
  pinMode(STOP_SW_PIN, INPUT_PULLUP);
  pinMode(DOOR_SW_PIN, INPUT_PULLUP);
  pinMode(SPK_PIN, OUTPUT);
  pinMode(LAMP_PIN, OUTPUT);
  pinMode(L_PIN, OUTPUT);

  digitalWrite(L_PIN, LOW);
  digitalWrite(LAMP_PIN, LOW);
  
  Serial.begin(115200);
  Serial.println("Fire Detector Start");

  analogRead(A0); // 何故か１回目の数値が安定しないので捨て読み込み
  delay(500);
}

void loop()
{
  int sensorValue;

  sensorValue = analogRead(A0);
  if(sensorValue>THRESHOLD)    fDetected = true;
  
  Serial.print(sensorValue);
  if(fDetected)    Serial.print("(Fire detected)");
  Serial.println();

  int f,p=100,c2=0;
  while(fDetected)
  {
    if(digitalRead(STOP_SW_PIN)==LOW || digitalRead(DOOR_SW_PIN)==LOW)
    {
      noTone(SPK_PIN);
      digitalWrite(L_PIN,LOW);
      digitalWrite(LAMP_PIN,LOW);
      fDetected=false;
      break;
    }

    digitalWrite(L_PIN,HIGH);
    digitalWrite(LAMP_PIN,HIGH);
    if(c2<4) p=1000;
    else if(c2<16) p=100;
    else
    {
      c2=0;
      continue;
    }
    
    for(f=635;f<=912;f++)
    {
      tone(SPK_PIN, f);
      delayMicroseconds(p);
      if(digitalRead(STOP_SW_PIN)==LOW || digitalRead(DOOR_SW_PIN)==LOW)  break;
    }
    for(f=911;f>=634;f--)
    {
      tone(SPK_PIN, f);
      delayMicroseconds(100);
      if(digitalRead(STOP_SW_PIN)==LOW || digitalRead(DOOR_SW_PIN)==LOW)  break;
    }
    c2++;
  }
  digitalWrite(L_PIN,(c1++ & 0x40 && digitalRead(DOOR_SW_PIN)));

  delay(10);
}
