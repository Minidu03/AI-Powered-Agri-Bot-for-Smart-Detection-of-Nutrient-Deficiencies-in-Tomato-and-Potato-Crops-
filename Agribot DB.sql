CREATE DATABASE agrirobot;
USE agrirobot;



CREATE TABLE scans (
    id              INT AUTO_INCREMENT PRIMARY KEY,
    timestamp       DATETIME DEFAULT CURRENT_TIMESTAMP,
    crop            VARCHAR(20),
    condition_name  VARCHAR(50),
    full_prediction VARCHAR(60),
    confidence      FLOAT,
    image_b64       LONGTEXT,
    pump_triggered  VARCHAR(20),
    relay_number    INT
);



CREATE TABLE pump_events (
    id              INT AUTO_INCREMENT PRIMARY KEY,
    scan_id         INT,
    timestamp       DATETIME DEFAULT CURRENT_TIMESTAMP,
    pump_name       VARCHAR(20),
    relay_number    INT,
    duration_sec    INT,
    FOREIGN KEY (scan_id) REFERENCES scans(id)
);


CREATE TABLE sensor_readings (
    id                  INT AUTO_INCREMENT PRIMARY KEY,
    timestamp           DATETIME DEFAULT CURRENT_TIMESTAMP,
    temperature_c       FLOAT,
    humidity_percent    FLOAT,
    soil_moisture_raw   INT,
    soil_moisture_pct   FLOAT,
    soil_status         VARCHAR(20)
);


CREATE TABLE alerts (
    id              INT AUTO_INCREMENT PRIMARY KEY,
    timestamp       DATETIME DEFAULT CURRENT_TIMESTAMP,
    alert_type      VARCHAR(50),
    prediction      VARCHAR(60),
    message         TEXT,
    dismissed       BOOLEAN DEFAULT FALSE
);



CREATE TABLE sessions (
    id              INT AUTO_INCREMENT PRIMARY KEY,
    started_at      DATETIME DEFAULT CURRENT_TIMESTAMP,
    ended_at        DATETIME,
    total_scans     INT DEFAULT 0,
    notes           TEXT
);


USE agrirobot;
SHOW TABLES;