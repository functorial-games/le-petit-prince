#ifndef PILOT_FACE_PHOTO_GUESS_H
#define PILOT_FACE_PHOTO_GUESS_H
/* Generated projection of pilot-expressions/intervals.idr. */
/* Run: python3 pilot-expressions/check_intervals.py --write */
/* Midpoints are native rig activations, NOT measured muscle positions. */
static const double pilot_photo_guess_midpoint[FACE_ACTUATORS] = {
    [0] = 0.560, /* frontalis_medial_L [45, 67] percent */
    [1] = 0.495, /* frontalis_medial_R [38, 61] percent */
    [2] = 0.125, /* frontalis_lateral_L [5, 20] percent */
    [3] = 0.110, /* frontalis_lateral_R [4, 18] percent */
    [6] = 0.390, /* corrugator_supercilii_L [28, 50] percent */
    [7] = 0.435, /* corrugator_supercilii_R [32, 55] percent */
    [8] = 0.195, /* depressor_supercilii_L [11, 28] percent */
    [9] = 0.220, /* depressor_supercilii_R [13, 31] percent */
    [10] = 0.240, /* procerus [14, 34] percent */
    [11] = 0.130, /* orbicularis_oculi_palpebral_upper_L [6, 20] percent */
    [12] = 0.150, /* orbicularis_oculi_palpebral_upper_R [8, 22] percent */
    [13] = 0.125, /* orbicularis_oculi_palpebral_lower_L [6, 19] percent */
    [14] = 0.150, /* orbicularis_oculi_palpebral_lower_R [8, 22] percent */
    [15] = 0.115, /* orbicularis_oculi_orbital_superior_L [5, 18] percent */
    [16] = 0.140, /* orbicularis_oculi_orbital_superior_R [7, 21] percent */
    [17] = 0.120, /* orbicularis_oculi_orbital_inferior_L [6, 18] percent */
    [18] = 0.135, /* orbicularis_oculi_orbital_inferior_R [7, 20] percent */
    [19] = 0.305, /* levator_palpebrae_superioris_L [20, 41] percent */
    [20] = 0.275, /* levator_palpebrae_superioris_R [17, 38] percent */
    [40] = 0.130, /* orbicularis_oris_upper_medial_L [7, 19] percent */
    [41] = 0.120, /* orbicularis_oris_upper_medial_R [6, 18] percent */
    [44] = 0.115, /* orbicularis_oris_lower_medial_L [6, 17] percent */
    [45] = 0.110, /* orbicularis_oris_lower_medial_R [6, 16] percent */
    [48] = 0.210, /* depressor_anguli_oris_L [12, 30] percent */
    [49] = 0.185, /* depressor_anguli_oris_R [10, 27] percent */
    [50] = 0.070, /* depressor_labii_inferioris_L [1, 13] percent */
    [51] = 0.065, /* depressor_labii_inferioris_R [1, 12] percent */
    [52] = 0.090, /* mentalis_L [3, 15] percent */
    [53] = 0.085, /* mentalis_R [3, 14] percent */
};
#endif
