<h3 align="center"><img src="https://i.imgur.com/QSjtgzS.png" alt="logo" height="500px"></h3>

# CCSS Garrobo: Registro Médico Digital 🦎
## Description
CCSS Garrobo is a medical mobile application with patient, doctor and lab technician differentiation that allows you to view personal data, review medical appointments, review and add lab tests and consult the medication assigned to a patient.

## Table of Contents
- [Installation 📥](#installation)
- [Usage 📘](#usage)
- [Features ✨](#features)
- [User Cases 💼](#use-cases-)
- [Dependencies 📦](#dependencies)
- [Configuration ⚙️](#configuration)
- [Screenshots 📸](#screenshots)
- [Troubleshooting 🛠️](#troubleshooting)
- [Contributors 👥](#contributors)

## Installation 📥
To install the servers, follow these steps:
1. Clone the repository: `git clone https://github.com/andressero/firewalls.git`
2. Navigate to the project directory: `cd src/<servers_name>/server`
3. Install the necessary dependencies: `make`  

To install client (application), follow these steps:
1. Same as with servers.
2. Navigate to the project directory: cd src/firewalls-iu-prototype
3. Install the necessary dependencies: `qmake`

## Usage 📘
To run the servers, execute the following command:
```sh
make run
```
To run the application, you can either run it on a Desktop or Android device: 
### Running on Desktop

1. **Open Qt Creator**: Launch Qt Creator on your desktop.

2. **Open the Project**:
   - Go to `File` > `Open File or Project`.
   - Navigate to the directory containing your project and open the `.pro` file.

3. **Configure the Project**:
   - Qt Creator will prompt you to configure the project. Select the appropriate kits (e.g., Desktop Qt 5.15.2 MinGW 64-bit).

4. **Build the Project**:
   - Click on the `Build` button (hammer icon) or press `Ctrl+B` to build the project.

5. **Run the Application**:
   - Click on the `Run` button (green play icon) or press `Ctrl+R` to run the application.

### Running on Android

#### Preparing for Android

1. **Set Up Android Kits**:
   - Open Qt Creator.
   - Go to `Tools` > `Options` > `Kits`.
   - Ensure you have an Android kit set up. If not, configure one by adding the Android SDK and NDK paths.

2. **Configure the Project for Android**:
   - Open your project in Qt Creator.
   - Go to `Projects` mode.
   - Select the Android kit in the `Build & Run` section.

#### Deploying to an Android Device

1. **Connect Your Android Device**:
   - Ensure Developer Mode and USB Debugging are enabled on your Android device.
   - Connect your device to your computer via USB.

2. **Build the Project for Android**:
   - Select the Android kit.
   - Click on the `Build` button (hammer icon) or press `Ctrl+B`.

3. **Run the Application on Android**:
   - Click on the `Run` button (green play icon) or press `Ctrl+R`.
   - Qt Creator will deploy the application to your connected Android device.

#### Exporting Signed APK

1. **Open Project in Qt Creator**:
   - Open your project in Qt Creator and select the Android kit.

2. **Build Release Version**:
   - Go to `Projects` mode.
   - Under `Build & Run`, select the Android kit.
   - Click on the `Release` build configuration.

3. **Open the Release APK Wizard**:
   - Go to `Build` > `Build Android APK`.

4. **Set Up Signing Credentials**:
   - If you haven't set up signing credentials, click `Create...` next to the `Keystore` field to create a new keystore.
   - Fill in the required fields (e.g., keystore path, alias, password).

5. **Sign the APK**:
   - Once your keystore is set up, select it in the `Keystore` field.
   - Fill in the alias and password fields.
   - Click `Next`.

6. **Export the Signed APK**:
   - Click `Finish` to build and sign the APK.
   - The signed APK will be generated in the specified output directory.

7. **Verify the APK**:
   - Use `jarsigner` or another APK verification tool to ensure the APK is signed correctly.
   - Example: `jarsigner -verify -verbose -certs my_application.apk`


------------
## Features ✨
- **User Authentication**: Secure login for doctors, laboratorists, and patients.
- **Data Viewing**: Access personal data, laboratory tests, insurance status and medical information.
- **Data Management**: Doctors and laboratorists can add and update laboratory test results.
- **Patient Management**: View and select patient information.

## Use Cases 💼
![uc1](https://i.imgur.com/jVfnkgH.png)
![uc2](https://i.imgur.com/5F8Dbhy.png)
![uc3](https://i.imgur.com/xT4JeGj.png)

## Dependencies 📦
The project relies on the following dependencies:
- Qt Framework
  - Qt SDK installed
  - Java Development Kit (JDK) installed
  - Android NDK installed (for Android development)
  - Android SDK installed
- Sqlite3 - db server

## Configuration ⚙️
Configuration involves setting up the necessary server and client parameters for proper communication. Ensure that the server details (IP address and port) are correctly configured in the `IP-addresses.txt` files on every server and on the client.

## Screenshots 📸
![1](https://i.imgur.com/z1joItZ.png)
![2](https://i.imgur.com/TKwim3a.png)
![3](https://i.imgur.com/J6N7gP4.png)
![4](https://i.imgur.com/RPxLAbD.png)
![5](https://i.imgur.com/UZ7kbhj.png)
![6](https://i.imgur.com/GTTenlK.png)
![7](https://i.imgur.com/aj0y4h1.png)
![8](https://i.imgur.com/s7ASJuE.png)
![9](https://i.imgur.com/LPBFFP4.png)

## Troubleshooting 🛠️
If you encounter any issues while running the CCSS Garrobo, consider the following steps:
1. Verify that all dependencies are correctly installed.
2. Ensure that the server is running and accessible.
3. Check the configuration settings in `IP-addresses.txt`.

## Contributors 👥
- Andres Quesada Gonzalez
  - andres.quesadagonzalez@ucr.ac.cr
- Pablo Cascante Torres
  - pablo.cascantetorres@ucr.ac.cr 
- Diego Bolanos Villalobos
  - diego.bolanos.ccal@gmail.com
- Andres Serrano Robles
  - andres.serranorobles@ucr.ac.cr
