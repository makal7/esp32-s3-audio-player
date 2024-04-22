#include "main.hpp"

App app;

void audioLoop(void* a) {
  while(1)
  {
    app.getAudio()->loop();
    vTaskDelay(1/portTICK_PERIOD_MS);  //watchdog timer
  }
}

extern "C" void app_main()
{
  xTaskCreatePinnedToCore(audioLoop, "audio loop", 10000, NULL, 31, NULL, 0);
  app.run();
}