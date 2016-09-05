/**
  Main file for implementing the TCP Server Demo functionality
	
  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    TCP Server Demo implementation

  Description:
    This is the main file for the TCP Server Demo Implementation.

 */

/*

©  [2015] Microchip Technology Inc. and its subsidiaries.  You may use this software 
and any derivatives exclusively with Microchip products. 
  
THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER EXPRESS, 
IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF 
NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE, OR ITS 
INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE 
IN ANY APPLICATION. 

IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL 
OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED 
TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY 
OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S 
TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED 
THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE TERMS. 

*/

#include <xc.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "hardware.h"
#include "lcd.h"
#include "syslog.h"
#include "tcpip_config.h"
#include "network.h"
#include "ipv4.h"
#include "udpv4.h"
#include "ethernet_driver.h"
#include "led_driver.h"
#include "tcpv4.h"
#include "../board_support_package/picdem2_net/rtcc.h"


void interrupt high_priority high_isr(void)
{
    rtcc_handler();
}

void interrupt low_priority low_isr(void)
{
#ifdef USE_ENCx97J60
    ETH_EventHandler();
#else
    NOP();
#endif
}

/*******************************************************************************/
                            /* TCP Demo */
/*******************************************************************************/

//Implement an echo server over TCP
void TCP_Server(void)
{
    // create the socket for the TCP Server
    static tcpTCB_t port7TCB;

    // create the TX and RX Server's buffers
    static uint8_t rxdataPort7[20];
    static uint8_t txdataPort7[20];
    static uint8_t message[20];
    uint16_t rxLen, txLen, i,lmessage;
    socketState_t socket_state;
    static uint8_t U1,U2;

    socket_state = TCP_SocketPoll(&port7TCB);
    //lcd_goto2(0);
    //lcd_puts("NO LINKED");
    SYSLOG_Write("NO LINKED");
    switch(socket_state)
    {
        case NOT_A_SOCKET:
            // Inserting and initializing the socket
            TCP_SocketInit(&port7TCB);
            break;
        case SOCKET_CLOSED:
            //configure the local port
            TCP_Bind(&port7TCB, 7);
            // add receive buffer
            TCP_InsertRxBuffer(&port7TCB, rxdataPort7, sizeof(rxdataPort7));
            // start the server
            TCP_Listen(&port7TCB);
            break;
        case SOCKET_CONNECTED:                   
            //lcd_goto2(0);
            //lcd_puts("LINKED   ");
            SYSLOG_Write("LINKED");
            // check if the buffer was sent, if yes we can send another buffer
            if(TCP_SendDone(&port7TCB))
            {
                // check to see  if there are any received data
                rxLen = TCP_GetRxLength(&port7TCB);
                if(rxLen > 0)
                {
                    rxLen = TCP_GetReceivedData(&port7TCB);
//                    if (rxdataPort7[0] == '1')
//                        {
//                        //PORTA = 0b11111111;
//                        PORTD = 0b11111111;
//                  
//                        PORTAbits.RA0 = 0b1;
//                        PORTAbits.RA1 = 0b1;
////                        PORTAbits.RA2 = 1;
//                        PORTAbits.RA3 = 0b1;
//                        PORTAbits.RA4 = 0b1;
//                        PORTAbits.RA5 = 0b1;
//                        
////                        PORTCbits.RC0 = 1;
////                        PORTCbits.RC1 = 1;
////                        PORTCbits.RC2 = 1;
////                        PORTCbits.RC3 = 1;
//                        PORTCbits.RC4 = 1;
////                        PORTCbits.RC5 = 1;
//                        PORTBbits.RB1 = 1;
//                        PORTBbits.RB2 = 1;
//                        PORTBbits.RB3 = 1;
//                        PORTBbits.RB4 = 1;
//                        PORTBbits.RB5 = 1;
//                        PORTBbits.RB6 = 1;
//                        PORTBbits.RB7 = 1;
// //                        __delay_ms(10);
//                        }
//                    else
//                        {
//                        
//                        PORTD = 0b00000000;
//                        
//                        PORTAbits.RA0 = 0b0;
//                        PORTAbits.RA1 = 0b0;
// //                       PORTAbits.RA2 = 0;
//                        PORTAbits.RA3 = 0b0;
//                        PORTAbits.RA4 = 0b0;
//                        PORTAbits.RA5 = 0b0;
//                        
////                        PORTCbits.RC0 = 0;
////                        PORTCbits.RC1 = 0;
////                        PORTCbits.RC2 = 0;
////                        PORTCbits.RC3 = 0;
//                        PORTCbits.RC4 = 0;
////                        PORTCbits.RC5 = 0;
//                        PORTBbits.RB1 = 0;
//                        PORTBbits.RB2 = 0;
//                        PORTBbits.RB3 = 0;                        
//                        PORTBbits.RB4 = 0;
//                        PORTBbits.RB5 = 0;
//                        PORTBbits.RB6 = 0;
//                        PORTBbits.RB7 = 0;
//                         __delay_ms(10);
//                        }
                    
//                    //U1 and U2 TED 2 (Byte 11) tipos comprobar = 2               
                U1 = 0x30 + ( rxdataPort7[11] & 0b00000111 );
                PORTJ=0;
                switch( U1 )
                {
                case '0':
                PORTBbits.RB1 = 0b0;
                PORTBbits.RB2 = 0b0;
                PORTBbits.RB3 = 0b0;
                //PORTJbits.RJ0=1;

                break;
                case '1':
                PORTBbits.RB1 = 0b1;
                PORTBbits.RB2 = 0b0;
                PORTBbits.RB3 = 0b0;
                //PORTJbits.RJ1=1;
                
                break;
                case '2':
                PORTBbits.RB1 = 0b0;
                PORTBbits.RB2 = 0b1;
                PORTBbits.RB3 = 0b0;
                //PORTJbits.RJ2=1;
                break;
                case '3':
                PORTBbits.RB1 = 0b1;
                PORTBbits.RB2 = 0b1;
                PORTBbits.RB3 = 0b0;
                //PORTJbits.RJ3=1;
                break;
                case '4':
                PORTBbits.RB1 = 0b0;
                PORTBbits.RB2 = 0b0;
                PORTBbits.RB3 = 0b1;
                //PORTJbits.RJ4=1;
                break;
                case '5':
                PORTBbits.RB1 = 0b1;
                PORTBbits.RB2 = 0b0;
                PORTBbits.RB3 = 0b1;
                //PORTJbits.RJ5=1;
                break;
                case '6':
                PORTBbits.RB1 = 0b0;
                PORTBbits.RB2 = 0b1;
                PORTBbits.RB3 = 0b1;
                //PORTJbits.RJ6=1;
                break;
                case '7':
                PORTBbits.RB1 = 0b1;
                PORTBbits.RB2 = 0b1;
                PORTBbits.RB3 = 0b1;
                //PORTJbits.RJ7=1;
                break;
                default:
                    PORTJ=0;
                     }
                U2 = rxdataPort7[11] & 0b00111000 ;
//                lcd_goto2(0);
//                lcd_putch(U2);
//                
                switch( U2 )
                {
                case 0 :
                PORTBbits.RB4 = 0b0;
                PORTBbits.RB5 = 0b0;
                PORTBbits.RB6 = 0b0;
                PORTJbits.RJ0=1;

                break;
                case 8 :
                PORTBbits.RB4 = 0b1;
                PORTBbits.RB5 = 0b0;
                PORTBbits.RB6 = 0b0;
                PORTJbits.RJ1=1;
                
                break;
                case 16:
                PORTBbits.RB4 = 0b0;
                PORTBbits.RB5 = 0b1;
                PORTBbits.RB6 = 0b0;
                PORTJbits.RJ2=1;
                break;
                case 24:
                PORTBbits.RB4 = 0b1;
                PORTBbits.RB5 = 0b1;
                PORTBbits.RB6 = 0b0;
                PORTJbits.RJ3=1;
                break;
                case 32:
                PORTBbits.RB4 = 0b0;
                PORTBbits.RB5 = 0b0;
                PORTBbits.RB6 = 0b1;
                PORTJbits.RJ4=1;
                break;
                case 40:
                PORTBbits.RB4 = 0b1;
                PORTBbits.RB5 = 0b0;
                PORTBbits.RB6 = 0b1;
                PORTJbits.RJ5=1;
                break;
                case 48:
                PORTBbits.RB4 = 0b0;
                PORTBbits.RB5 = 0b1;
                PORTBbits.RB6 = 0b1;
                PORTJbits.RJ6=1;
                break;
                case 56:
                PORTBbits.RB4 = 0b1;
                PORTBbits.RB5 = 0b1;
                PORTBbits.RB6 = 0b1;
                PORTJbits.RJ7=1;
                break;
                default:
                    PORTJ=0;
                     }
                            
//                    if (rxdataPort7[9] == 2)
//                    {
//                    PORTJbits.RJ0=1;
//                    PORTBbits.RB1 = (int) ( rxdataPort7[11] & 0b00000001 );
//                    PORTBbits.RB2 = (int) (( rxdataPort7[11] << 1 ) & 0b00000001 );
//                    PORTBbits.RB2 = (int) (( rxdataPort7[11] << 2 ) & 0b00000001 );
//                    
//                   // message = "U1 Change";
//                   // TCP_InsertRxBuffer(&port7TCB, message, sizeof(message));
//                   
//                    //TCP_Send(&port7TCB, message, rxLen);
//                    }
//                    else if (rxdataPort7[9] != 2 )
//                    {
//                        PORTJbits.RJ4=1;
//                    
//                    
//                    }
//                    
                    
                    //simulate some buffer processing
                    for(i = 0; i < rxLen; i++)
                    {
                        txdataPort7[i] = rxdataPort7[i];
                    }

                    // reuse the RX buffer
                    TCP_InsertRxBuffer(&port7TCB, rxdataPort7, sizeof(rxdataPort7));
                    txLen = rxLen;
                    //send data back to the source
                    TCP_Send(&port7TCB, txdataPort7, txLen);
                }
            }
            break;
        default:
            // we should not end up here
            break;
    }
}

