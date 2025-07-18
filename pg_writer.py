import psycopg2

def write_to_postgres(data):
    try:
        conn = psycopg2.connect(
            dbname="sensor_logs",
            user="user",
            password="password",
            host="localhost",
            port="5432"
        )
        cur = conn.cursor()

        cur.execute("""
            INSERT INTO sensor_data (device_id, temperature, humidity, motion, ldr, ds18b20)
            VALUES (%s, %s, %s, %s, %s, %s)
        """, (
            data.get("device_id", "esp32"),
            data.get("temperature"),
            data.get("humidity"),
            data.get("motion"),
            data.get("ldr"),
            data.get("ds18b20")
        ))

        conn.commit()
        cur.close()
        conn.close()

    except Exception as e:
        print("Error writing to PostgreSQL:", e)
