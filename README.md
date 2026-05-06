# MD_TCS230_FLEX

[![License: LGPL v2.1](https://img.shields.io/badge/License-LGPL_v2.1-blue.svg)](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html)
[![Hardware](https://img.shields.io/badge/Hardware-TCS230%20%2F%20TCS3200-orange.svg)]()

A flexible, fully-featured Arduino library for the **TCS230 / TCS3200** color light-to-frequency converter.

This library is an extended/flexible version of the original `MD_TCS230` library. It allows seamless color reading, frequency scaling, and hardware control with minimal latency. It supports both blocking and non-blocking reads, making it ideal for fast-paced robotics applications (such as Mini Sumo robots) or industrial color-sorting setups where your microcontroller needs to process other tasks concurrently.

---

## 🚀 Key Features

*   **Non-blocking (Asynchronous) Reads:** Read RGB values in the background without freezing your main loop.
*   **Blocking Reads:** Simple, straightforward reading methods for less timing-critical setups.
*   **Sensor Calibration:** Easily calibrate dark (black) and white levels for highly accurate RGB interpolation and lighting adjustments.
*   **Hardware Integration:** Full programmatic control over Output Enable (`OE`) and frequency prescalers (`S0`, `S1`).
*   **Flexible Pin Mapping:** Configure the `S2` and `S3` photodiode selection pins dynamically to avoid hardware conflicts.
*   **High Compatibility:** Operates smoothly alongside motor drivers, ultrasonic sensors, and interrupt-heavy tasks.

---

## ⚙️ Hardware Dependencies

**IMPORTANT:** This library relies on the [**FreqCount**](https://github.com/PaulStoffregen/FreqCount) library to accurately measure the sensor's frequency output. 

`FreqCount` utilizes hardware timers and imposes specific hardware limitations. You **must** connect the sensor's `OUT` pin to the specific hardware timer pin of your Arduino:

| Board | Sensor `OUT` Pin | Pins Unusable with `analogWrite()` |
| :--- | :---: | :--- |
| **Arduino Uno** | **5** | 3, 9, 10, 11 |
| **Arduino Nano** | **5** | 3, 9, 10, 11 |
| **Arduino Mega** | **47** | 9, 10, 44, 45, 46 |
| **Arduino Leonardo** | **12** | 3, 9, 10, 11 |

*(Please ensure you have downloaded and installed the `FreqCount` library via the Arduino IDE Library Manager before compiling).*

---

## 🔌 Pinout & Connections

The TCS230/TCS3200 module has 8 pins. A typical connection for an **Arduino Uno** is as follows:

| TCS230 Pin | Description | Arduino Connection |
| :--- | :--- | :--- |
| **GND** | Ground | `GND` |
| **VCC** | Power (2.7V - 5.5V) | `5V` |
| **OUT** | Frequency Output | **Pin 5** *(Strictly required by FreqCount)* |
| **S0** | Frequency Scaling | Any Digital Pin *(Optional)* |
| **S1** | Frequency Scaling | Any Digital Pin *(Optional)* |
| **S2** | Filter Selection | Any Digital Pin *(e.g., Pin 6)* |
| **S3** | Filter Selection | Any Digital Pin *(e.g., Pin 7)* |
| **OE** | Output Enable (Active Low) | Any Digital Pin / `GND` *(Optional)* |

---

## 🛠️ Installation

1.  Download this repository as a `.zip` file.
2.  Open the **Arduino IDE**.
3.  Navigate to **Sketch** -> **Include Library** -> **Add .ZIP Library...**
4.  Select the downloaded `.zip` file.
5.  Open the **Library Manager** (`Ctrl+Shift+I`) and install **FreqCount**.

---

## 📖 Quick Start Example

Below is a minimal code example to quickly get the sensor reading basic RGB values.

```cpp
#include <MD_TCS230_FLEX.h>
#include <FreqCount.h>

// Define color sensor control pins
#define S2_PIN  6
#define S3_PIN  7

// Initialize the flexible sensor object
MD_TCS230_FLEX colorSensor(S2_PIN, S3_PIN);

void setup() {
  Serial.begin(115200);
  
  // Initialize sensor pins and default states
  colorSensor.begin();
  Serial.println("TCS230_FLEX Sensor Initialized.");
}

void loop() {
  // Start the reading sequence
  colorSensor.read();

  // Wait until the non-blocking read is completely finished
  while (!colorSensor.available()) {
    // Other tasks can be performed here without blocking!
  }

  // Fetch the newly calculated RGB data
  colorData_FLEX rgb;
  colorSensor.getRGB(&rgb);

  // Print results
  Serial.print("Red: "); Serial.print(rgb.value[TCS230_FLEX_RGB_R]);
  Serial.print(" | Green: "); Serial.print(rgb.value[TCS230_FLEX_RGB_G]);
  Serial.print(" | Blue: "); Serial.println(rgb.value[TCS230_FLEX_RGB_B]);
  
  delay(250); // Small delay for readability
}
```

---

## ⚖️ License & Credits

Copyright (C) 2013 Marco Colli. 
Modified, refactored, and maintained as the **`FLEX`** variant.

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.
