#define IR_SENSOR_PIN1 6
#define IR_SENSOR_PIN2 7

unsigned long race_start_ms;
unsigned long race_end_ms = 0;

void setup()  {
  pinMode(IR_SENSOR_PIN1, INPUT);
  pinMode(IR_SENSOR_PIN2, INPUT);

  digitalWrite(IR_SENSOR_PIN1, HIGH); 
  digitalWrite(IR_SENSOR_PIN2, HIGH); 
  
  Serial.begin(9600);

  race_start_ms = millis();
  Serial.println("Race started");
}

void loop() {
  int ir_sensor_state1 = digitalRead(IR_SENSOR_PIN1);
  int ir_sensor_state2 = digitalRead(IR_SENSOR_PIN2);

  if (race_end_ms != 0) {
    return;
  }

  if (beam_broken(ir_sensor_state1) || beam_broken(ir_sensor_state2)) {
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
