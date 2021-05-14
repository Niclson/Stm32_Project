#include "polling_i2c.h"

int PollingI2C_Init(I2C_TypeDef *regInst, uint32_t sysFreq, uint32_t outBusFreq) {
    int      status   = POLLING_I2C_ERROR;
    uint32_t ccrField = sysFreq / outBusFreq;

    if ( (regInst == NULL)                           || 
         (sysFreq > 50000000) || (sysFreq < 2000000) || 
         (outBusFreq == 0) || (outBusFreq > 400000)  || 
         (ccrField == 0) ) {
        /* Не допустимое значение аргумента */
        goto error;
    }


    /* Сброс модуля i2c */
    regInst->CR1 |=  I2C_CR1_SWRST;
    regInst->CR1 &= ~I2C_CR1_SWRST;

    regInst->CR1  = 0x00;
    regInst->CR2  = 0x00;
    regInst->OAR1 = 0x00;
    regInst->OAR2 = 0x00;
    regInst->CCR  = 0x00;

    sysFreq /= 1000000;
    regInst->CR2  = (sysFreq << I2C_CR2_FREQ_Pos);

    /* У стандартаного режима какие-то проблемы (см Errata),
     * рекомендуется использовать Fast mode */
    regInst->CCR  |= I2C_CCR_FS;
    
    regInst->CCR  |= (ccrField << I2C_CCR_CCR_Pos) & I2C_CCR_CCR_Msk;
    regInst->TRISE = 9;

    /* Сброс всех флагов регистра статуса */
    regInst->SR1 = 0x00;

    regInst->CR1  |= I2C_CR1_PE;

    status = POLLING_I2C_OK;

error:
    return (status);
}





static int PollingI2C_StartComm(I2C_TypeDef *regInst, uint8_t addrSlave, int ack) {
    int status = POLLING_I2C_ERROR;

   addrSlave <<= 1;
    if (ack) {
        addrSlave    |= 0x01;
        regInst->CR1 |= I2C_CR1_ACK;
    } else {
        regInst->CR1 &= ~I2C_CR1_ACK;
    }

    /* Старт бит */
    regInst->CR1 |= I2C_CR1_START;

    /* Отслеживаем событие по выставлению
     * бита SB в регистре статуса */
    while (!(regInst->SR1 & I2C_SR1_SB));
    
    /* Отправка адреса slave-устройства,
     * также должен очиститься бит SB в регистре статуса */
    regInst->DR = addrSlave;
    while (!(regInst->SR1 & I2C_SR1_ADDR) || !(regInst->SR2));

    status = POLLING_I2C_OK;

    return (status);
}



static int PollingI2C_StopComm(I2C_TypeDef *regInst) {
    regInst->CR1 |= I2C_CR1_STOP;

    /* Ждем пока аппаратура сбросит этот бит,
     * что будет говорить об успешном завершении операции */
    while(regInst->CR1 & I2C_CR1_STOP);

    /* Пока нет таймаута других вариантов нет */
    return (POLLING_I2C_OK);
}




int PollingI2C_SendData(I2C_TypeDef *regInst, uint8_t addrSlave, uint8_t *pData, size_t sizeData) {
    register size_t i;

    PollingI2C_StartComm(regInst, addrSlave, 0);

    for (i = 0; i < sizeData; ++i) {
        /* Ожидаем пока регистр данных освободится */
        while(!(regInst->SR1 & I2C_SR1_TXE));
        regInst->DR = pData[i];
    }

    /* Ожидаем завершение передачи байта */
    while(!(regInst->SR1 & I2C_SR1_BTF));

    PollingI2C_StopComm(regInst);

    /* Пока нет таймаута других вариантов нет */
    return (POLLING_I2C_OK);
}



int PollingI2C_SendByte(I2C_TypeDef *regInst, uint8_t addrSlave, uint8_t dataByte) {
    return ( PollingI2C_SendData(regInst, addrSlave, &dataByte, sizeof(dataByte)) );
}


