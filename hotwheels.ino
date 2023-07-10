const int IR_SENSOR_PIN1 = 6;
const int IR_SENSOR_PIN2 = 7;
const int BUTTON_PIN = 12;

class Race {
  public:
    unsigned long start_ms;
    unsigned long end_ms;

    Race() {
      start_ms = 0;
      end_ms = 0;
    }

    void start() {
      start_ms = millis();
    }

    void end() {
      end_ms = millis();
    }

    bool in_progress() {
      return start_ms != 0 && end_ms == 0;
    }

    unsigned long duration() {
      return end_ms - start_ms;
    }

    String duration_formatted() {
      unsigned long d = duration();
      return (d / 1000) + (String) "s" + (d % 1000) + (String) "ms";
    }
};

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
Race race;

void log(String msg) {
  Serial.println(millis() + (String) ": " + msg);
}

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

  if (race.in_progress()) {
    if (current_state.button_pressed(previous_state)) {
      // Race in progress, button pressed
      race = Race();
      return;
    }

    if (current_state.beam_broken()) {
      // Race in progress, beam broken
      race.end();
      log("Race Over: " + race.duration_formatted());
      return;
    }

    log("Race in progress");
  } else {
    if (current_state.button_pressed(previous_state)) {
      // Button pressed, race not in progress
      race.start();
      log("Race started");
      return;
    }
  }

  previous_state = current_state;
}
