// Gorra para ciegos - Arduino Nano
// Sensor izquierdo -> vibrador izquierdo
// Sensor derecho   -> vibrador derecho
// Sensor frontal   -> los dos vibradores

// ---------- PINES ----------
// Sensor izquierdo: Trig envía el sonido, Echo recibe el rebote
const int TRIG_IZQ = 2;
const int ECHO_IZQ = 3;

// Sensor frontal
const int TRIG_FRE = 4;
const int ECHO_FRE = 5;

// Sensor derecho
const int TRIG_DER = 6;
const int ECHO_DER = 7;

// Motores vibradores (conectados a los transistores)
const int VIB_IZQ = 9;
const int VIB_DER = 10;

// ---------- DISTANCIA ----------
// Si hay un objeto a menos de estos centímetros, vibra el motor
const int UMBRAL_CM = 150;

// ---------- FUNCIÓN PARA MEDIR ----------
// Mide la distancia en cm. Devuelve 999 si no hay eco.
long medirCm(int trig, int echo) {
  // Ponemos Trig en LOW para empezar desde cero
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  // Enviamos un pulso de 10 microsegundos para lanzar el sonido
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Medimos cuánto tarda en volver el eco (máx. 25 ms, unos 4 m)
  long duracion = pulseIn(echo, HIGH, 25000);

  // Si no volvió el eco, no hay nada cerca: devolvemos 999
  if (duracion == 0) return 999;

  // Convertimos el tiempo a centímetros
  return duracion / 58;
}

// ---------- CONFIGURACIÓN INICIAL ----------
void setup() {
  // Cada Trig es una salida (envía) y cada Echo una entrada (recibe)
  pinMode(TRIG_IZQ, OUTPUT);
  pinMode(ECHO_IZQ, INPUT);
  pinMode(TRIG_FRE, OUTPUT);
  pinMode(ECHO_FRE, INPUT);
  pinMode(TRIG_DER, OUTPUT);
  pinMode(ECHO_DER, INPUT);

  // Los vibradores son salidas
  pinMode(VIB_IZQ, OUTPUT);
  pinMode(VIB_DER, OUTPUT);

  // Monitor Serie para ver las distancias en el computador
  Serial.begin(9600);
}

// ---------- PROGRAMA PRINCIPAL (se repite siempre) ----------
void loop() {
  // Medimos cada sensor, uno detrás de otro, con una pausa entre ellos
    long distanciaIzquierdaCm = medirCm(TRIG_IZQ, ECHO_IZQ);
    delay(30);
    long distanciaFrontalCm = medirCm(TRIG_FRE, ECHO_FRE);
    delay(30);
    long distanciaDerechaCm = medirCm(TRIG_DER, ECHO_DER);
    delay(30);

  // Guardamos si cada sensor detecta algo a menos de 150 cm
  bool hayObjetoIzquierda = distanciaIzquierdaCm < UMBRAL_CM;
  bool hayObjetoFrente    = distanciaFrontalCm < UMBRAL_CM;
  bool hayObjetoDerecha   = distanciaDerechaCm < UMBRAL_CM;

  // Vibra el izquierdo si lo detecta el sensor izquierdo o el frontal
digitalWrite(VIB_IZQ, (hayObjetoIzquierda || hayObjetoFrente) ? HIGH : LOW);

  // Vibra el derecho si lo detecta el sensor derecho o el frontal
  digitalWrite(VIB_DER, (hayObjetoDerecha || hayObjetoFrente) ? HIGH : LOW);

  // Mostramos las tres distancias en el Monitor Serie (para pruebas)
Serial.print("Izq: "); Serial.print(distanciaIzquierdaCm);
Serial.print(" cm | Frente: "); Serial.print(distanciaFrontalCm);
Serial.print(" cm | Der: "); Serial.print(distanciaDerechaCm);
Serial.println(" cm");
}