static void PollingI2C_Recv2Bytes(I2C_TypeDef *regInst, uint8_t addrSlave, uint8_t *pRecvBuffer) {
    addrSlave   <<= 1;
    addrSlave    |= 0x01;
    regInst->CR1 |= I2C_CR1_ACK;

    /* Старт бит */
    regInst->CR1 |= I2C_CR1_START;

    /* Отслеживаем событие по выставлению
     * бита SB в регистре статуса */
    while (!(regInst->SR1 & I2C_SR1_SB));
    
    /* Отправка адреса slave-устройства,
     * также должен очиститься бит SB в регистре статуса */
    regInst->DR   = addrSlave;
    regInst->CR1 |= I2C_CR1_POS;
    
    /* Согласно Application Note AN2824, 
     * необходимо отключить прерывания в этом критическом 
     * месте */
    __disable_irq();
        while (!(regInst->SR1 & I2C_SR1_ADDR) || !(regInst->SR2));
        regInst->CR1 &= ~I2C_CR1_ACK;
    __enable_irq();

    while(!(regInst->SR1 & I2C_SR1_BTF));

    __disable_irq();
        regInst->CR1  |= I2C_CR1_STOP;
        pRecvBuffer[0] = regInst->DR;
    __enable_irq();
    pRecvBuffer[1] = regInst->DR;

    while(regInst->CR1 & I2C_CR1_STOP);

    regInst->CR1 &= ~I2C_CR1_POS;
}




int PollingI2C_RecvData(I2C_TypeDef *regInst, uint8_t addrSlave, uint8_t *pRecvBuffer, size_t recvLength) {
    size_t i = recvLength;

    if (recvLength > 2) {
        PollingI2C_StartComm(regInst, addrSlave, 1);
        while (i > 3) {
            while(!(regInst->SR1 & I2C_SR1_RXNE));
            pRecvBuffer[recvLength - i] = regInst->DR;
            i--;
        }
        while(!(regInst->SR1 & I2C_SR1_BTF));
        regInst->CR1 &= ~I2C_CR1_ACK;
        __disable_irq();
            regInst->CR1  |= I2C_CR1_STOP;
            pRecvBuffer[recvLength - i] = regInst->DR;
            i--;
        __enable_irq();
       
        while(!(regInst->SR1 & I2C_SR1_RXNE));
       
        pRecvBuffer[recvLength - i] = regInst->DR;
       
        while(regInst->CR1 & I2C_CR1_STOP);
    } else if (recvLength == 2) {
        PollingI2C_Recv2Bytes(regInst, addrSlave, pRecvBuffer);
    } else if (recvLength == 1) {
        *pRecvBuffer = PollingI2C_RecvByte(regInst, addrSlave);
    }

    return (POLLING_I2C_OK);
}



uint8_t PollingI2C_RecvByte(I2C_TypeDef *regInst, uint8_t addrSlave) {
    uint8_t recvByte = 0x00;

    addrSlave <<= 1;
    addrSlave    |= 0x01;
    regInst->CR1 |= I2C_CR1_ACK;

    /* Старт бит */
    regInst->CR1 |= I2C_CR1_START;

    /* Отслеживаем событие по выставлению
     * бита SB в регистре статуса */
    while (!(regInst->SR1 & I2C_SR1_SB));
    
    /* Отправка адреса slave-устройства,
     * также должен очиститься бит SB в регистре статуса */
    regInst->DR   = addrSlave;
    regInst->CR1 &= ~I2C_CR1_ACK;
    /* Согласно Application Note AN2824, 
     * необходимо отключить прерывания в этом критическом 
     * месте */
    __disable_irq();
        while (!(regInst->SR1 & I2C_SR1_ADDR) || !(regInst->SR2));
        regInst->CR1 |=  I2C_CR1_STOP;
    __enable_irq();

    while(!(regInst->SR1 & I2C_SR1_RXNE));
    recvByte = regInst->DR;

    /* Ждем пока аппаратура сбросит этот бит,
     * что будет говорить об успешном завершении операции */
    while(regInst->CR1 & I2C_CR1_STOP);

    return (recvByte);
}
