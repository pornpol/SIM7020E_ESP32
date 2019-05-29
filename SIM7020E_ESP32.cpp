/*
SIM7020E v0.1
Author: 9pol
Create Date: 5 Sep 2018
*/

#include "SIM7020E_ESP32.h"

void SIM7020E::powerOn()
{

  digitalWrite(_pwr, HIGH);
  digitalWrite(_key, HIGH);
  delay(1000);
  digitalWrite(_key, LOW);
}

void SIM7020E::powerOff()
{
  digitalWrite(_key, HIGH);
  delay(1000);
  digitalWrite(_key, LOW);
  digitalWrite(_pwr, LOW);
}

bool SIM7020E::reboot()
{
  MODEM_SERIAL->println("AT+CRESET");
}

void SIM7020E::init(Stream &serial, Stream &debug, uint8_t key, uint8_t pwr)
{
  MODEM_SERIAL = &serial;
  _debug = &debug;
  _pwr = pwr;
  _key = key;
  pinMode(_pwr, OUTPUT);
  pinMode(_key, OUTPUT);
}

bool SIM7020E::initModem()
{
  _debug->println(F("######### SIM7020E by 9pol V0.1 ##########"));
  _debug->println("initial Modem to connect NB-IoT Network (Default)");
  // MODEM_SERIAL->flush();
  MODEM_SERIAL->println(F("AT+CFUN=1"));
  delay(2000);
  MODEM_SERIAL->println(F("AT"));
  delay(1000);
  ////////////////////////////
  _debug->println("IMEI = " + getIMEI());
  _debug->println("IMSI = " + getIMSI());
  // while (!register_network());
  // delay(1000);
  // _debug->println(check_ipaddr());
}

bool SIM7020E::initModemTRUE()
{
  _debug->println(F("######### SIM7020E by 9pol V0.1 ##########"));
  _debug->println("initial Modem to connect NB-IoT Network (TRUE)");
  delay(5000);
  MODEM_SERIAL->println(F("AT+CFUN=0"));
  delay(5000);
  //Serial.println(F("AT*MCGDEFCONT=\"IP\",\"aistest.nb\""));
  MODEM_SERIAL->print(F("AT*MCGDEFCONT=\"IP\",")); // nb.developer aistest.nb
  delay(500);
  MODEM_SERIAL->println(F("\"\""));
  delay(10000);
  MODEM_SERIAL->println(F("AT+CFUN=1"));
  delay(5000);
  MODEM_SERIAL->println(F("AT*MCGDEFCONT?"));
  delay(5000);
  MODEM_SERIAL->println(F("AT+CGREG?"));
  delay(5000);
  MODEM_SERIAL->println(F("AT+CGCONTRDP"));
  delay(5000);
  MODEM_SERIAL->println(F("AT"));
  delay(5000);
  ////////////////////////////
  _debug->println("IMEI = " + getIMEI());
  _debug->println("IMSI = " + getIMSI());
  // while (!register_network());
  // delay(1000);
  // _debug->println(check_ipaddr());
}

bool SIM7020E::initModemAIS()
{
  _debug->println(F("######### SIM7020E by 9pol V0.1 ##########"));
  _debug->println("initial Modem to connect NB-IoT Network (AIS)");
  delay(5000);
  MODEM_SERIAL->println(F("AT+CFUN=0"));
  delay(5000);
  //Serial.println(F("AT*MCGDEFCONT=\"IP\",\"aistest.nb\""));
  MODEM_SERIAL->println("AT*MCGDEFCONT=\"IP\",\"nb.developer\""); // nb.developer aistest.nb
  // delay(500);
  // MODEM_SERIAL->println("\"nb.developer\"");
  // delay(5000);
  expect_rx_str(5000, "OK", 2);
  MODEM_SERIAL->println(F("AT+CFUN=1"));
  delay(5000);
  MODEM_SERIAL->println(F("AT*MCGDEFCONT?"));
  // delay(5000);
  expect_rx_str(5000, "OK", 2);
  MODEM_SERIAL->println(F("AT+CGREG?"));
  delay(5000);
  MODEM_SERIAL->println(F("AT+CGCONTRDP"));
  delay(5000);
  MODEM_SERIAL->println(F("AT"));
  delay(5000);
  MODEM_SERIAL->println("AT+CDNSCFG=208.67.222.222,208.67.220.220");
  expect_rx_str(5000, "OK", 2);
  MODEM_SERIAL->println(F("AT+CDNSCFG?"));
  delay(5000);
  ////////////////////////////
  _debug->println("IMEI = " + getIMEI());
  _debug->println("IMSI = " + getIMSI());
  // while (!register_network());
  // delay(1000);
  // _debug->println(check_ipaddr());
}

String SIM7020E::getIMEI()
{
  MODEM_SERIAL->println(F("AT+CGSN=1"));
  char waitForCGSN[] = "+CGSN:";
  String re_str;
  re_str = expect_rx_str(1000, waitForCGSN, 6);

  if (re_str != "")
  {
    return re_str;
  }
  return "";
}

