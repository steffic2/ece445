void setup() {
  Serial.begin(9600);
}
String data;
String xStr;
String yStr;
int coordDiv;
int xCoord;
int yCoord;

void loop() {
  //if (Serial.available() > 0) {
    //String data = Serial.readStringUntil('\n');
    if(Serial.available() > 0){
      data = Serial.readStringUntil('\n');
      coordDiv = data.indexOf(",");
      xStr = data.substring(0, coordDiv);
      yStr = data.substring(coordDiv+1);
      xCoord = xStr.toInt();
      yCoord = xStr.toInt();
      Serial.println(xCoord);
    }
      
    
  }
//}
