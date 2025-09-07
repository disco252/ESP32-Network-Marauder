/* FLASH SETTINGS
Board: ESP32C6 Dev Module
Flash Frequency: 80MHz
Partition Scheme: Minimal SPIFFS
*/

// ========================================
// PIN DEFINITIONS FOR ESP32-C6 NETWORK MARAUDER
// ========================================

// Ethernet LAN8720 PHY Pins (RMII Interface)
#define ETH_TXD0_PIN      23
#define ETH_TXD1_PIN      22
#define ETH_TX_EN_PIN     21
#define ETH_RXD0_PIN      19
#define ETH_RXD1_PIN      20
#define ETH_CRS_DV_PIN    16
#define ETH_MDC_PIN       17
#define ETH_MDIO_PIN      18
#define ETH_CLK_PIN       0    // Optional: ESP32 clock output to PHY
#define ETH_POWER_PIN     -1   // Not used, PHY always powered
#define ETH_ADDR          0    // PHY address (usually 0 or 1)

// I2C Bus (Shared between OLED and RTC)
#define I2C_SDA_PIN       6
#define I2C_SCL_PIN       7
#define I2C_FREQ          100000  // 100kHz I2C frequency

// OLED Display (1.3" SH1106/SSD1306 - I2C)
#define OLED_SDA_PIN      I2C_SDA_PIN  // Same as I2C SDA
#define OLED_SCL_PIN      I2C_SCL_PIN  // Same as I2C SCL
#define OLED_RST_PIN      -1           // Reset pin (not used, tie to VCC)
#define OLED_WIDTH        128
#define OLED_HEIGHT       64
#define OLED_ADDR         0x3C         // I2C address (0x3C or 0x3D)

// RTC Module DS3231 (I2C)
#define RTC_SDA_PIN       I2C_SDA_PIN  // Same as I2C SDA
#define RTC_SCL_PIN       I2C_SCL_PIN  // Same as I2C SCL
#define RTC_ADDR          0x68         // Fixed I2C address for DS3231

// 5-Channel Joystick Module
#define JOYSTICK_SW_PIN   0            // Center press button (digital)
#define JOYSTICK_VRX_PIN  1            // X-axis analog (ADC)
#define JOYSTICK_VRY_PIN  2            // Y-axis analog (ADC)

// MicroSD Card Module (SPI Interface)
#define SD_CS_PIN         10           // Chip Select
#define SD_MOSI_PIN       11           // Master Out Slave In
#define SD_MISO_PIN       13           // Master In Slave Out
#define SD_SCK_PIN        12           // Serial Clock

// Power Management
#define BATTERY_PIN       3            // Battery voltage monitoring (ADC)
#define CHARGING_PIN      4            // Charging status input
#define POWER_EN_PIN      5            // Power enable/control

// Status LEDs (Optional)
#define LED_POWER_PIN     8            // Power status LED
#define LED_WIFI_PIN      9            // WiFi status LED  
#define LED_ETH_PIN       14           // Ethernet status LED
#define LED_ERROR_PIN     15           // Error/warning LED

// Hardware Configuration Constants
#define SPI_FREQ          1000000      // 1MHz for SD card
#define ADC_RESOLUTION    12           // 12-bit ADC
#define JOYSTICK_DEBOUNCE 50           // Joystick debounce time (ms)
#define DHCP_TIMEOUT      10000        // DHCP timeout (10 seconds)
#define PING_TIMEOUT      5000         // Ping timeout (5 seconds)
#define TRACEROUTE_MAX_HOPS 30         // Maximum traceroute hops

// Feature enables
#define ENABLE_ETHERNET    true
#define ENABLE_RTC         true
#define ENABLE_JOYSTICK    true
#define DEBUG_ETHERNET     true

// ESP32-C6 specific
#define BOARD_ESP32_C6
#define BUFFER_SIZE        4096
#define LOG_BUFFER_SIZE    2048

// ========================================
// INCLUDES SECTION
// ========================================

#include "configs.h"
#include "EthernetDiagnostics.h"
#include "RTCManager.h"

#ifndef HAS_SCREEN
#define MenuFunctions_h
#define Display_h
#endif

#include <WiFi.h>
#include "EvilPortal.h"
#include <Wire.h>
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include <Arduino.h>

#ifdef HAS_GPS
#include "GpsInterface.h"
#endif

#include "Assets.h"
#include "WiFiScan.h"
#ifdef HAS_SD
#include "SDInterface.h"
#endif
#include "Buffer.h"

#ifdef HAS_FLIPPER_LED
#include "flipperLED.h"
#elif defined(XIAO_ESP32_S3)
#include "xiaoLED.h"
#elif defined(MARAUDER_M5STICKC) || defined(MARAUDER_M5STICKCP2)
#include "stickcLED.h"
#elif defined(HAS_NEOPIXEL_LED)
#include "LedInterface.h"
#endif

