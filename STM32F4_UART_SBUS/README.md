This project shows reading 16 chanels datas from R9DS reciever over UART by using Sbus output of R9DS. Sbus is inverted version of UART so data bus is inverted 
by using a NOT gate. We could read chanel datas by using timer input capture which will be slower and less efficient. However, we use less CPU source and only one pin
by using Sbus.
