/******************************************************************************************************
** Using this platform, complete the following steps�� 
** 
    1、create a platform structure and Initialize the platform structure(Platform structure type :ComCtrl)
** 
    2、Modify the interrupt handler   
**  
    3、Cyclic scheduling platform processing function(void ComDataProcess(PComCtrl Pcomctrl))
**
    4、Modify the resend time(RESENTWAITTIME(time))
**
        Notice:
**
            (1)Modify the function WriteDataIntoCommBuff,Add interrupts enabled 
**
            (2)Application layer call the function MsgBuffRead to Parse the message 
**
**
********************************************************************************************************/