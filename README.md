# videomedian
Temporal-median noise reduction video filter

For denoising heavily glitched video with frequent flickering artifacts.

# Usage

    Usage: videomedia w h  y_med u_med v_med < yuv420p > yuv420
    
    Example:
    
    ffmpeg -v warning -i big_buck_bunny_480p_stereo.avi -f rawvideo - | videomedian 854 480 7 7 7 | ffplay -v warning -f rawvideo -s 854x480 -

# Limitations

* Slow
* Beginning of video is not processed
* Not integrated with FFmpeg or Gstreamer => tricky to use

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


## Natural

(only U and V planes, queue size 7)

![sample2](https://cloud.githubusercontent.com/assets/173219/22272605/2148d0f8-e2ac-11e6-8c6e-c919da950d8d.gif) -> ![sample1](https://cloud.githubusercontent.com/assets/173219/22272622/3318de90-e2ac-11e6-9262-3023a55c80ba.gif)

