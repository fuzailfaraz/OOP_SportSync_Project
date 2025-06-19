# 🏆 SportSync – Smart Sports Week Management System

**SportSync** is a C++ OOP-based console application designed to streamline match scheduling, player registration, and event conflict resolution for university sports weeks. Built using core object-oriented programming concepts and file handling, it offers an end-to-end solution for managing events efficiently.

## 📌 Project Overview

SportSync simulates a real-world sports event system with three user roles:

- 👨‍💼 **Admin (Organizer)** – Add, edit, and delete matches with built-in clash detection.
- 🧑‍🎓 **Player** – Register, view personalized team schedules.
- 👁️ **Viewer** – Public access to view complete match schedules.

All data (matches, players, logs) is stored using plain-text files.

## 🎯 Key Features

- ✅ **Role-Based Access**: Admin, Player, Viewer modes
- 🧠 **Conflict Detection**: Prevents overlapping matches by venue, time, and team
- 📝 **File Handling**: Match data (`schedule.txt`), player data (`players.txt`), and admin logs (`admin_log.txt`)
- 📅 **Preloaded Weekly Schedule**: Auto-populates matches if none exist
- 🔒 **Secure Admin Login** with fallback system password

## 🧠 OOP Concepts Used

| Concept        | Usage                                  |
|----------------|-----------------------------------------|
| Encapsulation  | Private player/schedule data            |
| Inheritance    | Admin, Player, Viewer inherit `User`    |
| Polymorphism   | `login()` function overridden by roles  |
| Abstraction    | `User` is an abstract base class        |

## 🛠️ Technologies Used

- 💻 Language: C++
- 📁 Storage: File I/O (`fstream`)
- 🧰 STL: `vector`, `stringstream`
- 🧮 Time Handling: `ctime`

## 🧑‍💻 OutPut ScreenShot
ADMIN LOGIN :
![image](https://github.com/user-attachments/assets/3a2db7c4-0381-4522-80da-7eacc31b3b70)
![image](https://github.com/user-attachments/assets/3ee91822-a485-47cd-b474-972612ccbb15)

VIEWER PAGE:
![image](https://github.com/user-attachments/assets/ed6f0bb0-e199-4474-bb37-209f7ae91022)

PLAYER REGISTRATION:
![image](https://github.com/user-attachments/assets/8fc45661-c9e3-4ebc-8fd8-b24d8b1a20f5)
![image](https://github.com/user-attachments/assets/5c520741-1be0-45c4-b3d2-23f9a4058e22)








