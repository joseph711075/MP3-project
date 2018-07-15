#include "tasks.hpp"
#include "uart0_min.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "printf_lib.h"
#include "utilities.h"
#include "handlers.hpp"
#include "command_handler.hpp"
#include "gpio.hpp"
#include "i2c2.hpp"
#include "lpc_sys.h"
#include "event_groups.h"
#include "sys_config.h"
#include "storage.hpp"
#include "stdio.h"
#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include "ssp0.h"
#include "ssp1.h"
#include "string.h"
#include <string>
#include <DISPLAY.h>
#include <sstream>
#include "lpc17xx.h"

using namespace std;
#define MP3_XCS 12 //    Control Chip Select Pin
#define MP3_XDCS 14 //   Data Chip Selec (BSYNC Pin)
#define MP3_DREQ 26 //   Data Request Pin (User adds songs)

//VS1053 SCI Registers
#define SCI_MODE 0x00
#define SCI_STATUS 0x01
#define SCI_BASS 0x02
#define SCI_CLOCKF 0x03
#define SCI_DECODE_TIME 0x04
#define SCI_AUDATA 0x05
#define SCI_WRAM 0x06
#define SCI_WRAMADDR 0x07
#define SCI_HDAT0 0x08
#define SCI_HDAT1 0x09
#define SCI_AIADDR 0x0A
#define SCI_VOL 0x0B
#define SCI_AICTRL0 0x0C
#define SCI_AICTRL1 0x0D
#define SCI_AICTRL2 0x0E
#define SCI_AICTRL3 0x0F

//VS1053 Opcode
#define VS1053_WRITE 0x02
#define VS1053_READ 0x03

//SD card
#define BUFFER_SIZE     64 // 32

//using namespace std;

QueueHandle_t q;

int mp3 =0;

typedef enum
{
    previous, next
} PB_MODE_t;

/*GLOBALS*/

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


vector <string>  player;


char  playlist[50][30];
char (*current_track)[30] = &playlist[0];

int volDisplay = 5;
uint8_t trackNumber = 0;
uint8_t leftVol = 0x2E;
uint8_t rightVol = 0x2E;
SemaphoreHandle_t pause_resume_sem = NULL;
SemaphoreHandle_t nextsong_sem = NULL; //fast forward (next song)
SemaphoreHandle_t previoussong_sem = NULL; // rewind (previous song)
SemaphoreHandle_t Vup_sem = NULL;
SemaphoreHandle_t Vdown_sem = NULL;
bool isPlaying = true;
int TRACK0   = 0;


//from spi lab
uint8_t send8bit(uint8_t out)
{
    LPC_SSP0->DR = out;
    while (LPC_SSP0->SR & (1 << 4)); // polling for transferring is over
    return LPC_SSP0->DR;
}

bool checkDREQhigh()
{
    if (LPC_GPIO0->FIOPIN & (1 << 1))
    {
        return true;
    }
    else
    {
        return false;
    }

}
void CSsetlow()
{
    // Select CS
    LPC_GPIO0->FIOCLR |= (1 << 29);
}
void CSsethigh()
{
    // Deselect CS
    LPC_GPIO0->FIOSET |= (1 << 29);
}
void DCSsetlow()
{
    // Select DCS
    LPC_GPIO0->FIOCLR |= (1 << 30);
}
void DCSsethigh()
{
    // Deselect DCS
    LPC_GPIO0->FIOSET |= (1 << 30);
}



void Getsonglist()
{

    int numofsong =0 ;
    //int length;
    FRESULT res;
    DIR dir;
    static FILINFO fno;

    //char snl [numofsong] [30] ;
   // int cnt = 0;

    res = f_opendir(&dir, "1:");    //open the SD card directory, which is 1

    if (res == FR_OK)
    {

        while(1)
        {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0)
            {
                break;
            }


            if (!(fno.fattrib & AM_DIR) && fno.fname)
            {
                char songName[30] = {*fno.fname};

                strcpy(songName, fno.fname);

                player.push_back(songName);
            }


        }
        //read all the files included in the SD card

        for(int a=0;a<player.size();a++)
        {

            sprintf(playlist[a],"1:%s",player[a].c_str());
//            printf("%s\n",playlist[a]);

        }

        f_closedir(&dir);


    }
}



