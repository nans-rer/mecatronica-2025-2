#include <WiFi.h>
#include <WebServer.h>

// ================= Pines =================
// Motor A (lado izquierdo)
const int AIN1 = 23;
const int AIN2 = 4;
const int PWMA = 22;

// Motor B (lado derecho)
const int BIN1 = 18;
const int BIN2 = 19;
const int PWMB = 21;

// STBY (habilita el TB6612FNG)
const int STBY = 5;

// LEDs (ajústalos si quieres)
const int LED_1 = 32;
const int LED_2 = 26;
const int LED_3 = 33;
const int LED_4 = 25;

// ================= PWM =================
const int pwmFreq = 1000;
const int pwmResolution = 8;
const int pwmChannelA = 0;
const int pwmChannelB = 1;

// Velocidad base (0–255)
int motorSpeed = 180;

// ================= WiFi =================
// Insertar el nombre de su red WiFi
const char* ssid = "Nannn";
const char* password = "876543210";

// Servidor web en el puerto 80
WebServer server(80);

// ================= HTML (WASD + Flechas) =================
const char* html = R"rawliteral(
<!DOCTYPE html>
<html>
<head><meta name="viewport" content="width=device-width, initial-scale=1", meta charset="UTF-8">
<style>
 body{font-family:system-ui,Segoe UI,Roboto,Arial;margin:16px}
 button{padding:16px 18px;font-size:18px;margin:8px;border-radius:10px}
 .grid{display:grid;gap:8px;grid-template-columns:repeat(3,120px);justify-content:center;margin-top:16px}
 .kbd{padding:2px 6px;border:1px solid #ddd;border-radius:6px;background:#f7f7f7}
</style>
</head>
<body>
<h2>Desafío 1: "Fats & Rats"</h2>
<h3>Mecatrónica ME4250</h3>
<p>Usa <span class="kbd">W</span> <span class="kbd">A</span> <span class="kbd">S</span> <span class="kbd">D</span> o las flechas. <span class="kbd">Space</span> = Stop</p>

<div class="grid">
 <div></div>
 <button onclick="send('forward')">⏩ Adelante</button>
 <div></div>

 <button onclick="send('left')">⬅️ Izquierda</button>
 <button onclick="send('stop')">⏹ Stop</button>
 <button onclick="send('right')">➡️ Derecha</button>

 <div></div>
 <button onclick="send('backward')">⏪ Atrás</button>
 <div></div>
</div>

<p>Velocidad: <span id="v">180</span>
 <input id="spd" type="range" min="0" max="255" value="180" oninput="chgSpeed(this.value)">
</p>

<hr> <div class="grid" style="grid-template-columns: repeat(2, 1fr);">
    <button onclick="send('function1')">💡 Función 1</button>
    <button onclick="send('function2')">⚙️ Función 2</button>
</div>

<script>
 const v = document.getElementById('v');
 function send(cmd){ fetch('/' + cmd).catch(()=>{}); }
 function chgSpeed(val){ v.textContent = val; fetch('/speed?val=' + val).catch(()=>{}); }

 // Teclado: WASD y flechas
 const mapDown = {
  'w':'forward','W':'forward','ArrowUp':'forward',
  's':'backward','S':'backward','ArrowDown':'backward',
  'a':'left','A':'left','ArrowLeft':'left',
  'd':'right','D':'right','ArrowRight':'right',
  ' ':'stop'
 };
 const pressed = new Set();
 window.addEventListener('keydown',(e)=>{
  if(!(e.key in mapDown)) return;
  if(!pressed.has(e.key)){
   pressed.add(e.key);
   send(mapDown[e.key]);
  }
  e.preventDefault();
 },{passive:false});
 window.addEventListener('keyup',(e)=>{
  if(!(e.key in mapDown)) return;
  pressed.delete(e.key);
  if(e.key !== ' '){ send('stop'); }
  e.preventDefault();
 },{passive:false});
</script>
</body>
</html>
)rawliteral";

// ================= Util LEDs =================
void ledsOff() {
    // Apaga todas las leds cambiando los estados a LOW
digitalWrite(LED_1, LOW);
digitalWrite(LED_2, LOW);
digitalWrite(LED_3, LOW);
digitalWrite(LED_4, LOW);
}

void ledsOn1() {
    // Prende la led 1 cambiando a HIGH mientras mantiene el resto en LOW
digitalWrite(LED_1, HIGH);
digitalWrite(LED_2, LOW);
digitalWrite(LED_3, LOW);
digitalWrite(LED_4, LOW);
}

