#include "face_core.h"
#include "face_photo_guess.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FaceResult result;
static double maximum_error;
static size_t comparisons;
static void require(int ok,const char *message) { if(!ok) { fprintf(stderr,"FAIL: %s\n",message);exit(1); } }
static void compare(FILE *expected,double actual) {
    double want;
    require(fscanf(expected,"%lf",&want)==1,"missing fixture quantity");
    double error=fabs(actual-want);
    if(error>maximum_error) maximum_error=error;
    if(!isfinite(actual)||error>2e-9*(1+fabs(want))) {
        fprintf(stderr,"quantity %zu: want %.17g got %.17g\n",comparisons,want,actual);exit(1);
    }
    comparisons++;
}
static void point(FILE *f,FacePoint p) { compare(f,p.x);compare(f,p.y);compare(f,p.z); }
static void invariants(void) {
    double c[FACE_CONTROLS]={0},q[FACE_ACTUATORS]={0};
    require(face_from_controls(c,&result),"neutral executes");
    for(int i=0;i<FACE_VERTICES;i++) {
        FacePoint a=result.vertices[i],b=face_rest_vertices[i];
        require(hypot(hypot(a.x-b.x,a.y-b.y),a.z-b.z)<1e-12,"neutral identity");
    }
    require(result.pose.opening==0&&result.tissue_scale==1,"neutral pose/strain");
    c[18]=1;require(face_from_controls(c,&result),"left smile executes");
    require(result.junctions[0].y>0&&result.junctions[1].y==0,"left smile asymmetry");
    memset(c,0,sizeof(c));c[32]=1;require(face_from_controls(c,&result)&&result.pose.excursion>0,"left jaw direction");
    memset(c,0,sizeof(c));c[33]=1;require(face_from_controls(c,&result)&&result.pose.excursion<0,"right jaw direction");
    memset(c,0,sizeof(c));c[31]=1;require(face_from_controls(c,&result)&&result.pose.opening>0,"jaw opening");
    memset(c,0,sizeof(c));c[0]=NAN;require(!face_from_controls(c,&result),"NaN rejected");
    c[0]=INFINITY;require(!face_from_controls(c,&result),"infinity rejected");
    c[0]=-0.1;require(!face_from_controls(c,&result),"negative rejected");
    c[0]=1.1;require(!face_from_controls(c,&result),"over-one rejected");
    q[0]=NAN;require(!face_deform(q,&result),"invalid direct actuator rejected");
    require(pilot_photo_guess_midpoint[0]>.4&&pilot_photo_guess_midpoint[1]>.3,
            "photo guess lifts both medial brows");
    require(pilot_photo_guess_midpoint[6]>.25&&pilot_photo_guess_midpoint[7]>.3,
            "photo guess also recruits corrugators");
    require(pilot_photo_guess_midpoint[2]<.2&&pilot_photo_guess_midpoint[3]<.2,
            "photo guess preserves medial/lateral contrast");
    require(face_deform(pilot_photo_guess_midpoint,&result),
            "photo guess actuators produce safe connected skin");
    require(result.pose.opening==0,"photo guess jaw remains closed");
    for(int j=0;j<FACE_ACTUATORS;j++)
        require(result.activations[j]>=0&&result.activations[j]<=1,
                "photo midpoint within activation domain");
    unsigned char used[FACE_VERTICES]={0},reached[FACE_VERTICES]={0};reached[0]=1;
    for(int i=0;i<FACE_TRIANGLES;i++)for(int j=0;j<3;j++) {require(face_triangles[i][j]<FACE_VERTICES,"valid mesh index");used[face_triangles[i][j]]=1;}
    for(int pass=0;pass<FACE_VERTICES;pass++) {
        int changed=0;
        for(int i=0;i<FACE_TRIANGLES;i++) {const unsigned short *t=face_triangles[i];
            if(reached[t[0]]||reached[t[1]]||reached[t[2]]) for(int j=0;j<3;j++) if(!reached[t[j]]) {reached[t[j]]=1;changed=1;}}
        if(!changed)break;
    }
    for(int i=0;i<FACE_VERTICES;i++)require(used[i]&&reached[i],"connected mesh without orphans");
}
int main(int argc,char **argv) {
    require(argc==3,"face-test INPUTS EXPECTED");invariants();
    FILE *inputs=fopen(argv[1],"r"),*expected=fopen(argv[2],"r");require(inputs&&expected,"fixtures open");
    char name[128],reference_name[128];int kind,cases=0;
    while(fscanf(inputs,"%127s %d",name,&kind)==2) {
        double values[FACE_ACTUATORS]={0};int count=kind?FACE_ACTUATORS:FACE_CONTROLS;
        require(kind==0||kind==1,"fixture input kind");
        for(int i=0;i<count;i++)require(fscanf(inputs,"%lf",&values[i])==1,"fixture input vector");
        require(kind?face_deform(values,&result):face_from_controls(values,&result),name);
        require(fscanf(expected,"%127s",reference_name)==1&&!strcmp(name,reference_name),"fixture names match");
        for(int i=0;i<FACE_ACTUATORS;i++)compare(expected,result.activations[i]);
        compare(expected,result.pose.opening);compare(expected,result.pose.protrusion);compare(expected,result.pose.excursion);compare(expected,result.pose.clench);
        point(expected,result.pose.hyoid);compare(expected,result.pose.fixation);
        point(expected,result.junctions[0]);point(expected,result.junctions[1]);compare(expected,result.tissue_scale);
        for(int i=0;i<FACE_VERTICES;i++)point(expected,result.vertices[i]);
        for(int i=0;i<7;i++)point(expected,result.mandible[i]);
        for(int i=0;i<3;i++)point(expected,result.hyoid[i]);
        cases++;
    }
    require(feof(inputs),"all inputs consumed");require(fscanf(expected,"%127s",name)==EOF,"no extra expected data");
    require(cases==213,"all 213 expected reference cases executed");
    fclose(inputs);fclose(expected);
    printf("PASS native invariants and Beauty comparison: %d cases; %zu quantities; max absolute error %.17g\n",cases,comparisons,maximum_error);
    return 0;
}
