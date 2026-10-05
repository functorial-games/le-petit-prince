#include "face_ui.h"
#include <math.h>
#include <string.h>
static double clamp(double n,double lo,double hi) {return fmax(lo,fmin(hi,n));}
void face_ui_reset(FaceUI *s) {memset(s,0,sizeof(*s));s->bilateral=1;}
void face_ui_drag(FaceUI *s,double x,double y) {s->yaw+=x*4;s->pitch=clamp(s->pitch+y*4,-1.2,1.2);}
static void preset(FaceUI *s) {
    memset(s->controls,0,sizeof(s->controls));
    switch(s->preset) {
        case 1:s->controls[18]=s->controls[19]=.8;s->controls[10]=s->controls[11]=.45;break;
        case 2:s->controls[22]=s->controls[23]=.8;s->controls[4]=s->controls[5]=.4;break;
        case 3:s->controls[18]=.9;s->controls[10]=.3;break;
        case 4:s->controls[31]=.85;break;
        case 5:s->controls[32]=.9;break;
        case 6:s->controls[0]=s->controls[1]=s->controls[2]=s->controls[3]=.8;break;
        default:break;
    }
}
bool face_ui_press(FaceUI *s,double x,double y) {
    if(!isfinite(x)||!isfinite(y)||x<0||x>=1||y<0||y>=1)return false;
    if(y<.07&&x>=.75){s->view_mode=!s->view_mode;return true;}
    if(y<.72||y>=.96)return false;
    int column=(int)(x*4);
    if(y<.80) {
        if(column==0)s->selected=(s->selected+35)%36;
        if(column==1)s->selected=(s->selected+1)%36;
        if(column>=2) {
            double value=clamp(s->controls[s->selected]+(column==2?-.1:.1),0,1);
            s->controls[s->selected]=value;
            if(s->bilateral&&s->selected<28)s->controls[s->selected^1]=value;
        }
    } else if(y<.88) {
        if(column==0)s->bilateral=!s->bilateral;
        if(column==1)face_ui_reset(s);
        if(column==2) {s->preset=(s->preset+1)%7;preset(s);}
        if(column==3) {s->selected=8;s->controls[8]=s->controls[9]=s->controls[8]>.5?0:1;}
    } else {
        if(column<3) {s->selected=column==0?31:column==1?32:33;s->controls[s->selected]=s->controls[s->selected]>.5?0:.85;}
        else {s->selected=0;s->controls[0]=s->controls[1]=s->controls[2]=s->controls[3]=s->controls[0]>.5?0:.8;}
    }
    return true;
}