String SIM7020E::getIMSI()
{
  MODEM_SERIAL->println(F("AT+CIMI"));
  char waitForMISI[] = "\r\n";
  String re_str;
  re_str = expect_rx_str(1000, waitForMISI, 2);

  if (re_str != "")
  {
    return re_str;
  }
  return "";
}

bool SIM7020E::operator_lists()
{
  bool regist = 0;
  MODEM_SERIAL->println(F("AT*MCGDEFCONT?"));
  //delay(1000);
  char waitForOK[] = "OK";
  if (expect_rx_str(5000, waitForOK, 2) != "")
  {
    _debug->println("Operator List(s)");
    return true;
  }
  else
  {
    _debug->println("Operator List(s) Fail!");
    return false;
  }
}

bool SIM7020E::register_network()
{
  bool regist = 0;
  // delay(2000);
  // MODEM_SERIAL->println(F("AT+CGATT=1"));
  //delay(2000);
  // MODEM_SERIAL->flush();
  while (MODEM_SERIAL->available())
    MODEM_SERIAL->read();

  MODEM_SERIAL->println(F("AT+CGATT?"));
  //delay(1000);
  char waitForCGATT[] = "+CGATT: 1";
  if (expect_rx_str2(2000, waitForCGATT, 9) != "")
  {
    _debug->println("regiester network Done");
    return true;
  }
  else
  {
    _debug->println("register network Fail!");
    return false;
  }
}

String SIM7020E::check_ipaddr()
{
  MODEM_SERIAL->println(F("AT+CGPADDR=1"));
  delay(1000);
  char waitForCGPADDR[] = "+CGPADDR: 1,";
  String re_str;
  re_str = expect_rx_str(1000, waitForCGPADDR, 11);
  if (re_str != "")
  {
    return re_str.substring(2, re_str.length() - 1);
  }
  return "";
}

int SIM7020E::check_modem_signal()
{
  char resp_result[BUF_MAX_SIZE];
  int index = 0;
  int ssi;
  char ssi_str[3];
  MODEM_SERIAL->println("AT+CSQ");
  char waitForCSQ[] = "+CSQ:";
  String re_str;
  re_str = expect_rx_str(1000, waitForCSQ, 5);
  if (re_str != "")
  {
    ssi_str[0] = re_str[0];
    // check the next char is not "," It is not single digit
    if (re_str[1] != 0x2c)
    {
      //Serial.println( resp_result[index+2]);
      ssi_str[1] = re_str[1];
      ssi_str[2] = '\0';
      ssi = atoi(ssi_str);
      ssi = -1 * (113 - ssi * 2);
      return ssi;
    }
    // it is single digit
    ssi_str[1] = '\0';
    ssi = atoi(ssi_str);
    ssi = -1 * (113 - ssi * 2);
    return ssi;
  }
  else
  {
    return -200;
  }
}

bool SIM7020E::check_modem_status()
{

  MODEM_SERIAL->println(F("AT"));
  char waitForOK[] = "OK";
  if (expect_rx_str(2000, waitForOK, 2) != "")
  {
    return true;
  }
  return false;
}

bool SIM7020E::check_match(char target[], char pattern[], int len_check)
{
  int target_length = strlen(target);
  int count = 0;
  for (int i = 0; i < target_length; i++)
  {
    // find the 1st char in target
    if (target[i] == pattern[0])
    {
      // start at index of target[index] that match[pattern]
      // loop and count the match char
      for (int j = 0; j < len_check; j++)
      {
        if (target[i] == pattern[j])
        {
          count++;
          i++;
        }
      } // for loop all char in pattern
    }   // if target
  }     // for
  if (count == len_check)
  {
    return true;
  }
  return false;
}

int SIM7020E::check_match_index(char target[], char pattern[], int len_check)
{
  int target_length = strlen(target);
  int count = 0;
  int i = 0, index = 0;
  for (i = 0; i < target_length; i++)
  {
    // find the 1st char in target
    if (target[i] == pattern[0])
    {
      // start at index of target[index] that match[pattern]
      // loop and count the match char
      for (int j = 0; j < len_check; j++)
      {
        if (target[i] == pattern[j])
        {
          count++;
          index = i;
          i++;
        }
      } // loop all char in pattern
    }
  }
  if (count == len_check)
  {
    return index;
  }
  return -1;
}

String SIM7020E::expect_rx_str(unsigned long period, char exp_str[], int len_check)
{
  unsigned long cur_t = millis();
  unsigned long start_t = millis();
  bool str_found = 0;
  bool time_out = 0;
  bool loop_out = 0;
  int i = 0;
  int found_index = 0, end_index = 0;
  int modem_said_len = 0;
  char c;
  char modem_said[MODEM_RESP];
  String re_str;
  char str[BUF_MAX_SIZE];
  char *x;

  while (!loop_out)
  {
    if (MODEM_SERIAL->available())
    {
      c = MODEM_SERIAL->read();
      modem_said[i++] = c;
      // _debug->print(c); /////////////////////// <-- Test
    }
    else
    {
    }
    cur_t = millis();
    if (cur_t - start_t > period)
    {
      time_out = true;
      start_t = cur_t;
      loop_out = true;
    }
  } //while
  modem_said[i] = '\0';
  end_index = i;
  x = strstr(modem_said, exp_str);
  found_index = x ? x - modem_said : -1;
  if (found_index >= 0)
  {
    i = 0;
    while (modem_said[found_index + i + len_check] != 0x0D | i == 0)
    {
      str[i] = modem_said[found_index + i + len_check];
      re_str += String(str[i]);
      i++;
    }
    str[i] = '\0';
    return re_str;
  }
  return "";
}