void DisplayArrow()
{


    DISPLAY lcd;
    lcd.initDisplay();
    lcd.showDisplay();
    int playersize=player.size();
    ////how to highlight or just add an arrow...
    if ( trackNumber<7)
    {
        lcd.stringtobuffer( 1,0, "   ");
        lcd.stringtobuffer( 2,0, "   ");
        lcd.stringtobuffer( 3,0, "   ");
        lcd.stringtobuffer( 4,0, "   ");
        lcd.stringtobuffer( 5,0, "   ");
        lcd.stringtobuffer( 6,0, "   ");
        lcd.stringtobuffer( 7,0, "   ");
        lcd.stringtobuffer((trackNumber+1),0, ">>");

        lcd.showDisplay();
    }

    else   {
        lcd.stringtobuffer( 1,0, "   ");
        lcd.stringtobuffer( 2,0, "   ");
        lcd.stringtobuffer( 3,0, "   ");
        lcd.stringtobuffer( 4,0, "   ");
        lcd.stringtobuffer( 5,0, "   ");
        lcd.stringtobuffer( 6,0, "   ");
        lcd.stringtobuffer( 7,0, "   ");
        lcd.stringtobuffer((trackNumber%7 + 1),0, ">>");
    }
    lcd.showDisplay();
}

void DisplayMenu()
{
    DISPLAY lcd;

    lcd.initDisplay();


    int playersize=player.size();


    int page = trackNumber%7;
    if(trackNumber<7)
    {
        for (int i =0; i <playersize; i++)
        {
            lcd.stringtobuffer(i+1,20, (char *) player[i].c_str());



        }
    }
    else
    {


        int pgnum=(trackNumber%7);
        int first = trackNumber-(trackNumber % 7);
        int j=0;
        for (int i =first; i < playersize; i++)
        {
            j++;
            lcd.stringtobuffer(j,20, (char *) player[i].c_str());


        }
    }



    lcd.showDisplay();


}

void DisplayVolume()
{


    DISPLAY lcd;
    lcd.initDisplay();

    char v[5];
    sprintf(v,"%d",volDisplay);
    string s ;
    lcd.stringtobuffer( 0,0,"Volume: ");

    lcd.stringtobuffer( 0,40,v);
    lcd.stringtobuffer( 0,50," / 7");

    lcd.showDisplay();



}


//for sending cmd
void writeReg(uint8_t address, uint8_t high, uint8_t low)
{
    while (checkDREQhigh() == 0);
    CSsetlow();

    send8bit(VS1053_WRITE);
    send8bit(address);
    send8bit(high);
    send8bit(low);

    CSsethigh();
    while (checkDREQhigh() == 0);

}

void suspendsend2decoder()
{
    vTaskSuspend(
            scheduler_task::getTaskPtrByName("send2decoder")->getTaskHandle());
}
void resumesend2decoder()
{
    vTaskResume(
            scheduler_task::getTaskPtrByName("send2decoder")->getTaskHandle());
}
void volumeUp()
{

    if(leftVol != 0x0E)
    {
        leftVol -= 0x10;
        rightVol -= 0x10;
        writeReg(SCI_VOL, leftVol, rightVol);
        volDisplay++;

    }
}
void volumeDown()
{
    if (leftVol != 0xFE)
    {
        leftVol += 0x10;
        rightVol += 0x10;
        writeReg(SCI_VOL, leftVol, rightVol);
        volDisplay--;


    }
}


//read mp3 from SD card --> sends to "send2decoder" through queue
class ReadFromSdCard: public scheduler_task
{
public:
    ReadFromSdCard(uint8_t priority) : scheduler_task("ReadFromSdCard", 1024, priority) {}