#include "settings.h"
#include "CommandLine.h"
#include "lang_var.h"

#ifdef HAS_BATTERY
#include "BatteryInterface.h"
#endif

#ifdef HAS_SCREEN
#include "Display.h"
#include "MenuFunctions.h"
#endif

#ifdef HAS_BUTTONS
#include "Switches.h"

#if (U_BTN >= 0)
Switches u_btn = Switches(U_BTN, 1000, U_PULL);
#endif
#if (D_BTN >= 0)
Switches d_btn = Switches(D_BTN, 1000, D_PULL);
#endif
#if (L_BTN >= 0)
Switches l_btn = Switches(L_BTN, 1000, L_PULL);
#endif
#if (R_BTN >= 0)
Switches r_btn = Switches(R_BTN, 1000, R_PULL);
#endif
#if (C_BTN >= 0)
Switches c_btn = Switches(C_BTN, 1000, C_PULL);
#endif
#endif

// ========================================
// GLOBAL OBJECTS AND VARIABLES
// ========================================

// Custom Ethernet and RTC objects
EthernetDiagnostics ethDiag;
RTCManager rtcManager;

// Original Marauder objects
WiFiScan wifi_scan_obj;
EvilPortal evil_portal_obj;
Buffer buffer_obj;
Settings settings_obj;
CommandLine cli_obj;

#ifdef HAS_GPS
GpsInterface gps_obj;
#endif

#ifdef HAS_BATTERY
BatteryInterface battery_obj;
#endif

#ifdef HAS_SCREEN
Display display_obj;
MenuFunctions menu_function_obj;
#endif

#if defined(HAS_SD) && !defined(HAS_C5_SD)
SDInterface sd_obj;
#endif

#ifdef MARAUDER_M5STICKC
AXP192 axp192_obj;
#endif

#ifdef HAS_FLIPPER_LED
flipperLED flipper_led;
#elif defined(XIAO_ESP32_S3)
xiaoLED xiao_led;
#elif defined(MARAUDER_M5STICKC) || defined(MARAUDER_M5STICKCP2)
stickcLED stickc_led;
#else
LedInterface led_obj;
#endif

const String PROGMEM version_number = MARAUDER_VERSION;

#ifdef HAS_NEOPIXEL_LED
Adafruit_NeoPixel strip = Adafruit_NeoPixel(Pixels, PIN, NEO_GRB + NEO_KHZ800);
#endif

// Timing and menu variables
uint32_t currentTime = 0;
uint32_t lastJoystickRead = 0;
bool ethernetInitialized = false;
bool rtcInitialized = false;

// Menu state for ethernet diagnostics
enum MenuState {
    MAIN_MENU,
    WIFI_MENU,
    BLUETOOTH_MENU,
    ETHERNET_MENU,    // New ethernet diagnostics menu
    LOGGING_MENU,
    SETTINGS_MENU
};

MenuState currentMenu = MAIN_MENU;

// ========================================
// UTILITY FUNCTIONS
// ========================================

void backlightOn() {
#ifdef HAS_SCREEN
#ifdef MARAUDER_MINI
  digitalWrite(TFT_BL, LOW);
#endif
#ifndef MARAUDER_MINI
  digitalWrite(TFT_BL, HIGH);
#endif
#endif
}

void backlightOff() {
#ifdef HAS_SCREEN
#ifdef MARAUDER_MINI
  digitalWrite(TFT_BL, HIGH);
#endif
#ifndef MARAUDER_MINI
  digitalWrite(TFT_BL, LOW);
#endif
#endif
}

#ifdef HAS_C5_SD
SPIClass sharedSPI(SPI);
SDInterface sd_obj = SDInterface(&sharedSPI, SD_CS);
#endif

// ========================================
// JOYSTICK HANDLING
// ========================================

void handleJoystickInput() {
    if (millis() - lastJoystickRead < JOYSTICK_DEBOUNCE) return;
    
    int x = analogRead(JOYSTICK_VRX_PIN);
    int y = analogRead(JOYSTICK_VRY_PIN);
    bool pressed = !digitalRead(JOYSTICK_SW_PIN);
    
    // Navigation logic
    if (y < 1000) {
        // Up pressed
        Serial.println("Joystick: Up");
        if (currentMenu == ETHERNET_MENU) {
            // Navigate ethernet menu up
        }
    }
    else if (y > 3000) {
        // Down pressed
        Serial.println("Joystick: Down");
        if (currentMenu == ETHERNET_MENU) {
            // Navigate ethernet menu down
        }
    }
    else if (x < 1000) {
        // Left pressed
        Serial.println("Joystick: Left");
        if (currentMenu == ETHERNET_MENU) {
            currentMenu = MAIN_MENU;
        }
    }
    else if (x > 3000) {
        // Right pressed
        Serial.println("Joystick: Right");
        if (currentMenu == MAIN_MENU) {
            currentMenu = ETHERNET_MENU;
        }
    }
    
    if (pressed) {
        Serial.println("Joystick: Center pressed");
        if (currentMenu == ETHERNET_MENU) {
            // Execute selected ethernet diagnostic
        }
    }
    
    lastJoystickRead = millis();
}

