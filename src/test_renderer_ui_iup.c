#include "test_renderer_ui_iup.h"

int main(int argc, char* argv[])
{
  srand(time(NULL));
  app_t * app = new_app(argc, argv);
  app->init_app_param = iup_init_app_param;
  app->init_app = iup_init_app;
  app->run_app = iup_run_app;
  app->free_app = iup_free_app;
  run_app(app);
  free_app(app);
  return EXIT_SUCCESS;
}
