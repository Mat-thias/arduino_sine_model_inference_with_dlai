#include "sine_model.h"

#include "dlai.h"
#include "Layers/src/layers.h"
#include "Models/src/models.h"

#include "Layers/src/layers.cpp"
#include "Models/src/models.cpp"

#define ANALOG_LED_PIN    11

#define PI                3.14159265359
// The maximum workspace size, it is the second line of the sine_model.h file
#define MAX_WORKSPACE_SIZE  16 * 2

// The number of layers in the file 
#define LAYER_SIZE          5

// Macro to convert sine output in range [-1, 1] to PWM value in [0, 255]
#define sine_value_to_brightness(x) (int)((x + 1) * 255 / 2)

// Declare a static array of layer pointers to represent the model graph
Layer *graph[LAYER_SIZE];

// Allocate workspace memory used for internal computations (activations, temp buffers)
float workspace[MAX_WORKSPACE_SIZE];

// Construct the neural network model using the `sine_model` definition
Sequential model(
  sine_model,          // Model weights and parameters (from header)
  sine_model_len,      // Length of model array
  graph,               // Layer storage
  LAYER_SIZE,           // Number of layers
  workspace,           // Scratch buffer for inference
  MAX_WORKSPACE_SIZE   // Size of buffer
);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ANALOG_LED_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  float *input = model.input;
  float *output = model.output;

  long start_time;

  for (int x=0; x<360; x++) {

    // Input to the network: a single float representing an angle in radians
    float *input = model.input;

    // Record start time to measure inference duration
    start_time = millis();

    // Output of the network: the predicted sine value
    float *output = model.output;

    // Convert degrees to radians
    *input = (float)x * 2 * PI / 360;

    // Predict using the model
    model.predict();

    // Convert sine value [-1,1] to brightness [0,255] and write to LED
    int y = sine_value_to_brightness(*output);
    analogWrite(ANALOG_LED_PIN, y);
    
    Serial.print("sine("); Serial.print(x); Serial.print("°) = ");
    Serial.print(y);
    Serial.print(" -- [");
    Serial.print(millis() - start_time); Serial.println(" ms]");
  }

}
