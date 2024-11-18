#include <furi.h>
#include <gui/gui.h>
#include <gui/icon_i.h>
#include <gui/scene_manager.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/menu.h>
#include <gui/modules/popup.h>
#include <gui/modules/variable_item_list.h>


#define TAG "my-app"

// Define scenes
typedef enum {
    Scene_Home,
    Scene_NearbyDevices,
    Scene_DeviceDetails,
    Scene_ConnectedDevices,
    Scene_count
} AppScene;

typedef enum {View_Menu, View_Popup, View_VariableItemList} AppView;


// Context for the application
typedef struct {
    SceneManager* scene_manager;
    ViewDispatcher* view_dispatcher;
    Menu* menu;
    Popup* popup;
    VariableItemList* variable_item_list;
    // TODO: add data structures to track whether devices are blocked
} AppContext;

typedef enum { HomeEvent_ShowNearbyDevices, HomeEvent_ShowConnectedDevices, NearbyDevicesEvent_ShowDeviceDetails } MyAppEvent;

/** indices for home menu items */
typedef enum { HomeMenuSelection_One, HomeMenuSelection_Two } HomeMenuSelection;

// Scenes

// Home Scene

// Menu callback for Home menu
static void home_menu_callback(void* context, uint32_t index) {
    FURI_LOG_T(TAG, "home_menu_callback");
    AppContext* app = context;
    switch(index) {
    case HomeMenuSelection_One:
        scene_manager_handle_custom_event(app->scene_manager, HomeEvent_ShowNearbyDevices);
        break;
    case HomeMenuSelection_Two:
        scene_manager_handle_custom_event(app->scene_manager, HomeEvent_ShowConnectedDevices);
        break;
    }
}

static void home_scene_on_enter(void* context) {
    AppContext* app = context;
    menu_reset(app->menu);
    // Initialize Home menu
    menu_add_item(app->menu, "Nearby Devices", NULL, 0, home_menu_callback, app);
    menu_add_item(app->menu, "Connected Device", NULL, 1, home_menu_callback, app);
    view_dispatcher_switch_to_view(app->view_dispatcher, View_Menu);
}

bool home_scene_on_event(void* context, SceneManagerEvent event) {
    FURI_LOG_T(TAG, "home_scene_on_event");
    AppContext* app = context;
    bool consumed = false;
    switch(event.type) {
    case SceneManagerEventTypeCustom:
        switch(event.event) {
        case HomeMenuSelection_One:
            scene_manager_next_scene(app->scene_manager, Scene_NearbyDevices);
            consumed = true;
            break;
        case HomeMenuSelection_Two:
            scene_manager_next_scene(app->scene_manager, Scene_ConnectedDevices);
            consumed = true;
            break;
        }
        break;
    default: // Use default behaviors for events such as SceneManagerEventTypeBack, SceneManagerEventTypeTick
        consumed = false;
        break;
    }
    return consumed;
}

void home_scene_on_exit(void* context){
    FURI_LOG_T(TAG, "home_scene_on_exit");
    AppContext* app = context;
    menu_reset(app->menu);
}

// NearbyDevices Scene

// Menu callback for NearbyDevices menu
// TODO: update for dynamic bluetooth devices
static void nearby_devices_menu_callback(void* context, uint32_t index) {
    UNUSED(index);
    FURI_LOG_T(TAG, "nearby_devices_menu_callback");
    AppContext* app = context;
    scene_manager_handle_custom_event(app->scene_manager, NearbyDevicesEvent_ShowDeviceDetails);
}

static void nearby_devices_scene_on_enter(void* context) {
    AppContext* app = context;
    menu_reset(app->menu);
    // Initialize Home menu
    menu_add_item(app->menu, "device 1", NULL, 0, nearby_devices_menu_callback, app);
    menu_add_item(app->menu, "device 2", NULL, 1, nearby_devices_menu_callback, app);
    menu_add_item(app->menu, "device 3", NULL, 2, nearby_devices_menu_callback, app);
    menu_add_item(app->menu, "device 4", NULL, 3, nearby_devices_menu_callback, app);
    view_dispatcher_switch_to_view(app->view_dispatcher, View_Menu);
}

bool nearby_devices_scene_on_event(void* context, SceneManagerEvent event) {
    FURI_LOG_T(TAG, "nearby_devices_scene_on_event");
    AppContext* app = context;
    bool consumed = false;
    switch(event.type) {
    case SceneManagerEventTypeCustom:
        switch(event.event) {
        case NearbyDevicesEvent_ShowDeviceDetails:
            scene_manager_next_scene(app->scene_manager, Scene_DeviceDetails);
            consumed = true;
            break;
        }
        break;
    default: // Use default behaviors for events such as SceneManagerEventTypeBack, SceneManagerEventTypeTick
        consumed = false;
        break;
    }
    return consumed;
}

void nearby_devices_scene_on_exit(void* context){
    FURI_LOG_T(TAG, "nearby_devices_scene_on_exit");
    AppContext* app = context;
    menu_reset(app->menu);
};

// DeviceDetails Scene
// Functions for variable item list
char* is_blocked_options[] = {"No", "Yes"};
void is_blocked_change_callback(VariableItem* item) {
   uint8_t index = variable_item_get_current_value_index(item);
   variable_item_set_current_value_text(item, is_blocked_options[index]);
}