// ========================================
// ETHERNET DIAGNOSTICS HANDLING
// ========================================

void handleEthernetMenu() {
    static uint32_t lastEthernetUpdate = 0;
    
    if (millis() - lastEthernetUpdate > 1000) { // Update every second
        Serial.println("=== Ethernet Diagnostics Menu ===");
        
        if (ethDiag.isConnected()) {
            Serial.println("✓ Ethernet connected - diagnostics available");
            Serial.println("1. DHCP Analysis");
            Serial.println("2. Ping Test");
            Serial.println("3. Traceroute");
            Serial.println("4. Network Scan");
            
            // Example: Auto-run a simple diagnostic
            ethDiag.performBasicDiagnostics();
        } else {
            Serial.println("✗ Ethernet not connected");
            Serial.println("Check cable and PHY connection");
        }
        
        // Log timestamp if RTC is available
        if (rtcInitialized) {
            Serial.println("Time: " + rtcManager.getTimestamp());
        }
        
        lastEthernetUpdate = millis();
    }
    
    // Handle joystick input for ethernet menu
    handleJoystickInput();
}

// ========================================
// SETUP FUNCTION
// ========================================

void setup() {
#ifndef HAS_DUAL_BAND
  esp_spiram_init();
#endif

#ifdef HAS_C5_SD
  Serial.println("Starting shared SPI for C5 SD configuration...");
  sharedSPI.begin(SD_SCK, SD_MISO, SD_MOSI);
  delay(100);
#endif

#ifdef defined(MARAUDER_M5STICKC) && !defined(MARAUDER_M5STICKCP2)
  axp192_obj.begin();
#endif

#if defined(MARAUDER_M5STICKCP2)
  pinMode(POWER_HOLD_PIN, OUTPUT);
  digitalWrite(POWER_HOLD_PIN, HIGH);
#endif

  // Initialize joystick pins
  pinMode(JOYSTICK_SW_PIN, INPUT_PULLUP);
  pinMode(JOYSTICK_VRX_PIN, INPUT);
  pinMode(JOYSTICK_VRY_PIN, INPUT);

#ifdef HAS_SCREEN
  pinMode(TFT_BL, OUTPUT);
#endif

  backlightOff();

#if BATTERY_ANALOG_ON == 1
  pinMode(BATTERY_PIN, INPUT);  // Changed from OUTPUT to INPUT for ADC
  pinMode(CHARGING_PIN, INPUT);
#endif

#ifdef HAS_SCREEN
  digitalWrite(TFT_CS, HIGH);
#endif

#if defined(HAS_SD) && !defined(HAS_C5_SD)
  pinMode(SD_CS, OUTPUT);
  delay(10);
  digitalWrite(SD_CS, HIGH);
  delay(10);
#endif

  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  Serial.println("ESP-IDF version is: " + String(esp_get_idf_version()));
  Serial.println("=== ESP32-C6 Network Marauder Starting ===");

  // Initialize I2C for RTC and OLED
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  delay(100);

  // Initialize custom modules FIRST
  Serial.println("Initializing RTC...");
  rtcManager.init();
  rtcInitialized = true;
  
  Serial.println("Initializing Ethernet diagnostics...");
  ethDiag.init();
  ethernetInitialized = true;
  
  Serial.println("ESP32 Network Marauder Ready!");

#ifdef HAS_PSRAM
  if (psramInit()) {
    Serial.println("PSRAM is correctly initialized");
  } else {
    Serial.println("PSRAM not available");
  }
#endif

#ifdef HAS_SCREEN
  display_obj.RunSetup();
  display_obj.tft.setTextColor(TFT_WHITE, TFT_BLACK);
#endif

  backlightOff();

#ifdef HAS_SCREEN
#ifndef MARAUDER_CARDPUTER
  display_obj.tft.drawCentreString("ESP32 Network Marauder", TFT_WIDTH / 2, TFT_HEIGHT * 0.33, 1);
  display_obj.tft.drawCentreString("Ethernet + WiFi/BT", TFT_WIDTH / 2, TFT_HEIGHT * 0.5, 1);
  display_obj.tft.drawCentreString(display_obj.version_number, TFT_WIDTH / 2, TFT_HEIGHT * 0.66, 1);
#else
  display_obj.tft.drawCentreString("ESP32 Network Marauder", TFT_HEIGHT / 2, TFT_WIDTH * 0.33, 1);
  display_obj.tft.drawCentreString("Ethernet + WiFi/BT", TFT_HEIGHT / 2, TFT_WIDTH * 0.5, 1);
  display_obj.tft.drawCentreString(display_obj.version_number, TFT_HEIGHT / 2, TFT_WIDTH * 0.66, 1);
#endif
#endif

  backlightOn();
  delay(2000); // Show startup screen

#ifdef HAS_SCREEN
#ifdef HAS_BUTTONS
  if (c_btn.justPressed()) {
    display_obj.headless_mode = true;
    backlightOff();
    Serial.println("Headless Mode enabled");
  }
#endif
#endif

  // Initialize remaining Marauder components
  settings_obj.begin();
  buffer_obj = Buffer();

#if defined(HAS_SD)
  if (!sd_obj.initSD())
    Serial.println(F("SD Card NOT Supported"));
  else
    Serial.println(F("SD Card initialized for logging"));
#endif

  wifi_scan_obj.RunSetup();

#ifdef HAS_SCREEN
  display_obj.tft.setTextColor(TFT_GREEN, TFT_BLACK);
  display_obj.tft.drawCentreString("Initializing...", TFT_WIDTH / 2, TFT_HEIGHT * 0.82, 1);
#endif

  evil_portal_obj.setup();

#ifdef HAS_BATTERY
  battery_obj.RunSetup();
  battery_obj.battery_level = battery_obj.getBatteryLevel();
#endif

#ifdef HAS_FLIPPER_LED
  flipper_led.RunSetup();
#elif defined(XIAO_ESP32_S3)
  xiao_led.RunSetup();
#elif defined(MARAUDER_M5STICKC)
  stickc_led.RunSetup();
#else
  led_obj.RunSetup();
#endif

#ifdef HAS_GPS
  gps_obj.begin();
#endif

#ifdef HAS_SCREEN
  display_obj.tft.setTextColor(TFT_WHITE, TFT_BLACK);
  menu_function_obj.RunSetup();
#endif

  wifi_scan_obj.StartScan(WIFI_SCAN_OFF);

  Serial.println(F("CLI Ready"));
  Serial.println(F("Use joystick to navigate: Left/Right to switch menus"));
  Serial.println(F("Current menu: MAIN_MENU (move right for ETHERNET_MENU)"));
  cli_obj.RunSetup();
}

