from influxdb import InfluxDBClient

def write_to_influx(data):
    client = InfluxDBClient(host='localhost', port=8086)
    client.switch_database('sensordb')

    json_body = [
        {
            "measurement": "sensor_readings",
            "fields": {
                "temperature_dht": float(data.get("temperature_dht", 0.0)),
                "temperature_ds": float(data.get("temperature_ds", 0.0)),
                "humidity": float(data.get("humidity", 0.0)),
                "ldr": int(data.get("ldr", 0)),
                "motion": int(data.get("motion", 0)),
                "mq2": int(data.get("mq2", 0)),
                "distance": int(data.get("distance", 0)),
                "relay_temp": int(data.get("relay_temp", 0)),
                "relay_ldr": int(data.get("relay_ldr", 0)),
                "relay_pir": int(data.get("relay_pir", 0)),
                "led_red": int(data.get("led_red", 0)),
                "led_green": int(data.get("led_green", 0)),
                "led_blue": int(data.get("led_blue", 0))
            }
        }
    ]

    client.write_points(json_body)
    client.close()