void ledsOn2() {
    // Prende la led 2 cambiando a HIGH mientras mantiene el resto en LOW
digitalWrite(LED_1, LOW);
digitalWrite(LED_2, HIGH);
digitalWrite(LED_3, LOW);
digitalWrite(LED_4, LOW);
}

void ledsOn3() {
    // Prende la led 3 cambiando a HIGH mientras mantiene el resto en LOW
digitalWrite(LED_1, LOW);
digitalWrite(LED_2, LOW);
digitalWrite(LED_3, HIGH);
digitalWrite(LED_4, LOW);
}

void ledsOn4() {
    // Prende la led 4 cambiando a HIGH mientras mantiene el resto en LOW
digitalWrite(LED_1, LOW);
digitalWrite(LED_2, LOW);
digitalWrite(LED_3, LOW);
digitalWrite(LED_4, HIGH);
}
// ================= Motores =================
void motorsStop() {
    // Funcion que apaga los motores cambiandole la velocidad a 0 usando ledcWrite()
// Llama la funcion para apagar las leds
ledsOff();

// Da direccion al motor A seteando el primer pin en LOW y el segundo en HIGH
digitalWrite(AIN1, LOW);
digitalWrite(AIN2, HIGH);

// Da direccion al motor B, usando la direccion contraria al motor A
digitalWrite(BIN1, HIGH);
digitalWrite(BIN2, LOW); 

ledcWrite(PWMA, 0); // Apaga el motor A seteandole velocidad 0
ledcWrite(PWMB, 0); // Apaga el motor B seteandole velocidad 0
}

void motorsForward() {
    // Funcion que hace que el auto se mueva hacia adelante cambiandole la velocidad
    // a los motores. Usa la velocidad motorSpeed definida al inicio del codigo
// Llama la funcion para prender el led 1
ledsOn1();

// Da direccion al motor A seteando el primer pin en HUGH y el segundo en LOW
digitalWrite(AIN1, HIGH);
digitalWrite(AIN2, LOW);

// Da direccion al motor B, usando la direccion contraria al motor A
digitalWrite(BIN1, LOW);
digitalWrite(BIN2, HIGH); 

ledcWrite(PWMA, motorSpeed); // Prende el motor A con velocidad motorSpeed
ledcWrite(PWMB, motorSpeed); // Prende el motor B con velocidad motorSpeed
}

void motorsBackward() {
    // Funcion que hace que el auto se mueva hacia atras cambiandole la velocidad y direccion
    // a los motores. Usa la velocidad motorSpeed definida al inicio del codigo
// Prende la led 2
ledsOn2();

// Da direccion al motor A, seteando el primer pin en LOW y el segundo en HIGH
digitalWrite(AIN1, LOW);
digitalWrite(AIN2, HIGH);

// Da direccion al motor B, usando la direccion contraria al motor A
digitalWrite(BIN1, HIGH);
digitalWrite(BIN2, LOW); 

ledcWrite(PWMA, motorSpeed); // Prende el motor A con velocidad motorSpeed
ledcWrite(PWMB, motorSpeed); // Prende el motor B con velocidad motorSpeed
}

void motorsLeft() {// gira sobre eje: A atrás, B adelante
    // Funcion que hace que el auto se mueva hacia la izquierda cambiandole la velocidad y direccion
    // a los motores. Usa la velocidad motorSpeed definida al inicio del codigo
ledsOn4();

// Da direccion al motor A, seteando el primer pin en HIGH y el segundo en LOW
digitalWrite(AIN1, HIGH);
digitalWrite(AIN2, LOW);

// Da direccion al motor B, usando la direccion contraria al motor A
digitalWrite(BIN1, LOW);
digitalWrite(BIN2, HIGH); 

ledcWrite(PWMA, motorSpeed); // Prende el motor A con velocidad motorSpeed
ledcWrite(PWMB, 0); // Apaga el motor B, setenado su velocidad en 0
}

void motorsRight() {// gira sobre eje: A adelante, B atrás
    // Funcion que hace que el auto se mueva hacia la derecha cambiandole la velocidad y direccion
    // a los motores. Usa la velocidad motorSpeed definida al inicio del codigo
ledsOn3();

// Da direccion al motor A, seteando el primer pin en HIGH y el segundo en LOW
digitalWrite(AIN1, HIGH);
digitalWrite(AIN2, LOW);

// Da direccion al motor B, con la direccion contraria al motor B
digitalWrite(BIN1, LOW);
digitalWrite(BIN2, HIGH); 

ledcWrite(PWMA, 0); // Apaga el motor A, seteando su velocidad en 0
ledcWrite(PWMB, motorSpeed); // Prende el motor B con velocidad motorSpeed
}

