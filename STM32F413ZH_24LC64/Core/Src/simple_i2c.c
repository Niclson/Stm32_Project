#include "simple_i2c.h"



int SimpleI2C_Init(I2C_TypeDef *regInst, uint32_t sysFreq, uint32_t outBusFreq) {
    int      status   = SIMPLE_I2C_ERROR;
    uint32_t ccrField = sysFreq / outBusFreq;

    if ( (regInst == NULL)                           || 
         (sysFreq > 50000000) || (sysFreq < 2000000) || 
         (outBusFreq == 0) || (outBusFreq > 400000)  || 
         (ccrField == 0) ) {
        /* Не допустимое значение аргумента */
        goto error;
    }

    sysFreq /= 1000000;

    regInst->CR1 = I2C_CR1_SWRST;

    regInst->CR1  = 0x00;
    regInst->CR2  = 0x00;
    regInst->OAR1 = 0x00;
    regInst->OAR2 = 0x00;
    regInst->CCR  = 0x00;

    regInst->CR2  = (sysFreq << I2C_CR2_FREQ_Pos);

    if (outBusFreq > 100000) {
        /* Всё что больше 200кГц - режим Fast */
        regInst->CCR  |= I2C_CCR_FS;
    }

    regInst->CCR  |= (ccrField << I2C_CCR_CCR_Pos) & I2C_CCR_CCR_Msk;
    regInst->TRISE = 9;

    /* Сброс всех флагов регистра статуса */
    regInst->SR1 = 0x00;

    regInst->CR1  |= I2C_CR1_PE;

    status = SIMPLE_I2C_OK;

error:
    return (status);
}





int SimpleI2C_StartComm(I2C_TypeDef *regInst, uint8_t addrSlave, i2cCommType_t typeComm) {
    int status = SIMPLE_I2C_ERROR;
    uint32_t sr1Reg;

   addrSlave <<= 1;
    if (typeComm == I2C_RECV) {
        addrSlave    |= 0x01;
        regInst->CR1 |= I2C_CR1_ACK;
    } else {
        regInst->CR1 &= ~I2C_CR1_ACK;
    }

    regInst->CR1 |= I2C_CR1_START;
    while (1) {
        sr1Reg = regInst->SR1;
        if (sr1Reg & I2C_SR1_TIMEOUT) {
            goto error;
        } else if (sr1Reg & I2C_SR1_SB) {
            break;
        }
    }

    (void)regInst->SR1;
    regInst->DR = addrSlave;
    while (!(regInst->SR1 & I2C_SR1_ADDR) || !(regInst->SR2));

    status = SIMPLE_I2C_OK;

error:
    return (status);
}



int SimpleI2C_StopComm(I2C_TypeDef *regInst) {
    regInst->CR1 |= I2C_CR1_STOP;
    while(regInst->SR1, !(regInst->SR2 & I2C_SR2_MSL));

    return (SIMPLE_I2C_OK);
}



int SimpleI2C_SendByte(I2C_TypeDef *regInst, uint8_t dataByte) {
    int status = SIMPLE_I2C_ERROR;
    uint32_t sr1Reg;

    while(!(regInst->SR1 & I2C_SR1_TXE));
    regInst->DR = dataByte;
    while (1) {
        sr1Reg = regInst->SR1;
        if (sr1Reg & (I2C_SR1_TIMEOUT | I2C_SR1_AF | I2C_SR1_OVR |
                      I2C_SR1_ARLO    | I2C_SR1_BERR)) {
            goto error;
        } else if (sr1Reg & I2C_SR1_BTF) {
            break;
        }
    }

    status = SIMPLE_I2C_OK;
error:
    return (status);
}



uint8_t SimpleI2C_RecvByte(I2C_TypeDef *regInst, i2cAck_t answerToSlave) {
    uint8_t recvByte = 0x00;
    
    if (answerToSlave == I2C_ACK) {
        regInst->CR1 |= I2C_CR1_ACK;
    } else {
        regInst->CR1 &= ~I2C_CR1_ACK;
    }

    while(!(regInst->SR1 & I2C_SR1_RXNE));
    recvByte = regInst->DR;

    return (recvByte);
}
