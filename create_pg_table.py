import psycopg2

conn = psycopg2.connect(
    dbname="sensor_logs",
    user="user",
    password="password",
    host="localhost",
    port="5432"
)
cur = conn.cursor()

cur.execute("""
    CREATE TABLE IF NOT EXISTS sensor_data (
        id SERIAL PRIMARY KEY,
        device_id TEXT,
        temperature_dht REAL,
        temperature_ds REAL,
        humidity REAL,
        ldr INTEGER,
        motion INTEGER,
        mq2 INTEGER,
        distance INTEGER,
        relay_temp INTEGER,
        relay_ldr INTEGER,
        relay_pir INTEGER,
        led_red INTEGER,
        led_green INTEGER,
        led_blue INTEGER,
        timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    )
""")

conn.commit()
cur.close()
conn.close()
