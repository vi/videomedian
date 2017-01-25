# videomedian
Temporal-median noise reduction video filter

For denoising glitched video with heavy, but transient artifacts.

# Usage

    Usage: videomedia w h  y_med u_med v_med < yuv420p > yuv420p
    
    Example:
    
    ffmpeg -v warning -i big_buck_bunny_480p_stereo.avi -f rawvideo - | videomedian 854 480 7 7 7 | ffplay -v warning -f rawvideo -s 854x480 -

# Limitations

* Slow (qsort call for every byte)
* Only yuv420p
* Beginning of video is not processed
* Stand-alone, not integrated info a framework.
    * I've also implemented it as filter in FFmpeg: https://github.com/vi/FFmpeg/commits/vf_mediandenoise

# Samples

## Synthetic

Queue size 7 for all planes.

```
$ ffmpeg -v warning -ss 55 -i big_buck_bunny_480p_stereo.avi -t 3 -s 200x200 -f rawvideo - > 200x200.yuv 
$ cat 200x200.yuv | for((i=0; i<3000; ++i)) { if [ $RANDOM -lt 25000 ] ; then dd bs=1440 iflag=fullblock count=1 2> /dev/null;  else   dd bs=1440 iflag=fullblock of=/dev/null count=1 2> /dev/null;  dd bs=1440 if=/dev/frandom iflag=fullblock count=1 2> /dev/null;   fi } > 200x200_glitch.yuv
$ cat 200x200.yuv        | videomedian 200 200 7 7 7 | dd iflag=fullblock bs=$((200*300)) skip=7 2> /dev/null > 200x200_filter.yuv
$ cat 200x200_glitch.yuv | videomedian 200 200 7 7 7 | dd iflag=fullblock bs=$((200*300)) skip=7 2> /dev/null > 200x200_repair.yuv
```

### Original (`200x200.yuv`)

![200x200](https://cloud.githubusercontent.com/assets/173219/22273126/a4fa3556-e2af-11e6-94ba-fcc523d9de68.gif)

### Filtered (`200x200_filtered.yuv`)

![200x200_filter](https://cloud.githubusercontent.com/assets/173219/22273136/b2186c44-e2af-11e6-9984-f20e728ae776.gif)


### Glitched (`200x200_glitch.yuv`)

![200x200_glitch](https://cloud.githubusercontent.com/assets/173219/22273141/b876845e-e2af-11e6-91ca-dc220441713b.gif)


### Reparied (`200x200_repair.yuv`)

![200x200_repair](https://cloud.githubusercontent.com/assets/173219/22273146/c11626a0-e2af-11e6-8664-59f09c319dbc.gif)

### ATA (for comparison)

    ffmpeg -f rawvideo -s 200x200 -i 200x200_glitch.yuv -vf atadenoise=s=7:p=7:0a=0:0b=0:1a=0:1b=0:2a=0:2b=0 -y 200x200_atarep.gif
    
Unglitched: Glitched:

![200x200_ata](https://cloud.githubusercontent.com/assets/173219/22309436/48e5f98c-e35c-11e6-8494-21e37ef2cff1.gif) ![200x200_atarep](https://cloud.githubusercontent.com/assets/173219/22309447/54623ee2-e35c-11e6-99e8-7ef2920490ad.gif)

## Natural

(only U and V planes, queue size 7)

![sample2](https://cloud.githubusercontent.com/assets/173219/22272605/2148d0f8-e2ac-11e6-8c6e-c919da950d8d.gif) -> ![sample1](https://cloud.githubusercontent.com/assets/173219/22272622/3318de90-e2ac-11e6-9262-3023a55c80ba.gif)

For comparison, ATA in dumb mode (`0a=0:0b=0:1a=0:1b=0:2a=0:2b=0:s=7:p=6`):

![b_200x200_ata](https://cloud.githubusercontent.com/assets/173219/22309819/f5c3e50a-e35d-11e6-8777-b96a10dba66e.gif)


