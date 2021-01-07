# Ard_Node_HEM
Yet another home energy monitor

*Arduino and NodeMCU Home energy monitor, home climate and air quality*

My first project as a complet newbie with arduino and Nextion displays.

Hardware:
  
  **-Arduino Mega**
	
  **-NodeMCU 1.0 ESP 12F**
	
  **-Nextion Display 4.3" Enhanced Version NX4827K043** 
	
  **-Adafruit CCS811 Air Quality Sensor Breakout**
	
  **-Adafruit BME280 Humidity + Barometric Pressure + Temperature Sensor Breakout**
	
  **-Current Sensor SCT-013-000 (100A)**
	
  **-ZMPT101B Module for voltage measures**

  **-InfluxDB v2.00** 
  
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  In the "Nextion" folder you can find the .hmi file (nextion editor files), and all the images I used for my display
  
  Inside the other 2 folders you'll find the code for the mega and the code for the NodeMCU
  
  I'm sure that there's a million things to improve, if you want to collaborate, you'll be very welcome
  
  # Wiring Schema
  
  ![alt text](https://github.com/Damiasroca/Ard_Node_HEM/blob/main/Conexions_schem.png?raw=true)
  
  
  Note the 470µF capacitor hooked on the Nextion display, as **@PerryBebbington** pointed out in this [thread](https://forum.arduino.cc/index.php?topic=604185.68).

