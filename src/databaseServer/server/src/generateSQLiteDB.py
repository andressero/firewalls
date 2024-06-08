import sqlite3
import pandas as pd

conn = sqlite3.connect('medical_data.db')
cursor = conn.cursor()

def csv_to_sqlite(csv_file, table_name):
    df = pd.read_csv(csv_file)
    df.to_sql(table_name, conn, if_exists='replace', index=False)

csv_to_sqlite('user_data.csv', 'users')
csv_to_sqlite('assignation_list.csv', 'assignations')
csv_to_sqlite('laboratory_results.csv', 'lab_results')

conn.commit()
conn.close()

print("Gen correct.")
