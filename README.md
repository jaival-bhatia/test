
# SensaVerse

**“The universe of smart sensing.”**

SensaVerse is a comprehensive IoT-based real-time data acquisition and analysis platform designed for multi-domain applications such as agriculture, healthcare, environmental monitoring, industrial systems, and more.
<img width="1919" height="1079" alt="Screenshot 2025-07-18 011858" src="https://github.com/user-attachments/assets/5a61e4cf-47c1-4074-a9d5-bc8ca1284875" />

---

## 🌐 Project Overview

SensaVerse enables real-time sensor data collection using a range of hardware sensors integrated with a powerful data pipeline. It supports data streaming, storage, and analytics using robust backend systems such as:

- **InfluxDB** for time-series storage.
- **PostgreSQL** for structured relational data.
- **Firebase** for real-time database and alerts.

Data flows through MQTT or HTTP (flexibly configured), is processed, and visualized on a local dashboard.

---

## 📦 Features

- Modular sensor data acquisition (DHT11, BMP180, DS18B20, LDR, PIR, etc.)
- Multi-protocol support (MQTT, HTTP)
- Real-time data streaming & monitoring
- RESTful APIs via FastAPI
- Dockerized environment for seamless deployment
- Integrated with PostgreSQL, Firebase, and InfluxDB
- Visualization using Grafana
- Authentication and user management

---

## 🏗️ Project Structure

```
data_logger_final/
├── backend/                 # FastAPI-based backend services
├── sensors/                 # Python scripts to read sensor data
├── docker-compose.yml       # Docker services setup
├── influxdb/                # InfluxDB configuration
├── postgres/                # PostgreSQL configuration and seeding
├── firebase/                # Firebase real-time DB integration
└── README.md                # Project documentation
```

---

## 🚀 How to Run

### Requirements

- Docker Desktop installed
- WSL 2 (Windows)
- Python 3.10+ in WSL (for manual testing)

### Step 1: Clone the Repo

```bash
git clone https://github.com/jaival-bhatia/test.git
cd test/data_logger_final
```

### Step 2: Start Docker Environment

```bash
docker-compose up --build
```

### Step 3: Run Sensor Scripts (inside WSL)

```bash
cd sensors
python dht_sensor.py
```

---

## 🔐 Services Used

| Service     | Purpose                       |
|-------------|-------------------------------|
| FastAPI     | REST API server                |
| PostgreSQL  | Main relational DB             |
| Firebase    | Real-time data & alerts        |
| InfluxDB    | Time-series sensor data        |
| Grafana     | Visualization Dashboard        |

---

## 📊 Future Scope (Phase 3 and Beyond)

- 📱 **Mobile App** for real-time data monitoring and alert notifications
- ☁️ **Cloud deployment** using AWS/GCP for global scale
- 🔒 **RBAC**: Role-based access control for enterprise use
- 🧠 **ML Integration**: Predictive analytics for anomalies and alerts
- ⏱️ **Auto-scheduler**: Auto trigger devices based on sensor thresholds
- 🌍 **Edge Device Support**: Raspberry Pi / ESP32 for remote sensing
- 📦 **Kubernetes Support**: Microservices orchestration

---

## 🤝 Contributors

- Jaival Bhatia – AI & Backend
- Additional collaborators welcome!

---

## 📝 License

This project is licensed under the MIT License.
