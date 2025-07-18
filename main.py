from fastapi import FastAPI, Request
from kafka import KafkaProducer
import json

app = FastAPI()

# Kafka setup
producer = KafkaProducer(
    bootstrap_servers='localhost:9092',
    value_serializer=lambda v: json.dumps(v).encode('utf-8')
)

@app.post("/sensor-data")
async def ingest(request: Request):
    try:
        print("🔥 Incoming request...")
        data = await request.json()
        print("📥 Received data:", data)

        # Send to Kafka
        producer.send("sensor_topic", value=data)
        producer.flush()
        print("📤 Sent to Kafka topic: sensor_topic")

        return {"status": "success", "message": "Data sent to Kafka"}

    except Exception as e:
        print("❌ Error in FastAPI:", e)
        return {"status": "error", "message": str(e)}
