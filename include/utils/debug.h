#ifndef __SL_UTILS_DEBUG
#define __SL_UTILS_DEBUG

#include "../structures.h"

/// @brief Unique identifier for a timer
typedef uint8 timer;

/// @brief Initialize a new timer
/// @param start If the timer is active right away
/// @return The identifier of the newly created timer
timer debugInitTimer(bool start);
/// @brief Read the elapsed time for this timer
/// @param ID The identifier of the timer
/// @return The total active time of this timer
double debugReadTimer(timer ID);
/// @brief Activate the timer
/// @param ID The identifier of the timer
void debugStartTimer(timer ID);
/// @brief Reset the timer to 0.0
/// @param ID The identifier of the timer
void debugRestartTimer(timer ID);
/// @brief Deactivate the timer
/// @param ID The identifier of the timer
void debugStopTimer(timer ID);
/// @brief Destroy the timer, freeing its space for other timers to be used
/// @param ID The identifier of the timer
/// @return The total active time of this timer
double debugDestroyTimer(timer ID);
/// @brief Print the time the timer was active for
/// @param ID The identifier of the timer
void debugPrintTimer(timer ID);

#ifdef _glfw3_h_
/// @brief Print an OpenGL error message if one is present
void debugCheckGLError();
#endif

#endif