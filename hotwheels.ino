#include <Wire.h>
#include <Adafruit_GFX.h>

#include "Adafruit_LEDBackpack.h"

Adafruit_7segment matrix = Adafruit_7segment();

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

    int duration_ms() {
      return end_ms - start_ms;
    }

    int elapsed_ms() {
      return millis() - start_ms;
    }
};

class Log {
  public:
    static void info(String msg) {
      Serial.println(millis() + (String) ": " + msg);
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

void setup()  {
  pinMode(IR_SENSOR_PIN1, INPUT);
  pinMode(IR_SENSOR_PIN2, INPUT);
  pinMode(BUTTON_PIN, INPUT);

  digitalWrite(IR_SENSOR_PIN1, HIGH); 
  digitalWrite(IR_SENSOR_PIN2, HIGH); 

  matrix.begin(0x70);
  
  Serial.begin(9600);
}

void write_display(int elapsed_ms) {
  matrix.println(String(elapsed_ms / 1000.0, 2));
  matrix.writeDisplay();
}

State previous_state;
State current_state;
Race race;

void hotloop() {
  if (!race.in_progress() && current_state.button_pressed(previous_state)) {
    Log::info("Race started");
    race = Race();
    race.start();
    return;
  }

  if (race.in_progress() && current_state.button_pressed(previous_state)) {
    Log::info("Race reset");
    race = Race();
    return;
  }

  if (race.in_progress() && current_state.beam_broken()) {
    Log::info("Race ended");
    race.end();
    return;
  }

  if (race.in_progress()) {
    write_display(race.elapsed_ms());
    return;
  }
}

void loop() {
  current_state = State();
  hotloop();
  previous_state = current_state;
}
