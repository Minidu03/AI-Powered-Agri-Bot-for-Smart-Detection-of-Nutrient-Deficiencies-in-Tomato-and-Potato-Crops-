#include "esp_camera.h"
#include <WiFi.h>

// ===================
// Select your board
// ===================
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

// ===================
// WiFi — CHANGE THESE
// ===================
const char* ssid = "Dialog 4G 082";
const char* password = "Fbfm72nH";

void startCameraServer();  // Built-in function from ESP32 Camera library

void setup() {
  Serial.begin(115200);

  // ── Camera config ──
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;

  // ════════════════════════════════════════
  //  RHYX M21-45 FIXES — KEEP THESE
  // ════════════════════════════════════════
  config.pixel_format = PIXFORMAT_RGB565;   // No JPEG on RHYX M21-45
  config.frame_size = FRAMESIZE_QVGA;       // 320x240
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.fb_count = 1;

  // ── Init camera ──
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed: 0x%x\n", err);
    return;
  }

  // ── WiFi ──
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // ── Start stream server ──
  startCameraServer();

  Serial.println("");
  Serial.print("Stream ready: http://");
  Serial.print(WiFi.localIP());
  Serial.println(":81/stream");
}

void loop() {
  // Nothing needed — server runs in background
  delay(10000);
}