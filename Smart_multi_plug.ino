#define BLYNK_TEMPLATE_ID "" // past your template ID
#define BLYNK_TEMPLATE_NAME ""//past your template 
#define BLYNK_AUTH_TOKEN ""//past your torken

#define BLYNK_TIMEOUT_MS 3000
#define BLYNK_HEARTBEAT  30

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WebServer.h>

char ssid[] = "";// add your wifi router SSID
char pass[] = "";// Add your wifi router password 

ESP8266WebServer server(80);

#define R1 D1
#define R2 D2
#define R3 D5
#define R4 D6
#define R5 D7
#define R6 D0
#define R7 D3

bool mainPower = true;
bool rState[6] = {false,false,false,false,false,false};

const char webpage[] PROGMEM = R"====(
<!DOCTYPE html>
<html>
<head>
<title>Smart Multi Plug</title>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{background:#0a0f1e;font-family:Arial,sans-serif;color:#e2e8f0}
.wrap{max-width:480px;margin:0 auto;padding:20px 16px 32px}
.header{text-align:center;padding:20px 0 24px}
.logo-ring{width:64px;height:64px;border-radius:50%;border:2px solid #3b82f6;display:flex;align-items:center;justify-content:center;margin:0 auto 12px;background:rgba(59,130,246,0.1);font-size:28px}
h1{font-size:20px;font-weight:600;color:#f1f5f9;margin-bottom:4px}
.subtitle{font-size:11px;color:#64748b;letter-spacing:1px}
.status-bar{display:flex;align-items:center;justify-content:center;gap:8px;margin-top:10px}
.dot{width:7px;height:7px;border-radius:50%;background:#22c55e;animation:pulse 2s infinite}
@keyframes pulse{0%,100%{opacity:1}50%{opacity:0.4}}
.status-txt{font-size:11px;color:#22c55e;letter-spacing:1px}
.section-label{font-size:10px;color:#475569;letter-spacing:2px;text-transform:uppercase;margin:20px 0 10px}
.grid{display:grid;grid-template-columns:1fr 1fr;gap:10px}
.plug-card{background:#111827;border:1px solid #1e293b;border-radius:16px;padding:16px;cursor:pointer;transition:all 0.25s;position:relative;overflow:hidden;-webkit-tap-highlight-color:transparent}
.plug-card:active{transform:scale(0.96)}
.plug-card.on{background:#0f2040;border:2px solid #3b82f6;box-shadow:0 0 20px rgba(59,130,246,0.25)}
@keyframes flash{0%{background:#1d4ed8}100%{background:#0f2040}}
.plug-card.on{animation:flash 0.3s ease-out}
.plug-card.on .plug-icon{background:#1e3a5f}
.plug-card.on .toggle{background:#1d4ed8}
.plug-card.on .toggle::after{left:23px;background:#fff}
.plug-card.on .plug-num{color:#1d4ed8}
.plug-card.on .plug-name{color:#60a5fa;font-weight:600}
.plug-card.on .plug-state{color:#3b82f6;font-weight:600}
.plug-card.disabled{opacity:0.35;cursor:not-allowed;pointer-events:none}
.plug-top{display:flex;align-items:center;justify-content:space-between;margin-bottom:14px}
.plug-icon{width:36px;height:36px;border-radius:10px;background:#1e293b;display:flex;align-items:center;justify-content:center;font-size:18px;transition:background 0.25s}
.toggle{width:44px;height:24px;border-radius:12px;background:#1e293b;position:relative;transition:background 0.25s;flex-shrink:0}
.toggle::after{content:'';position:absolute;top:3px;left:3px;width:18px;height:18px;border-radius:50%;background:#475569;transition:all 0.25s}
.plug-num{font-size:10px;color:#334155;font-weight:600;letter-spacing:1px;margin-bottom:2px}
.plug-name{font-size:15px;font-weight:500;color:#94a3b8;transition:color 0.25s}
.plug-state{font-size:10px;color:#334155;margin-top:4px;letter-spacing:1px;transition:color 0.25s}
.active-count{text-align:center;margin-top:14px;font-size:12px;color:#475569;letter-spacing:1px}
.active-count span{color:#3b82f6;font-weight:700;font-size:14px}
.ctrl-row{display:grid;grid-template-columns:1fr 1fr;gap:10px;margin-bottom:10px}
.btn-kill{background:#1a0a0a;border:1px solid #7f1d1d;border-radius:14px;padding:16px;cursor:pointer;text-align:center;color:#ef4444;font-size:13px;font-weight:600;width:100%;transition:all 0.2s;-webkit-tap-highlight-color:transparent}
.btn-kill:active{transform:scale(0.96);background:#ef4444;color:#fff}
.btn-power{background:#071a10;border:1px solid #14532d;border-radius:14px;padding:16px;cursor:pointer;text-align:center;color:#22c55e;font-size:13px;font-weight:600;width:100%;transition:all 0.2s;-webkit-tap-highlight-color:transparent}
.btn-power:active{transform:scale(0.96);background:#22c55e;color:#fff}
.btn-emergency{width:100%;border-radius:14px;padding:18px 16px;cursor:pointer;font-size:15px;font-weight:700;letter-spacing:1px;border:2px solid #dc2626;background:#1a0505;color:#ef4444;transition:all 0.3s;-webkit-tap-highlight-color:transparent;display:flex;align-items:center;justify-content:center;gap:10px}
.btn-emergency.powered{border-color:#22c55e;background:#051a0a;color:#22c55e}
.btn-emergency:active{transform:scale(0.97)}
.emg-icon{font-size:24px}
.emg-label{display:flex;flex-direction:column;align-items:flex-start}
.emg-title{font-size:14px;font-weight:700;letter-spacing:1px}
.emg-sub{font-size:10px;font-weight:400;opacity:0.7;margin-top:2px;letter-spacing:0.5px}
.powercut-banner{display:none;background:#2d0505;border:1px solid #7f1d1d;border-radius:12px;padding:12px 16px;text-align:center;color:#ef4444;font-size:12px;font-weight:600;letter-spacing:1px;margin-bottom:10px;animation:blink 1s infinite}
@keyframes blink{0%,100%{opacity:1}50%{opacity:0.5}}
.powercut-banner.show{display:block}
.btn-icon{font-size:20px;display:block;margin-bottom:5px}
.footer{text-align:center;margin-top:28px;font-size:10px;color:#1e293b;letter-spacing:1px}
</style>
</head>
<body>
<div class="wrap">
  <div class="header">
    <div class="logo-ring">&#9889;</div>
    <h1>Smart Multi Plug</h1>
    <div class="subtitle">ESP8266 &middot; LOCAL CONTROL</div>
    <div class="status-bar">
      <div class="dot"></div>
      <span class="status-txt">ONLINE</span>
    </div>
  </div>
  <div class="section-label">Plug Controls</div>
  <div class="grid">
    <div class="plug-card" id="c0" onclick="toggle(0,'r1')">
      <div class="plug-top"><div class="plug-icon">&#128268;</div><div class="toggle"></div></div>
      <div class="plug-num">P01</div><div class="plug-name">Plug 01</div><div class="plug-state">STANDBY</div>
    </div>
    <div class="plug-card" id="c1" onclick="toggle(1,'r2')">
      <div class="plug-top"><div class="plug-icon">&#128268;</div><div class="toggle"></div></div>
      <div class="plug-num">P02</div><div class="plug-name">Plug 02</div><div class="plug-state">STANDBY</div>
    </div>
    <div class="plug-card" id="c2" onclick="toggle(2,'r3')">
      <div class="plug-top"><div class="plug-icon">&#128268;</div><div class="toggle"></div></div>
      <div class="plug-num">P03</div><div class="plug-name">Plug 03</div><div class="plug-state">STANDBY</div>
    </div>
    <div class="plug-card" id="c3" onclick="toggle(3,'r4')">
      <div class="plug-top"><div class="plug-icon">&#128268;</div><div class="toggle"></div></div>
      <div class="plug-num">P04</div><div class="plug-name">Plug 04</div><div class="plug-state">STANDBY</div>
    </div>
    <div class="plug-card" id="c4" onclick="toggle(4,'r5')">
      <div class="plug-top"><div class="plug-icon">&#128268;</div><div class="toggle"></div></div>
      <div class="plug-num">P05</div><div class="plug-name">Plug 05</div><div class="plug-state">STANDBY</div>
    </div>
    <div class="plug-card" id="c5" onclick="toggle(5,'r6')">
      <div class="plug-top"><div class="plug-icon">&#128268;</div><div class="toggle"></div></div>
      <div class="plug-num">P06</div><div class="plug-name">Plug 06</div><div class="plug-state">STANDBY</div>
    </div>
  </div>
  <div class="active-count"><span id="cnt">0</span> of 6 plugs active</div>
  <div class="section-label" style="margin-top:22px">Controls</div>
  <div class="ctrl-row">
    <button class="btn-kill" onclick="killAll()">
      <span class="btn-icon">&#9209;</span>KILL ALL
    </button>
    <button class="btn-power" onclick="powerAll()">
      <span class="btn-icon">&#9889;</span>POWER ALL
    </button>
  </div>
  <div class="powercut-banner" id="banner">&#9888; MAIN POWER CUT OFF &#9888;</div>
  <div class="section-label">Emergency</div>
  <button class="btn-emergency powered" id="emgBtn" onclick="toggleEmergency()">
    <span class="emg-icon" id="emgIcon">&#128308;</span>
    <span class="emg-label">
      <span class="emg-title" id="emgTitle">CUT OFF POWER</span>
      <span class="emg-sub" id="emgSub">Tap to disconnect main relay</span>
    </span>
  </button>
  <div class="footer">SMART PLUG v2.0 &middot; BLYNK ENABLED</div>
</div>
<script>
var st=[false,false,false,false,false,false];
var emgOn=false;
function upd(){var n=0;for(var i=0;i<6;i++)if(st[i])n++;document.getElementById('cnt').textContent=n;}
function setCard(i,on){var c=document.getElementById('c'+i);var s=c.querySelector('.plug-state');if(on){c.classList.add('on');s.textContent='POWERED ON';}else{c.classList.remove('on');s.textContent='STANDBY';}}
function setDisabled(d){for(var i=0;i<6;i++){var c=document.getElementById('c'+i);if(d)c.classList.add('disabled');else c.classList.remove('disabled');}}
function toggle(i,route){if(emgOn)return;st[i]=!st[i];setCard(i,st[i]);upd();setTimeout(function(){fetch('/'+route);},10);}
function killAll(){for(var i=0;i<6;i++){st[i]=false;setCard(i,false);}upd();setTimeout(function(){fetch('/off');},10);}
function powerAll(){if(emgOn){emgOn=false;updateEmgUI();setTimeout(function(){fetch('/emerOn');},10);}for(var i=0;i<6;i++){st[i]=true;setCard(i,true);}setDisabled(false);upd();setTimeout(function(){fetch('/on');},20);}
function toggleEmergency(){emgOn=!emgOn;updateEmgUI();if(emgOn){setDisabled(true);setTimeout(function(){fetch('/emerOff');},10);}else{setDisabled(false);setTimeout(function(){fetch('/emerOn');},10);}}
function updateEmgUI(){var btn=document.getElementById('emgBtn');var icon=document.getElementById('emgIcon');var title=document.getElementById('emgTitle');var sub=document.getElementById('emgSub');var banner=document.getElementById('banner');if(emgOn){btn.classList.remove('powered');icon.textContent='&#128309;';title.textContent='TURN ON POWER';sub.textContent='Tap to restore main relay';banner.classList.add('show');}else{btn.classList.add('powered');icon.innerHTML='&#128308;';title.textContent='CUT OFF POWER';sub.textContent='Tap to disconnect main relay';banner.classList.remove('show');}}
</script>
</body>
</html>
)====";

// -------- RELAY CONTROL --------
void updateRelay(int index, int pin){
  if(mainPower){
    digitalWrite(pin, rState[index] ? LOW : HIGH);
  }
}

// -------- BLYNK --------
BLYNK_WRITE(V1){ rState[0]=param.asInt(); updateRelay(0,R1); }
BLYNK_WRITE(V2){ rState[1]=param.asInt(); updateRelay(1,R2); }
BLYNK_WRITE(V3){ rState[2]=param.asInt(); updateRelay(2,R3); }
BLYNK_WRITE(V4){ rState[3]=param.asInt(); updateRelay(3,R4); }
BLYNK_WRITE(V5){ rState[4]=param.asInt(); updateRelay(4,R5); }
BLYNK_WRITE(V6){ rState[5]=param.asInt(); updateRelay(5,R6); }

BLYNK_WRITE(V7){
  int s=param.asInt();
  if(s==1){ mainPower=false; digitalWrite(R7,HIGH); }
  else     { mainPower=true;  digitalWrite(R7,LOW);  }
}

// -------- WEB ROUTES --------
void setupRoutes(){
  server.on("/",[](){
    server.send_P(200,"text/html",webpage);
  });
  server.on("/r1",[](){ rState[0]=!rState[0]; updateRelay(0,R1); server.send(200,"text/plain","OK"); });
  server.on("/r2",[](){ rState[1]=!rState[1]; updateRelay(1,R2); server.send(200,"text/plain","OK"); });
  server.on("/r3",[](){ rState[2]=!rState[2]; updateRelay(2,R3); server.send(200,"text/plain","OK"); });
  server.on("/r4",[](){ rState[3]=!rState[3]; updateRelay(3,R4); server.send(200,"text/plain","OK"); });
  server.on("/r5",[](){ rState[4]=!rState[4]; updateRelay(4,R5); server.send(200,"text/plain","OK"); });
  server.on("/r6",[](){ rState[5]=!rState[5]; updateRelay(5,R6); server.send(200,"text/plain","OK"); });
  server.on("/off",[](){
    for(int i=0;i<6;i++) rState[i]=false;
    digitalWrite(R1,HIGH); digitalWrite(R2,HIGH);
    digitalWrite(R3,HIGH); digitalWrite(R4,HIGH);
    digitalWrite(R5,HIGH); digitalWrite(R6,HIGH);
    server.send(200,"text/plain","OK");
  });
  server.on("/on",[](){
    mainPower=true;
    digitalWrite(R7,LOW);
    for(int i=0;i<6;i++) rState[i]=true;
    digitalWrite(R1,LOW); digitalWrite(R2,LOW);
    digitalWrite(R3,LOW); digitalWrite(R4,LOW);
    digitalWrite(R5,LOW); digitalWrite(R6,LOW);
    server.send(200,"text/plain","OK");
  });
  server.on("/emerOff",[](){
    mainPower=false;
    digitalWrite(R7,HIGH);
    server.send(200,"text/plain","OK");
  });
  server.on("/emerOn",[](){
    mainPower=true;
    digitalWrite(R7,LOW);
    server.send(200,"text/plain","OK");
  });
}

// -------- SETUP --------
void setup(){
  Serial.begin(115200);
  delay(100);

  pinMode(R1,OUTPUT); pinMode(R2,OUTPUT);
  pinMode(R3,OUTPUT); pinMode(R4,OUTPUT);
  pinMode(R5,OUTPUT); pinMode(R6,OUTPUT);
  pinMode(R7,OUTPUT);

  digitalWrite(R1,HIGH); digitalWrite(R2,HIGH);
  digitalWrite(R3,HIGH); digitalWrite(R4,HIGH);
  digitalWrite(R5,HIGH); digitalWrite(R6,HIGH);
  digitalWrite(R7,LOW);

  // WiFi - no Blynk involved here
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,pass);
  Serial.print("Connecting WiFi");
  int retry=0;
  while(WiFi.status()!=WL_CONNECTED && retry<40){
    delay(500);
    ESP.wdtFeed();
    Serial.print(".");
    retry++;
  }

  if(WiFi.status()!=WL_CONNECTED){
    Serial.println("\nWiFi FAILED - restarting");
    ESP.restart();
  }

  Serial.println();
  Serial.println("==========================");
  Serial.print(">>> Browser: http://");
  Serial.println(WiFi.localIP());
  Serial.println("==========================");

  // Web server starts FIRST - before Blynk
  setupRoutes();
  server.begin();
  Serial.println("Web server started OK");

  // Blynk config only - short timeout so it never blocks web server
  Blynk.config(BLYNK_AUTH_TOKEN);
  ESP.wdtFeed();
}

// -------- LOOP --------
unsigned long lastBlynkRun  = 0;
unsigned long lastBlynkConn = 0;
bool blynkStarted = false;

void loop(){
  unsigned long now = millis();

  // 1. Web server ALWAYS first - highest priority
  for(int i=0; i<5; i++){
    server.handleClient();
    delay(1);
  }

  // 2. Try to connect Blynk in background every 5s if not connected
  if(!blynkStarted && (now - lastBlynkConn > 5000)){
    lastBlynkConn = now;
    Serial.println("Trying Blynk connect...");
    if(Blynk.connect(500)){
      blynkStarted = true;
      Serial.println("Blynk connected!");
    } else {
      Serial.println("Blynk retry later...");
    }
  }

  // 3. Run Blynk only every 25ms - never starves web server
  if(blynkStarted && Blynk.connected() && (now - lastBlynkRun > 25)){
    lastBlynkRun = now;
    Blynk.run();
  }

  // 4. If Blynk drops, reset flag so it reconnects
  if(blynkStarted && !Blynk.connected() && (now - lastBlynkConn > 5000)){
    lastBlynkConn = now;
    blynkStarted  = false;
    Serial.println("Blynk dropped - retrying...");
  }

  yield();
}