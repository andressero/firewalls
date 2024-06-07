import os

# Complete list of patient IDs
patient_ids = [
    123456789, 234567890, 345678901, 456789012, 567890123,
    678901234, 789012345, 890123456, 901234567, 123450987,
    234561098, 345672109, 456783210, 567894321, 678905432,
    789016543, 890127654, 901238765, 123459876, 234560987,
    345672098, 456783209, 567894320, 678905431, 789016542,
    890127653, 901238764, 123459875, 234560986, 345672097,
    456783208, 567894319, 678905430, 789016541, 890127652,
    901238763, 123459874
]

lab_test_types = ['Blood Test', 'X-Ray', 'MRI', 'CT Scan', 'Ultrasound']
dates = [
    '2024-01-10', '2024-02-12', '2024-01-15', '2024-03-18', '2024-04-20',
    '2024-05-22', '2024-06-25', '2024-07-27', '2024-08-30', '2024-09-01'
]

# Ensure the output directory exists
output_dir = './data'
os.makedirs(output_dir, exist_ok=True)

# Initialize records list
records = [
    (1, 123456789, 'Blood Test', './data/1.txt', '2024-01-10'),
    (2, 234567890, 'X-Ray', './data/2.txt', '2024-02-12'),
    (3, 345678901, 'Blood Test', './data/3.txt', '2024-01-15')
]

index = 1  # Starting index for labResultID
for i, patient_id in enumerate(patient_ids):
    lab_test_type = lab_test_types[i % len(lab_test_types)]
    date = dates[i % len(dates)]
    file_path = f'./data/{index}.txt'
    record = (index, patient_id, lab_test_type, file_path, date)
    records.append(record)

    result_content = f"""\
Patient ID: {patient_id}
Lab Test: {lab_test_type}
Date: {date}
Result: Normal
"""
    with open(file_path, 'w') as f:
        f.write(result_content)

    index += 1

print("Dummy lab result files created.")
