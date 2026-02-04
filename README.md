## 🏥 Bed Management System (C++)

- A console-based Bed Management System developed in C++ to help manage hospital beds efficiently.
- This system allows users to add beds, admit patients, discharge patients, transfer patients, and generate bed availability reports.

## 📌 Features

-✅ Add hospital beds (Bed ID, Ward, Room)
-✅ Admit patients and automatically assign an available bed
-✅ Discharge patients and free the occupied bed
-✅ Transfer patients to another available bed
-✅ Search patient details using Patient ID
-✅ View reports:

        - All beds

        -Available beds

        -Occupied beds

## 🛠️ Technologies Used

-Programming Language: C++

-Concepts Used: Object-Oriented Programming (OOP)

-Application Type: Console Application

## 📂 Project Structure
Bed-Management-System/
│
├── main.cpp
└── README.md

## ⚙️ Installation & Setup
-✅ Requirements

-To run this project, you need:

-Any C++ compiler (GCC / MinGW / CodeBlocks / Dev-C++)

## ▶️ How to Compile and Run
-🔹 Using g++
-g++ main.cpp -o bed_system
-./bed_system

-🔹 Recommended (C++17)
-g++ main.cpp -std=c++17 -o bed_system
-./bed_system

## 🧑‍⚕️ How to Use the System

- When you run the program, you will see a menu like this:

========== Bed Management System ==========
1) Add Bed
2) Admit Patient
3) Discharge Patient
4) Transfer Patient
5) Search Patient
6) List All Beds
7) List Available Beds
8) List Occupied Beds
0) Exit
==========================================

## 🧾 System Modules
1️⃣ Add Bed

-Allows the user to add a new bed using:

    -Bed ID

    -Ward Name

    -Room Number

2️⃣ Admit Patient

- Admits a patient by taking:

      -Patient ID

      -Name

       -Age

      - Gender

- Diagnosis
  - Then assigns the patient an available bed automatically.

3️⃣ Discharge Patient

- Discharges a patient using Patient ID and frees the bed.

4️⃣ Transfer Patient

- Transfers a patient to another available bed (based on ward/room preference).

5️⃣ Search Patient

- Finds a patient by Patient ID and shows:

- Assigned Bed ID

- Patient details

6️⃣ Reports

- Displays:

      -All beds

       -Available beds

         -Occupied beds

## 📌 Sample Output
          -Admitted. Assigned Bed ID: 101
         -Bed[BedID=101, Ward=Ward-A, Room=Room-1, Status=OCCUPIED, Patient[ID=1, Name=John, Age=25, Gender=Male, Dx=Fever]]

## 🚀 Future Enhancements

-✨ Save and load patient/bed data using files (CSV / TXT)
-✨ Add database support (SQLite / MySQL)
-✨ Add login system (Admin / Nurse)
-✨ Add admission time and discharge history
-✨ Build GUI version using Qt or Web App

## 🤝 Contributing

-Contributions are welcome!

-Fork the repository

-Create a new branch

-Make changes

-Submit a Pull Request

## 📄 License

-This project is free to use for learning, academic, and educational purposes.
