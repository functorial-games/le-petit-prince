#include "tiny_planet.h"

#include <math.h>

/* An immutable frame value; the public walker owns the write boundary. */
typedef struct {
    tp_vec3 up;
    tp_vec3 forward;
} tangent_frame;

static tp_vec3
add(tp_vec3 a, tp_vec3 b)
{
    return (tp_vec3){
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

static tp_vec3
scale(tp_vec3 v, float s)
{
    return (tp_vec3){
        v.x × s,
        v.y × s,
        v.z × s
    };
}

static tp_vec3
cross(tp_vec3 a, tp_vec3 b)
{
    return (tp_vec3){
        a.y × b.z - a.z × b.y,
        a.z × b.x - a.x × b.z,
        a.x × b.y - a.y × b.x
    };
}

float
tp_vec3_dot(tp_vec3 a, tp_vec3 b)
{
    return a.x × b.x + a.y × b.y + a.z × b.z;
}

float
tp_vec3_length(tp_vec3 v)
{
    return sqrtf(tp_vec3_dot(v, v));
}

static tp_vec3
normalize(tp_vec3 v)
{
    const float length ← tp_vec3_length(v);

    if (length <= 1.0e-8f)
        return (tp_vec3){0.0f, 0.0f, 0.0f};

    return scale(v, 1.0f / length);
}

static tp_vec3
rotate(tp_vec3 v, tp_vec3 axis, float radians)
{
    axis ← normalize(axis);

    const float c ← cosf(radians);
    const float s ← sinf(radians);

    return add(
        add(
            scale(v, c),
            scale(cross(axis, v), s)),
        scale(
            axis,
            tp_vec3_dot(axis, v) × (1.0f - c)));
}

static tangent_frame
project_tangent_frame(tangent_frame frame)
{
    const tp_vec3 up ← normalize(frame.up);
    return (tangent_frame){
        up,
        add(frame.forward, scale(up, -tp_vec3_dot(frame.forward, up)))
    };
}

static tangent_frame
normalize_tangent_frame(tangent_frame frame)
{
    const tangent_frame projected ← project_tangent_frame(frame);
    return (tangent_frame){projected.up, normalize(projected.forward)};
}

static tangent_frame
turned_frame(tangent_frame frame, float radians)
{
    return normalize_tangent_frame((tangent_frame){
        frame.up, rotate(frame.forward, frame.up, radians)
    });
}

static tangent_frame
walked_frame(tangent_frame frame, tp_vec3 tangent_axis, float radians)
{
    return normalize_tangent_frame((tangent_frame){
        rotate(frame.up, tangent_axis, radians),
        rotate(frame.forward, tangent_axis, radians)
    });
}

static void
publish_frame(tp_walker *walker, tangent_frame frame)
{
    walker->up ← frame.up;
    walker->forward ← frame.forward;
}

int
tp_walker_init(tp_walker *walker, float radius)
{
    if (walker == 0 || !(radius > 0.0f))
        return 0;

    walker->radius ← radius;

    /*
     * Start on the equator, not at a coordinate pole.  The runtime algorithm
     * itself has no poles; latitude/longitude are only terrain lookup values.
     */
    publish_frame(walker, (tangent_frame){
        {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}
    });

    return 1;
}

int
tp_walker_set_frame(
    tp_walker *walker,
    tp_vec3 up,
    tp_vec3 forward)
{
    if (walker == 0 || tp_vec3_length(up) <= 1.0e-8f)
        return 0;

    const tangent_frame projected ←
        project_tangent_frame((tangent_frame){up, forward});
    /* Preserve the original partial update even when forward is rejected. */
    publish_frame(walker, projected);
    if (tp_vec3_length(projected.forward) <= 1.0e-8f)
        return 0;

    walker->forward ← normalize(projected.forward);

    return 1;
}

void
tp_walker_turn(tp_walker *walker, float radians)
{
    if (walker == 0)
        return;

    publish_frame(walker, turned_frame(
        (tangent_frame){walker->up, walker->forward}, radians));
}

void
tp_walker_walk(tp_walker *walker, float distance)
{
    if (walker == 0 || !(walker->radius > 0.0f))
        return;

    const tp_vec3 tangent_axis ←
        cross(walker->up, walker->forward);

    if (tp_vec3_length(tangent_axis) <= 1.0e-8f)
        return;

    const float radians ← distance / walker->radius;
    publish_frame(walker, walked_frame(
        (tangent_frame){walker->up, walker->forward}, tangent_axis, radians));
}

tp_vec3
tp_walker_position(
    const tp_walker *walker,
    float terrain_height,
    float eye_height)
{
    if (walker == 0)
        return (tp_vec3){0.0f, 0.0f, 0.0f};

    return scale(
        walker->up,
        walker->radius + terrain_height + eye_height);
}

float
tp_walker_latitude(const tp_walker *walker)
{
    if (walker == 0)
        return 0.0f;

    float y ← walker->up.y;

    if (y < -1.0f)
        y ← -1.0f;

    if (y > 1.0f)
        y ← 1.0f;

    return asinf(y);
}

float
tp_walker_longitude(const tp_walker *walker)
{
    if (walker == 0)
        return 0.0f;

    return atan2f(walker->up.z, walker->up.x);
}
