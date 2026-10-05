#include "pilot_renderer.h"
#include "face_ui.h"
#include <GLES2/gl2.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* One small GLES2 triangle/color pipeline for both surface and diagnostic UI. */
typedef struct {float x,y,z,r,g,b;} Vertex;
#define CAPACITY 100000
static Vertex batch[CAPACITY];
static int used,width,height;
static GLuint program,buffer;
static GLint position_attribute,color_attribute;
static FaceUI ui;
static FaceResult result;
static bool initialized;
static const char *const preset_names[]={"NEUTRAL","SMILE","FROWN","LEFT SMILE","JAW OPEN","JAW LEFT","BROWS"};
static void vertex(float x,float y,float z,float r,float g,float b) {
    if(used<CAPACITY)batch[used++]=(Vertex){x,y,z,r,g,b};
}
static void flush(void) {
    glUseProgram(program);glBindBuffer(GL_ARRAY_BUFFER,buffer);
    glBufferData(GL_ARRAY_BUFFER,(GLsizeiptr)(used*sizeof(Vertex)),batch,GL_STREAM_DRAW);
    glEnableVertexAttribArray((GLuint)position_attribute);glEnableVertexAttribArray((GLuint)color_attribute);
    glVertexAttribPointer((GLuint)position_attribute,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0);
    glVertexAttribPointer((GLuint)color_attribute,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)(3*sizeof(float)));
    glDrawArrays(GL_TRIANGLES,0,used);used=0;
}
static GLuint shader(GLenum type,const char *text) {
    GLuint s=glCreateShader(type);glShaderSource(s,1,&text,NULL);glCompileShader(s);
    GLint ok=0;glGetShaderiv(s,GL_COMPILE_STATUS,&ok);if(!ok){glDeleteShader(s);return 0;}return s;
}
bool pilot_renderer_start(int w,int h,int gles_major) {
    (void)gles_major;
    const char *vs="attribute vec3 position; attribute vec3 color; varying lowp vec3 shade; void main(){gl_Position=vec4(position,1.0);shade=color;}";
    const char *fs="precision mediump float; varying lowp vec3 shade; void main(){gl_FragColor=vec4(shade,1.0);}";
    GLuint v=shader(GL_VERTEX_SHADER,vs),f=shader(GL_FRAGMENT_SHADER,fs);
    if(!v||!f){if(v)glDeleteShader(v);if(f)glDeleteShader(f);return false;}
    program=glCreateProgram();glAttachShader(program,v);glAttachShader(program,f);glLinkProgram(program);
    glDeleteShader(v);glDeleteShader(f);GLint ok;glGetProgramiv(program,GL_LINK_STATUS,&ok);
    if(!ok){glDeleteProgram(program);program=0;return false;}
    position_attribute=glGetAttribLocation(program,"position");color_attribute=glGetAttribLocation(program,"color");
    glGenBuffers(1,&buffer);pilot_renderer_resize(w,h);
    if(!initialized){face_ui_reset(&ui);initialized=true;}
    return face_from_controls(ui.controls,&result);
}
void pilot_renderer_stop(void){if(buffer)glDeleteBuffers(1,&buffer);if(program)glDeleteProgram(program);buffer=program=0;}
void pilot_renderer_resize(int w,int h){width=w;height=h;}
void pilot_renderer_drag(float x,float y){face_ui_drag(&ui,x,y);}
bool pilot_renderer_press(float x,float y){return face_ui_press(&ui,x,y);}
void *pilot_renderer_save(unsigned long *size){FaceUI *s=malloc(sizeof(*s));if(!s){*size=0;return NULL;}*s=ui;*size=sizeof(*s);return s;}
void pilot_renderer_restore(const void *data,unsigned long size) {
    if(size!=sizeof(FaceUI)||!data)return;
    FaceUI saved;memcpy(&saved,data,size);
    double q[FACE_ACTUATORS];
    if(!face_controls_to_muscles(saved.controls,q)||!isfinite(saved.yaw)||!isfinite(saved.pitch)||saved.pitch<-1.2||saved.pitch>1.2||saved.selected<0||saved.selected>=36||saved.preset<0||saved.preset>6||saved.bilateral<0||saved.bilateral>1)return;
    ui=saved;initialized=true;
}
static FacePoint project(FacePoint p){return face_project(p,ui.yaw,ui.pitch);}
static void face_vertex(FacePoint p,float r,float g,float b) {
    /* Orthographic inspection. Depth is mapped explicitly; front is smaller. */
    double unit=fmin(width/190.0,height*.56/240.0);
    vertex((float)(2*p.x*unit/width),(float)(.30+2*p.y*unit/height),(float)(-p.z/240),r,g,b);
}
static void eye(double cx) {
    FacePoint center=project((FacePoint){cx,34,42});
    for(int i=0;i<40;i++) {
        double a=i*6.283185307179586/40,b=(i+1)*6.283185307179586/40;
        face_vertex(center,.85f,.80f,.67f);
        face_vertex(project((FacePoint){cx+18*cos(a),34+8*sin(a),42}),.85f,.80f,.67f);
        face_vertex(project((FacePoint){cx+18*cos(b),34+8*sin(b),42}),.85f,.80f,.67f);
        face_vertex(project((FacePoint){cx,34,43}),.04f,.035f,.025f);
        face_vertex(project((FacePoint){cx+4*cos(a),34+4*sin(a),43}),.04f,.035f,.025f);
        face_vertex(project((FacePoint){cx+4*cos(b),34+4*sin(b),43}),.04f,.035f,.025f);
    }
}
/* Compact 5x7 uppercase diagnostic font. Bit 4 is the left pixel. */
static const unsigned char glyphs[39][7]={
 {14,17,17,31,17,17,17},{30,17,17,30,17,17,30},{14,17,16,16,16,17,14},
 {30,17,17,17,17,17,30},{31,16,16,30,16,16,31},{31,16,16,30,16,16,16},
 {14,17,16,23,17,17,14},{17,17,17,31,17,17,17},{14,4,4,4,4,4,14},
 {7,2,2,2,18,18,12},{17,18,20,24,20,18,17},{16,16,16,16,16,16,31},
 {17,27,21,21,17,17,17},{17,25,21,19,17,17,17},{14,17,17,17,17,17,14},
 {30,17,17,30,16,16,16},{14,17,17,17,21,18,13},{30,17,17,30,20,18,17},
 {15,16,16,14,1,1,30},{31,4,4,4,4,4,4},{17,17,17,17,17,17,14},
 {17,17,17,17,17,10,4},{17,17,17,21,21,21,10},{17,17,10,4,10,17,17},
 {17,17,10,4,4,4,4},{31,1,2,4,8,16,31},
 {14,17,19,21,25,17,14},{4,12,4,4,4,4,14},{14,17,1,2,4,8,31},
 {30,1,1,14,1,1,30},{2,6,10,18,31,2,2},{31,16,16,30,1,1,30},
 {14,16,16,30,17,17,14},{31,1,2,4,8,8,8},{14,17,17,14,17,17,14},
 {14,17,17,15,1,1,14},{0,4,4,31,4,4,0},{0,0,0,31,0,0,0},{0,0,0,0,0,6,6}
};
static void rectangle(double x,double y,double w,double h,float r,float g,float b) {
    float x0=(float)(x*2-1),x1=(float)((x+w)*2-1),y0=(float)(1-y*2),y1=(float)(1-(y+h)*2);
    vertex(x0,y0,0,r,g,b);vertex(x1,y0,0,r,g,b);vertex(x1,y1,0,r,g,b);
    vertex(x0,y0,0,r,g,b);vertex(x1,y1,0,r,g,b);vertex(x0,y1,0,r,g,b);
}
static void text(double x,double y,const char *s,double pixels) {
    for(;*s;s++,x+=6*pixels/width) {
        unsigned char c=(unsigned char)*s;if(c>='a'&&c<='z')c=(unsigned char)(c-'a'+'A');
        int i=c>='A'&&c<='Z'?c-'A':c>='0'&&c<='9'?26+c-'0':c=='+'?36:c=='-'||c=='_'?37:c=='.'?38:-1;
        if(i<0)continue;
        for(int row=0;row<7;row++)for(int col=0;col<5;col++)if(glyphs[i][row]&(1<<(4-col)))
            rectangle(x+col*pixels/width,y+row*pixels/height,pixels/width,pixels/height,.92f,.88f,.76f);
    }
}
static void button(int column,double y,const char *label) {
    rectangle(column*.25+.01,y+.004,.23,.068,.19f,.22f,.23f);
    text(column*.25+.025,y+.025,label,width/250.0);
}
void pilot_renderer_draw(void) {
    if(width<=0||height<=0||!program)return;
    glViewport(0,0,width,height);glClearColor(.07f,.09f,.11f,1);glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);glDisable(GL_CULL_FACE);glEnable(GL_SCISSOR_TEST);
    glScissor(0,(int)(height*.37),width,(int)(height*.56));used=0;
    bool ok=face_from_controls(ui.controls,&result);
    if(ok) {
        for(int i=0;i<FACE_TRIANGLES;i++) {
            const unsigned short *t=face_triangles[i];FacePoint a=project(result.vertices[t[0]]),b=project(result.vertices[t[1]]),c=project(result.vertices[t[2]]);
            double ux=b.x-a.x,uy=b.y-a.y,uz=b.z-a.z,vx=c.x-a.x,vy=c.y-a.y,vz=c.z-a.z;
            double nx=uy*vz-uz*vy,ny=uz*vx-ux*vz,nz=ux*vy-uy*vx,n=sqrt(nx*nx+ny*ny+nz*nz);
            double shade=.36+.64*fmax(0,(nx*.2+ny*.4+nz*.9)/(n?n:1)/sqrt(1.01));
            face_vertex(a,(float)(.56*shade),(float)(.35*shade),(float)(.23*shade));
            face_vertex(b,(float)(.56*shade),(float)(.35*shade),(float)(.23*shade));
            face_vertex(c,(float)(.56*shade),(float)(.35*shade),(float)(.23*shade));
        }
        eye(32);eye(-32);flush();
    }
    glDisable(GL_SCISSOR_TEST);glDisable(GL_DEPTH_TEST);
    rectangle(0,0,1,.07,.12f,.15f,.16f);text(.03,.018,"PILOT FACE - NATIVE TEST",width/210.0);
    text(.03,.635,"DRAG FACE TO ORBIT",width/260.0);
    char line[96];snprintf(line,sizeof(line),"%02d %s %.2f",ui.selected+1,face_control_names[ui.selected],ui.controls[ui.selected]);
    text(.03,.67,line,width/235.0);
    button(0,.72,"PREV");button(1,.72,"NEXT");button(2,.72,"- 0.1");button(3,.72,"+ 0.1");
    button(0,.80,ui.bilateral?"PAIR ON":"PAIR OFF");button(1,.80,"NEUTRAL");button(2,.80,"PRESET");button(3,.80,"BLINK");
    button(0,.88,"JAW OPEN");button(1,.88,"JAW LEFT");button(2,.88,"JAW RIGHT");button(3,.88,"BROW");
    snprintf(line,sizeof(line),"%s - STRAIN %.3f",ok?preset_names[ui.preset]:"MODEL ERROR",result.tissue_scale);
    text(.03,.965,line,width/270.0);flush();
}