// bool SIM7020E::postData(String path, String data) {

//   bool result = false;
//   char waitForOK[] = "OK";

//   while(MODEM_SERIAL->available()) MODEM_SERIAL->read();

//   if(connectHttp(0)) {
//     result = postHttp(0, path, data);
//   } else result = false;

//   // Disconnected from server
//   disconnectHttp(0);

//   return result;
// }

bool SIM7020E::createHttp(String host)
{
  while (MODEM_SERIAL->available())
    MODEM_SERIAL->read();
  char waitForOK[] = "OK";
  MODEM_SERIAL->println("AT+CHTTPCREATE=\"" + host + "\"");
  if (expect_rx_str2(5000, waitForOK, 2) != "")
  {
    return true;
  }
  else
    return false;
}

bool SIM7020E::destroyHttp(uint8_t id)
{
  while (MODEM_SERIAL->available())
    MODEM_SERIAL->read();
  char waitForOK[] = "OK";
  MODEM_SERIAL->println("AT+CHTTPDESTROY=" + String(id));
  if (expect_rx_str2(2000, waitForOK, 2) != "")
  {
    return true;
  }
  else
    return false;
}

bool SIM7020E::connectHttp(uint8_t id)
{
  while (MODEM_SERIAL->available())
    MODEM_SERIAL->read();
  char waitForOK[] = "OK";
  MODEM_SERIAL->println("AT+CHTTPCON=" + String(id));
  if (expect_rx_str2(5000, waitForOK, 2) != "")
  {
    return true;
  }
  else
    return false;
}

bool SIM7020E::disconnectHttp(uint8_t id)
{
  while (MODEM_SERIAL->available())
    MODEM_SERIAL->read();
  char waitForOK[] = "OK";
  MODEM_SERIAL->println("AT+CHTTPDISCON=" + String(id));
  if (expect_rx_str2(2000, waitForOK, 2) != "")
  {
    return true;
  }
  else
    return false;
}

bool SIM7020E::postHttp(uint8_t id, String path, String data)
{

  // Clear Recieve Buffer
  MODEM_SERIAL->flush();
  while (MODEM_SERIAL->available())
    MODEM_SERIAL->read();

  char waitForHeader[] = "+CHTTPNMIH";
  MODEM_SERIAL->print("AT+CHTTPSEND=" + String(id) + ",1");
  MODEM_SERIAL->print(",\"" + path + "\""); // path
  MODEM_SERIAL->print(",");                 // Header
  MODEM_SERIAL->print(",\"application/json\"");
  MODEM_SERIAL->print(",");
  for (int i = 0; i < data.length(); i++)
  {
    MODEM_SERIAL->print(data[i], HEX);
  }
  // delay(500);
  MODEM_SERIAL->println("");
  delay(data.length());
  if (expect_rx_str2(10000, waitForHeader, 10) != "")
  {
    return true;
  }
  else
    return false;
}

String SIM7020E::expect_rx_str2(unsigned long period, char exp_str[], int len_check)
{
  unsigned long cur_t = millis();
  unsigned long start_t = millis();
  bool time_out = 0;
  bool loop_out = 0;
  int i = 0;
  int found_index = 0;
  char c;
  char modem_said[MODEM_RESP] = {0};
  char *x;

  // memset(modem_said, 0, MODEM_RESP);

  while (!loop_out)
  {
    if (MODEM_SERIAL->available())
    {
      c = MODEM_SERIAL->read();
      modem_said[i++] = c;
      // _debug->print(c); /////////////////////// <-- Test
      delay(1);

      x = strstr(modem_said, exp_str);
      found_index = x ? x - modem_said : -1;
      if (found_index >= 0)
      {
        delay(500);            //<------
        MODEM_SERIAL->flush(); // <------
        return "OK";
      }
    }
    else
    {
    }
    cur_t = millis();
    if (cur_t - start_t > period)
    {
      time_out = true;
      start_t = cur_t;
      loop_out = true;
    }
  } //while

  // delay(500);
  // MODEM_SERIAL->flush();

  return "";
}

String SIM7020E::initNTP(String ntpServer)
{
  while (MODEM_SERIAL->available())
    MODEM_SERIAL->read();

  char waitForHeader[] = "+CSNTP:";
  MODEM_SERIAL->println("AT+CSNTPSTART=\"" + ntpServer + "\"");
  String re_str;
  re_str = expect_rx_str(5000, waitForHeader, 7);
  if (re_str != "")
  {
    return re_str.substring(1, re_str.length());
  }
  return "";
}
