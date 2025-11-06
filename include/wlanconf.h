const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <style>
  body {
    text-align:center;
    font-family: helvetica;
  }
  </style>
  <head>
    <title>
      Webradio
    </title>
  <head>
  <body>
    <h1>Webradio WLAN Settings</h1>
    *mark1begin*
    <h3>WLAN Settings</h3>
    <form method="POST">
      <p>
        <h4>WLAN Name (SSID):</h4>
        <input type="text" name="ssid" value="*ssid*">
      </p>
      <p>
        <h4>WLAN Password:</h4>
        <input type="password" name="password" value="*password*">
      </p>
      <!--<p>
        <h4>Time Server (NTP):</h4>
        If you don't know what this is,<br/>it should remain <b>at.pool.ntp.org</b>.<br/>
        <input type="text" name="ntpserver" value="*ntpserver*">
      </p>-->
      <input type="submit" value="Save">
    </form>
    *mark1end*
    <p>
      *feedback*
    </p>
  </body>
</html>
)=====";