    bool init(void)
    {


        LPC_SC->PCONP |= (1 << 21);//Power on SSP0

        LPC_SC->PCLKSEL1 &= ~(3 << 10); //clear bits
        LPC_SC->PCLKSEL1 |= (1 << 10); //register 01 set to CPU = CLK/1


        /*SCK, MISO, MOSI pins SSP0 Configuration */
        LPC_PINCON->PINSEL1 &= ~(3 << 2);  //P0.17 MISO0
        LPC_PINCON->PINSEL1 |= (1 << 3);

        LPC_PINCON->PINSEL0 &= ~(3 << 30); //P0.15 SCK0
        LPC_PINCON->PINSEL0 |= (1 << 31);

        LPC_PINCON->PINSEL1 &= ~(3 << 4);  //P0.18 MOSI0
        LPC_PINCON->PINSEL1 |= (1 << 5);

        LPC_SSP0->CPSR = 0; // SCK

        LPC_SSP0->CR0 |= (7<< 0); // 8 bit transfer

        LPC_SSP0->CR1 &= ~(1 << 2); //master mode

        /*GPIO Configuration for DREQ, DCS, Reset, SD-CS, CS*/
        LPC_PINCON->PINSEL0 &= ~(3 << 2); // P0.1 DREQ
        LPC_PINCON->PINSEL0 &= ~(3 << 0); // P0.0 CCS
        LPC_PINCON->PINSEL1 &= ~(3 << 26); // P0.29 MCS
        LPC_PINCON->PINSEL1 &= ~(3 << 28); // P0.30 Dcs
        LPC_PINCON->PINSEL4 &= ~(3 << 12); // P2.6  reset

        /*Input and output configuration*/
        LPC_GPIO0->FIODIR &= ~(1 << 1);
        LPC_GPIO0->FIODIR |= (1 << 29);
        LPC_GPIO0->FIODIR |= (1 << 0);
        LPC_GPIO0->FIODIR |= (1 << 30);
        LPC_GPIO2->FIODIR |= (1 << 6);

        /*Deassert pins*/
        CSsethigh();
        DCSsethigh();
        LPC_GPIO0->FIOPIN |= (1 << 0);
        LPC_GPIO2->FIOPIN |= (1 << 6);

        writeReg(SCI_CLOCKF, 0X88, 0x00);
        writeReg(SCI_VOL, leftVol, rightVol);



        return 1;
    }
    bool run(void *p)
    {

        bool fileOpen;
        PB_MODE_t playback_ctr = PB_MODE_t::next;
        char *buffer_sd;
        buffer_sd = (char*) malloc(sizeof(char) * BUFFER_SIZE);
        FILE *filepointer = fopen(*current_track, "r");
        if (filepointer != NULL)
            fileOpen = true;


        fseek(filepointer, 0, SEEK_END);
        int fileSize = ftell(filepointer);
        fseek(filepointer, 0, SEEK_SET);




        //   pass 64 bytes of data to  send to decoder task
        while (fileOpen) {

            for (int i = 0; i < fileSize; i = i + 64)
            {
                if (xSemaphoreTake(nextsong_sem, 0))
                {

                    vTaskDelay(100);
//                    u0_dbg_printf("\n next song button pressed");
                    vTaskDelay(50);

                    playback_ctr = PB_MODE_t::next;



                    break;
                }



                if (xSemaphoreTake(previoussong_sem, 0))
                {

                    vTaskDelay(100);
//                    u0_dbg_printf("\n previous song button pressed");
                    vTaskDelay(50);


                    playback_ctr = previous;
                    break;
                }
                fread(buffer_sd, 1, (size_t) BUFFER_SIZE, filepointer); // Read 64byte block from SD card

                xQueueSend(q, &buffer_sd, portMAX_DELAY);
            }
            fclose(filepointer);
            fileOpen = false;
        }

        switch (playback_ctr)
        {
            case previous:
                if (trackNumber == 0)
                {
                    trackNumber = player.size() - 1;
                    current_track = &playlist[player.size() - 1];
                    DisplayMenu();

                }
                else
                {
                    trackNumber--;
                    current_track--;
                    if(trackNumber%7 == 0)
                    {
                        DisplayMenu();
                    }
                }
                break;
            default:

                if (trackNumber == player.size() - 1)
                {
                    trackNumber = 0;
                    current_track = &playlist[0];
                    DisplayMenu();
                }
                else
                {
                    trackNumber++;
                    DisplayArrow();
                    current_track++;
                    DisplayMenu();
                }
                break;
        }
        DisplayArrow();
        free(buffer_sd);

        return true;
    }
};

