#ifndef PILOT_FACE_CORE_H
#define PILOT_FACE_CORE_H
#include <stdbool.h>
#define FACE_CONTROLS 36
#define FACE_ACTUATORS 102
#include "face_data_sizes.h"
typedef struct { double x,y,z; } FacePoint;
typedef struct {
    double opening, protrusion, excursion, clench;
    FacePoint hyoid;
    double fixation;
} FacePose;
typedef struct {
    double activations[FACE_ACTUATORS];
    FacePose pose;
    FacePoint junctions[2];
    double tissue_scale;
    FacePoint vertices[FACE_VERTICES], mandible[7], hyoid[3];
} FaceResult;
extern const char *const face_control_names[FACE_CONTROLS];
extern const char *const face_actuator_names[FACE_ACTUATORS];
extern const char *const face_landmark_names[FACE_LANDMARKS];
extern const FacePoint face_rest_vertices[FACE_VERTICES];
extern const unsigned short face_triangles[FACE_TRIANGLES][3];
extern const unsigned short face_landmarks[FACE_LANDMARKS];
/* Invalid input or folded skeletal surface: false; result must not be used. */
bool face_controls_to_muscles(const double controls[FACE_CONTROLS], double activations[FACE_ACTUATORS]);
bool face_deform(const double activations[FACE_ACTUATORS], FaceResult *result);
bool face_from_controls(const double controls[FACE_CONTROLS], FaceResult *result);
FacePoint face_transform_jaw(FacePoint point, FacePose pose);
FacePoint face_project(FacePoint point, double yaw, double pitch);
#endif
