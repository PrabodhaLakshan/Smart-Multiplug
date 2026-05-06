# Smart-Multiplug
This is the Smart Multiplug . it can control web application and mobile application

# 🚀 Smart Multi Plug System (ESP8266)

A smart multi plug system built using NodeMCU (ESP8266) that allows **dual control via mobile app and web browser**, with an added **emergency power cut-off system**.

---

## 📌 Features

* 🔌 Control up to 6 devices individually
* 📱 Mobile App Control using Blynk IoT
* 🌐 Web Control using local IP address (browser)
* 🔄 Cross-Control Capability

  * Turn ON using app → Turn OFF using web (and vice versa)
* 🚨 Emergency Master Switch

  * Instantly cuts off the main 230V power supply
  * Disconnects power to all relays
* 📊 Real-time response system
* 📱 Mobile-friendly web interface

---

## 🛠️ Technologies Used

* ESP8266 (NodeMCU)
* 8-Channel Relay Module
* Blynk IoT Cloud
* Embedded Web Server (HTML, CSS, JavaScript)

---

## ⚙️ Blynk Setup Guide

### 1. Create Template

* Go to Blynk IoT Dashboard
* Create a new Template

### 2. Copy Credentials

Replace these in your code:

```cpp
#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_TEMPLATE_NAME "SmartPlug"
#define BLYNK_AUTH_TOKEN "YourAuthToken"
```

### 3. Create Datastreams

| Function  | Virtual Pin |
| --------- | ----------- |
| Relay 1   | V1          |
| Relay 2   | V2          |
| Relay 3   | V3          |
| Relay 4   | V4          |
| Relay 5   | V5          |
| Relay 6   | V6          |
| Emergency | V7          |

* Type: Integer
* Min: 0
* Max: 1

### 4. Add Widgets

* Add 7 Buttons in dashboard
* Set mode to **Switch**
* Assign V1–V7 accordingly

---

## 🌐 Web Control

After uploading the code, open Serial Monitor:

```
IP Address: 192.168.x.x
```

Open in browser:

```
http://192.168.x.x
```

You can now control all relays using the web interface.

---

## 🔌 Pin Configuration

| Relay           | NodeMCU Pin |
| --------------- | ----------- |
| R1              | D1          |
| R2              | D2          |
| R3              | D5          |
| R4              | D6          |
| R5              | D7          |
| R6              | D0          |
| R7 (Main Power) | D8          |

---

## ⚡ Safety Notes

* Ensure proper isolation when working with 230V AC
* Use fuse/MCB protection
* Avoid using boot pins (D3, D4) for relay control
* Always test with low voltage before full deployment

---

## 💡 How It Works

* ESP8266 connects to WiFi
* Blynk app sends commands via cloud
* Web server allows local browser control
* Relay states are synchronized across both interfaces
* Emergency switch cuts off main power supply instantly

---





## ⭐ If you like this project

Give it a ⭐ on GitHub and share!

