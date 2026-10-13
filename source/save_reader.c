#include "save_reader.h"

#include <ogc/isfs.h>
#include <stddef.h>

typedef struct {
    const char *path;
    wsr_save_probe_result_t result;
} save_candidate_t;

/*
 * Confirmed/candidate paths for the first prototype only.
 * RZTP (PAL) is documented; RZTE (North America) is the corresponding
 * regional title ID. Additional regions will be added after verification.
 */
static const save_candidate_t k_candidates[] = {
    { "/title/00010000/525a5445/data/Sports2.dat", WSR_SAVE_PROBE_FOUND_NTSC_U },
    { "/title/00010000/525a5450/data/Sports2.dat", WSR_SAVE_PROBE_FOUND_PAL }
};

wsr_save_probe_result_t wsr_probe_save(const char **path_out)
{
    size_t i;
    s32 rc;

    if (path_out != NULL) {
        *path_out = NULL;
    }

    rc = ISFS_Initialize();
    if (rc < 0) {
        return WSR_SAVE_PROBE_ACCESS_ERROR;
    }

    for (i = 0; i < (sizeof(k_candidates) / sizeof(k_candidates[0])); ++i) {
        s32 fd = ISFS_Open(k_candidates[i].path, ISFS_OPEN_READ);
        if (fd >= 0) {
            (void)ISFS_Close(fd);
            (void)ISFS_Deinitialize();

            if (path_out != NULL) {
                *path_out = k_candidates[i].path;
            }
            return k_candidates[i].result;
        }
    }

    (void)ISFS_Deinitialize();
    return WSR_SAVE_PROBE_NOT_FOUND;
}

const char *wsr_save_probe_result_name(wsr_save_probe_result_t result)
{
    switch (result) {
        case WSR_SAVE_PROBE_NOT_CHECKED:
            return "Not checked";
        case WSR_SAVE_PROBE_FOUND_NTSC_U:
            return "North American save file is readable";
        case WSR_SAVE_PROBE_FOUND_PAL:
            return "PAL save file is readable";
        case WSR_SAVE_PROBE_NOT_FOUND:
            return "No supported save file found";
        case WSR_SAVE_PROBE_ACCESS_ERROR:
            return "NAND filesystem access unavailable";
        default:
            return "Unknown save status";
    }
}
