
#include <logger.h>
#include <microwave_application_builder.h>
#include <interface_microwave_application.h>
#include <microwave_gl_application.h>
#include <microwave_ui.h>

#include <config.h>

static void error_callback(int, const char*);

int
main (void)
{
    Logger::log("main", "Hello there, starting application");

    MicrowaveApplicationBuilder builder = MicrowaveApplicationBuilder();
    MicrowaveUI *ui = new MicrowaveUI();
    IMicrowaveApplication *app = builder
        .create_gl_application()
        .set_window_title("cuptor cu microunde pofta buna youtube :)")
        .set_window_width(WINDOW_WIDTH)
        .set_window_height(WINDOW_HEIGHT)
        .set_glsl_version(std::string(GLSL_VERSION))
        .set_gl_major(GL_MAJOR)
        .set_gl_minor(GL_MINOR)
        .set_error_callback(error_callback)
        .set_run_callback([&ui]() -> void {
            ui->render_ui();
        })
        .build();

    app->main_loop();

    delete ui;
    ui = nullptr;

    Logger::log("main", "Bye bye");

    return 0;
}

static void
error_callback(int error, const char* description)
{
    Logger::log("GLFW", "Error %d: %s", error, description);
}
