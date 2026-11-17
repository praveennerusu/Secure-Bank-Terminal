# SecureBank Terminal

### RFID and PIN-Based ATM Transaction Simulator using LPC2148 and Linux C

SecureBank Terminal is an embedded systems project that simulates basic ATM operations using an **ARM7 LPC2148 microcontroller**, RFID reader, 4×4 keypad, 16×2 LCD, and a Linux-based PC application.

The LPC2148 handles the hardware and user interface, while the Linux PC application manages user accounts, verifies PINs, processes transactions, and stores account details in text files.

### Project Information

| Category | Details |
|---|---|
| Project Name | SecureBank Terminal |
| Project Type | Embedded Systems |
| Microcontroller | ARM7 LPC2148 |
| Programming Language | Embedded C and Linux C |
| Firmware IDE | Keil µVision |
| Simulation | Proteus |
| PC Application | Linux |
| Communication | UART0 and UART1 |
| Database | Text File Handling |



## 1. Features

- RFID card detection and identification.
- PIN-based user authentication.
- Masked PIN entry on the LCD.
- Account balance inquiry.
- Cash deposit simulation.
- Cash withdrawal with balance checking.
- Account blocking after three incorrect PIN attempts.
- Active and blocked account management.
- Mini-statement to view recent transactions.
- PIN change functionality.
- 16×2 LCD display for messages and menus.
- 4×4 keypad for user input.
- UART communication between LPC2148 and Linux PC.
- User account management using text files.
- Transaction history storage.

---

## 2. System Architecture

The project has two main parts:

1. **Embedded System:** LPC2148 handles RFID reading, keypad input, LCD display, and ATM operations.
2. **PC Application:** Linux C program handles account verification, PIN validation, balance updates, and transaction records.

### Block Diagram

```text
         
                                   +-------------------+
                    Keypad  --->   |                   | ---> LCD Display
                                   |                   |
                                   |                   | 
                                   |                   |
                                   |     LPC2148       |
                                   |                   |      
                                   |                   |          
     RFID Card --> RFID Reader --> | UART1       UART0 | <---> MAX232  <---> PC Database
                                   |                   |
                                   +-------------------+
```

### Working of the Architecture

- RFID reader sends the card ID to LPC2148 through UART1.
- LPC2148 sends the card ID to the Linux PC through UART0.
- Linux checks the card details in `users.txt`.
- The user enters the PIN using the keypad.
- The PC verifies the PIN and account status.
- The user selects an ATM operation.
- Linux processes the request and updates the database.
- The result is sent back to LPC2148 and displayed on the LCD.

---

## 3. Hardware Requirements

| Component | Purpose |
|---|---|
| LPC2148 | Main microcontroller |
| RFID Reader | Reads RFID card number |
| RFID Card | Identifies the user |
| 4×4 Keypad | PIN and transaction input |
| 16×2 LCD | Displays ATM messages |
| MAX232 | Converts UART and RS-232 voltage levels |
| USB-to-TTL Adapter | Connects MCU UART to PC |
| Linux PC | Runs the banking application |
| Jumper Wires | Hardware connections |
| Power Supply | Provides required power |

### MAX232 Connection

LPC2148 uses TTL-level UART communication, whereas traditional RS-232 ports use different voltage levels.

- Use MAX232 when connecting to a traditional RS-232 interface.
- Use a USB-to-TTL adapter when connecting directly to a PC USB port.
- A MAX232 is generally not required with a compatible USB-to-TTL adapter.
- Always check voltage compatibility before connecting.

---

## 4. Pin Connections

The following connections are based on the project source code. Verify them with your actual circuit before connecting the hardware.

### 4.1 LCD Connections (16×2, 8-bit Mode)

| LCD Pin | LPC2148 Pin |
|---|---|
| D0–D7 | P0.16–P0.23 |
| RS | P0.5 |
| RW | P0.6 |
| Enable | P0.4 |
| VSS | GND |
| VDD | Suitable supply |
| V0 | Contrast adjustment |

The LCD uses an 8-bit data interface.

### 4.2 Keypad Connections (4×4)

| Keypad Line | LPC2148 Pin |
|---|---|
| Row 0 | P1.16 |
| Row 1 | P1.17 |
| Row 2 | P1.18 |
| Row 3 | P1.19 |
| Column 0 | P1.20 |
| Column 1 | P1.21 |
| Column 2 | P1.22 |
| Column 3 | P1.23 |

**Keypad Layout:**

```text
  1    2    3    A
  4    5    6    B
  7    8    9    C
  *    0    #    E
```

The numeric keys are used for PIN and amount entry. Other keys perform control operations such as backspace, confirmation, and exit.

### 4.3 UART0 Connections (LPC2148 to PC)

UART0 is used for communication between the microcontroller and Linux PC.

| Signal | LPC2148 Pin |
|---|---|
| TXD0 | P0.0 |
| RXD0 | P0.1 |
| GND | GND |

Connect TX to RX and RX to TX between the two devices.

**Communication Settings:**

- Baud Rate: 9600
- Format: 8-N-1

The baud-rate divisor must match the LPC2148 peripheral clock configuration.

### 4.4 UART1 Connections (RFID Reader)

UART1 is used to receive RFID card information.

| Signal | LPC2148 Pin |
|---|---|
| TXD1 | P0.8 |
| RXD1 | P0.9 |
| GND | GND |

**Connections:**

- RFID TX → LPC2148 RXD1 (P0.9).
- RFID RX → LPC2148 TXD1 (P0.8), if required by the reader.
- RFID GND → Common GND.

The firmware expects a framed card ID containing an 8-digit identifier. Different RFID readers may require changes to the receiving code.


---

## 5. Working Process

The ATM system works in the following steps:

**Step 1: Start the System**

