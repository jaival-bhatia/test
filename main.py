from fastapi import FastAPI, Request
from kafka import KafkaProducer
import json

app = FastAPI()

# Connect to Kafka
producer = KafkaProducer(
    bootstrap_servers='localhost:9092',
    value_serializer=lambda v: json.dumps(v).encode('utf-8')
)

# ✅ Updated route to match C++ code
@app.post("/sensor-data")
async def ingest(request: Request):
    data = await request.json()
    print("Received:", data)

    # Send to Kafka topic
    producer.send("sensor-data", value=data)

    return {"status": "ok"}
