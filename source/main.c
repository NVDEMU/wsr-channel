#include <gccore.h>
#include <ogc/console.h>
#include <ogc/system.h>
#include <wiiuse/wpad.h>

#include <stdbool.h>
#include <stdio.h>

#include "save_reader.h"

typedef enum {
    PAGE_HOME = 0,
    PAGE_RECORDS,
    PAGE_CHALLENGES,
    PAGE_SETTINGS,
    PAGE_ABOUT
} page_t;

static const char *const k_menu_items[] = {
    "Sports Records",
    "Offline Challenges",
    "Settings",
    "About Resort Records"
};

#define MENU_ITEM_COUNT ((int)(sizeof(k_menu_items) / sizeof(k_menu_items[0])))

static wsr_save_probe_result_t g_save_result = WSR_SAVE_PROBE_NOT_CHECKED;
static const char *g_save_path = NULL;
static bool g_save_probe_attempted = false;

static void render_home(int selection)
{
    int i;

    console_clear();
    printf("============================================================\n");
    printf("                    RESORT RECORDS                          \n");
    printf("             Wii Sports Resort companion                    \n");
    printf("============================================================\n\n");
    printf("  A personal, offline home for your Resort records.\n\n");

    for (i = 0; i < MENU_ITEM_COUNT; ++i) {
        printf("  %s %s\n", (i == selection) ? ">" : " ", k_menu_items[i]);
    }

    printf("\n  UP/DOWN: Select       A: Open       HOME: Exit\n\n");
    printf("  Prototype 0.1.0 | Offline mode | No network required\n");
}

static void render_records(void)
{
    if (!g_save_probe_attempted) {
        g_save_result = wsr_probe_save(&g_save_path);
        g_save_probe_attempted = true;
    }

    console_clear();
    printf("============================================================\n");
    printf("                      SPORTS RECORDS                        \n");
    printf("============================================================\n\n");
    printf("Save detection: %s\n\n", wsr_save_probe_result_name(g_save_result));

    if (g_save_path != NULL) {
        printf("Readable save path:\n%s\n\n", g_save_path);
        printf("The save file was opened read-only and immediately closed.\n");
        printf("Its contents have NOT been decoded in this prototype.\n");
    } else {
        printf("No supported save was confirmed readable.\n");
        printf("This can mean the save is absent or ISFS access is restricted.\n");
        printf("No game data has been changed.\n");
    }

    printf("\n  B: Back to menu       HOME: Exit\n");
}

static void render_challenges(void)
{
    console_clear();
    printf("============================================================\n");
    printf("                    OFFLINE CHALLENGES                      \n");
    printf("============================================================\n\n");
    printf("Challenge tracking is planned for the next milestone.\n\n");
    printf("Planned challenge examples:\n");
    printf("  - Beat a verified Bowling personal best.\n");
    printf("  - Improve a record in a chosen sport.\n");
    printf("  - Complete a set of sport-specific milestones.\n\n");
    printf("These are examples, not active challenges yet.\n");
    printf("The challenge engine will use only verified statistics.\n");
    printf("Channel progress will be stored separately from the game save.\n");
    printf("\n  B: Back to menu       HOME: Exit\n");
}

static void render_settings(void)
{
    console_clear();
    printf("============================================================\n");
    printf("                         SETTINGS                           \n");
    printf("============================================================\n\n");
    printf("Network: Disabled (offline-only prototype)\n");
    printf("Game save: Read-only probe; no writes are performed\n");
    printf("Channel-owned storage: Not implemented yet\n");
    printf("Supported save paths: North America and PAL candidates\n\n");
    printf("This screen is informational in the current prototype.\n");
    printf("\n  B: Back to menu       HOME: Exit\n");
}

static void render_about(void)
{
    console_clear();
    printf("============================================================\n");
    printf("                    ABOUT RESORT RECORDS                    \n");
    printf("============================================================\n\n");
    printf("Resort Records is an independent homebrew project.\n");
    printf("It is not affiliated with or endorsed by Nintendo.\n\n");
    printf("Version: 0.1.0 (bootable prototype)\n");
    printf("Build: devkitPPC + libogc\n");
    printf("Online services: none\n\n");
    printf("A custom installable WAD and banner are not available yet.\n");
    printf("\n  B: Back to menu       HOME: Exit\n");
}

static void render_page(page_t page, int selection)
{
    switch (page) {
        case PAGE_HOME:
            render_home(selection);
            break;
        case PAGE_RECORDS:
            render_records();
            break;
        case PAGE_CHALLENGES:
            render_challenges();
            break;
        case PAGE_SETTINGS:
            render_settings();
            break;
        case PAGE_ABOUT:
            render_about();
            break;
        default:
            render_home(selection);
            break;
    }
}

int main(void)
{
    GXRModeObj *rmode;
    void *framebuffer;
    page_t page = PAGE_HOME;
    int selection = 0;
    bool redraw = true;

    VIDEO_Init();
    WPAD_Init();

    rmode = VIDEO_GetPreferredMode(NULL);
    framebuffer = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

    console_init(framebuffer,
                 20,
                 20,
                 rmode->fbWidth,
                 rmode->xfbHeight,
                 rmode->fbWidth * VI_DISPLAY_PIX_SZ);

    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(framebuffer);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if (rmode->viTVMode & VI_NON_INTERLACE) {
        VIDEO_WaitVSync();
    }

    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS);

    while (true) {
        u32 pressed;

        if (redraw) {
            render_page(page, selection);
            redraw = false;
        }

        WPAD_ScanPads();
        pressed = WPAD_ButtonsDown(WPAD_CHAN_0);

        if ((pressed & WPAD_BUTTON_HOME) != 0) {
            SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
        }

        if (page == PAGE_HOME) {
            if ((pressed & WPAD_BUTTON_UP) != 0) {
                selection = (selection + MENU_ITEM_COUNT - 1) % MENU_ITEM_COUNT;
                redraw = true;
            } else if ((pressed & WPAD_BUTTON_DOWN) != 0) {
                selection = (selection + 1) % MENU_ITEM_COUNT;
                redraw = true;
            } else if ((pressed & WPAD_BUTTON_A) != 0) {
                page = (page_t)(selection + 1);
                redraw = true;
            }
        } else if ((pressed & WPAD_BUTTON_B) != 0) {
            page = PAGE_HOME;
            redraw = true;
        }

        VIDEO_WaitVSync();
    }

    return 0;
}
