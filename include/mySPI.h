void mySPI_Init(void);
void oled_Write(unsigned char);
void oled_Write_Cmd(unsigned char);
void oled_Write_Data(unsigned char);
void oled_Config(void);
void refresh_OLED(int freq_local);
void myTIM3_Init(void);