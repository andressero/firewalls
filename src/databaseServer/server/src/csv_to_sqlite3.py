import pandas as pd
import sqlite3

# Read the CSV file into a pandas DataFrame
df = pd.read_csv("../data/userDataTable.csv")

df['organDonor'] = df['organDonor'].map({'Yes': True, 'No': False})
df['isInsured'] = df['isInsured'].map({'Yes': True, 'No': False})

# Connect to SQLite database (will create if not exists)
conn = sqlite3.connect("../data/data.db")

# Write the DataFrame to a SQLite table named "patients"
df.to_sql("userData", conn, if_exists="replace", index=False)

# Commit changes and close connection
conn.commit()
conn.close()

print("Database created successfully.")

# Connect to SQLite database (or create it if it doesn't exist)
conn = sqlite3.connect('../data/data.db')
cursor = conn.cursor()

# Create LabResults table
cursor.execute('''
CREATE TABLE IF NOT EXISTS LabResults (
    labResultID INTEGER PRIMARY KEY,
    patientID INTEGER,
    testName TEXT NOT NULL,
    testContentFilePath TEXT NOT NULL,
    testDate TEXT NOT NULL,
    FOREIGN KEY (patientID) REFERENCES userData(patientID)
)
''')


# Insert sample data into LabResults table
cursor.executemany('''
INSERT OR IGNORE INTO LabResults (labResultID, patientID, testName, testContentFilePath, testDate) VALUES (?, ?, ?, ?, ?)
''', [
(1, 123456789, 'Blood Test', '../data/1.txt', '2024-01-10'),
(2, 234567890, 'X-Ray', '../data/2.txt', '2024-02-12'),
(3, 345678901, 'Blood Test', '../data/3.txt', '2024-01-15'),
(4, 123456789, 'Blood Test', '../data/4.txt', '2024-01-10'),
(5, 234567890, 'X-Ray', '../data/5.txt', '2024-02-12'),
(6, 345678901, 'MRI', '../data/6.txt', '2024-01-15'),
(7, 456789012, 'CT Scan', '../data/7.txt', '2024-03-18'),
(8, 567890123, 'Ultrasound', '../data/8.txt', '2024-04-20'),
(9, 678901234, 'Blood Test', '../data/9.txt', '2024-05-22'),
(10, 789012345, 'X-Ray', '../data/10.txt', '2024-06-25'),
(11, 890123456, 'MRI', '../data/11.txt', '2024-07-27'),
(12, 901234567, 'CT Scan', '../data/12.txt', '2024-08-30'),
(13, 123450987, 'Ultrasound', '../data/13.txt', '2024-09-01'),
(14, 234561098, 'Blood Test', '../data/14.txt', '2024-01-10'),
(15, 345672109, 'X-Ray', '../data/15.txt', '2024-02-12'),
(16, 456783210, 'MRI', '../data/16.txt', '2024-01-15'),
(17, 567894321, 'CT Scan', '../data/17.txt', '2024-03-18'),
(18, 678905432, 'Ultrasound', '../data/18.txt', '2024-04-20'),
(19, 789016543, 'Blood Test', '../data/19.txt', '2024-05-22'),
(20, 890127654, 'X-Ray', '../data/20.txt', '2024-06-25'),
(21, 901238765, 'MRI', '../data/21.txt', '2024-07-27'),
(22, 123459876, 'CT Scan', '../data/22.txt', '2024-08-30'),
(23, 234560987, 'Ultrasound', '../data/23.txt', '2024-09-01'),
(24, 345672098, 'Blood Test', '../data/24.txt', '2024-01-10'),
(25, 456783209, 'X-Ray', '../data/25.txt', '2024-02-12'),
(26, 567894320, 'MRI', '../data/26.txt', '2024-01-15'),
(27, 678905431, 'CT Scan', '../data/27.txt', '2024-03-18'),
(28, 789016542, 'Ultrasound', '../data/28.txt', '2024-04-20'),
(29, 890127653, 'Blood Test', '../data/29.txt', '2024-05-22'),
(30, 901238764, 'X-Ray', '../data/30.txt', '2024-06-25'),
(31, 123459875, 'MRI', '../data/31.txt', '2024-07-27'),
(32, 234560986, 'CT Scan', '../data/32.txt', '2024-08-30'),
(33, 345672097, 'Ultrasound', '../data/33.txt', '2024-09-01'),
(34, 456783208, 'Blood Test', '../data/34.txt', '2024-01-10'),
(35, 567894319, 'X-Ray', '../data/35.txt', '2024-02-12'),
(36, 678905430, 'MRI', '../data/36.txt', '2024-01-15'),
(37, 789016541, 'CT Scan', '../data/37.txt', '2024-03-18'),
(38, 890127652, 'Ultrasound', '../data/38.txt', '2024-04-20'),
(39, 901238763, 'Blood Test', '../data/39.txt', '2024-05-22'),
(40, 123459874, 'X-Ray', '../data/40.txt', '2024-06-25')
])

# Commit the transactions
conn.commit()

print("LabResults table created successfully.")

# Function to get lab results for a specific patient
def get_lab_results(patient_id):
    cursor.execute('''
    SELECT LabResults.labResultID, LabResults.testName, LabResults.testContentFilePath, LabResults.testDate
    FROM LabResults
    JOIN userData ON LabResults.patientID = userData.patientID
    WHERE userData.patientID = ?
    ''', (patient_id,))
    results = cursor.fetchall()
    return results

# Get lab results for patient with ID 1
patient_id = 123456789
results = get_lab_results(patient_id)

# Print the results
for result in results:
    print(f'LabResultID: {result[0]}, Test: {result[1]}, Path: {result[2]}, Date: {result[3]}')


def get_lab_result(lab_result_id):
    cursor.execute('''
    SELECT LabResults.labResultID, LabResults.testName, LabResults.testContentFilePath, LabResults.testDate
    FROM LabResults
    WHERE LabResults.labResultID = ?
    ''', (lab_result_id,))
    result = cursor.fetchone()
    return result

# Get lab result with ID 1
lab_result_id = 1
result = get_lab_result(lab_result_id)
with open(result[2], 'r') as file:
    content = file.read()
    print(f'LabResultID: {result[0]}, Test: {result[1]}, Content: {content}, Date: {result[3]}')

# Close the connection
conn.close()




