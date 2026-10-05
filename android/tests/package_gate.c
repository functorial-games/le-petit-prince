/* Inspect finished package evidence; no broad Android runtime claims. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void check(int ok,const char *message) {if(!ok){fprintf(stderr,"FAIL package: %s\n",message);exit(1);}}
static char *read_text(const char *path) {
    FILE *f=fopen(path,"rb");check(f!=NULL,path);check(fseek(f,0,SEEK_END)==0,"seek");
    long n=ftell(f);check(n>=0&&n<4000000,"report size");rewind(f);
    char *s=malloc((size_t)n+1);check(s!=NULL,"allocation");check(fread(s,1,(size_t)n,f)==(size_t)n,"report read");s[n]=0;fclose(f);return s;
}
static void elf(const char *path) {
    FILE *f=fopen(path,"rb");check(f!=NULL,"native ELF exists");unsigned char h[52];check(fread(h,1,52,f)==52,"ELF header");fclose(f);
    check(!memcmp(h,"\177ELF",4)&&h[4]==1&&h[5]==1,"ELF32 little endian");
    check(h[16]==3&&h[17]==0&&h[18]==40&&h[19]==0,"ARM shared object");
    unsigned flags=(unsigned)h[36]|((unsigned)h[37]<<8)|((unsigned)h[38]<<16)|((unsigned)h[39]<<24);
    check((flags&0xff000000)==0x05000000&&!(flags&0x400),"EABI5 without hard-float public ABI");
}
int main(int argc,char **argv) {
    if(argc==3&&!strcmp(argv[1],"elf")){elf(argv[2]);puts("PASS ELF32 ARM EABI5 softfp target");return 0;}
    check(argc==6,"package-gate LIB CONTENTS MANIFEST SIGNER ELF-REPORT");elf(argv[1]);
    char *contents=read_text(argv[2]),*manifest=read_text(argv[3]),*signer=read_text(argv[4]),*symbols=read_text(argv[5]);
    int libraries=0;char *cursor=contents;
    while(*cursor){char *end=strchr(cursor,'\n');if(!end)end=cursor+strlen(cursor);char saved=*end;*end=0;
        if(!strncmp(cursor,"lib/",4)){libraries++;check(!strcmp(cursor,"lib/armeabi-v7a/libpilot_face.so"),"only intended ABI/library");}
        check(!strstr(cursor,".dex")&&!strstr(cursor,".js")&&!strstr(cursor,".html"),"no DEX/JS/HTML payload");
        *end=saved;cursor=saved?end+1:end;
    }
    check(libraries==1,"one intended native library");
    check(strstr(manifest,"org.isomorphismes.lepetitprince.pilotface")!=NULL,"package identity");
    check(strstr(manifest,"android.app.NativeActivity")!=NULL,"framework NativeActivity");
    check(strstr(manifest,"android.app.lib_name")&&strstr(manifest,"pilot_face"),"native library metadata");
    const char *has=strstr(manifest,"android:hasCode");check(has!=NULL,"hasCode present");
    const char *line_end=strchr(has,'\n');check(line_end!=NULL,"hasCode line complete");
    const char *value=strstr(has,"=false");check(value&&value<line_end,"hasCode=false");
    check(!strstr(manifest,"android.permission.INTERNET"),"no network permission");
    check(strstr(signer,"certificate SHA-256 digest: de9b1d47c5a65e6d46a204b79dd9ee566b9d3c9832ba81ebc4213d3392e92ff9")!=NULL,"finished APK uses pinned public test certificate");
    check(strstr(signer,"Number of signers: 1")!=NULL,"exactly one signer");
    check(strstr(symbols,"ANativeActivity_onCreate")&&strstr(symbols,"android_main"),"native entry symbols");
    check(strstr(symbols,"libandroid.so")&&strstr(symbols,"libEGL.so")&&strstr(symbols,"libGLESv2.so"),"Android/EGL/GLES dependencies");
    check(!strstr(symbols,"Java_")&&!strstr(symbols,"JNI_OnLoad"),"no application JNI entry");
    free(contents);free(manifest);free(signer);free(symbols);
    puts("PASS package contents, NativeActivity, hasCode=false, entry symbols and pinned signer");return 0;
}
