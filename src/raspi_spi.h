unsigned char transferAndWait(unsigned char * msg);

void clearBuffer(unsigned char * buff, int len);

void copyBuffer(unsigned char * dest, unsigned char * src, int len);                                         

void transferPacket(unsigned char * buff, int len);

void receivePacket(unsigned char * buff, int len);

void sendRequest(unsigned char * request, unsigned char * response);

//Sends a WAKE_REQ and returns a pointer to the response.
void wakeRequest(unsigned char * wakeResp);

//Sends a SLEEP_REQ and returns a pointer to the response.
void sleepRequest(unsigned char * sleepResp);

//Sends a CONFIG_REQ and returns a pointer to the response.
void configRequest(unsigned char * configResp, long timeInterval, unsigned char int0enable, unsigned char int0type, unsigned char int1enable, unsigned char int1type);

//Sends a PING_REQ and returns a pointer to the response.
void pingRequest(unsigned char * pingResp);

