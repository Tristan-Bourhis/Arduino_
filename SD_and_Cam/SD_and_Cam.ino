#include <SPI.h>
#include <arduino.h>
#include <SD.h>

#define PIC_PKT_LEN    256                  //data length of each read, dont set this too big because ram is limited
#define PIC_FMT_VGA    7
#define PIC_FMT_CIF    5
#define PIC_FMT_OCIF   3
#define CAM_ADDR       0
#define CAM_SERIAL     Serial

#define PIC_FMT        PIC_FMT_VGA

File myFile;


const byte cameraAddr = (CAM_ADDR << 5); // address
unsigned char pkt[PIC_PKT_LEN];
unsigned int pktCnt;
unsigned int lastPacketLength;
unsigned long picTotalLen = 0;            // picture length
int picNameNum = 0;

/*********************************************************************/
void setup()
{
    Serial.begin(9600);
    Serial.println("Initializing SD card....");
    pinMode(10,OUTPUT);          // CS pin of SD Card Shield

    if (!SD.begin(10)) 
    {
        Serial.print("sd init failed");
        return;
    }
    Serial.println("sd init done.");
    cameraInit();
}
/*********************************************************************/
void loop()

{                              
    if(millis() < 5000 ) {
      Serial.println("\r\nbegin to take picture");
      delay(200);
      Capture();
      GetData();
      Serial.println("pic done");
    } 
    delay(5000);
             
}
/*********************************************************************/
void clearRxBuf()
{
    while (Serial.available())
    {
        Serial.read();
    }
}
/*********************************************************************/
void sendCmd(char cmd[], int cmd_len)
{
    for (char i = 0; i < cmd_len; i++) Serial.print(cmd[i]);
}
/*********************************************************************/
void cameraInit(){
    char cmd[] = {0xaa,0x0d|cameraAddr,0x00,0x00,0x00,0x00} ;
    unsigned char resp[6];

    Serial.setTimeout(500);
    while (1){
        sendCmd(cmd,6);

        if(Serial.readBytes((char *)resp, 6) != 6)
            continue;

        if(resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x0d && resp[4] == 0 && resp[5] == 0) {
            if(Serial.readBytes((char *)resp, 6) != 6) continue;
            if(resp[0] == 0xaa && resp[1] == (0x0d | cameraAddr) && resp[2] == 0 && resp[3] == 0 && resp[4] == 0 && resp[5] == 0) break;
        }
    }

    cmd[1] = 0x0e | cameraAddr;
    cmd[2] = 0x0d;
    sendCmd(cmd, 6);

    char cmd2[] = { 0xaa, 0x01 | cameraAddr, 0x00, 0x07, 0x03, 0x05};

    Serial.setTimeout(100);

    while (1){
        clearRxBuf();
        sendCmd(cmd2, 6);
        if(Serial.readBytes((char *)resp, 6) != 6) continue;
        if(resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x01 && resp[4] == 0 && resp[5] == 0) break;
    }

    char cmd3[] = { 0xaa, 0x06 | cameraAddr, 0x08, PIC_PKT_LEN & 0xff, (PIC_PKT_LEN>>8) & 0xff ,0};

    while (1) {
        clearRxBuf();
        sendCmd(cmd3, 6);
        if(Serial.readBytes((char *)resp, 6) != 6) continue;
        if(resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x06 && resp[4] == 0 && resp[5] == 0) break;
    }

  //Serial.println("Camera initialization done.");
}
/*********************************************************************/
void preCapture()
{
    char cmd[] = { 0xaa, 0x01 | cameraAddr, 0x00, 0x07, 0x00, PIC_FMT };
    unsigned char resp[6];

    Serial.setTimeout(100);
    while (1)
    {
        clearRxBuf();
        sendCmd(cmd, 6);
        if (Serial.readBytes((char *)resp, 6) != 6) continue;
        if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x01 && resp[4] == 0 && resp[5] == 0) break;
    }
}
void Capture()
{
     char cmd[] = { 0xaa, 0x04 | cameraAddr, 0x01, 0x00, 0x00, 0x00 };
  unsigned char resp[6];
  unsigned long picTotalLen = 0; // picture length

    while (1){
        clearRxBuf();
        sendCmd(cmd, 6);

        if(Serial.readBytes((char *)resp, 6) != 6) continue;
        if(resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x04 && resp[4] == 0 && resp[5] == 0){
            Serial.setTimeout(1000);
            if(Serial.readBytes((char *)resp, 6) != 6)
                continue;

            if(resp[0] == 0xaa && resp[1] == (0x0a | cameraAddr) && resp[2] == 0x01){
                picTotalLen = (resp[3]) | (resp[4] << 8) | (resp[5] << 16);
                break;
            }
        }
    }

    pktCnt = (picTotalLen) / (PIC_PKT_LEN - 6);
  lastPacketLength = PIC_PKT_LEN;

    if((picTotalLen % (PIC_PKT_LEN-6)) != 0){
    pktCnt += 1;
    lastPacketLength = picTotalLen % (PIC_PKT_LEN - 6) + 6;
  }
}
/*********************************************************************/
void GetData()
{
    unsigned int pktCnt = (picTotalLen) / (PIC_PKT_LEN - 6);
    if ((picTotalLen % (PIC_PKT_LEN-6)) != 0) pktCnt += 1;

    char cmd[] = { 0xaa, 0x0e | cameraAddr, 0x00, 0x00, 0x00, 0x00 };
    unsigned char pkt[PIC_PKT_LEN];

    char picName[] = "pic00.jpg";
    picName[3] = picNameNum/10 + '0';
    picName[4] = picNameNum%10 + '0';

    if (SD.exists(picName))
    {
        SD.remove(picName);
    }

    myFile = SD.open(picName, FILE_WRITE);
    if(!myFile){
        Serial.println("myFile open fail...");
    }
    else{
        Serial.print("opening File : ");
        Serial.println(picName);
        Serial.setTimeout(1000);
        for (unsigned int i = 0; i < pktCnt; i++)
        {
            cmd[4] = i & 0xff;
            cmd[5] = (i >> 8) & 0xff;

            int retry_cnt = 0;
            retry:
            delay(10);
            clearRxBuf();
            sendCmd(cmd, 6);
            uint16_t cnt = Serial.readBytes((char *)pkt, PIC_PKT_LEN);

            unsigned char sum = 0;
            for (int y = 0; y < cnt - 2; y++)
            {
                sum += pkt[y];
            }
            if (sum != pkt[cnt-2])
            {
                if (++retry_cnt < 100) goto retry;
                else break;
            }

            myFile.write((const uint8_t *)&pkt[4], cnt-6);
            //if (cnt != PIC_PKT_LEN) break;
        }
        cmd[4] = 0xf0;
        cmd[5] = 0xf0;
        sendCmd(cmd, 6);
    }
    myFile.close();
    picNameNum ++;
}
