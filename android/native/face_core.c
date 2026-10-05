/* Deliberate numerical port of Beauty model.mjs at the revision in PROVENANCE.md. */
#include "face_core.h"
#include <math.h>
#include <string.h>

enum { PULL,SKELETAL,HYOID,NONE,BULGE,CHEEK_COMPRESS,MENTALIS,LIP_RING,
       LID_RAISE,LID_UPPER,LID_LOWER,EYE_RING,MODIOLUS };
typedef struct {
    FacePoint origin,insertion,radius,jaw;
    double hyoid[2];
    int field,side,jaw_origin,jaw_insertion,conditional;
} Actuator;
typedef struct { Actuator a; FacePoint center; } PosedActuator;
#include "face_data.inc"
static double clamp(double n,double lo,double hi) { return fmax(lo,fmin(hi,n)); }
static FacePoint add(FacePoint a,FacePoint b) { return (FacePoint){a.x+b.x,a.y+b.y,a.z+b.z}; }
static FacePoint sub(FacePoint a,FacePoint b) { return (FacePoint){a.x-b.x,a.y-b.y,a.z-b.z}; }
static FacePoint scale(FacePoint a,double n) { return (FacePoint){a.x*n,a.y*n,a.z*n}; }
static double dot(FacePoint a,FacePoint b) { return a.x*b.x+a.y*b.y+a.z*b.z; }
static double length(FacePoint a) { return hypot(hypot(a.x,a.y),a.z); }
static FacePoint unit(FacePoint a) { double n=length(a);return scale(a,1/(n?n:1)); }
static FacePoint cross(FacePoint a,FacePoint b) { return (FacePoint){a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x}; }
static double smooth(double n) { double t=clamp(n,0,1);return t*t*(3-2*t); }
static double mask_width(double y) { double u=(y-18)/20;return 80-44*smooth((-y-60)/50)-19*smooth((y-75)/35)+11*exp(-u*u); }
static double jaw_weight(FacePoint p) { return smooth((-p.y-20)/42); }