The LCD displays the project title and waits for an RFID card.

**Step 2: RFID Card Detection**

The RFID reader reads the card number and sends it to LPC2148 through UART1.

**Step 3: Card Verification**

LPC2148 sends the card number to the Linux PC through UART0. The PC checks whether the card exists and whether the account is active.

**Step 4: PIN Entry**

If the card is valid, the LCD asks the user to enter the PIN using the keypad. The entered digits are masked on the LCD.

**Step 5: PIN Authentication**

The Linux application checks the entered PIN.

If the PIN is correct, the user can access the ATM menu. Incorrect PIN attempts are counted, and the account can be blocked after three consecutive failures.

**Step 6: Select ATM Operation**

The user can select:

- Balance Inquiry
- Deposit
- Withdrawal
- Mini-Statement
- PIN Change

**Step 7: Process Transaction**

The Linux PC processes the selected operation and updates the account balance when required.

**Step 8: Display Result**

The PC sends the result to LPC2148. The LCD displays the transaction result.

**Step 9: Exit**

The user can exit the session and return to the RFID card scanning screen.

---

## 6. Menu and Keypad Operations

The ATM menu contains two pages.

| Key | Operation |
|---|---|
| 1 | Balance Inquiry |
| 2 | Deposit |
| 3 | Withdrawal |
| 4 | Next Menu / Previous Menu |
| 5 | Mini-Statement |
| 6 | PIN Change |
| # | Exit Session |
| B | Backspace |
| E | Confirm Input / Screen-specific Exit |

The exact function of the control keys depends on the current LCD screen and firmware implementation.

---

## 7. UART Communication Protocol

UART0 is used to exchange messages between LPC2148 and the Linux PC.

The messages use `$` as the end marker.

### MCU to PC Messages

**1. RFID Card Number**

```text
#CARD:12535708$
```

**2. Card Number and PIN**

```text
#CARD:12535708#PIN:1234$
```

**3. Balance Inquiry**

```text
#TXN:BALANCE#REQ$
```

**4. Deposit Request**

```text
#TXN:DEPOSIT#AMOUNT:100$
```

**5. Withdrawal Request**

```text
#TXN:WITHDRAW#AMOUNT:200$
```

### PC to MCU Responses

**1. Successful Response**

```text
@OK$
```

**2. Invalid Card**

```text
@ERR#INVALID_CARD$
```

**3. Invalid PIN**

```text
@ERR#INVALID_PIN$
```

**4. Balance Information**

```text
@OK#BALANCE:7500.00$
```

These are example messages. The actual message format must match the selected MCU firmware and Linux PC application.

Both programs must use the same message format and end marker.

---

## 8. Project Folder Structure

```text
SecureBankTerminal/
│
├── linux/
│   ├── pc_main.c
│   ├── uart.c
│   ├── uart.h
│   ├── users.txt
│   └── transactions.txt
│
└── mcu/
    ├── main.c
    ├── rfid_uart1.c
    ├── uart0_pc.c
    ├── uart0_pc.h
    ├── KPM.c
    ├── KPM.h
    ├── KPM_defines.h
    ├── lcd.c
    ├── lcd.h
    ├── lcd_defines.h
    ├── delay.c
    ├── delay.h
    ├── define.h
    ├── types.h
    └── Startup.s
```



## 9. Setup and Execution

### A. Linux PC Application

**Requirements:**

- Linux operating system
- GCC compiler
- UART serial connection

**Step 1: Check the serial device**



**Step 2: Open the Linux project directory**



**Step 3: Compile the program**



**Step 4: Run the application**


**Step 5: Check database files**

Make sure `users.txt` and `transactions.txt` are available in the working directory.




## 10. User Database Format

The Linux application stores user details in `users.txt`.

Each user record contains six fields separated by `|`.

**Format:**

```text
CARD_ID|PIN|NAME|BALANCE|STATUS|FAILED_ATTEMPTS
```



### Field Description

| Field | Description |
|---|---|
| CARD_ID | RFID card number |
| PIN | User PIN |
| NAME | Account holder name |
| BALANCE | Available account balance |
| STATUS | ACTIVE or BLOCKED |
| FAILED_ATTEMPTS | Incorrect PIN attempt count |

The PC application reads these records to authenticate users and manage accounts.

**Security Note:** These are sample credentials for demonstration. Do not use real banking PINs or personal account information.

---

## 11. Transaction History

The application stores deposit and withdrawal records in `transactions.txt`.

**Format:**

```text
CARD_ID|TRANSACTION_TYPE|AMOUNT
```


The program appends new transaction records to the file.

The mini-statement feature reads the transaction history and displays recent transactions for the authenticated user.

The exact transaction order and supported transaction types depend on the Linux application implementation.

---



## 12. Limitations and Security

- This project is developed for educational and demonstration purposes.
- It does not connect to real banking servers.
- It does not dispense physical cash.
- User information is stored in plain-text files.
- PINs are not encrypted.
- UART communication is not encrypted or authenticated.
- Hardware connections and baud-rate settings must be verified.
- The system should not be used for real financial transactions.

---

## 13. Learning Outcomes

Through this project, I gained practical experience in:

- ARM7 LPC2148 microcontroller programming.
- Embedded C programming.
- GPIO and peripheral interfacing.
- UART serial communication.
- UART receive interrupts.
- RFID reader interfacing.
- LCD and matrix keypad interfacing.
- Embedded application development.
- Linux system programming using C.
- Serial-port communication in Linux.
- File handling and database management.
- Linked lists and data structures.
- ATM transaction processing.
- Hardware and software debugging.
- Communication between an embedded system and a PC application.

---

## 14. Author

**NERUSU PRAVEEN KUMAR**

Embedded Systems| Firmware Development
