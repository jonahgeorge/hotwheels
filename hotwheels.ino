const int IR_SENSOR_PIN1 = 6;
const int IR_SENSOR_PIN2 = 7;
const int BUTTON_PIN = 12;

unsigned long race_start_ms;
unsigned long race_end_ms;

class State {
  public:
    int button_state;
    int ir_sensor_state1;
    int ir_sensor_state2;

    State() {
      button_state = digitalRead(BUTTON_PIN);
      ir_sensor_state1 = digitalRead(IR_SENSOR_PIN1);
      ir_sensor_state2 = digitalRead(IR_SENSOR_PIN2);
    }

    bool beam_broken() {
      return ir_sensor_state1 == LOW || ir_sensor_state2 == LOW;
    }

    bool button_pressed(State prev) {
      return button_state == HIGH && prev.button_state == LOW;
    }
};

State previous_state;
State current_state;

void setup()  {
  pinMode(IR_SENSOR_PIN1, INPUT);
  pinMode(IR_SENSOR_PIN2, INPUT);
  pinMode(BUTTON_PIN, INPUT);

  digitalWrite(IR_SENSOR_PIN1, HIGH); 
  digitalWrite(IR_SENSOR_PIN2, HIGH); 
  
  Serial.begin(9600);
}

void loop() {
  current_state = State();

  if (current_state.button_pressed(previous_state)) {
    if (race_in_progress()) {
      reset_race();
    } else {
      start_race();
    }
  }

  if (current_state.beam_broken() && race_in_progress()) {
    race_end_ms = millis();
    unsigned long duration_ms = race_end_ms - race_start_ms;
    String duration_formatted = (duration_ms / 1000) + (String) "s" + (duration_ms % 1000) + (String) "ms";
    log("Race Over: " + duration_formatted);
  }

  if (race_in_progress()) {
    log("Race in progress");
  }

  previous_state = current_state;
}

void log(String msg) {
  Serial.println(millis() + (String) ": " + msg);
}

bool reset_race() {
  log("Race reset");
  race_start_ms = 0;
  race_end_ms = 0;
}

bool start_race() {
  log("Race started");
  race_start_ms = millis();
  race_end_ms = 0;
}

bool race_in_progress() {
  return race_start_ms != 0 && race_end_ms == 0;
}