bool face_controls_to_muscles(const double controls[FACE_CONTROLS],double q[FACE_ACTUATORS]) {
    if(!controls||!q) return false;
    for(int i=0;i<FACE_CONTROLS;i++) if(!isfinite(controls[i])||controls[i]<0||controls[i]>1) return false;
    memset(q,0,sizeof(double)*FACE_ACTUATORS);
    for(int i=0;i<FACE_CONTROLS;i++) for(int j=0;j<FACE_ACTUATORS;j++)
        q[j]=clamp(q[j]+controls[i]*control_gains[i][j],0,1);
    for(int j=0;j<FACE_ACTUATORS;j++) if(rest_actuators[j].jaw.x<0) q[j]*=1-controls[31];
    return true;
}
static FacePose solve_skeleton(const double *q) {
    FacePose p={0};double opening=0,protrusion=0,excursion=0,hy=0,hz=0,stabilizers=0,closure=0;
    for(int i=0;i<FACE_ACTUATORS;i++) if(rest_actuators[i].hyoid[0]<0)
        stabilizers+=q[i]*fabs(rest_actuators[i].hyoid[0]);
    p.fixation=clamp(stabilizers/1.4,0,1);
    for(int i=0;i<FACE_ACTUATORS;i++) {
        const Actuator *a=&rest_actuators[i];double v=q[i];
        opening+=v*a->jaw.x*(a->conditional?p.fixation:1);
        closure+=v*fmax(0,-a->jaw.x);
        protrusion+=v*a->jaw.y;excursion+=v*a->jaw.z*(a->side==-1?-1:1);
        hy+=v*a->hyoid[0]*(a->hyoid[0]>0?1-p.fixation:.15);
        hz+=v*a->hyoid[1]*(1-p.fixation);
    }
    p.opening=clamp(opening*.23,0,.48);p.protrusion=clamp(protrusion*5,-7,9);
    p.excursion=clamp(excursion*6,-7,7);p.clench=clamp(closure/2,0,1);
    p.hyoid=(FacePoint){0,clamp(hy*5,-5,8),clamp(hz*5,-5,7)};
    return p;
}
FacePoint face_transform_jaw(FacePoint p,FacePose pose) {
    double y=p.y-12,z=p.z,c=cos(pose.opening),s=sin(pose.opening);
    return (FacePoint){p.x+pose.excursion,12+c*y-s*z,z*c+y*s+pose.protrusion+pose.opening*8};
}
static PosedActuator posed_actuator(Actuator a,FacePose pose) {
    if(a.jaw_origin) a.origin=face_transform_jaw(a.origin,pose);
    if(a.jaw_insertion) a.insertion=face_transform_jaw(a.insertion,pose);
    else a.insertion=add(a.insertion,scale(sub(face_transform_jaw(a.insertion,pose),a.insertion),jaw_weight(a.insertion)));
    FacePoint center=(a.field==BULGE||a.field==MENTALIS)?scale(add(a.origin,a.insertion),.5):a.insertion;
    return (PosedActuator){a,center};
}
static double influence(PosedActuator a,FacePoint p) {
    if(a.a.field==LID_UPPER||a.a.field==LID_LOWER) {
        double dx=p.x*80/mask_width(p.y)-(a.a.side==-1?-32:32);
        double margin=8*sqrt(fmax(0,1-(dx/20)*(dx/20)));
        return (1-smooth((fabs(dx)-18)/10))*(1-smooth((fabs(p.y-34)-margin)/12))*(1-smooth((fabs(p.z-57)-16)/22));
    }
    FacePoint d=sub(p,a.center);
    double r2=(d.x/a.a.radius.x)*(d.x/a.a.radius.x)+(d.y/a.a.radius.y)*(d.y/a.a.radius.y)+(d.z/a.a.radius.z)*(d.z/a.a.radius.z);
    if(r2>=1) return 0;
    return (1-r2)*(1-r2)*(1-r2);
}
static FacePoint contraction(PosedActuator a) { FacePoint d=sub(a.a.origin,a.a.insertion);return scale(unit(d),fmin(7,length(d)*.15)); }
static FacePoint field_delta(PosedActuator a,FacePoint p) {
    const FacePoint zero={0};double w=influence(a,p);if(!w) return zero;
    FacePoint base=contraction(a),offset=sub(p,a.center);
    switch(a.a.field) {
        case SKELETAL:case HYOID:case NONE:return zero;
        case BULGE: {
            FacePoint fiber=unit(sub(a.a.origin,a.a.insertion)),parallel=scale(fiber,dot(offset,fiber));
            return scale(add(scale(parallel,.82-1),scale(sub(offset,parallel),1/sqrt(.82)-1)),w);
        }
        case CHEEK_COMPRESS:return scale(add(base,(FacePoint){0,0,-3}),w);
        case MENTALIS:return scale(add(base,(FacePoint){0,1.5,4}),w);
        case LIP_RING:return scale((FacePoint){-p.x*.24,(-31-p.y)*.38,3.5},w);
        case LID_RAISE:return p.y>=34?scale((FacePoint){0,6,0},w):zero;
        case LID_UPPER:case LID_LOWER:
            if((p.y>=34)!=(a.a.field==LID_UPPER)) return zero;
            return (FacePoint){0,(34-p.y)*w*.96,0};
        case EYE_RING:return scale((FacePoint){((a.a.side==-1?-32:32)-p.x)*.12,(34-p.y)*.18,1.8},w);
        default:return scale(base,w);
    }
}
static bool couples(PosedActuator a) { return a.a.field==LIP_RING||a.a.field==CHEEK_COMPRESS; }
static double corner_weight(FacePoint corner,FacePoint p) {
    PosedActuator a={0};a.center=corner;a.a.radius=(FacePoint){32,30,28};return influence(a,p);
}
static FacePoint normal(const FacePoint *vs,const unsigned short t[3]) { return cross(sub(vs[t[1]],vs[t[0]]),sub(vs[t[2]],vs[t[0]])); }
static bool safe(const FacePoint *vs) {
    for(int i=0;i<FACE_TRIANGLES;i++) {
        FacePoint n=normal(vs,face_triangles[i]),rest=normal(face_rest_vertices,face_triangles[i]);
        if(!(dot(n,rest)>0&&length(n)>.025*length(rest))) return false;
    }
    return true;
}
bool face_deform(const double q[FACE_ACTUATORS],FaceResult *r) {
    if(!q||!r) return false;
    for(int i=0;i<FACE_ACTUATORS;i++) if(!isfinite(q[i])||q[i]<0||q[i]>1) return false;
    memcpy(r->activations,q,sizeof(r->activations));r->pose=solve_skeleton(q);
    PosedActuator fields[FACE_ACTUATORS];
    for(int i=0;i<FACE_ACTUATORS;i++) fields[i]=posed_actuator(rest_actuators[i],r->pose);
    FacePoint corners[2]={{29,-31,63},{-29,-31,63}};
    for(int s=0;s<2;s++) {
        corners[s]=add(corners[s],scale(sub(face_transform_jaw(corners[s],r->pose),corners[s]),jaw_weight(corners[s])));
        r->junctions[s]=(FacePoint){0};
    }
    for(int i=0;i<FACE_ACTUATORS;i++) if(q[i]>0&&fields[i].a.field==MODIOLUS) {
        int s=fields[i].a.side==-1?1:0;
        r->junctions[s]=add(r->junctions[s],scale(contraction(fields[i]),q[i]));
    }
    for(int i=0;i<FACE_ACTUATORS;i++) if(q[i]>0&&couples(fields[i])) for(int s=0;s<2;s++)
        r->junctions[s]=add(r->junctions[s],scale(field_delta(fields[i],corners[s]),q[i]));
    for(int s=0;s<2;s++) { double n=length(r->junctions[s]);if(n>9)r->junctions[s]=scale(r->junctions[s],9/n); }
    FacePoint skeletal[FACE_VERTICES],deltas[FACE_VERTICES];
    for(int i=0;i<FACE_VERTICES;i++) {
        FacePoint p=face_rest_vertices[i];
        FacePoint posed=add(p,scale(sub(face_transform_jaw(p,r->pose),p),jaw_weight(p))),delta={0};
        skeletal[i]=posed;
        double wl=corner_weight(corners[0],posed),wr=corner_weight(corners[1],posed);
        for(int j=0;j<FACE_ACTUATORS;j++) if(q[j]>0&&fields[j].a.field!=MODIOLUS) {
            double free_tissue=couples(fields[j])?1-fmax(wl,wr):1;
            delta=add(delta,scale(field_delta(fields[j],posed),q[j]*free_tissue));
        }
        delta=add(delta,scale(r->junctions[0],wl));delta=add(delta,scale(r->junctions[1],wr));
        double u=p.x/32,v=(p.y+96)/20;
        delta=add(delta,scale(r->pose.hyoid,exp(-u*u-v*v)*.65));
        double n=length(delta);if(n>13)delta=scale(delta,13/n);
        r->vertices[i]=add(posed,delta);
        deltas[i]=sub(r->vertices[i],skeletal[i]);
    }
    r->tissue_scale=1;
    while(!safe(r->vertices)&&r->tissue_scale>1.0/4096) {
        r->tissue_scale*=.5;
        for(int i=0;i<FACE_VERTICES;i++) r->vertices[i]=add(skeletal[i],scale(deltas[i],r->tissue_scale));
    }
    if(!safe(r->vertices)) return false;
    for(int i=0;i<7;i++) r->mandible[i]=face_transform_jaw(rest_mandible[i],r->pose);
    for(int i=0;i<3;i++) r->hyoid[i]=add(rest_hyoid[i],r->pose.hyoid);
    return true;
}
bool face_from_controls(const double c[FACE_CONTROLS],FaceResult *r) { double q[FACE_ACTUATORS];return face_controls_to_muscles(c,q)&&face_deform(q,r); }
FacePoint face_project(FacePoint p,double yaw,double pitch) {
    double c=cos(yaw),s=sin(yaw),x=c*p.x+s*p.z,z=-s*p.x+c*p.z;
    return (FacePoint){x,cos(pitch)*p.y-sin(pitch)*z,sin(pitch)*p.y+cos(pitch)*z};
}
