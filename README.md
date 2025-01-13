# SmartMailbox-ESP32

I woke up sweating - I just realized that in the last five years I've been living in my apartment, I must have checked the mailbox (like, for snail mail) once a day.  
I live in Germany, so I get actual paper mail about twice a week.  
That amounts to 365 × 5 × 15 seconds × (5/7) = about 5½ hours of wasted time!  
I realized this waste cannot stand, and now I'm only checking the mailbox when there's something in it!  
Considering the time I've put into the project, if I continue living in this apartment for the next 100 years, it's going to be worth my while!  
<br/>
## How does it work

I don’t have Wi-Fi or power where my mailbox is located, so the next best option is a BLE device running on batteries.  
When I walk by, my iPhone connects to the device. It then checks the contents of the mailbox using an array of ultrasonic sensors (HC-SR04).  
If there’s something in the mailbox, the iPhone is notified and displays a notification.  
It’s a rather simple setup.  
Initially, I wanted to use a couple of reed switches instead.  
The idea was to connect one to the hatch and another to the door, count how many times the hatch is opened, and reset the count when the mailbox door is opened.  
The ESP32 can wake up from light sleep on a reed switch change event and keep track of the mail count.  
However, I couldn’t make the magnets stick. I’m a software guy and terrible at handling hardware, so I ended up using ultrasonic sensors instead.  

## Components

* ESP32-C6 dev board (I could'nt find a bare board, which is good, cause I can't solder to save my life)
* 4xAA battery pack
* DC to DC stepdown buck, with 3.3v and 5v output
* 2x ultra sonic sensors (HC-SR04)

![ESP32C6 dev board](https://raw.githubusercontent.com/moshegottlieb/libsense/SmartMailbox-ESP32/master/images/esp32c6.jpg)
![HC-SR04 ultrasonic sensor](https://raw.githubusercontent.com/moshegottlieb/SmartMailbox-ESP32/master/images/ultrasonic.jpg)
![Battery pack](https://raw.githubusercontent.com/moshegottlieb/SmartMailbox-ESP32/master/images/battery-pack.jpg)
