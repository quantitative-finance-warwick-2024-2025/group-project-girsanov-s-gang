# IB9JHO: Barrier Option Pricing

## Project Overview
This project focuses on the quantitative pricing of barrier options derivatives whose outcomes depend on whether an asset’s price breaches a set barrier during its life. Our approach employs Monte Carlo simulation enhanced by variance reduction techniques to obtain precise and efficient pricing estimates. The methods implemented include:

- **Naive Monte Carlo (GBM)**
- **Antithetic Variates**
- **Importance Sampling**
- **Control Variates**
- **Stratified Sampling**

## Architecture and Components of the Project 

### 1. Option Models
- **Base Option Class (`Option.cpp`, `Option.hpp`):**  
  Contains common attributes such as strike price, expiry, and option type (Call/Put).
  
- **Barrier Option Class (`BarrierOption.cpp`, `BarrierOption.hpp`):**  
  Inherits from the base option class and introduces barrier-specific properties:
  - Barrier level
  - Barrier type (Knock-In/Knock-Out)
  - Custom payoff logic based on barrier activation.


### 2. Pricing Engines
- **Core Pricing Interface:**  
  Multiple implementations of Monte Carlo methods encapsulated within their respective classes for flexibility:
  - `OptionPricerBarrier.cpp/hpp`: **Naive Monte Carlo** pricing.
  - `OptionPricerBarrierAntithetic.cpp/hpp`: **Antithetic Variates** method.
  - `OptionPricerBarrierIS.cpp/hpp`: **Importance Sampling** method.
  - `OptionPricerBarrierStratified.cpp/hpp`: **Stratified Sampling** method.

- **Pricing Engine (`PricingEngineBarrier.hpp`):**  
  Centralized interface for integrating various pricing models, following a loosely defined adapter pattern.


### 3. Analysis and Reporting
- **CSV Report Generators:**  
  Modules designed to output analysis data for:
  - Convergence behavior
  - Sensitivity to volatility changes
  - Spot price impact analysis
  - Error tolerance levels
  - Efficiency metrics

### 4. Testing Framework
- **Automated Unit Tests:**  
  A series of tests (using Catch2) ensure that every component, from option pricing to data analysis, functions correctly.

### 5. External Files

- **UMLDiagram.plantuml (PlantUML File)**   
  Contains the code for generating our UML diagram, which provides a high-level view of the project’s structure and dependencies.

- **testing_barr.py (Barrier Pricing Test in Python)**  
  This script was initially used to verify the correctness of our Naive model.



# Build and Run Instructions

### Requirements
- C++ compiler supporting C++17 or newer
- CMake (v3.14+)
- (Optional) Catch2 framework for running unit tests
- Clang (v10.0+)





## Recommended Method (Using VSCode)

For the smoothest experience—just like in our lectures—follow these steps:
1. **Clone the Repository**  
   Run the following command in your terminal:  
   ```bash
   git clone https://github.com/quantitative-finance-warwick-2024-2025/group-project-girsanov-s-gang.git
   ```
1. **Open VSCode.**
2. **Navigate to the project directory:** `group-project-girsanov-s-gang`
3. **Click "Build"** at the bottom of the VSCode window.
4. When prompted, select the main `CMakeLists.txt` (not the test version).
5. **Click "Run"** at the bottom.

This process automatically creates a `build` folder that contains all the linked and compiled outputs. Within the build folder, you will find a `results` folder that holds various analysis files for reviewing our outputs. Additionally, multiple executables are generated. You can run them within the build folder using the following commands:

```bash
./Girsanov_gang_proj
./generate_convergence_output
./option_pricing_convergence
```

### Alternate Run Solution
If the program has been succefully compiled but not ran you can run the main executable from the build directory manually by navigating to he build directory and running the following command:
- **Linux/Mac:**
  ```bash
  ./Girsanov_gang_proj
  ```
- **Windows:**
  ```bash
  Girsanov_gang_proj.exe
  ```

The application should also create the exact same analysis report


### Executing the Tests

After building the project, you can run the tests in two ways:

1. **Run All Tests at Once (Using CTest)**  
   From your build directory, simply run:
   ```bash
   ctest

This automatically discovers and executes all test executables (including 
`naive_pricing`, `antithetic_pricing`, `control_variates_pricing`, 
`importance_sampling_pricing`, `test_option`, `test_barrier_option`, and 
`test_main`).

2. **Run Individual Test Executables**  
If you prefer to run a specific test on its own, navigate to the tests directory within your build folder (note: there are two tests folders, one in `src/tests` and one in `build/tests`; you must use the latter) and execute the desired binary directly.

**Linux/macOS:**
```bash
./naive_pricing
./antithetic_pricing
./control_variates_pricing
./importance_sampling_pricing
./option_tests
./barrier_tests
./stratified_sampling_pricing
```

**Windows:**
```bash
.\naive_pricing.exe
.\antithetic_pricing.exe
.\control_variates_pricing.exe
.\importance_sampling_pricing.exe
.\option_tests.exe
.\barrier_tests.exe
.\stratified_sampling_pricing.exe
```


## UML Diagram
Below is a concise graphcial overview of our project only including our *key* classes:
![image](https://github.com/user-attachments/assets/188d73a1-2efa-4cbe-b729-f3b69aba539b)
The following PlantUML diagram provides a high-level overview of the code and its dependencies. It intentionally omits the full subclass relationships for Analysis and Testing to avoid overcomplicating the diagram and to maintain clarity.


---



