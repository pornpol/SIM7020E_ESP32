#include <Stream.h>
#include <Arduino.h>

#define BUF_MAX_SIZE 1024 //64

#define MODEM_RESP 1024//128

class SIM7020E {
  public:
    void init(Stream &serial, Stream& debug, uint8_t key, uint8_t pwr);
    void powerOn();
    void powerOff();
    bool initModem();
    bool initModemAIS();
    bool initModemTRUE();
    bool reboot();
    String getIMSI();
    String getIMEI();
    bool check_match( char target[], char pattern[], int len_check);
    int check_match_index( char target[], char pattern[], int len_check );
    String expect_rx_str( unsigned long period, char exp_str[], int len_check);
    bool register_network();
    String check_ipaddr();
    int check_modem_signal();
    bool check_modem_status();
    bool operator_lists();

    bool postData(String path, String data);

    bool createHttp(String host);
    bool destroyHttp(uint8_t id);
    bool connectHttp(uint8_t id);
    bool disconnectHttp(uint8_t id);
    bool postHttp(uint8_t id, String path, String data);
    String expect_rx_str2( unsigned long period, char exp_str[], int len_check);

    String initNTP(String ntpServer);
    // String getNTPTime();

  private:
    Stream* MODEM_SERIAL;
    Stream* _debug;
    uint8_t _pwr, _key;  
};