void main(void)
{
    time_t t;
    unsigned long ip, old_ip;
    char str[32];
    bool no_ip_display = true;

    initHW();
    Network_Init();
    SYSLOG_Init();

    // enable the high priority interrupts
    IPEN = 1;
    GIEH = 1;
    GIEL = 1;

    SYSLOG_Write("Waiting for Link");
    Network_WaitForLink();
    SYSLOG_Write("Link started");

    while(1)
    {
//        LATAbits.LA4 = 0;
//       PORTAbits.RA5 = 0;
//
//        if ( PORTBbits.RB0 == 0 )
//        {
//            PORTAbits.RA4 = 0b1;
//            PORTAbits.RA5 = 0b1;
//            lcd_puts("Entra");
//            PORTCbits.RC0 ^= 1;
//            PORTCbits.RC1 ^= 1;
//            PORTBbits.RB1 = 1;
//            PORTBbits.RB2 = 1;
//            PORTBbits.RB3 = 1;
//        }
//        __delay_ms(10);
        Network_Manage();
        time(&t);

        ip = IPV4_GetMyIP();
        // make sure we update the IP if we get another from DHCP
        if((no_ip_display) || (old_ip != ip))
        {
            old_ip = ip;
            memset(str,0,sizeof(str));
            sprintf(str,"%d.%d.%d.%d",((char*)&ip)[3],((char*)&ip)[2],((char*)&ip)[1],((char*)&ip)[0]);
            lcd_goto1(0);
            lcd_puts(str);
            SYSLOG_Write(str);
            no_ip_display = false;
        }
        TCP_Server();
    }
    return;
}
