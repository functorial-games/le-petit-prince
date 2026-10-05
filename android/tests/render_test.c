/* Host Mesa pbuffer evidence only; not Android or MIRO execution. */
#include "pilot_renderer.h"
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define WIDTH 576
#define HEIGHT 1152
static unsigned char pixels[WIDTH*HEIGHT*4],neutral[WIDTH*HEIGHT*4];
static void require(int ok,const char *message){if(!ok){fprintf(stderr,"FAIL render: %s\n",message);exit(1);}}
static void frame(const char *name,int neutral_expected) {
    pilot_renderer_draw();glFinish();glReadPixels(0,0,WIDTH,HEIGHT,GL_RGBA,GL_UNSIGNED_BYTE,pixels);
    require(glGetError()==GL_NO_ERROR,"GLES draw/read succeeds");
    size_t changed=0,face_pixels=0;
    for(int y=(int)(HEIGHT*.37);y<(int)(HEIGHT*.93);y++)for(int x=0;x<WIDTH;x++) {
        int i=(y*WIDTH+x)*4;
        if(memcmp(pixels+i,neutral+i,3))changed++;
        unsigned char brightest=pixels[i]>pixels[i+1]?pixels[i]:pixels[i+1];
        if(pixels[i+2]>brightest)brightest=pixels[i+2];
        if(brightest>45)face_pixels++;
    }
    require(face_pixels>10000,"face visible in native renderer");
    if(!strcmp(name,"neutral"))memcpy(neutral,pixels,sizeof(pixels));
    else if(neutral_expected)require(changed==0,"neutral reset restores exact face pixels");
    else require(changed>100,"state materially changes visible face pixels");
    char filename[256];snprintf(filename,sizeof(filename),"build/render/%s.ppm",name);
    FILE *out=fopen(filename,"wb");require(out!=NULL,"render output open");fprintf(out,"P6\n%d %d\n255\n",WIDTH,HEIGHT);
    for(int y=HEIGHT-1;y>=0;y--)for(int x=0;x<WIDTH;x++)require(fwrite(pixels+(y*WIDTH+x)*4,1,3,out)==3,"render output write");
    require(fclose(out)==0,"render output close");printf("PASS host GLES %s: %zu changed face pixels\n",name,changed);
}
int main(void) {
    PFNEGLGETPLATFORMDISPLAYEXTPROC platform=(PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");
    require(platform!=NULL,"Mesa surfaceless extension");EGLDisplay display=platform(EGL_PLATFORM_SURFACELESS_MESA,EGL_DEFAULT_DISPLAY,NULL);
    require(eglInitialize(display,NULL,NULL),"EGL initialize");require(eglBindAPI(EGL_OPENGL_ES_API),"GLES API");
    EGLint attr[]={EGL_SURFACE_TYPE,EGL_PBUFFER_BIT,EGL_RENDERABLE_TYPE,EGL_OPENGL_ES2_BIT,EGL_RED_SIZE,8,EGL_GREEN_SIZE,8,EGL_BLUE_SIZE,8,EGL_ALPHA_SIZE,8,EGL_DEPTH_SIZE,16,EGL_NONE};
    EGLConfig config;EGLint count;require(eglChooseConfig(display,attr,&config,1,&count)&&count==1,"EGL config");
    EGLint surface_attr[]={EGL_WIDTH,WIDTH,EGL_HEIGHT,HEIGHT,EGL_NONE},context_attr[]={EGL_CONTEXT_CLIENT_VERSION,2,EGL_NONE};
    EGLSurface surface=eglCreatePbufferSurface(display,config,surface_attr);EGLContext context=eglCreateContext(display,config,EGL_NO_CONTEXT,context_attr);
    require(surface!=EGL_NO_SURFACE&&context!=EGL_NO_CONTEXT&&eglMakeCurrent(display,surface,surface,context),"pbuffer context");
    printf("Host GL vendor: %s; renderer: %s\n",glGetString(GL_VENDOR),glGetString(GL_RENDERER));
    require(pilot_renderer_start(WIDTH,HEIGHT,2),"native renderer starts");frame("neutral",1);
    require(pilot_renderer_press(.875f,.035f),"rainbow view input");frame("rainbow-selected-control",0);
    require(pilot_renderer_press(.875f,.035f),"skin view input");frame("skin-restored",1);
    const char *names[]={"smile","frown","unilateral","jaw-open","jaw-left","brow"};
    for(int i=0;i<6;i++){require(pilot_renderer_press(.625f,.84f),"preset input");frame(names[i],0);}
    require(pilot_renderer_press(.375f,.84f),"neutral input");frame("reset",1);
    require(pilot_renderer_press(.625f,.92f),"jaw right input");frame("jaw-right",0);
    pilot_renderer_drag(.15f,.07f);frame("orbit",0);
    unsigned long size;void *saved=pilot_renderer_save(&size);require(saved!=NULL,"save state");
    memcpy(neutral,pixels,sizeof(pixels));pilot_renderer_stop();require(pilot_renderer_start(WIDTH,HEIGHT,2),"surface recreation");
    pilot_renderer_restore(saved,size);free(saved);frame("recreated",1);
    pilot_renderer_stop();eglMakeCurrent(display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);
    eglDestroyContext(display,context);eglDestroySurface(display,surface);eglTerminate(display);return 0;
}
