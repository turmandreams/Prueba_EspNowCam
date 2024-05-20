

#include "esp_camera.h"
#include "Arduino.h"
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"


#include <ESPNowCam.h>
ESPNowCam radio;

//#include <drivers/CamAIThinker.h>
//CamAIThinker Camera;


// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

int contador=0;


void processFrame() {

  camera_fb_t * fb = esp_camera_fb_get();
/*  uint8_t *out_jpg = NULL;
  size_t out_jpg_len = 0;
  frame2jpg(fb, 12, &out_jpg, &out_jpg_len);
 */
  radio.sendData(fb->buf,fb->len);
//  free(out_jpg);
  esp_camera_fb_return(fb);

  /*
  camera_fb_t * fb = esp_camera_fb_get();
  uint8_t *out_jpg = NULL;
  size_t out_jpg_len = 0;
  frame2jpg(fb, 12, &out_jpg, &out_jpg_len);
  radio.sendData(out_jpg, out_jpg_len);
  free(out_jpg);
  esp_camera_fb_return(fb);
  */

  /*
  if (Camera.get()) {
    uint8_t *out_jpg = NULL;
    size_t out_jpg_len = 0;
    frame2jpg(Camera.fb, 12, &out_jpg, &out_jpg_len);
    radio.sendData(out_jpg, out_jpg_len);
    free(out_jpg);
    Camera.free();
  }
  */
}



void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
 
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  //Serial.println();
  

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
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QQVGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
  config.jpeg_quality = 10;
  config.fb_count = 2;
  
  // Init Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }else{
    Serial.println("Se ha cargado la camara correctamente");    
  }

  sensor_t * s = esp_camera_sensor_get();
  s->set_vflip(s,1);
  
  radio.init();
  
  delay(1000); 
  
  
}


void loop() {

  Serial.println("vamos a capturar");
  processFrame();

}
