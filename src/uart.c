#include "uart.h"
#include "system.h"
#include "sysControl.h"
#include "tool.h"
#include "rammap.h"

uint8_t msgType = 0;
uint8_t msgLen = 0;
volatile uint8_t msgAvl = 0;
uint8_t dataMsg[259];
void logicUart();

void uartWrite(uint8_t d){
    while(!(USART1->ISR & USART_ISR_TXE));
    USART1->TDR=d;
}

void uartInit(){
    //USART1->BRR = (F_CPU+BOAD/2)/BOAD;
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE;
    USART1->CR2 = USART_CR2_ABREN | USART_CR2_ABRMODE_Msk;
    USART1->CR1 |= USART_CR1_UE;
    while(!(USART1->ISR & (USART_ISR_TEACK | USART_ISR_REACK)));
    USART1->CR1 |= USART_CR1_RXNEIE;
    USART1->CR3 = USART_CR3_EIE | USART_CR3_DMAT;
    NVIC_EnableIRQ(USART1_IRQn);
    DMA1_Channel2->CPAR = (uint32_t) (&(USART1->TDR));
    DMA1_Channel2->CMAR = (uint32_t)dataMsg;
    DMA1_Channel2->CCR = DMA_CCR_MINC | DMA_CCR_DIR;
}

uint16_t crc;

void crcf(uint8_t d){
    crc+=d*211;
    crc^=crc>>8;
}

void sendPack(uint8_t type, uint8_t* data, uint8_t len){
    crc=0;
    dataMsg[0]=0x55;crcf(0x55);
    dataMsg[1]=type;crcf(type);
    dataMsg[2]=len;crcf(len);
    for(uint8_t i=0; i<len; i++){
        dataMsg[i+3] = data[i];
        crcf(data[i]);
    }
    dataMsg[len+3]=crc;

    while(DMA1_Channel2->CNDTR) continue;
    DMA1_Channel2->CCR &=~DMA_CCR_EN;
    DMA1_Channel2->CNDTR = len+4;
    DMA1_Channel2->CCR |= DMA_CCR_EN;
}

void USART1_IRQHandler(void){
    static uint16_t crc;
    static uint8_t status=0;
    static uint8_t count;
    static uint32_t tim;

    if(USART1->ISR & USART_ISR_RXNE){
        uint8_t d = USART1->RDR;
        if(tick-tim>10 || msgAvl)status=0;
        tim=tick;
        switch(status){
            case 0: if(d==0x55){status=1;crc=0;} break;
            case 1: msgType=d; status=2; break;
            case 2: msgLen=d; count=0; status=3; break;
            case 3: 
                if(count!=msgLen) dataMsg[count++]=d;
                else{
                    if(d==(0xFF&crc)) msgAvl = 1;
                    status=0;
                }
        }
        crc+=d*211;
        crc^=crc>>8;
    }

    if(USART1->ISR & USART_ISR_FE){     // Framing error
        USART1->ICR = USART_ICR_FECF;
    }
    if(USART1->ISR & USART_ISR_ORE){    // Overrun error    
        USART1->ICR = USART_ICR_ORECF;
    }
    if(USART1->ISR & USART_ISR_NE){     //  START bit Noise detection flag
        USART1->ICR = USART_ICR_NCF;
    }
}

void logicUart(){
    static uint32_t tim;
    if(sec-tim>20){
        tim=sec;
        USART1->CR1 &= ~USART_CR1_UE;
        USART1->BRR = 0;
        USART1->CR1 |= USART_CR1_UE;
    }
    if(msgAvl){
        tim=sec;
        switch(msgType){
            case 0:
                sendPack(0, (uint8_t*)&coreInfo, sizeof(coreInfo_t));
                break;
            case 1:
                sendPack(1, (uint8_t*)&coreStatus, sizeof(coreStatus_t));
                break;
            case 2:{
                uint8_t buf[2];
                if(msgLen==0)sendPack(2, (uint8_t*)&coreSetting, sizeof(coreSetting_t));
                else{
                    buf[1] = checkSettingParam((coreSetting_t*)dataMsg);
                    if(buf[1]){
                        buf[0]=1;
                        sendPack(0x82, buf, 2);
                    }else{
                        saveFlashSetting((coreSetting_t*)dataMsg);
                        buf[0]=0;
                        buf[1]=0;
                        sendPack(0x82, buf, 2);  
                    }
                }
                break;
            }
            case 3:
                sendPack(3, (uint8_t*)&coreDebug, sizeof(coreDebug_t));
                break;
            case 0x72:
                sendPack(0xF2, 0, 0);
                coreStatus.mode=watiTestMode;
                coreStatus.status &= 0xFC0003FF;
                break;
            case 0x70:
                sendPack(0xF0, 0, 0);
                coreStatus.mode=offMode;
                break;
            case 0x71:
                sendPack(0xF1, 0, 0);
                coreStatus.mode=waitMode;
                break;
            case 0x7F:
                NVIC_SystemReset();
                break;
        }
        msgAvl=0;
    }
}
