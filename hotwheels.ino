#define IR_SENSOR_PIN 4

int ir_sensor_state = 0;

unsigned long race_start_ms;
unsigned long race_end_ms = 0;

void setup()  {
  pinMode(IR_SENSOR_PIN, INPUT);
  digitalWrite(IR_SENSOR_PIN, HIGH); 
  
  Serial.begin(9600);

  race_start_ms = millis();
  Serial.println("Race started");
}

void loop() {
  ir_sensor_state = digitalRead(IR_SENSOR_PIN);

  if (race_end_ms != 0) {
    return;
  }

  if (beam_broken(ir_sensor_state)) {
    race_end_ms = millis();

    unsigned long duration_ms = race_end_ms - race_start_ms;
    String duration_formatted = (duration_ms / 1000) + (String) "s" + (duration_ms % 1000) + (String) "ms";

    Serial.println("Race Over: " + duration_formatted);
    Serial.println("Reset device to start new race");
  }
}

bool beam_broken(int ir_sensor_state) {
  return ir_sensor_state == LOW;
}
