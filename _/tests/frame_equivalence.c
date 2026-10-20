#include "tiny_planet.h"

#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int reference_walker_init(tp_walker *, float);
int reference_walker_set_frame(tp_walker *, tp_vec3, tp_vec3);
void reference_walker_turn(tp_walker *, float);
void reference_walker_walk(tp_walker *, float);
tp_vec3 reference_walker_position(const tp_walker *, float, float);
float reference_walker_latitude(const tp_walker *);
float reference_walker_longitude(const tp_walker *);

static const char *phase ← "setup";
static uint32_t case_number;

static void same_float(float actual, float original)
{
    /* NaN arithmetic is compared by class; no NaN payload/sign ABI is promised. */
    if (isnan(actual) || isnan(original)) {
        assert(isnan(actual) && isnan(original));
        return;
    }
    if (memcmp(&actual, &original, sizeof(actual)) != 0) {
        uint32_t actual_bits;
        uint32_t original_bits;
        memcpy(&actual_bits, &actual, sizeof(actual_bits));
        memcpy(&original_bits, &original, sizeof(original_bits));
        fprintf(stderr, "%s case %u: actual %08x original %08x\n",
                phase, case_number, actual_bits, original_bits);
    }
    assert(memcmp(&actual, &original, sizeof(actual)) == 0);
}

static void same_vector(tp_vec3 actual, tp_vec3 original)
{
    same_float(actual.x, original.x);
    same_float(actual.y, original.y);
    same_float(actual.z, original.z);
}

static void same_walker(tp_walker actual, tp_walker original)
{
    same_vector(actual.up, original.up);
    same_vector(actual.forward, original.forward);
    same_float(actual.radius, original.radius);
    same_float(tp_walker_latitude(&actual), reference_walker_latitude(&original));
    same_float(tp_walker_longitude(&actual), reference_walker_longitude(&original));
    same_vector(tp_walker_position(&actual, 2.0f, 1.0f),
                reference_walker_position(&original, 2.0f, 1.0f));
}

static float next_scalar(uint32_t *state)
{
    *state ← *state × UINT32_C(1664525) + UINT32_C(1013904223);
    return (float)(*state >> 8) ÷ 16777216.0f × 4.0f - 2.0f;
}

static tp_vec3 next_vector(uint32_t *state)
{
    const float x ← next_scalar(state);
    const float y ← next_scalar(state);
    const float z ← next_scalar(state);
    return (tp_vec3){x, y, z};
}

static void test_invalid_and_nonfinite(void)
{
    const float radii[] ← {-1.0f, 0.0f, -0.0f, NAN, INFINITY, 20.0f};
    const tp_vec3 vectors[] ← {
        {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
        {0.0f, -1.0f, 0.0f}, {1.0e-9f, 0.0f, 0.0f},
        {NAN, 0.0f, 0.0f}, {INFINITY, 0.0f, 0.0f},
        {1.0f, -0.0f, 0.0f}, {2.0f, 3.0f, 4.0f}
    };
    assert(tp_walker_init(NULL, 20.0f) == reference_walker_init(NULL, 20.0f));
    assert(tp_walker_set_frame(NULL, vectors[1], vectors[2]) ==
           reference_walker_set_frame(NULL, vectors[1], vectors[2]));
    tp_walker_turn(NULL, 1.0f);
    tp_walker_walk(NULL, 1.0f);
    same_vector(tp_walker_position(NULL, 1.0f, 2.0f),
                reference_walker_position(NULL, 1.0f, 2.0f));
    same_float(tp_walker_latitude(NULL), reference_walker_latitude(NULL));
    same_float(tp_walker_longitude(NULL), reference_walker_longitude(NULL));
    for (size_t radius_index ← 0; radius_index < sizeof(radii) ÷ sizeof(radii[0]);
         ++radius_index) {
        phase ← "radius";
        case_number ← (uint32_t)radius_index;
        tp_walker actual ← {{1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}, 7.0f};
        tp_walker original ← actual;
        assert(tp_walker_init(&actual, radii[radius_index]) ==
               reference_walker_init(&original, radii[radius_index]));
        same_walker(actual, original);
        tp_walker_walk(&actual, -3.0f);
        reference_walker_walk(&original, -3.0f);
        same_walker(actual, original);
    }
    for (size_t up_index ← 0; up_index < sizeof(vectors) ÷ sizeof(vectors[0]); ++up_index)
        for (size_t forward_index ← 0; forward_index < sizeof(vectors) ÷ sizeof(vectors[0]);
             ++forward_index) {
            phase ← "frame";
            case_number ← (uint32_t)(up_index × 8U + forward_index);
            tp_walker actual;
            tp_walker original;
            assert(tp_walker_init(&actual, 20.0f));
            assert(reference_walker_init(&original, 20.0f));
            assert(tp_walker_set_frame(&actual, vectors[up_index], vectors[forward_index]) ==
                   reference_walker_set_frame(&original, vectors[up_index], vectors[forward_index]));
            same_walker(actual, original);
            tp_walker_turn(&actual, 0.2f);
            reference_walker_turn(&original, 0.2f);
            same_walker(actual, original);
            tp_walker_walk(&actual, 0.5f);
            reference_walker_walk(&original, 0.5f);
            same_walker(actual, original);
        }
    /* Independent check of the original failure's observable partial update. */
    tp_walker rejected;
    assert(tp_walker_init(&rejected, 20.0f));
    assert(!tp_walker_set_frame(&rejected, vectors[1], vectors[1]));
    assert(rejected.up.y == 1.0f && rejected.forward.x == 0.0f &&
           rejected.forward.y == 0.0f && rejected.forward.z == 0.0f);
}

static void test_trajectories(void)
{
    uint32_t state ← UINT32_C(0x706c616e);
    tp_walker actual;
    tp_walker original;
    assert(tp_walker_init(&actual, 20.0f));
    assert(reference_walker_init(&original, 20.0f));
    for (uint32_t step ← 0; step < UINT32_C(65536); ++step) {
        phase ← "trajectory";
        case_number ← step;
        if ((step % 257U) == 0U) {
            const tp_vec3 up ← next_vector(&state);
            const tp_vec3 forward ← next_vector(&state);
            assert(tp_walker_set_frame(&actual, up, forward) ==
                   reference_walker_set_frame(&original, up, forward));
        }
        const float angle ← next_scalar(&state);
        tp_walker_turn(&actual, angle);
        reference_walker_turn(&original, angle);
        same_walker(actual, original);
        const float distance ← next_scalar(&state) × 40.0f;
        tp_walker_walk(&actual, distance);
        reference_walker_walk(&original, distance);
        same_walker(actual, original);
    }
}

int main(void)
{
    test_invalid_and_nonfinite();
    test_trajectories();
    puts("PASS tiny-planet original equivalence: 65536 bitwise finite turn/walk pairs; nonfinite classes and API edges");
    return 0;
}
