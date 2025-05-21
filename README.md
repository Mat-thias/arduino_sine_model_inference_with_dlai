# 🚀 Arduino Sine Model Inference with DLAI

This project demonstrates how to deploy and run a simple neural network model—specifically a sine regression model—on an **Arduino UNO** using the **DLAI (Deep Learning Algorithm Implementation)** library. It showcases the process of integrating a model trained in TensorFlow into a low-resource microcontroller environment.

---

## 📌 Project Description

This is a simple, educational TinyML project that performs sine wave prediction on an Arduino UNO. The model used is created in **TensorFlow**, consisting of:

* **Input layer** with 1 neuron
* **Two hidden layers**, each with **16 neurons**
* **Random activation functions** (e.g., ReLU, Tanh, etc.)
* **All operations are in floating point** (no quantization)

The model is exported and converted using the DLAI Python tool and then deployed to the Arduino.

---

## 📦 What is DLAI?

**DLAI (Deep Learning Algorithm Implementation)** is a lightweight C/C++ library for deploying deep learning models on embedded systems and microcontrollers.

🔗 **DLAI GitHub Repository**: [https://github.com/Mat-thias/dlai](https://github.com/Mat-thias/dlai)

---

## ✅ What This Project Demonstrates

* Running a **neural network inference** on Arduino UNO
* Converting a model from PyTorch/TensorFlow into C header format
* Using floating point operations for inference
* Driving an **LED output using sine predictions** (brightness mapped from sine value)

---

## 🛠️ Setup Instructions

### 1. Create an Arduino Sketch

Create a new sketch for your Arduino UNO project.

### 2. Download and Add the DLAI Library

* Download `dlai.zip` from: [https://github.com/Mat-thias/dlai](https://github.com/Mat-thias/dlai)
* Open **Arduino IDE**
* Go to **Sketch > Include Library > Add .ZIP Library...**
* Select the downloaded file to install.

### 3. Include Required Headers in Your Sketch

```cpp
#include "sine_model.h"
#include "dlai.h"
#include "Layers/src/layers.h"
#include "Models/src/models.h"
```

### 4. Identify Layer Count and Workspace Size

From `sine_model.h`, find:

```cpp
#define LAYER_SIZE 5
#define MAX_WORKSPACE_SIZE 32
```

Use these constants in your sketch.

### 5. Declare Layer Graph and Workspace

```cpp
Layer *graph[LAYER_SIZE];
float workspace[MAX_WORKSPACE_SIZE];
```

### 6. Initialize the Model

```cpp
Sequential model(
  sine_model,         // Model weights and parameters
  sine_model_len,     // Length of model array
  graph,              // Layer array
  LAYER_SIZE,         // Number of layers
  workspace,          // Scratch buffer
  MAX_WORKSPACE_SIZE  // Buffer size
);
```

### 7. Arduino `setup()` Function

```cpp
void setup() {
  Serial.begin(9600);
  pinMode(ANALOG_LED_PIN, OUTPUT);
}
```

### 8. Arduino `loop()` Function

```cpp
void loop() {
  float *input = model.input;
  float *output = model.output;

  long start_time;

  for (int x = 0; x < 360; x++) {
    *input = (float)x * 2 * PI / 360;  // Convert degrees to radians

    start_time = millis();
    model.predict();                  // Run inference

    int y = sine_value_to_brightness(*output);  // Convert [-1,1] to [0,255]
    analogWrite(ANALOG_LED_PIN, y);             // Drive LED brightness

    Serial.print("sine("); Serial.print(x); Serial.print("°) = ");
    Serial.print(y); Serial.print(" -- [");
    Serial.print(millis() - start_time); Serial.println(" ms]");
  }
}
```

### 9. Brightness Mapping Helper Function

```cpp
int sine_value_to_brightness(float val) {
  val = constrain(val, -1.0, 1.0);   // Ensure range
  return (int)((val + 1.0) * 127.5); // Scale to [0, 255]
}
```

---

## 📁 Folder Structure Example

```
your-sketch/
├── your-sketch.ino
├── sine_model.h         ← Converted model header
├── dlai/                ← DLAI library folder
│   ├── dlai.h
│   ├── Layers/
│   └── Models/
```

---

## 🧠 Summary

With this setup, you're running a floating-point neural network inference on one of the smallest Arduino boards available. This is a solid entry point into **TinyML** development and embedded AI, showcasing how neural models can control real-world hardware like LEDs based on predictions.
