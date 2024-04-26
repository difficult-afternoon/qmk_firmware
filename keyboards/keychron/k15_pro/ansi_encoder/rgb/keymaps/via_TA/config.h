#pragma once


/* Tien Anh add beg */
#define TAPPING_TERM 200
#define HOLD_ON_OTHER_KEY_PRESS 1
/* Tien Anh add end */

#ifdef RGB_MATRIX_ENABLE
/* RGB Matrix Driver Configuration */
#    define DRIVER_COUNT 2
#    define DRIVER_ADDR_1 0b1110111
#    define DRIVER_ADDR_2 0b1110100

/* RGB Matrix Configuration */
#    define DRIVER_1_LED_TOTAL 47
#    define DRIVER_2_LED_TOTAL 42
#    define RGB_MATRIX_LED_COUNT (DRIVER_1_LED_TOTAL + DRIVER_2_LED_TOTAL)

/* Set to infinit, which is use in USB mode by default */
#    define RGB_MATRIX_TIMEOUT RGB_MATRIX_TIMEOUT_INFINITE

/* Allow to shutdown driver to save power */
#    define RGB_MATRIX_DRIVER_SHUTDOWN_ENABLE

/* Turn off backllit if brightness value is low */
#    define RGB_MATRIX_BRIGHTNESS_TURN_OFF_VAL 48

/* Indication led */
#    define CAPS_LOCK_INDEX 48
#    define LOW_BAT_IND_INDEX 81

/* RGB Matrix Animation modes. Explicitly enabled
 * For full list of effects, see:
 * https://docs.qmk.fm/#/feature_rgb_matrix?id=rgb-matrix-effects
 */
#    define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#    define RGB_MATRIX_KEYPRESSES

/* Use first 9 channels of LED driver */
#    define PHASE_CHANNEL MSKPHASE_9CHANNEL

/* Set LED driver current */
#    define CKLED2001_CURRENT_TUNE \
        { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }
#endif

/* Note on QMK
Fork keychron/qmk
get 1 branch only

Get token
https://github.com/settings/tokens/new

*** Initial setup
qmk config user.qmk_home=/d/Git_Work/Repo/K15_Pro
git config --global user.email "woutcome@gmail.com"
git config --global user.name "difficult-afternoon"
git config --global http.postBuffer 157286400
git config --global credential.helper store
git credential approve
<press Enter then copy below 4 rows>
protocol=https
host=github.com
username=difficult-afternoon
password=insert_token_here
<press Enter twice>
git clone --recurse-submodules https://github.com/difficult-afternoon/qmk_firmware.git /d/Git_Work/Repo/K15_Pro
qmk config user.keyboard=keychron/k15_pro/ansi_encoder/rgb

*** To make firmware
qmk compile -kb keychron/k15_pro/ansi_encoder/rgb -km via_TA

*** To save to github
cd /d/Git_Work/Repo/K15_pro
git add .
git commit -m "some note here"
git push

*** To flash firmware:
Full link: https://www.keychron.com/blogs/archived/k15-pro-factory-reset-and-firmware-flash
Step:
 - Unplug the power cable from the keyboard.
 - Open the QMK Toolbox.
 - Slide the switch toggle on the side of the keyboard to "Off" and connect the USB cable. 
 - Hold down the "Reset" button under the space bar, and then slide the toggle to "Cable".
 - QMK Toolbox will display in yellow words "***DFU device connected".
 
 
*** Link
Tap dance: https://www.reddit.com/r/ploopy/comments/on2unc/tap_dance_examples_for_ploopy/
*/