//send mp3 file to decoder
class send2decoder: public scheduler_task {
public:
    send2decoder(uint8_t priority) :scheduler_task("send2decoder", 1024, priority) {}

    bool run(void *p)
    {
        char *mp3ptr;
        uint16_t x = 0;

        if (xQueueReceive(q, &mp3ptr, portMAX_DELAY))
        {


            if (LPC_GPIO1->FIOPIN3 & (1 << 4))
            { // If Vup_sem button pressed 1.29
                vTaskDelay(400);
                xSemaphoreGive(Vup_sem);
            }


            if (LPC_GPIO1->FIOPIN3 & (1 << 5))
            { // If Vdown_sem button pressed 1.19
                vTaskDelay(400);
                xSemaphoreGive(Vdown_sem);
            }



            while (x < 64)
            {

                while (checkDREQhigh() == 0);
                DCSsetlow();

                for (uint8_t i = 0; i < 32; i++)
                {

                    send8bit(*mp3ptr);
                    mp3ptr++;
                    x++;
                }
                DCSsethigh();


            } //while loop end here


        }

        return true;
    }
};


class ctrl_panel: public scheduler_task {
public:
    ctrl_panel(uint8_t priority) :
        scheduler_task("ctrl_panel", 1024, priority) {
    }


    bool init()

    {

        LPC_GPIO1->FIODIR |= (0 << 19); //set as input




        return 1;

    }

    bool run(void *p)
    {



        if((LPC_GPIO1->FIOPIN2 & (1 << 3))||(LPC_GPIO1->FIOPIN3 & (1 << 6)))
        { // checking pause button & ir signal
            vTaskDelay(100);

            if (isPlaying)
            {
//                u0_dbg_printf("\ncontrol panel: Pausing song");
                vTaskDelay(100);
                suspendsend2decoder();
                isPlaying = false;
                vTaskDelay(50);
            }
            else
            {
//                u0_dbg_printf("\ncontrol panel: Resume song");
                vTaskDelay(100);
                resumesend2decoder();
                isPlaying = 1;
                vTaskDelay(50);
            }

        }

        if (xSemaphoreTake(Vup_sem, 0))
        {
            vTaskDelay(100);
//            u0_dbg_printf("\ncontrol panel: Vup_sem");
            volumeUp();
            vTaskDelay(50);
            DisplayVolume();
        }


        if (xSemaphoreTake(Vdown_sem, 100))
        {    // control panel goes to sleep here send 2 decoder task start running
//            u0_dbg_printf("\ncontrol panel: Vdown_sem");
            volumeDown();

            //suspendsend2decoder();
            vTaskDelay(50);
            DisplayVolume();
        }

        if (LPC_GPIO1->FIOPIN2 & (1 << 6))  // next song control
        { // If next button pressed 1.22

            vTaskDelay(200);
            xSemaphoreGive(nextsong_sem);//nextsong_sem
            //suspendsend2decoder();
            //vTaskDelay(50);

        }

        if (LPC_GPIO1->FIOPIN2 & (1 << 4)) // pre song control
        { // If previous button pressed 1.20
            vTaskDelay(200);
            xSemaphoreGive(previoussong_sem);
            //suspendsend2decoder();
            //vTaskDelay(50);
        }



        return true;
    }
};

int main(void)
{
    DISPLAY test;
    test.initDisplay();

    Getsonglist();
    DisplayArrow();
    DisplayMenu();
    DisplayVolume();


    q = xQueueCreate(1, sizeof(char*));
    pause_resume_sem = xSemaphoreCreateBinary();
    nextsong_sem = xSemaphoreCreateBinary();
    previoussong_sem = xSemaphoreCreateBinary();
    Vup_sem = xSemaphoreCreateBinary();
    Vdown_sem = xSemaphoreCreateBinary();

    scheduler_add_task(new ReadFromSdCard(PRIORITY_LOW));
    scheduler_add_task(new send2decoder(PRIORITY_MEDIUM));

    scheduler_add_task(new ctrl_panel(PRIORITY_HIGH));
    scheduler_add_task(new terminalTask(4));

    scheduler_start();



    return -1;
}