// ================= Funciones extras =================

void function1() {
 // Funcion que hace que el auto gire sobre su propio eje. Logra esto girando ambos motores en sentidos contrarios a la misma velocidad
 // Llama la funcion para prender el led 1
ledsOn1();

// Da direccion al motor A, seteando el primer pin en HIGH y el segundo en LOW
digitalWrite(AIN1, HIGH);
digitalWrite(AIN2, LOW);

// Da direccion al motor B, esta vez usando la misma direccion que A
digitalWrite(BIN1, HIGH);
digitalWrite(BIN2, LOW); 

ledcWrite(PWMA, motorSpeed); // Prende el motor A con velocidad motorSpeed
ledcWrite(PWMB, motorSpeed); // Prende el motor B con velocidad motorSpeed
 Serial.println("Ejecutando Función 1");
}


void function2() {
 // Funcion que hace que el auto gire sobre su propio eje. 
 // Logra esto girando ambos motores en sentidos contrarios a la misma velocidad
 // Llama la funcion para prender el led 1
ledsOn1();

// Da direccion al motor A, seteando el primer pin en HIGH y el segundo en LOW
digitalWrite(AIN1, LOW);
digitalWrite(AIN2, HIGH);

// Da direccion al motor B, esta vez usando la misma direccion que A
digitalWrite(BIN1, LOW);
digitalWrite(BIN2, HIGH); 

ledcWrite(PWMA, motorSpeed); // Prende el motor A con velocidad motorSpeed
ledcWrite(PWMB, motorSpeed); // Prende el motor B con velocidad motorSpeed
 Serial.println("Ejecutando Función 1");
}

// ================= Setup =================
void setup() {
 Serial.begin(115200);

// definir el tipo de señal para los pines del driver y los leds
// Define todos los pines de los motores como output
pinMode(AIN1, OUTPUT); 
pinMode(AIN2, OUTPUT);
pinMode(BIN1, OUTPUT);
pinMode(BIN2, OUTPUT);
pinMode(STBY, OUTPUT);

// Define todos los pines led como output
pinMode(LED_1, OUTPUT);
pinMode(LED_2, OUTPUT);
pinMode(LED_3, OUTPUT);
pinMode(LED_4, OUTPUT);


 digitalWrite(STBY, HIGH); // habilitar driver

// Configurar PWM en ESP32

 ledcAttach(PWMA, pwmFreq, pwmResolution);
 ledcAttach(PWMB, pwmFreq, pwmResolution);


 motorsStop();

// Conexión WiFi (a tu router)
 WiFi.begin(ssid, password);
 Serial.print("Conectando a WiFi");
 while (WiFi.status() != WL_CONNECTED) {
  delay(400);
  Serial.print(".");
 }
 Serial.println("\nConectado. IP: ");
 Serial.println(WiFi.localIP());

// Rutas HTTP
 server.on("/", [](){ server.send(200, "text/html", html); });

 server.on("/forward", [](){ motorsForward(); server.send(200,"text/plain","Forward"); });
 server.on("/backward", [](){ motorsBackward(); server.send(200,"text/plain","Backward"); });
 server.on("/left", [](){ motorsLeft(); server.send(200,"text/plain","Left"); });
 server.on("/right", [](){ motorsRight(); server.send(200,"text/plain","Right"); });
 server.on("/stop", [](){ motorsStop(); server.send(200,"text/plain","Stop"); });

 // **Nuevas rutas para las funciones 1 y 2**
 server.on("/function1", [](){ function1(); server.send(200,"text/plain","Function 1 executed"); });
 server.on("/function2", [](){ function2(); server.send(200,"text/plain","Function 2 executed"); });

// Cambiar velocidad desde el slider
 server.on("/speed", [](){
  if (server.hasArg("val")) {
   motorSpeed = constrain(server.arg("val").toInt(), 0, 255);
  }
  server.send(200, "text/plain", String(motorSpeed));
 });

 server.begin();
 Serial.println("Servidor HTTP listo.");
}

// ================= Loop =================
void loop() {
 server.handleClient();
}