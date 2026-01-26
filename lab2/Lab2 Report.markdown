# Lab2 EE Fundamentals Lab Report

###### Linghao Meng 2430494

### 1 Calculation

#### 1.1 VOUT

LM317 regulator VOUT1: 2.02V

TPS79301 regulator VOUT2: 2.12V

MIC5377 regulator VOOUT3: 2.12V

#### 1.2 LM317

$$
V_{out} = V_{ref}(1+\frac{R_2}{R_1}) + I_{adj} \times R_2
$$

Vref = 1.25V  

Typcally 'R1' is 240Ω, so 2.02V = 1,25V * (1 + R2 / 240Ω), we can get 'R2'=147.8Ω

However, 'R2' is actually R1, 'R1' is actually R2, which could be a mistake.

So, R1 = 147.8Ω, R2 = 240Ω
$$
R_7 = \frac{V_{out}-V_F}{I_{led}}
$$


Forward current for LS R976 is 20mA,  Vf is 2V

R7 = 10Ω

#### 1.3 TPS79301

$$
V_{out} = V_{ref} \times (1+\frac{R_1}{R_2})
$$

Vref = 1.2246V

The recommended design procedure is to choose 'R2' = 30.1 kΩ

So the 'R1' = 22 kΩ

'R1' is actually R3, 'R2' is actually R4

So, R3 = 22kΩ, R4 = 30.1Ω

Forward current for LS R976 is 20mA, Vf is 2V

R8 = 6Ω

#### 1.4 MIC5377

$$
V_{out} = V_{ref} \times (1+\frac{R_1}{R_2})
$$

Vref = 1V

So, we can set  'R1' = 112 kΩ and 'R2' = 100 kΩ .

'R1' is actually R5, 'R2' is actually R6

So, R5 = 112kΩ, R6 = 100kΩ

#### 1.5 Screenshot

![image-20260112112747707](screenshot.png)

### 2 Voltage Measurement Exercise

![image-20260112112747707](board.jpg)

![image-20260112112747707](serial.png)

github link：https://github.com/menglh20/HelloXiao/blob/main/lab2/readVoltage/src/main.cpp