#include "adc.h"

/*
 * ������:adc��ʼ������
 * ����  :��
 * ���  :��
 */
void adc_init(void)
{
    TRISA = 0Xff;           //��PORTA����Ϊ���
    ADCON1 = 0X0E;          //��AN0����Ϊģ��ͨ��
    ADCON0 = 0X00;          //ѡͬAN0
    ADCON2 = 0XBA;          //�Ҷ���,20TAD,32TOSC
    ADCON0bits.ADON = 1;               //��ADģ���Դ
    PIR1bits.ADIF = 0;               //�����־λ
    PIE1bits.ADIE = 1;               //�ж�ʹ�� 
    INTCONbits.GIE = 1;                //�����ж�
    INTCONbits.PEIE = 1;               //�������ж�
    delay(5);               //��ʱ
    ADCON0bits.GO_DONE = 1;             //����һ��ADת��
}
/*
 * ������:adc�жϷ�����
 * ���  :��
 * ����  :��
 */ 
#pragma interruptlow pic18_int  //����ʹ��interruptlow����ؼ���������InterruptHandlerLow��������ǵ����ȼ��жϷ����������˹ؼ��ʺ�������������ɱ������Զ������������ֳ�������������������ķ��ؽ���ʹ��RETFIE���صġ�

 void pic18_int (void)
 {
    float nu;
    unsigned int high;												
    if(PIR1bits.ADIF == 1)                       //�ж�AD�ж��Ƿ���λ
    {
        PIR1bits.ADIF= 0; 
        high = ADRESH;                       //�����־λ
        sum = (high<<8) | ADRESL;       //������ֵ����Ϊ��ѹֵSUM
        nu = sum;
        nu = nu * 3.3 / 1023 * 1000;
        sum = nu;
    }
 }
/*
 * ������:�ж����
 * ���  :��
 * ����  :��
 */
 #pragma code InterruptVectorLow = 0x18 //��#pragmaαָ���һ�����ֽ�InterruptVectorLow�ĶΣ���������ηŵ�0x18��ַ��ʼ�Ĵ���ռ�
 void InterruptVectorLow (void)  //�����ȼ��ж���������
 {
	_asm
	goto pic18_int
	_endasm
 }
 #pragma code     //���ﲻ�Ƕ���ģ����Ǹ����������ص�Ĭ�ϵĴ���Σ�������ӵĻ����������ͻ�ɵɵ�ذѺ���Ĵ������������Ĵ���һֱ����ȥ����LKR�ļ��ﶨ������������ൽ0x29��ַ���������û�Ӵ���ͨ���ᱨ��
 
//void interrupt pic18_int()
//{
//    float num;
//    if(PIRbits.ADIF == 1)                       //�ж�AD�ж��Ƿ���λ
//    {
//        PIRbits.ADIF= 0;                        //�����־λ
//        sum = ADRESH<<8 | ADRESL;       //������ֵ����Ϊ��ѹֵSUM
//        num = sum;
//        num = num * 3.3 / 1023 * 10;
//        sum = num;
//    }
//}
