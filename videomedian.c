#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

static int cmpfunc (const void * a, const void * b)
{
   return ( ((int)*(uint8_t*)a) - ((int)*(uint8_t*)b) );
}

int main(int argc, char* argv[]) {
    if (argc!=6) {
        fprintf(stderr, "Usage: videomedia w h  y_med u_med v_med < yuv420p > yuv420\n");
        return 4;
    }
    
    
    uint32_t w = atoi(argv[1]);
    uint32_t h = atoi(argv[2]);
    uint32_t yw = atoi(argv[3]);
    uint32_t uw = atoi(argv[4]);
    uint32_t vw = atoi(argv[5]);
    uint32_t wmax = MAX(1, MAX(yw, MAX(uw, vw)));
    
    
    uint8_t** buffer = NULL;
    uint8_t* sortbuffer = NULL;
    uint32_t warmup_frames = wmax;
    uint32_t circual_buffer_i = 0;
    
    size_t framelen = w*h*3/2;
    
    int32_t i;
    buffer = malloc(wmax * sizeof(*buffer));
    if (!buffer) { perror("malloc"); return 5; }
    sortbuffer = malloc(wmax);
    if (!sortbuffer) { perror("malloc"); return 5; }
    
    for (i=0; i<wmax; ++i) {
        buffer[i] = malloc(framelen);
        if (!buffer[i]) { perror("malloc"); return 5; }
        memset(buffer[i], 127, framelen);
    }
    
#define OUT(b,off,l) \
        if (!fwrite(b + off, 1, l, stdout)) break;

    while(!feof(stdin)) {
        fread(buffer[circual_buffer_i], 1, framelen, stdin);
        
        if (warmup_frames!=0) {
            OUT(buffer[circual_buffer_i], 0, framelen);
            --warmup_frames;
        } else {
            if (yw <= 1) {
                OUT(buffer[circual_buffer_i], 0, w*h);
            } else {
                uint32_t x,y;
                for(y=0; y<h; ++y) for(x=0; x<w; ++x) {
                    for (i=0; i<yw; ++i) {
                        ssize_t index = circual_buffer_i;
                        index -= i;
                        if (index < 0) index+=wmax;
                        sortbuffer[i] = buffer[index][y*w+x];
                    }
#define SORT_AND_OUT(pw) \
    qsort(sortbuffer, pw, sizeof(sortbuffer[0]), &cmpfunc); \
    if (pw%2==0) { \
        uint32_t b = sortbuffer[pw/2] + sortbuffer[pw/2-1]; \
        uint8_t bb = (b/2); \
        OUT(&bb, 0, 1); \
    } else { \
        OUT(sortbuffer, (pw/2), 1); \
    }
                    SORT_AND_OUT(yw);
                }
            }
            
            if (uw <= 1) {
                OUT(buffer[circual_buffer_i], w*h, w*h/4);
            } else {
                uint32_t x,y;
                for(y=0; y<h/2; ++y) for(x=0; x<w/2; ++x) {
                    for (i=0; i<uw; ++i) {
                        ssize_t index = circual_buffer_i;
                        index -= i;
                        if (index < 0) index+=wmax;
                        sortbuffer[i] = buffer[index][h*w + y*w/2 + x];
                    }
                    SORT_AND_OUT(uw);
                }
            }
            
            if (vw <= 1) {
                OUT(buffer[circual_buffer_i], w*h*5/4, w*h/4);
            } else {
                uint32_t x,y;
                for(y=0; y<h/2; ++y) for(x=0; x<w/2; ++x) {
                    for (i=0; i<vw; ++i) {
                        ssize_t index = circual_buffer_i;
                        index -= i;
                        if (index < 0) index+=wmax;
                        sortbuffer[i] = buffer[index][h*w*5/4 + y*w/2 + x];
                    }
                    SORT_AND_OUT(vw);
                }
            }
        }
        
        circual_buffer_i+=1;
        if (circual_buffer_i == wmax) circual_buffer_i=0;
    }
    
    return 0;
}