static void device_details_scene_on_enter(void* context) {
    FURI_LOG_T(TAG, "device_details_scene_on_enter");
    AppContext* app = context;
    variable_item_list_reset(app->variable_item_list);
    // Initialize the variable item list
    VariableItem* device_name_item = variable_item_list_add(app->variable_item_list, "Device Name:", 0, NULL, app);
    VariableItem* device_mac_item = variable_item_list_add(app->variable_item_list, "MAC address:", 0, NULL, app);  
    VariableItem* is_blocked_item = variable_item_list_add(app->variable_item_list, "Is blocked:", 2, is_blocked_change_callback, app);  

    variable_item_set_current_value_index(device_name_item, 0);
    variable_item_set_current_value_text(device_name_item, "xyz");
    variable_item_set_current_value_index(device_mac_item, 0);
    variable_item_set_current_value_text(device_mac_item, "abc");  
    variable_item_set_current_value_index(is_blocked_item, 0);
    variable_item_set_current_value_text(is_blocked_item, is_blocked_options[0]); 
    view_dispatcher_switch_to_view(app->view_dispatcher, View_VariableItemList);
}

bool device_details_scene_on_event(void* context, SceneManagerEvent event) {
    FURI_LOG_T(TAG, "device_details_scene_on_event");
    AppContext* app = context;
    bool consumed = false;
    switch(event.type) {
    case SceneManagerEventTypeCustom:
        scene_manager_next_scene(app->scene_manager, Scene_Home);
        consumed = true;
        break;
    default: // Use default behaviors for events such as SceneManagerEventTypeBack, SceneManagerEventTypeTick
        consumed = false;
        break;
    }
    return consumed;
}

void device_details_scene_on_exit(void* context){
    FURI_LOG_T(TAG, "device_details_scene_on_exit");
    AppContext* app = context;
    variable_item_list_reset(app->variable_item_list);
}

// Register event handlers
void (*const my_app_on_enter_handlers[])(void*) = {
    home_scene_on_enter,
    nearby_devices_scene_on_enter,
    device_details_scene_on_enter
};

bool (*const my_app_on_event_handlers[])(void*, SceneManagerEvent) = {
    home_scene_on_event,
    nearby_devices_scene_on_event,
    device_details_scene_on_event
};

void (*const my_app_on_exit_handlers[])(void*) = {
    home_scene_on_exit,
    nearby_devices_scene_on_exit,
    device_details_scene_on_exit
};

const SceneManagerHandlers my_app_scene_manager_handlers = {
    .on_enter_handlers = my_app_on_enter_handlers,
    .on_event_handlers = my_app_on_event_handlers,
    .on_exit_handlers = my_app_on_exit_handlers,
    .scene_num = Scene_count
};

/** custom event handler - passes the event to the scene manager */
bool my_app_scene_manager_custom_event_callback(void* context, uint32_t custom_event) {
    FURI_LOG_T(TAG, "my_app_scene_manager_custom_event_callback");
    furi_assert(context);
    AppContext* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, custom_event);
}

/** navigation event handler - passes the event to the scene manager */
bool my_app_scene_manager_navigation_event_callback(void* context) {
    FURI_LOG_T(TAG, "my_app_scene_manager_navigation_event_callback");
    furi_assert(context);
    AppContext* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

void my_app_scene_manager_init(AppContext* app) {
    FURI_LOG_T(TAG, "my_app_scene_manager_init");
    app->scene_manager = scene_manager_alloc(&my_app_scene_manager_handlers, app);
}

void my_app_view_dispatcher_init(AppContext* app){
    FURI_LOG_T(TAG, "my_app_view_dispatcher_init");
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);

    // allocate each view
    FURI_LOG_D(TAG, "my_app_view_dispatcher_init allocating views");
    app->menu = menu_alloc();
    app->popup = popup_alloc();
    app->variable_item_list = variable_item_list_alloc();

    // assign callback that pass events from views to the scene manager
    FURI_LOG_D(TAG, "my_app_view_dispatcher_init setting callbacks");
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(
        app->view_dispatcher, my_app_scene_manager_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(
        app->view_dispatcher, my_app_scene_manager_navigation_event_callback);

    // add views to the dispatcher, indexed by their enum value
    FURI_LOG_D(TAG, "my_app_view_dispatcher_init adding view menu");
    view_dispatcher_add_view(app->view_dispatcher, View_Menu, menu_get_view(app->menu));

    FURI_LOG_D(TAG, "my_app_view_dispatcher_init adding view popup");
    view_dispatcher_add_view(app->view_dispatcher, View_Popup, popup_get_view(app->popup));

    FURI_LOG_D(TAG, "my_app_view_dispatcher_init adding view variable item list");
    view_dispatcher_add_view(app->view_dispatcher, View_VariableItemList, variable_item_list_get_view(app->variable_item_list));
}

// Main Application
int32_t app_main(void* p) {
    UNUSED(p);

    // Set log level to trace
    furi_log_set_level(FuriLogLevelTrace); 
    FURI_LOG_I(TAG, "My app starting...");

    // Initialization
    AppContext* app = malloc(sizeof(AppContext));
    my_app_scene_manager_init(app);
    my_app_view_dispatcher_init(app);

    // Set the scene
    Gui* gui = furi_record_open(RECORD_GUI);
    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);
    scene_manager_next_scene(app->scene_manager, Scene_Home);
    FURI_LOG_D(TAG, "Starting dispatcher...");
    view_dispatcher_run(app->view_dispatcher);

    // Free memory
    FURI_LOG_I(TAG, "My app exiting...");
    furi_record_close(RECORD_GUI);
    FURI_LOG_T(TAG, "test_app_free");
    scene_manager_free(app->scene_manager);
    view_dispatcher_remove_view(app->view_dispatcher, View_Menu);
    view_dispatcher_remove_view(app->view_dispatcher, View_Popup);
    view_dispatcher_remove_view(app->view_dispatcher, View_VariableItemList);
    view_dispatcher_free(app->view_dispatcher);
    menu_free(app->menu);
    popup_free(app->popup);
    free(app);
    return 0;
}
