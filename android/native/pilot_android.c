#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <android/input.h>
#include <android/log.h>
#include <android/native_window.h>
#include <android_native_app_glue.h>

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "pilot_renderer.h"

#define PILOT_LOG_TAG "PauliNative"
#define PILOT_LOG(...) __android_log_print(ANDROID_LOG_INFO, PILOT_LOG_TAG, __VA_ARGS__)

struct pilot_android_state {
    struct android_app *app;

    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;

    int gles_major;
    int width;
    int height;

    bool renderer_started;
    bool redraw;
    bool touching;
    bool moved;

    float previous_x;
    float previous_y;
};

static void pilot_stop_surface(struct pilot_android_state *state) {
    if (state->renderer_started) {
        pilot_renderer_stop();
        state->renderer_started = false;
    }

    if (state->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(
            state->display,
            EGL_NO_SURFACE,
            EGL_NO_SURFACE,
            EGL_NO_CONTEXT
        );

        if (state->context != EGL_NO_CONTEXT) {
            eglDestroyContext(state->display, state->context);
        }
        if (state->surface != EGL_NO_SURFACE) {
            eglDestroySurface(state->display, state->surface);
        }

        eglTerminate(state->display);
    }

    state->display = EGL_NO_DISPLAY;
    state->surface = EGL_NO_SURFACE;
    state->context = EGL_NO_CONTEXT;
    state->gles_major = 0;
    state->width = 0;
    state->height = 0;
    state->redraw = false;
}

static bool pilot_choose_config(
    EGLDisplay display,
    int gles_major,
    EGLConfig *config
) {
    const EGLint renderable =
        gles_major >= 3 ? EGL_OPENGL_ES3_BIT_KHR : EGL_OPENGL_ES2_BIT;

    const EGLint attributes[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, renderable,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 16,
        EGL_NONE
    };

    EGLint count = 0;
    return eglChooseConfig(display, attributes, config, 1, &count) == EGL_TRUE
        && count == 1;
}

static bool pilot_start_surface(struct pilot_android_state *state) {
    if (state->app->window == NULL) {
        return false;
    }

    pilot_stop_surface(state);

    state->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (state->display == EGL_NO_DISPLAY) {
        return false;
    }

    if (eglInitialize(state->display, NULL, NULL) != EGL_TRUE) {
        pilot_stop_surface(state);
        return false;
    }

    if (eglBindAPI(EGL_OPENGL_ES_API) != EGL_TRUE) {
        pilot_stop_surface(state);
        return false;
    }

    EGLConfig config = NULL;
    int gles_major = 2;

    if (!pilot_choose_config(state->display, gles_major, &config)) {
        gles_major = 2;
        if (!pilot_choose_config(state->display, gles_major, &config)) {
            pilot_stop_surface(state);
            return false;
        }
    }

    EGLint native_format = 0;
    if (eglGetConfigAttrib(
            state->display,
            config,
            EGL_NATIVE_VISUAL_ID,
            &native_format
        ) != EGL_TRUE) {
        pilot_stop_surface(state);
        return false;
    }

    ANativeWindow_setBuffersGeometry(
        state->app->window,
        0,
        0,
        native_format
    );

    const EGLint context_attributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, gles_major,
        EGL_NONE
    };

    state->context = eglCreateContext(
        state->display,
        config,
        EGL_NO_CONTEXT,
        context_attributes
    );
    if (state->context == EGL_NO_CONTEXT) {
        pilot_stop_surface(state);
        return false;
    }

    state->surface = eglCreateWindowSurface(
        state->display,
        config,
        state->app->window,
        NULL
    );
    if (state->surface == EGL_NO_SURFACE) {
        pilot_stop_surface(state);
        return false;
    }

    if (eglMakeCurrent(
            state->display,
            state->surface,
            state->surface,
            state->context
        ) != EGL_TRUE) {
        pilot_stop_surface(state);
        return false;
    }

    EGLint width = 0;
    EGLint height = 0;
    eglQuerySurface(state->display, state->surface, EGL_WIDTH, &width);
    eglQuerySurface(state->display, state->surface, EGL_HEIGHT, &height);

    state->gles_major = gles_major;
    state->width = width;
    state->height = height;

    if (!pilot_renderer_start(width, height, gles_major)) {
        pilot_stop_surface(state);
        return false;
    }

    state->renderer_started = true;
    state->redraw = true;
    PILOT_LOG("EGL surface ready: %dx%d GLES %d", width, height, gles_major);
    return true;
}

