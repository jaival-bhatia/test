from kafka import KafkaConsumer
from influxdb import InfluxDBClient
import psycopg2
import json

# Kafka consumer setup
consumer = KafkaConsumer(
    'sensor_topic',  # 🔄 Topic name must match what FastAPI is using
    bootstrap_servers='localhost:9092',
    value_deserializer=lambda m: json.loads(m.decode('utf-8'))
)

# InfluxDB client setup
influx_client = InfluxDBClient(host='localhost', port=8086)
influx_client.switch_database('sensordb')

# PostgreSQL connection setup
try:
    pg_conn = psycopg2.connect(
        dbname="sensor_logs",
        user="user",
        password="password",
        host="localhost",
        port="5432"
    )
    pg_cursor = pg_conn.cursor()
except Exception as e:
    print("❌ PostgreSQL connection failed:", e)
    pg_conn = None
    pg_cursor = None

def write_to_influx(data):
    json_body = [{
        "measurement": "sensor_readings",
        "fields": {
            "temperature_dht": float(data["temperature_dht"]),
            "temperature_ds": float(data["temperature_ds"]),
            "humidity": float(data["humidity"]),
            "ldr": int(data["ldr"]),
            "motion": int(data["motion"]),
            "mq2": int(data["mq2"]),
            "distance": int(data["distance"]),
            "relay_temp": int(data["relay_temp"]),
            "relay_ldr": int(data["relay_ldr"]),
            "relay_pir": int(data["relay_pir"]),
            "led_red": int(data["led_red"]),
            "led_green": int(data["led_green"]),
            "led_blue": int(data["led_blue"])
        }
    }]
    influx_client.write_points(json_body)
    print("✅ Written to InfluxDB")

def write_to_postgres(data):
    if pg_conn is None or pg_cursor is None:
        print("⚠️ PostgreSQL not connected.")
        return

    pg_cursor.execute("""
        INSERT INTO sensor_data (
            device_id, temperature_dht, temperature_ds, humidity,
            ldr, motion, mq2, distance,
            relay_temp, relay_ldr, relay_pir,
            led_red, led_green, led_blue
        ) VALUES (
            %s, %s, %s, %s,
            %s, %s, %s, %s,
            %s, %s, %s,
            %s, %s, %s
        )
    """, (
        "device_1",
        data["temperature_dht"],
        data["temperature_ds"],
        data["humidity"],
        data["ldr"],
        data["motion"],
        data["mq2"],
        data["distance"],
        data["relay_temp"],
        data["relay_ldr"],
        data["relay_pir"],
        data["led_red"],
        data["led_green"],
        data["led_blue"]
    ))
    pg_conn.commit()
    print("✅ Written to PostgreSQL")

# Kafka consume loop
print("🚀 Kafka consumer started. Listening on 'sensor_topic'...")

for message in consumer:
    data = message.value
    print("📥 Received from Kafka:", data)

    try:
        write_to_influx(data)
    except Exception as e:
        print("❌ Error writing to InfluxDB:", e)

    try:
        write_to_postgres(data)
    except Exception as e:
        print("❌ Error writing to PostgreSQL:", e)
