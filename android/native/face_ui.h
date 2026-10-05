#ifndef PILOT_FACE_UI_H
#define PILOT_FACE_UI_H
#include "face_core.h"
typedef struct {
    double controls[FACE_CONTROLS];
    double yaw,pitch;
    int selected,bilateral,preset;
} FaceUI;
void face_ui_reset(FaceUI *state);
/* Normalized screen coordinates; rows match native renderer layout. */
bool face_ui_press(FaceUI *state,double x,double y);
void face_ui_drag(FaceUI *state,double x,double y);
#endif
