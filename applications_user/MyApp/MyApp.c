#include <furi.h>
#include <gui/gui.h>
#include <furi_hal.h>

// Callback function to render "Hello, World!" on the screen
void my_draw_callback(Canvas* canvas, void* ctx) {
    UNUSED(ctx);  // Mark 'ctx' as unused to avoid warning
    canvas_clear(canvas);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 20, 32, "Hello Jimmy!");
    
}

int32_t MyApp_main(void* p) {
    UNUSED(p);
    Gui* gui = furi_record_open("gui");
    ViewPort* viewport = view_port_alloc();
    
    // Direct console print
    FURI_LOG_I("MyApp", "Hello, World!");

    // Set the draw callback
    view_port_draw_callback_set(viewport, my_draw_callback, NULL);
    gui_add_view_port(gui, viewport, GuiLayerFullscreen);
    
    // Display for 2 seconds
    furi_delay_ms(2000);
    
    // Cleanup
    gui_remove_view_port(gui, viewport);
    view_port_free(viewport);
    furi_record_close("gui");
    
    return 0;
}
