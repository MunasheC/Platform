void scaleSetup() {
  Serial.println("Initializing the scale");

  rtc_cpu_freq_config_t config;
  rtc_clk_cpu_freq_get_config(&config);
  rtc_clk_cpu_freq_to_config(RTC_CPU_FREQ_80M, &config);
  rtc_clk_cpu_freq_set_config_fast(&config);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());      // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));   // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
            // by the SCALE parameter (not set yet)
            
  scale.set_scale(20500/194);
  //scale.set_scale(-471.497);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0


}

void serveContent() {
    // Define the root route ("/") to serve the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<!DOCTYPE html><html>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; background-color: #f4f4f4; color: #333; text-align: center; padding: 50px; }";
    html += "h1 { color: #4CAF50; }";
    html += "p { font-size: 24px; }";
    html += ".container { background-color: #fff; padding: 20px; border-radius: 10px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); display: inline-block; }";
    html += "#reading { font-weight: bold; color: #ff5722; }";
    html += "</style>";
    html += "<title>Weight Readings</title>";
    html += "</head>";
    html += "<body>";
    html += "<div class='container'>";
    html += "<h1>Weight Readings</h1>";
    html += "<p>Reading: <span id='reading'></span> grams</p>";
    html += "</div>";
    html += "<script>";
    html += "setInterval(function() {";
    html += "fetch('/reading').then(response => response.text()).then(data => {";
    html += "document.getElementById('reading').innerText = data;";
    html += "});";
    html += "}, 1000);";  // Update every second
    html += "</script>";
    html += "</body></html>";
    request->send(200, "text/html", html);
});


  // Define the "/reading" route to provide the load cell data
  server.on("/reading", HTTP_GET, [](AsyncWebServerRequest *request){
    float reading = scale.get_units(10);  // Get average of 10 readings
    String readingStr = String(reading);
    request->send(200, "text/plain", readingStr);
  });

  // Start the server
  server.begin();
}