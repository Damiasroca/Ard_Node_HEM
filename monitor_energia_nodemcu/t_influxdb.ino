void influxdb() {
  // Clear fields for reusing the point. Tags will remain untouched
  sensor.clearFields();

  // Store measured value into point
  // Report RSSI of currently connected network
  sensor.addField("Potència", (data1));
  sensor.addField("Potència Aparent", (data2));
  sensor.addField("Factor Potència", data3);
  sensor.addField("Voltatge", data4);
  sensor.addField("Intensitat", data5);
  sensor.addField("Temperatura", data6);
  sensor.addField("Pressió", data7);
  sensor.addField("Humitat", data8);
  sensor.addField("CO2", data9);
  sensor.addField("TVOC", data10);

  // Print what are we exactly writing
  Serial.print("Writing: ");
  Serial.println(sensor.toLineProtocol());

  // If no Wifi signal, try to reconnect it
  if ((WiFi.RSSI() == 0) && (wifiMulti.run() != WL_CONNECTED)) {
    Serial.println("Wifi connection lost");
  }

  // Write point
  if (!client.writePoint(sensor)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  //Wait 10s
  Serial.println("Wait 10s");
  delay(10000);
}
