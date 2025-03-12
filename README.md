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
- **Base Option Class:**  
  Contains common attributes such as strike, expiry, and option type.
- **BarrierOption Class:**  
  Extends the base option with barrier specific properties (barrier level, barrier type) and custom payoff calculations based on barrier breaches.

### 2. Pricing Engine
- **Unified Interface:**  
  An abstraction that lets users switch between different Monte Carlo pricing methods without changing the interface.
- **Implementation Variants:**  
  Different classes implement the simulation techniques (naive, antithetic, importance sampling, control variates, stratified sampling) to price the options.
- **Adapter Pattern:**  
  Bridges the gap between the interface and concrete pricing implementations.

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


# Compilation Steps


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

This process automatically creates a `build` folder that contains all the linked and compiled outputs. Within the build folder, you will find a `results` folder that holds various analysis files for reviewing our outputs. Additionally, multiple executables are generated. You can run them using the following commands:

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
































[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-2972f46106e565e64193e422d61a12cf1da4916b45550586e14ef0a7c637dd04.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=18123827)
# IB9JHO Group Project
This is a blank template for your group to implement your project in the IB9JHO environment.

# Using CMake to build your main program:
When you add source files to the project which you want compiled and linked to your program, you need to add them to the CMakeLists.txt file at the root of the project. If you are getting LNK errors it is usually because you forgot to add one of your .cpp files

For example:
```
set(SRC_FILES ${CMAKE_SOURCE_DIR}/src/my_class1.cpp ${CMAKE_SOURCE_DIR}/src/my_class2.cpp)
add_executable(my_program ${SRC_FILES} ${CMAKE_SOURCE_DIR}/src/main.cpp)

```

Will create an executable called my_program by compiling my_class1.cpp, my_class2.cpp and main.cpp. Any cpp files you want to add to the project can be set in the first line.
To keep things simple, make sure you put all source and header files in the /src directory.

Note: Some functions from the standard math library may require that you link your executable explictly. This is usually only a problem on linux and is fixed by adding 
```
target_link_libraries(my_program m)
```
after add_executable

# Using CMake to add tests:

There is a seperate CMakeLists.txt file in the /tests directory for adding tests. For each test you would like to add to the testing suite you 
will add a cpp file which includes catch2 (see tests/my_test.cpp for an example).

Then in the CMakeLists.txt file:
```
add_executable(my_test ${SRC_FILES} my_test.cpp)
target_include_directories(my_test PUBLIC ${CMAKE_SOURCE_DIR}/extern/catch2 ${CMAKE_SOURCE_DIR}/src)
add_test(NAME my_test COMMAND my_test)
```

will create a test called my_test which will run the my_test.cpp file. Note that SRC_FILES is inherited from the main CMakeLists.txt file so in the case above your test has all the same source files as the main program.

# Using Git to manage your group:
It is suggested that each group member creates their own branch to prevent clashes. Each group member can create branches in github by clicking
'branches' when you are on the <> code tab of the repository. Each branch is independent from the main branch which will eventually contain your final project.

You can add the changes from your personal branch to the main branch using pull requests. You can create these in github, go to the pull requests tab and click new pull request. Then select the branch you want to merge into the main branch. It is recommended to then have the strongest programmer in the team review these pull requests and check your code can safely be merged without breaking anything.

# Submission
To submit, push your final project to your group's repository on GitHub on the main branch. Extract the contents of the repository to a zip file and submit it via my.wbs.

## Conclusion
This project demonstrates a comprehensive approach to barrier option pricing using multiple Monte Carlo methods and advanced variance reduction techniques, including naive Monte Carlo (GBM), antithetic variates, importance sampling, control variates, and stratified sampling. By maintaining a modular structure for the option models, pricing engines, and reporting tools, the codebase remains both extensible and maintainable. We hope this framework serves as a robust foundation for further exploration and enhancement within the field of computational finance.