static void pilot_resize(struct pilot_android_state *state) {
    if (!state->renderer_started || state->surface == EGL_NO_SURFACE) {
        return;
    }

    EGLint width = 0;
    EGLint height = 0;
    eglQuerySurface(state->display, state->surface, EGL_WIDTH, &width);
    eglQuerySurface(state->display, state->surface, EGL_HEIGHT, &height);

    if (width > 0 && height > 0) {
        state->width = width;
        state->height = height;
        pilot_renderer_resize(width, height);
        state->redraw = true;
    }
}

static void pilot_handle_command(struct android_app *app, int32_t command) {
    struct pilot_android_state *state =
        (struct pilot_android_state *)app->userData;

    switch (command) {
        case APP_CMD_INIT_WINDOW:
            (void)pilot_start_surface(state);
            break;

        case APP_CMD_TERM_WINDOW:
            pilot_stop_surface(state);
            break;

        case APP_CMD_WINDOW_RESIZED:
        case APP_CMD_CONFIG_CHANGED:
            pilot_resize(state);
            break;

        case APP_CMD_GAINED_FOCUS:
            state->redraw = true;
            break;

        case APP_CMD_SAVE_STATE: {
            unsigned long size=0;
            app->savedState=pilot_renderer_save(&size);
            app->savedStateSize=(size_t)size;
            break;
        }

        default:
            break;
    }
}

static int32_t pilot_handle_input(
    struct android_app *app,
    AInputEvent *event
) {
    struct pilot_android_state *state =
        (struct pilot_android_state *)app->userData;

    if (AInputEvent_getType(event) != AINPUT_EVENT_TYPE_MOTION) {
        return 0;
    }

    const int32_t action =
        AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
    const float x = AMotionEvent_getX(event, 0);
    const float y = AMotionEvent_getY(event, 0);

    if (action == AMOTION_EVENT_ACTION_DOWN) {
        if(state->width<=0||state->height<=0) return 0;
        if(pilot_renderer_press(x/state->width,y/state->height)) {
            state->redraw=true;
            return 1;
        }
        state->touching = y/state->height>=.07f&&y/state->height<.63f;
        state->moved = false;
        state->previous_x = x;
        state->previous_y = y;
        return 1;
    }

    if (action == AMOTION_EVENT_ACTION_MOVE && state->touching) {
        const int short_side_pixels =
            state->width > 0 && state->height > 0
                ? (state->width < state->height
                    ? state->width
                    : state->height)
                : 1;

        const float short_side = (float)short_side_pixels;
        const float delta_x = (x - state->previous_x) / short_side;
        const float delta_y = (y - state->previous_y) / short_side;

        if (fabsf(delta_x) + fabsf(delta_y) > 0.001f) {
            state->moved = true;
            pilot_renderer_drag(delta_x, delta_y);
            PILOT_LOG("drag: %.6f %.6f", delta_x, delta_y);
            state->redraw = true;
        }

        state->previous_x = x;
        state->previous_y = y;
        return 1;
    }

    if ((action == AMOTION_EVENT_ACTION_UP
         || action == AMOTION_EVENT_ACTION_CANCEL)
        && state->touching) {
        state->touching = false;
        state->moved = false;
        return 1;
    }

    return 0;
}

void android_main(struct android_app *app) {
    app_dummy();
    PILOT_LOG("native entry");

    struct pilot_android_state state;
    memset(&state, 0, sizeof(state));

    state.app = app;
    state.display = EGL_NO_DISPLAY;
    state.surface = EGL_NO_SURFACE;
    state.context = EGL_NO_CONTEXT;

    app->userData = &state;
    app->onAppCmd = pilot_handle_command;
    app->onInputEvent = pilot_handle_input;
    pilot_renderer_restore(app->savedState,(unsigned long)app->savedStateSize);

    for (;;) {
        int events = 0;
        struct android_poll_source *source = NULL;

        const int timeout =
            state.renderer_started && state.redraw ? 0 : -1;

        int ident = 0;
        while ((ident = ALooper_pollOnce(
                    timeout,
                    NULL,
                    &events,
                    (void **)&source
                )) >= 0) {
            (void)ident;

            if (source != NULL) {
                source->process(app, source);
            }

            if (app->destroyRequested != 0) {
                pilot_stop_surface(&state);
                return;
            }

            if (state.renderer_started && state.redraw) {
                break;
            }
        }

        if (app->destroyRequested != 0) {
            pilot_stop_surface(&state);
            return;
        }

        if (state.renderer_started && state.redraw) {
            pilot_renderer_draw();

            if (eglSwapBuffers(state.display, state.surface) != EGL_TRUE) {
                pilot_stop_surface(&state);
                continue;
            }

            PILOT_LOG("frame presented");
            state.redraw = false;
        }
    }
}