// ========================================
// MAIN LOOP
// ========================================

void loop() {
  currentTime = millis();
  bool mini = false;

#ifdef SCREEN_BUFFER
#ifndef HAS_ILI9341
  mini = true;
#endif
#endif

#if (defined(HAS_ILI9341) && !defined(MARAUDER_CYD_2USB))
#ifdef HAS_BUTTONS
  if (c_btn.isHeld()) {
    if (menu_function_obj.disable_touch)
      menu_function_obj.disable_touch = false;
    else
      menu_function_obj.disable_touch = true;

    menu_function_obj.updateStatusBar();

    while (!c_btn.justReleased())
      delay(1);
  }
#endif
#endif

  // Handle joystick input regardless of menu
  handleJoystickInput();

  // Update original Marauder objects
  cli_obj.main(currentTime);
#ifdef HAS_SCREEN
  display_obj.main(wifi_scan_obj.currentScanMode);
#endif
  wifi_scan_obj.main(currentTime);

#ifdef HAS_GPS
  gps_obj.main();
#endif

#if defined(HAS_SD)
  sd_obj.main();
#endif

  buffer_obj.save();

#ifdef HAS_BATTERY
  battery_obj.main(currentTime);
#endif
  
  settings_obj.main(currentTime);
  
  // Handle ethernet diagnostic menu
  if (currentMenu == ETHERNET_MENU && ethernetInitialized) {
    handleEthernetMenu();
  }

  if (((wifi_scan_obj.currentScanMode != WIFI_PACKET_MONITOR) && (wifi_scan_obj.currentScanMode != WIFI_SCAN_EAPOL)) || (mini)) {
#ifdef HAS_SCREEN
    menu_function_obj.main(currentTime);
#endif
  }

#ifdef HAS_FLIPPER_LED
  flipper_led.main();
#elif defined(XIAO_ESP32_S3)
  xiao_led.main();
#elif defined(MARAUDER_M5STICKC)
  stickc_led.main();
#else
  led_obj.main(currentTime);
#endif

#ifdef HAS_SCREEN
  delay(1);
#else
  delay(50);
#endif
}
